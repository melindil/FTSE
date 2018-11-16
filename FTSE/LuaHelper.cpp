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

#include "LuaHelper.h"

#include <sstream>


std::string LuaHelper::GetTableString(lua_State* l, int index, char const* name)
{
	lua_getfield(l, index, name);
	std::string ret("");
	if (lua_isstring(l, -1))
	{
		ret = std::string(lua_tostring(l, -1));
	}
	lua_pop(l, 1);
	return ret;
}
char* LuaHelper::GetPermTableString(lua_State* l, int index, char const* name)
{
	lua_getfield(l, index, name);
	std::string ret("");
	if (lua_isstring(l, -1))
	{
		ret = std::string(lua_tostring(l, -1));
	}
	lua_pop(l, 1);
	char* newstr = new char[ret.length() + 1];
	memcpy(newstr, ret.c_str(), ret.length()+1);
	return newstr;
}
int LuaHelper::GetTableInteger(lua_State* l, int index, char const* name)
{
	int ret = 0;
	lua_getfield(l, index, name);
	if (lua_isinteger(l, -1))
	{
		ret = (int)lua_tointeger(l, -1);
	}
	lua_pop(l, 1);
	return ret;
}
float LuaHelper::GetTableFloat(lua_State* l, int index, char const* name)
{
	float ret = 0.0f;
	lua_getfield(l, index, name);
	if (lua_isnumber(l, -1))
	{
		ret = (float)lua_tonumber(l, -1);
	}
	lua_pop(l, 1);
	return ret;
}

std::string LuaHelper::Dump(lua_State* l, int index)
{
	std::stringstream ss;
	if (lua_isinteger(l, index))
		ss << "Integer " << lua_tointeger(l, index);
	else if (lua_isnumber(l, index))
		ss << "Float " << (float)lua_tonumber(l, index);
	else if (lua_isstring(l, index))
		ss << "String (" << lua_tostring(l, index) << ")";
	else if (lua_istable(l, index))
		ss << "Table";
	else if (lua_isnoneornil(l, index))
		ss << "Nil/Empty";
	return ss.str();
}