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

#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "Logger.h"

class AttributesTable
{
public:
	static void Initialize(Logger* logger_);

	static std::string GetNameByOffset(uint32_t offset);
	static std::string GetGroupByOffset(uint32_t offset);
	static uint32_t GetOffsetByName(std::string const& name);
	static std::string GetGroupByName(std::string const& name);

	struct AttributeTableEntry
	{
		std::string name;
		uint32_t group;
	};

	static const int OFFSET_STATS = 0x009;
	static const int OFFSET_TRAITS = 0x029;
	static const int OFFSET_DERIVED = 0x059;
	static const int OFFSET_SKILLS = 0x0c5;
	static const int OFFSET_OTRAITS = 0x123;
	static const int OFFSET_PERKS = 0x14d;
	static const int OFFSET_CHEMS = 0x30d;

private:

	static std::map<uint32_t, AttributeTableEntry> namemap_;
	static std::map<std::string, uint32_t> reversemap_;
	static std::vector<std::string> groups_;

};

