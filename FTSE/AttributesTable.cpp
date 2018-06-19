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

#include "AttributesTable.h"

std::map<uint32_t, AttributesTable::AttributeTableEntry> AttributesTable::namemap_;
std::map<std::string, uint32_t> AttributesTable::reversemap_;
std::vector<std::string> AttributesTable::groups_;

void AttributesTable::Initialize(Logger* logger_)
{
	groups_.clear();
	namemap_.clear();
	reversemap_.clear();

	groups_.push_back("Error");
	groups_.push_back("stats");
	groups_.push_back("traits");
	groups_.push_back("derived");
	groups_.push_back("skills");
	groups_.push_back("otraits");
	groups_.push_back("perks");
	groups_.push_back("chems");

	// stats
	char** ptr = (char**)0x8a6b70;
	uint32_t offset = OFFSET_STATS;
	for (int i = 0; i < 7; i++)
	{
		AttributeTableEntry ate;
		ate.name = std::string(ptr[i]);
		ate.group = 1;
		namemap_[offset] = ate;
		reversemap_[ate.name] = offset;
		offset += 4;
	}

	// traits
	ptr = (char**)0x8a6b8c;
	offset = OFFSET_TRAITS;
	for (int i = 0; i < 11; i++)
	{
		AttributeTableEntry ate;
		ate.name = std::string(ptr[i]);
		ate.group = 2;
		namemap_[offset] = ate;
		reversemap_[ate.name] = offset;
		offset += 4;
	}

	// Derived
	ptr = (char**)0x8a41a0;
	offset = OFFSET_DERIVED;
	for (int i = 0; i < 26; i++)
	{
		AttributeTableEntry ate;
		ate.name = std::string(ptr[i]);
		ate.group = 3;
		namemap_[offset] = ate;
		reversemap_[ate.name] = offset;
		offset += 4;
	}
	
	// Skills
	ptr = (char**)0x8a6b18;
	offset = OFFSET_SKILLS;
	for (int i = 0; i < 18; i++)
	{
		AttributeTableEntry ate;
		ate.name = std::string(ptr[i]);
		ate.group = 4;
		namemap_[offset] = ate;
		reversemap_[ate.name] = offset;
		offset += 4;
	}
	for (int i = 0; i < 18; i++)
	{
		AttributeTableEntry ate;
		ate.name = std::string("tag_") + std::string(ptr[i]);
		ate.group = 4;
		namemap_[offset] = ate;
		reversemap_[ate.name] = offset;
		offset += 1;
	}

	// Optional Traits
	ptr = (char**)0x8a42d0;
	offset = OFFSET_OTRAITS;
	for (int i = 0; i < 38; i++)
	{
		AttributeTableEntry ate;
		ate.name = std::string(ptr[i]);
		ate.group = 5;
		namemap_[offset] = ate;
		reversemap_[ate.name] = offset;
		offset += 1;
	}

	// Perks
	ptr = (char**)0x8a4500;
	offset = OFFSET_PERKS;
	for (int i = 0; i < 111; i++)
	{
		AttributeTableEntry ate;
		ate.name = std::string(ptr[i * 19]);
		ate.group = 6;
		namemap_[offset] = ate;
		reversemap_[ate.name] = offset;
		offset += 4;
	}

	// Chems
	ptr = (char**)0x8a40cc;
	offset = OFFSET_CHEMS;
	for (int i = 0; i < 10; i++)
	{
		AttributeTableEntry ate;
		ate.name = std::string(ptr[i]);
		ate.group = 7;
		namemap_[offset] = ate;
		reversemap_[ate.name] = offset;
		offset += 4;
	}

}

std::string AttributesTable::GetNameByOffset(uint32_t offset)
{
	return namemap_[offset].name;
}

std::string AttributesTable::GetGroupByOffset(uint32_t offset)
{
	return groups_[namemap_[offset].group];
}
uint32_t AttributesTable::GetOffsetByName(std::string const& name)
{
	return reversemap_[name];
}
std::string AttributesTable::GetGroupByName(std::string const& name)
{
	return groups_[namemap_[reversemap_[name]].group];
}
