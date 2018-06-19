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

#include<map>
#include <string>
#include "lua.hpp"
#include "Logger.h"

class Entity
{
public:
	Entity(void* entityptr);
	Entity(uint16_t id);
	~Entity();

	struct AlterTableLocation
	{
		uint32_t offset;
		uint32_t size;
	};
	typedef std::map<AlterTableLocation, int32_t> AlterTable;

	void MakeLuaObject(lua_State* l);
	bool HasPerk(std::string const& perkname);
	int32_t GetTempPerkValue(std::string const& perkname);
	void SetTempPerkValue(std::string const& perkname, int32_t value);
	void DisplayMessage(std::string const& msg);
	void ApplyTempBonus(AlterTable& alters);
	void RemoveTempBonus(AlterTable& alters);

	static Entity* GetEntityByID(uint16_t id);
	static void RegisterLua(lua_State* l, Logger* tmp);

	// DummyClass is used whenever we call a FoT function which requires
	// the "this" pointer be set.  We override both the this pointer and
	// the function pointer when the call is made.
	class DummyClass
	{
	public:
		void Entity_ShowMessage(uint32_t, uint32_t) {}
		void AttributeTable_Constructor() {}
		void ApplyBuff(void* attributes, uint32_t unknown, float multiplier) {}
		void GetEntityName(wchar_t** ptrtobuf, void* entity) {}

	};

private:
	void* GetEntityPointer();
	std::string GetEntityName();

	typedef struct
	{
		void* entityptr;
		uint16_t unknown_1;
		uint16_t unknown_2;
	} EntityTableEntryType;

	static const uint32_t LOC_WORLD_OBJECT = 0x8bc564;
	static const uint32_t OFFSET_ENTITY_TABLE = 0x0743;
	static const uint32_t OFFSET_ENTITY_ID = 0x004;
	static const uint32_t OFFSET_ENTITY_NAME_PTR = 0x0222;
	static const uint32_t FXN_FOTHEAPALLOC = 0x6c4dd0;
	static const uint32_t FXN_ENTITY_SHOWMESSAGE = 0x5e6d20;
	static const uint32_t FXN_ATTRIBUTES_CONSTRUCTOR = 0x608d30;
	static const uint32_t FXN_ENTITY_APPLYBUFF = 0x56f300;
	static const uint32_t FXN_ENTITY_REMOVEBUFF = 0x56f510;
	static const uint32_t FXN_ENTITY_GETNAME = 0x64f5d0;
	static const uint32_t OBJECT_ENTITY_GETNAME = 0x8bd8b8;

	static const uint32_t OFFSET_ENTITY_ATTRIBUTES = 0x2a2;
	static const uint32_t OFFSET_ENTITY_TEMP_ATTRIBUTES = 0x914;
	static const uint32_t ATTRIBUTES_SIZE = 0x339;

	uint16_t entity_id_;

};

static bool operator<(Entity::AlterTableLocation lhs, Entity::AlterTableLocation rhs)
{
	return lhs.offset < rhs.offset;
}