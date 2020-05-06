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
#include "Helpers.h"
#include <string>

#include "Entity.h"

class LuaHelper
{
public:
	static std::string GetTableString(lua_State* l, int index, char const* name);
	static char* GetPermTableString(lua_State* l, int index, char const* name);
	static int GetTableInteger(lua_State* l, int index, char const* name);
	static float GetTableFloat(lua_State* l, int index, char const* name);
	static bool GetTableBool(lua_State* l, int index, char const* name);
	static std::string Dump(lua_State* l, int index);
	static EntityID GetEntityId(lua_State* l, int index = 1)
	{
		uint32_t id = (uint32_t)LuaHelper::GetTableInteger(l, index, "id");
		return EntityID(id);
	}

	template <typename T, int32_t (__thiscall T::*U)()>
	static constexpr auto THUNK()
	{
		return [](lua_State* l)
		{
			auto entity = std::dynamic_pointer_cast<T>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
			if (entity)
			{
				int32_t result = ((*entity).*U)();
				lua_pushinteger(l, result);
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		};
	}
	template <typename T, uint64_t(__thiscall T::*U)()>
	static constexpr auto THUNK()
	{
		return [](lua_State* l)
		{
			auto entity = std::dynamic_pointer_cast<T>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
			if (entity)
			{
				uint64_t result = ((*entity).*U)();
				lua_pushinteger(l, result);
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		};
	}
	template <typename T, bool(__thiscall T::*U)()>
	static constexpr auto THUNK()
	{
		return [](lua_State* l)
		{
			auto entity = std::dynamic_pointer_cast<T>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
			if (entity)
			{
				bool result = ((*entity).*U)();
				lua_pushboolean(l, result);
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		};
	}
	template <typename T, float(__thiscall T::*U)()>
	static constexpr auto THUNK()
	{
		return [](lua_State* l)
		{
			auto entity = std::dynamic_pointer_cast<T>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
			if (entity)
			{
				float result = ((*entity).*U)();
				lua_pushnumber(l, result);
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		};
	}
	template <typename T, std::shared_ptr<Entity>(__thiscall T::*U)()>
	static constexpr auto THUNK()
	{
		return [](lua_State* l)
		{
			auto entity = std::dynamic_pointer_cast<T>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
			if (entity)
			{
				std::shared_ptr<Entity> result = ((*entity).*U)();
				result->MakeLuaObject(l);
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		};
	}
	template <typename T, std::string (__thiscall T::*U)()>
	static constexpr auto THUNK()
	{
		return [](lua_State* l)
		{
			auto entity = std::dynamic_pointer_cast<T>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
			if (entity)
			{
				std::string result = ((*entity).*U)();
				lua_pushstring(l, result.c_str());
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		};
	}
	template <typename T, Vector3(__thiscall T::*U)()>
	static constexpr auto THUNK()
	{
		return [](lua_State* l)
		{
			auto entity = std::dynamic_pointer_cast<T>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
			if (entity)
			{
				Vector3 loc = ((*entity).*U)();
				lua_newtable(l);
				lua_pushnumber(l, loc.v[0]);
				lua_setfield(l, -2, "x");
				lua_pushnumber(l, loc.v[1]);
				lua_setfield(l, -2, "y");
				lua_pushnumber(l, loc.v[2]);
				lua_setfield(l, -2, "z");
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		};
	}
};

