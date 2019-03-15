/* MIT License

Copyright (c) 2018 melindil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "GenericPatcher.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/error/en.h"

#include <stdlib.h>
#include <Windows.h>
#include "Version.h"
#include <fstream>
#include <sstream>

GenericPatcher::ApplyType GenericPatcher::globals_[] = {

// following entries keep pointer to weapon available for OnChanceToHitCalc
{0x617911, {0x90}},
{0x61791e, {0x90}},
{0x617945, {0x90}},
{0x61796a, {0x90}},
{0x617a6e, {0x90}},
{0x617a8f, {0x90}},
{0x617aae,{0x90}},
{0x617ac1, {0x90}},
{0x617b0b, {0x90}},
{0x617b27, {0x90}},
};
GenericPatcher::GenericPatcher(Logger* log, std::string const& configname)
	: logger_(log)
{
	try
	{
		std::ifstream infile(configname);
		if (!infile)
		{
			throw std::runtime_error(std::string("Could not open configuration file ") + configname);
		}
		rapidjson::IStreamWrapper isw(infile);
		rapidjson::Document doc;
		if (doc.ParseStream(isw).HasParseError())
		{
			std::stringstream ss;
			ss << "JSON parse error (offset " << doc.GetErrorOffset()
				<< "): " << rapidjson::GetParseError_En(doc.GetParseError());
			throw std::runtime_error(ss.str());
		}
		if (!doc.HasMember("patches") || !doc["patches"].IsArray())
		{
			throw std::runtime_error("JSON document is missing the patches array element");
		}
		if (doc.HasMember("lua"))
		{
			luaname_ = doc["lua"].GetString();
		}
		else
		{
			luaname_ = "ftse.lua";
		}
		for (auto itr = doc["patches"].Begin();
			itr != doc["patches"].End();
			itr++)
		{
			std::string patchname = (*itr)["name"].GetString();
			try
			{
				std::string apply = (*itr)["apply"].GetString();
				if (apply != "true" && apply != "True" && apply != "TRUE")
				{
					throw std::runtime_error(std::string("Skipping apply for patch \"") + patchname + "\"");
				}
				else if (!itr->HasMember("changes") || !(*itr)["changes"].IsArray())
				{
					throw std::runtime_error(std::string("Patch \"" + patchname + "\" has no changes array, skipping"));
				}
				else
				{
					PatchType pt;
					pt.name = patchname;
					pt.changes = ParseChanges(patchname, (*itr)["changes"]);
					patches_.push_back(pt);
				}
			}
			catch (std::exception& e)
			{
				*logger_ << e.what() << std::endl;
			}
		}
	}
	catch (std::exception& e)
	{
		*logger_ << e.what() << std::endl;
	}
	
}
std::string GenericPatcher::getLuaName()
{
	return luaname_;
}

std::vector<GenericPatcher::ApplyType> GenericPatcher::ParseChanges(
	std::string const& patchname,
	rapidjson::Value const& val)
{
	std::vector<ApplyType> ats;
	
	for (auto itr = val.Begin();
		itr != val.End();
		itr++)
	{
		ApplyType at;
		at.offset = std::stoul((*itr)["offset"].GetString(),nullptr, 16);
		std::string patch = (*itr)["patch"].GetString();
		if (patch.length() % 2)
		{
			throw std::runtime_error("Patch \"" + patchname + "\" has a change with an odd number of hex digits. Skipping.");
		}
		at.patch = ConvertFromHex(patch);
		ats.push_back(at);
	}

	return ats;
}

std::vector<unsigned char> GenericPatcher::ConvertFromHex(std::string const& in)
{
	std::vector<unsigned char> out(in.length() / 2);
	for (size_t i = 0 ; i < in.length(); i+=2)
	{
		std::string ch(in, i, 2);
		out[i / 2] = (unsigned char)(std::stoul(ch, nullptr, 16));
	}
	return out;
}
GenericPatcher::~GenericPatcher()
{
}

void GenericPatcher::apply()
{
	for (auto patch : patches_)
	{
		*logger_ << "Applying patch " << patch.name << std::endl;
		for (auto change : patch.changes)
		{
			try
			{
				apply_impl(change);
			}
			catch (std::exception& e)
			{
				*logger_ << e.what() << std::endl;
			}
		}
	}
	for (auto change : globals_)
	{
		apply_impl(change);
	}

	ApplyType version_app;
	version_app.offset = 0x8aed98;
	char const* verstring = "1.27 + FTSE " FTSE_VERSION;
	char const** verloc = &verstring;
	version_app.patch.resize(4);
	memcpy(version_app.patch.data(), verloc, sizeof(char*));
	apply_impl(version_app);
}

void GenericPatcher::apply_impl(GenericPatcher::ApplyType const& app)
{
	DWORD old_protect = 0, bogus_protect = 0;
	if (!VirtualProtect((void*)(app.offset), app.patch.size(), PAGE_EXECUTE_READWRITE, &old_protect))
	{
		std::stringstream ss;
		ss << "Could not protect memory address 0x" << std::hex << app.offset << ", skipping";
		throw std::runtime_error(ss.str());
	}
	memcpy((void*)(app.offset), app.patch.data(), app.patch.size());
	VirtualProtect((void*)(app.offset), app.patch.size(), old_protect, &bogus_protect);
}