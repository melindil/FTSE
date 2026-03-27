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
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include <stdlib.h>
#include <Windows.h>
#include "Version.h"
#include "Helpers.h"
#include <fstream>
#include <sstream>
#include <iomanip>

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

// Disable old FTSE startup
{ 0x6be32d,{0x83,0xcf,0xff}}
};
GenericPatcher::GenericPatcher(Logger* log, std::string const& configbase)
	: logger_(log), basename_(configbase),customconfig_("FTSE_config.json")
{
	ApplyDocument(basename_+"FTSE_config_base.json", false);
}

void GenericPatcher::ApplyDocument(std::string const& configname, bool is_custom)
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
/*		if (doc.HasMember("lua"))
		{
			luaname_ = doc["lua"].GetString();
		}*/

		for (auto itr = doc["patches"].Begin();
			itr != doc["patches"].End();
			itr++)
		{
			try
			{
				std::string patchname = (*itr)["name"].GetString();
				PatchType& pt = patches_[patchname];

				if (itr->HasMember("changes") && (*itr)["changes"].IsArray())
				{
					pt.changes = ParseChanges(patchname, (*itr)["changes"]);
					pt.is_custom = is_custom;
				}
				if (itr->HasMember("apply"))
				{
					std::string apply = (*itr)["apply"].GetString();
					if (apply == "true" || apply == "True" || apply == "TRUE")
					{
						pt.apply = true;
					}
					else
					{
						pt.apply = false;
					}

				}
				if (itr->HasMember("description"))
				{
					pt.description = (*itr)["description"].GetString();
				}
				if (itr->HasMember("flags"))
				{
					pt.flagstring = (*itr)["flags"].GetString();
				}
			}
			catch (std::exception& e)
			{
				*logger_ << e.what() << std::endl;
			}
		}
		if (doc.HasMember("custom-config"))
		{
			customconfig_ = doc["custom-config"].GetString();
			try
			{
				std::string name(doc["custom-config"].GetString());
				ApplyDocument(basename_+name, true);
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
std::string GenericPatcher::ConvertToHex(std::vector<unsigned char>& in)
{
	std::stringstream ss;
	for (size_t i = 0; i < in.size(); i++)
	{
		ss << std::setw(2) << std::setfill('0') << std::hex << ((uint16_t)in[i]);
	}
	return ss.str();
}
GenericPatcher::~GenericPatcher()
{
}

void GenericPatcher::apply()
{
	flags_.clear();
	for (auto patch : patches_)
	{
		if (patch.second.apply)
		{
			*logger_ << "Applying patch " << patch.first << std::endl;
			for (auto change : patch.second.changes)
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
			if (patch.second.flagstring != "")
			{
				flags_.insert(patch.second.flagstring);
			}
		}
		else
		{
			*logger_ << "Skipping patch " << patch.first << std::endl;
		}
	}
	for (auto change : globals_)
	{
		apply_impl(change);
	}


}

bool GenericPatcher::IsFlagSet(std::string const& flagstring)
{
	return flags_.find(flagstring) != flags_.end();
}

std::vector<GenericPatcher::PatchDescriptor> GenericPatcher::GetPatchDescriptors()
{
	std::vector<PatchDescriptor> ret;
	ret.reserve(patches_.size());
	for (auto& entry : patches_)
	{
		ret.emplace_back(PatchDescriptor{ entry.first, entry.second.description, 0, entry.second.apply, entry.second.apply });
	}
	return ret;
}

void GenericPatcher::WriteNewConfig(std::vector<PatchDescriptor>& descriptors)
{
	std::string newname = basename_ + customconfig_;
	*logger_ << "Write to config file " << newname << std::endl;

	std::ofstream outcfg(newname);

	rapidjson::OStreamWrapper osw(outcfg);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value patches(rapidjson::kArrayType);

	for (auto& entry : descriptors)
	{
		auto& patch = patches_[entry.name];
		patch.apply = entry.new_apply;
		rapidjson::Value node(rapidjson::kObjectType);

		rapidjson::Value name;
		name.SetString(entry.name.c_str(), entry.name.length(), allocator);
		node.AddMember("name", name, allocator);

		rapidjson::Value apply;
		apply.SetString((entry.new_apply) ? "true" : "false", allocator);
		node.AddMember("apply", apply, allocator);

		if (patch.is_custom)
		{
			rapidjson::Value desc;
			desc.SetString(entry.description.c_str(),entry.description.length(), allocator);
			node.AddMember("description", desc, allocator);
			rapidjson::Value changes(rapidjson::kArrayType);
			for (auto& change : patch.changes)
			{
				std::stringstream ss;
				ss << std::hex << change.offset;
				rapidjson::Value offset;
				std::string offsetstr = ss.str();
				offset.SetString(offsetstr.c_str(), offsetstr.length(), allocator);
				rapidjson::Value changenode(rapidjson::kObjectType);
				changenode.AddMember("offset", offset, allocator);
				std::string patchstring = ConvertToHex(change.patch);
				rapidjson::Value patchdata;
				patchdata.SetString(patchstring.c_str(), patchstring.length(), allocator);
				changenode.AddMember("patch", patchdata, allocator);
				changes.PushBack(changenode, allocator);

			}
			node.AddMember("changes", changes, allocator);
		}
		patches.PushBack(node, allocator);
	}

	doc.AddMember("patches", patches, allocator);
	
	doc.Accept(writer);

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