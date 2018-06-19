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

#include "Entity.h"
#include "AttributesTable.h"
#include "LuaHelper.h"
#include <Windows.h>
#include <stringapiset.h>
#include <vector>

static const DWORD FXN_FOTHEAPALLOC = 0x6c4dd0;
static char* (*FOTHeapAlloc)(DWORD) = (char* (*)(DWORD))FXN_FOTHEAPALLOC;

Logger* logger_;

Entity::Entity(void* entityptr)
	: entity_id_(*((uint16_t*)(((char*)entityptr) + OFFSET_ENTITY_ID)))
{
}

Entity::Entity(uint16_t id)
	: entity_id_(id)
{

}

Entity::~Entity()
{
}

void* Entity::GetEntityPointer()
{
	char* WorldObject = *((char**)LOC_WORLD_OBJECT);
	EntityTableEntryType* EntityTable = *((EntityTableEntryType**)(WorldObject + OFFSET_ENTITY_TABLE));
	return EntityTable[entity_id_].entityptr;
}

void Entity::MakeLuaObject(lua_State* l)
{
	// Represent an entity as a table containing name and ID
	// Can add other elements here for quick access (if needed)
	// Otherwise, use the entity ID as the identifier to re-obtain
	// the pointer from the entity table if we have to make any
	// changes to it.
	
	std::string EntityName = GetEntityName();
	lua_newtable(l);
	lua_pushstring(l, EntityName.c_str());
	lua_setfield(l, -2, "name");
	lua_pushinteger(l, entity_id_);
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "EntityMetaTable");
	lua_setmetatable(l, -2);

}

int l_entity_hasperk(lua_State* l);
int l_entity_gettempperkvalue(lua_State* l);
int l_entity_settempperkvalue(lua_State* l);
int l_entity_displaymessage(lua_State* l);
int l_entity_applytempbonus(lua_State* l);
int l_entity_removetempbonus(lua_State* l);

void Entity::RegisterLua(lua_State* l, Logger* tmp)
{
	logger_ = tmp;
	luaL_newmetatable(l, "EntityMetaTable");
	lua_pushcfunction(l, l_entity_hasperk);
	lua_setfield(l, -2, "HasPerk");
	lua_pushcfunction(l, l_entity_gettempperkvalue);
	lua_setfield(l, -2, "GetTempPerkValue");
	lua_pushcfunction(l, l_entity_settempperkvalue);
	lua_setfield(l, -2, "SetTempPerkValue");
	lua_pushcfunction(l, l_entity_displaymessage);
	lua_setfield(l, -2, "DisplayMessage");
	lua_pushcfunction(l, l_entity_applytempbonus);
	lua_setfield(l, -2, "ApplyTempBonus");
	lua_pushcfunction(l, l_entity_removetempbonus);
	lua_setfield(l, -2, "RemoveTempBonus");
	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "EntityMetaTable");
}

bool Entity::HasPerk(std::string const& perkname)
{
	uint32_t offset = AttributesTable::GetOffsetByName(perkname);
	offset += ((uint32_t)GetEntityPointer()) + OFFSET_ENTITY_ATTRIBUTES;
	return *((uint32_t*)offset) != 0;
}

int l_entity_hasperk(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	std::string perkname (lua_tostring(l, 2));
	Entity e(id);
	bool result = e.HasPerk(perkname);
	lua_pushboolean(l, (result) ? 1 : 0);
	return 1;
}

int32_t Entity::GetTempPerkValue(std::string const& perkname)
{
	uint32_t offset = AttributesTable::GetOffsetByName(perkname);
	offset += ((uint32_t)GetEntityPointer()) + OFFSET_ENTITY_TEMP_ATTRIBUTES;
	return *(uint32_t*)offset;
}

int l_entity_gettempperkvalue(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	std::string perkname(lua_tostring(l, 2));
	Entity e(id);
	uint32_t result = e.GetTempPerkValue(perkname);
	lua_pushinteger(l, result);
	return 1;
}

void Entity::SetTempPerkValue(std::string const& perkname, int32_t value)
{
	uint32_t offset = AttributesTable::GetOffsetByName(perkname);
	offset += ((uint32_t)GetEntityPointer()) + OFFSET_ENTITY_TEMP_ATTRIBUTES;
	*(int32_t*)offset = value;
}

int l_entity_settempperkvalue(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	std::string perkname(lua_tostring(l, 2));
	int32_t value = (int32_t)lua_tointeger(l, 3);
	Entity e(id);
	e.SetTempPerkValue(perkname, value);
	return 0;
}

void Entity::DisplayMessage(std::string const& msg)
{
	void* entity = GetEntityPointer();
	DummyClass* c1 = (DummyClass*)entity;
	auto fxn = &DummyClass::Entity_ShowMessage;
	uint32_t offset = FXN_ENTITY_SHOWMESSAGE;
	memcpy(&fxn, &offset, 4);

	// Things work much better if we let FoT allocate the memory for the
	// message.  Note that there are three DWORDs before the message
	// content: A usage counter (which should start at 0 in this code
	// location), an entity size?, and a string length in chars
	uint32_t len = 14 + 2 * msg.length();
	char* alloced = FOTHeapAlloc(len);

	*((uint32_t*)alloced) = 0;	// Ref count
	*((uint32_t*)alloced + 1) = msg.length();
	*((uint32_t*)alloced + 2) = msg.length();
	alloced += 12;
	*((WCHAR*)alloced) = 0;
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, msg.c_str(), -1, (WCHAR*)alloced, msg.length() + 1);

	(c1->*fxn)(((uint32_t)&alloced), 0x8be1c8);	// Dunno what the parameter is?

}

