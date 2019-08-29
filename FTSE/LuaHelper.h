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

#include "lua.hpp"
#include <string>

class LuaHelper
{
public:
	static std::string GetTableString(lua_State* l, int index, char const* name);
	static char* GetPermTableString(lua_State* l, int index, char const* name);
	static int GetTableInteger(lua_State* l, int index, char const* name);
	static float GetTableFloat(lua_State* l, int index, char const* name);
	static bool GetTableBool(lua_State* l, int index, char const* name);
	static std::string Dump(lua_State* l, int index);
	static uint16_t GetEntityId(lua_State* l, int index = 1)
	{
		return (uint16_t)LuaHelper::GetTableInteger(l, index, "id");
	}
};