int l_entity_displaymessage(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	std::string msg(lua_tostring(l, 2));
	Entity e(id);
	e.DisplayMessage(msg);
	return 0;
}

void Entity::ApplyTempBonus(AlterTable& alters)
{
	void* entity = GetEntityPointer();

	// Let FoT construct an attributes object for us
	// Since FoT doesn't keep any pointers of it around,
	// we can use our own address space.
	std::vector<char> temp_attribute_table(ATTRIBUTES_SIZE, 0);
	DummyClass* c1 = (DummyClass*)temp_attribute_table.data();
	auto fxn = &DummyClass::AttributeTable_Constructor;
	size_t offset = FXN_ATTRIBUTES_CONSTRUCTOR;
	memcpy(&fxn, &offset, 4);
	(c1->*fxn)();

	for (auto entry : alters)
	{
		char* ptr = (char*)temp_attribute_table.data();
		*logger_ << "Editing offset 0x" << std::hex << entry.first.offset << std::dec << " to " << entry.second << std::endl;
		ptr += entry.first.offset;
		memcpy(ptr, &entry.second, entry.first.size);
	}

	// Now we need to call the routine that applies the bonus
	c1 = (DummyClass*)entity;
	auto fxn2 = &DummyClass::ApplyBuff;
	size_t offset2 = FXN_ENTITY_APPLYBUFF;
	memcpy(&fxn2, &offset2, 4);
	(c1->*fxn2)(temp_attribute_table.data(), 0, 1.0);

}

int l_entity_applytempbonus(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Entity e(id);

	Entity::AlterTable at;

	if (!lua_istable(l, 2))
	{
		// someone did something wrong, bail out
		return 0;
	}
	lua_pushvalue(l, 2);	// copy reference for the table
	lua_pushnil(l);
	while(lua_next(l, -2))
	{
		std::string key = lua_tostring(l, -2);
		int32_t value = (int32_t)lua_tointeger(l, -1);

		Entity::AlterTableLocation loc;
		loc.offset = AttributesTable::GetOffsetByName(key);
		if (std::string(key, 0, 4) == "tag_" || AttributesTable::GetGroupByName(key) == "otraits")
		{
			// otraits and skill tags use 1-byte fields
			// (we shouldn't be using these here, but just in case...)
			loc.size = 1;
		}
		else
		{
			loc.size = 4;
		}

		at[loc] = value;
		
		// pop the value, the key stays for next loop
		lua_pop(l, 1);
		
	}
	lua_pop(l, 1);	// pop the duplicate table
	
	e.ApplyTempBonus(at);
	return 0;

}

void Entity::RemoveTempBonus(AlterTable& alters)
{
	void* entity = GetEntityPointer();

	// Let FoT construct an attributes object for us
	// Since FoT doesn't keep any pointers of it around,
	// we can use our own address space.
	std::vector<char> temp_attribute_table(ATTRIBUTES_SIZE, 0);
	DummyClass* c1 = (DummyClass*)temp_attribute_table.data();
	auto fxn = &DummyClass::AttributeTable_Constructor;
	size_t offset = FXN_ATTRIBUTES_CONSTRUCTOR;
	memcpy(&fxn, &offset, 4);
	(c1->*fxn)();

	for (auto entry : alters)
	{
		char* ptr = (char*)temp_attribute_table.data();
		ptr += entry.first.offset;
		memcpy(ptr, &entry.second, entry.first.size);
	}

	// Now we need to call the routine that applies the bonus
	c1 = (DummyClass*)entity;
	auto fxn2 = &DummyClass::ApplyBuff;			// These have the same function definition
	size_t offset2 = FXN_ENTITY_REMOVEBUFF;
	memcpy(&fxn2, &offset2, 4);
	(c1->*fxn2)(temp_attribute_table.data(), 0, 1.0);

}

int l_entity_removetempbonus(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Entity e(id);

	Entity::AlterTable at;

	if (!lua_istable(l, 2))
	{
		// someone did something wrong, bail out
		return 0;
	}
	lua_pushvalue(l, 2);	// copy reference for the table
	lua_pushnil(l);
	while (lua_next(l, -2))
	{
		std::string key = lua_tostring(l, -2);
		int32_t value = (int32_t)lua_tointeger(l, -1);

		Entity::AlterTableLocation loc;
		loc.offset = AttributesTable::GetOffsetByName(key);
		if (std::string(key, 0, 4) == "tag_" || AttributesTable::GetGroupByName(key) == "otraits")
		{
			// otraits and skill tags use 1-byte fields
			// (we shouldn't be using these here, but just in case...)
			loc.size = 1;
		}
		else
		{
			loc.size = 4;
		}

		at[loc] = value;

		// pop the value, the key stays for next loop
		lua_pop(l, 1);

	}
	lua_pop(l, 1);	// pop the duplicate table

	e.RemoveTempBonus(at);
	return 0;

}

std::string Entity::GetEntityName()
{
	void* entity = GetEntityPointer();
	WCHAR* wcharname;

	DummyClass* c1 = (DummyClass*)OBJECT_ENTITY_GETNAME;
	auto fxn2 = &DummyClass::GetEntityName;			// These have the same function definition
	size_t offset2 = FXN_ENTITY_GETNAME;
	memcpy(&fxn2, &offset2, 4);
	(c1->*fxn2)(&wcharname, entity);

	int len = wcslen(wcharname);
	std::vector<char> tempname(len*4, 0);
	WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wcharname, -1, tempname.data(), len*4, nullptr, nullptr);

	// We need to decrement the usage counter for the name string, or it might leak
	DWORD* obj = (DWORD*)wcharname;
	obj[-3]--;

	return std::string(tempname.data());


}