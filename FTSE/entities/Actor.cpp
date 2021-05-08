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

#include "Actor.h"
#include "World.h"
#include "AttributesTable.h"
#include "LuaHelper.h"
#include <vector>

#include "Helpers.h"
#include "Inventory.h"

static const uint32_t FXN_FOTHEAPALLOC = 0x6c4dd0;
static char* (*FOTHeapAlloc)(uint32_t) = (char* (*)(uint32_t))FXN_FOTHEAPALLOC;

static Logger* logger_;

const std::map<std::string, Actor::ActorFOTOffset> Actor::offsets{
{ "playerindex", {0x163, Actor::FieldType::INTEGER}},
{ "hp", {0x281, Actor::FieldType::INTEGER}},
{ "bandaged", {0x285, Actor::FieldType::INTEGER}},
{ "ap", { 0x289, Actor::FieldType::FLOAT}},
{ "radpoints", { 0x28d, Actor::FieldType::INTEGER}},
{ "radlevel", { 0x0291, Actor::FieldType::INTEGER}},
{ "poisonpoints", { 0x295, Actor::FieldType::INTEGER}},
{ "poisonlevel", {0x299, Actor::FieldType::INTEGER}},
{ "overdosepoints", {0x29d, Actor::FieldType::INTEGER}},
{ "goneuplevel", {0x02a1, Actor::FieldType::BOOLEAN}},
{ "stance", {0x1fe, Actor::FieldType::INTEGER}},
{ "posture", {0xf8a, Actor::FieldType::WCHAR_STRING}},
{ "injured", {0xfb2, Actor::FieldType::BOOLEAN}},
{ "injuredtorso",{ 0xfb3, Actor::FieldType::BOOLEAN } },
{ "injuredhead",{ 0xfb4, Actor::FieldType::BOOLEAN } },
{ "injuredeyes",{ 0xfb5, Actor::FieldType::BOOLEAN } },
{ "injuredrightarm",{ 0xfb6, Actor::FieldType::BOOLEAN } },
{ "injuredleftarm",{ 0xfb7, Actor::FieldType::BOOLEAN } },
{ "injuredgroin",{ 0xfb8, Actor::FieldType::BOOLEAN } },
{ "injuredleftleg",{ 0xfb9, Actor::FieldType::BOOLEAN } },
{ "injuredrightleg",{ 0xfba, Actor::FieldType::BOOLEAN } },
{ "unconscious",{ 0xfbb, Actor::FieldType::BOOLEAN } },
{ "unconscioustime",{ 0xfbc, Actor::FieldType::INTEGER } },
{ "reputation",{ 0xfed, Actor::FieldType::INTEGER } },
{ "isgeneral", {0x1053, Actor::FieldType::BOOLEAN} },
{ "isrecruitmaster",{ 0x1054, Actor::FieldType::BOOLEAN } },
{ "isquartermaster",{ 0x1055, Actor::FieldType::BOOLEAN } }

};

Actor::Actor(void* entityptr)
	: Entity(entityptr)
{
}

Actor::Actor(EntityID id)
	: Entity(id)
{

}

Actor::~Actor()
{
}



void Actor::MakeLuaObject(lua_State* l)
{
	// Represent an entity as a table containing name and ID
	// Can add other elements here for quick access (if needed)
	// Otherwise, use the entity ID as the identifier to re-obtain
	// the pointer from the entity table if we have to make any
	// changes to it.
	
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ActorMetaTable");
	lua_setmetatable(l, -2);

}

int l_actor_getattribute(lua_State* l);
int l_actor_setattribute(lua_State* l);
int l_actor_applybonus(lua_State* l);
int l_actor_removebonus(lua_State* l);
int l_actor_displaymessage(lua_State* l);
int l_actor_getfield(lua_State* l);
int l_actor_setfield(lua_State* l);
int l_actor_getinventorylist(lua_State* l);
int l_actor_equipitem(lua_State* l);
int l_actor_unequipitem(lua_State* l);
int l_actor_getequippeditem(lua_State* l);

void Actor::RegisterLua(lua_State* l, Logger* tmp)
{
	logger_ = tmp;
	luaL_newmetatable(l, "ActorMetaTable");

	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, l_actor_getattribute);
	lua_setfield(l, -2, "GetAttribute");
	lua_pushcfunction(l, l_actor_setattribute);
	lua_setfield(l, -2, "SetAttribute");
	lua_pushcfunction(l, l_actor_displaymessage);
	lua_setfield(l, -2, "DisplayMessage");
	lua_pushcfunction(l, l_actor_applybonus);
	lua_setfield(l, -2, "ApplyBonus");
	lua_pushcfunction(l, l_actor_removebonus);
	lua_setfield(l, -2, "RemoveBonus");
	lua_pushcfunction(l, l_actor_getfield);
	lua_setfield(l, -2, "GetField");
	lua_pushcfunction(l, l_actor_setfield);
	lua_setfield(l, -2, "SetField");
	lua_pushcfunction(l, l_actor_getinventorylist);
	lua_setfield(l, -2, "GetInventory");
	lua_pushcfunction(l, l_actor_equipitem);
	lua_setfield(l, -2, "EquipItem");
	lua_pushcfunction(l, l_actor_unequipitem);
	lua_setfield(l, -2, "UnequipItem");
	lua_pushcfunction(l, l_actor_getequippeditem);
	lua_setfield(l, -2, "GetEquippedItem");

	lua_pushstring(l, "Actor");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ActorMetaTable");
}

int32_t Actor::GetAttribute(std::string const& name, int table)
{
	uint32_t offset = AttributesTable::GetOffsetByName(name);
	offset += ((uint32_t)GetEntityPointer()) + OFFSET_ACTOR_ATTRIBUTES
		+ table * ATTRIBUTES_SIZE;
	if (std::string(name, 0, 4) == "tag_" || AttributesTable::GetGroupByName(name) == "otraits")
	{
		char c = *(char*)offset;
		return (int)c;
	}

	return *(uint32_t*)offset;
}

void Actor::SetAttribute(std::string const& name, int table, int32_t value)
{
	uint32_t offset = AttributesTable::GetOffsetByName(name);
	offset += ((uint32_t)GetEntityPointer()) + OFFSET_ACTOR_ATTRIBUTES
		+ table * ATTRIBUTES_SIZE;
	if (std::string(name, 0, 4) == "tag_" || AttributesTable::GetGroupByName(name) == "otraits")
	{
		char c = (char)value;
		*((char*)offset) = c;
	}
	else
	{
		*((int32_t*)offset) = value;
	}

}

int l_actor_getinventorylist(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Actor>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		auto inventory_entity = std::dynamic_pointer_cast<Inventory>(entity->GetInventory());
		if (inventory_entity)
		{
			lua_newtable(l);
			auto items = inventory_entity->GetItemList();
			int i = 1;
			for (auto item : items)
			{
				item->MakeLuaObject(l);
				lua_rawseti(l, -2, i);
				i++;
			}
		}
		else
		{
			lua_pushnil(l);
		}
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

int l_actor_getattribute(lua_State* l)
{
	Actor e(LuaHelper::GetEntityId(l));
	std::string perkname(lua_tostring(l, 2));
	int table = (int)lua_tointeger(l, 3);

	uint32_t result = e.GetAttribute(perkname, table);
	lua_pushinteger(l, result);
	return 1;
}

int l_actor_setattribute(lua_State* l)
{
	Actor e(LuaHelper::GetEntityId(l));
	std::string perkname(lua_tostring(l, 2));
	int table = (int)lua_tointeger(l, 3);
	int32_t value = (int32_t)lua_tointeger(l, 4);
	e.SetAttribute(perkname, table, value);
	return 0;
}

int l_actor_getfield(lua_State* l)
{
	Actor e(LuaHelper::GetEntityId(l));
	std::string fieldname(lua_tostring(l, 2));
	e.GetField(l, fieldname);
	return 1;
}
int l_actor_setfield(lua_State* l)
{
	Actor e(LuaHelper::GetEntityId(l));
	std::string fieldname(lua_tostring(l, 2));
	e.SetField(l, fieldname);
	return 0;
}

int l_actor_displaymessage(lua_State* l)
{
	Actor e(LuaHelper::GetEntityId(l));
	std::string msg(lua_tostring(l, 2));
	e.DisplayMessage(msg);
	return 0;
}

void Actor::ApplyBonus(AlterTable& alters,bool permanent)
{
	void* entity = GetEntityPointer();

	// Let FoT construct an attributes object for us
	// Since FoT doesn't keep any pointers of it around,
	// we can use our own address space.
	std::vector<char> temp_attribute_table(ATTRIBUTES_SIZE, 0);
	auto fxn = (void(__thiscall*)(void*))FXN_ATTRIBUTES_CONSTRUCTOR;
	fxn(temp_attribute_table.data());

	if (permanent)
		temp_attribute_table[4] = 0;

	for (auto entry : alters)
	{
		char* ptr = (char*)temp_attribute_table.data();
		*logger_ << "Editing offset 0x" << std::hex << entry.first.offset << std::dec << " to " << entry.second << std::endl;
		ptr += entry.first.offset;
		memcpy(ptr, &entry.second, entry.first.size);
	}

	// Now we need to call the routine that applies the bonus
	auto fxn2 = (void(__thiscall*)(void*, void*, uint32_t, float))FXN_ACTOR_APPLYBUFF;
	fxn2(entity, temp_attribute_table.data(), 0, 1.0);

}

int l_actor_applybonus(lua_State* l)
{
	Actor e(LuaHelper::GetEntityId(l));

	Actor::AlterTable at;

	bool  perm = lua_toboolean(l, 3);

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

		Actor::AlterTableLocation loc;
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
	
	e.ApplyBonus(at,perm);
	return 0;

}

std::shared_ptr<Entity> Actor::GetInventory()
{
	return Entity::GetEntityByID(GetStruct()->inventory);
}

void Actor::RemoveBonus(AlterTable& alters, bool permanent)
{
	void* entity = GetEntityPointer();

	// Let FoT construct an attributes object for us
	// Since FoT doesn't keep any pointers of it around,
	// we can use our own address space.
	std::vector<char> temp_attribute_table(ATTRIBUTES_SIZE, 0);
	auto fxn = (void(__thiscall*)(void*))FXN_ATTRIBUTES_CONSTRUCTOR;
	fxn(temp_attribute_table.data());

	if (permanent)
		temp_attribute_table[4] = 0;

	for (auto entry : alters)
	{
		char* ptr = (char*)temp_attribute_table.data();
		ptr += entry.first.offset;
		memcpy(ptr, &entry.second, entry.first.size);
	}

	// Now we need to call the routine that applies the bonus
	auto fxn2 = (void(__thiscall*)(void*, void*, uint32_t, float))FXN_ACTOR_REMOVEBUFF;
	fxn2(entity, temp_attribute_table.data(), 0, 1.0);

}

int l_actor_removebonus(lua_State* l)
{
	Actor e(LuaHelper::GetEntityId(l));

	bool perm = lua_toboolean(l, 3);

	Actor::AlterTable at;

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

		Actor::AlterTableLocation loc;
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

	e.RemoveBonus(at,perm);
	return 0;

}

int l_actor_equipitem(lua_State* l)
{
	std::shared_ptr<Actor> e = std::dynamic_pointer_cast<Actor>(Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1)));
	auto i = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 2));
	int32_t slot = (int32_t)lua_tointeger(l, 3);;

	auto result = e->EquipItem(i, slot);

	result.second->MakeLuaObject(l);
	
	if (result.first.status != 0)
	{
		lua_pushstring(l, Helpers::WcharToUTF8(result.first.errstring).c_str());
		return 2;
	}
	return 1;

}

int l_actor_unequipitem(lua_State* l)
{
	std::shared_ptr<Actor> e = std::dynamic_pointer_cast<Actor>(Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1)));
	int32_t slot = (int32_t)lua_tointeger(l, 2);;

	auto result = e->UnequipItem(slot);

	result.second->MakeLuaObject(l);

	if (result.first.status != 0)
	{
		lua_pushstring(l, Helpers::WcharToUTF8(result.first.errstring).c_str());
		return 2;
	}
	return 1;
}

int l_actor_getequippeditem(lua_State* l)
{
	std::shared_ptr<Actor> e = std::dynamic_pointer_cast<Actor>(Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1)));
	int32_t slot = (int32_t)lua_tointeger(l, 2);;
	e->GetEquippedItem(slot)->MakeLuaObject(l);
	return 1;
}

void Actor::SetField(lua_State* l, std::string const& name)
{
	auto iter = offsets.find(name);
	if (iter == offsets.end())
	{
		return;
	}

	uint32_t base = (uint32_t)GetEntityPointer();
	switch (iter->second.type)
	{
	case FieldType::FLOATVECTOR:
	{
		float x = LuaHelper::GetTableFloat(l, 3, "x");
		*((float*)(base + iter->second.offset)) = x;
		x = LuaHelper::GetTableFloat(l, 3, "y");
		*((float*)(base + iter->second.offset + 4)) = x;
		x = LuaHelper::GetTableFloat(l, 3, "z");
		*((float*)(base + iter->second.offset + 8)) = x;
		break;
	}
	case FieldType::BOOLEAN:
		*((char*)(base + iter->second.offset)) = (lua_toboolean(l, 3)) ? 1 : 0;
		break;
	case FieldType::INTEGER:
		*((int32_t*)(base + iter->second.offset)) = (int32_t)lua_tointeger(l, 3);
		break;
	case FieldType::FLOAT:
		*((float*)(base + iter->second.offset)) = (float)lua_tonumber(l, 3);
		break;
	case FieldType::WCHAR_STRING:
		// This seems like a real bad idea
		// TODO: Check to expire old string and replace with new FOT-allocated one
		break;
	}

}
void Actor::GetField(lua_State* l, std::string const& name)
{
	auto iter = offsets.find(name);
	if (iter == offsets.end())
	{
		lua_pushnil(l);
		return;
	}

	uint32_t base = (uint32_t)GetEntityPointer();
	switch (iter->second.type)
	{
	case FieldType::FLOATVECTOR:
		lua_newtable(l);
		lua_pushnumber(l, *((float*)(base + iter->second.offset)));
		lua_setfield(l, -2, "x");
		lua_pushnumber(l, *((float*)(base + iter->second.offset + 4)));
		lua_setfield(l, -2, "y");
		lua_pushnumber(l, *((float*)(base + iter->second.offset + 8)));
		lua_setfield(l, -2, "z");
		break;
	case FieldType::BOOLEAN:
		lua_pushboolean(l, *((char*)(base + iter->second.offset)));
		break;
	case FieldType::INTEGER:
		lua_pushinteger(l, *((int32_t*)(base + iter->second.offset)));
		break;
	case FieldType::FLOAT:
		lua_pushnumber(l, *((float*)(base + iter->second.offset)));
		break;
	case FieldType::WCHAR_STRING:
		std::string fieldstr = Helpers::WcharToUTF8(*((wchar_t**)(base + iter->second.offset)));
		lua_pushstring(l, fieldstr.c_str());
		break;
	}

}

std::string Actor::GetFieldString(std::string const& name)
{
	auto iter = offsets.find(name);
	uint32_t base = (uint32_t)GetEntityPointer();
	return Helpers::WcharToUTF8(*((wchar_t**)(base + iter->second.offset)));
}



int Actor::GetTeamReaction(Entity & tgt)
{
	auto fxn = (int(__thiscall *)(void*,uint32_t))0x5e6790;
	return (*fxn)(GetEntityPointer(), tgt.GetID());
}

std::pair<InventoryActionResult, std::shared_ptr<Entity>> Actor::EquipItem(std::shared_ptr<Entity> item, int slot)
{
	auto fxn = (void* (__thiscall*)(void*, InventoryActionResult*, EntityID, EntityID*, int32_t, int32_t))(Entity::GetVtableFxn(VTABLE_EQUIP_ITEM));
	InventoryActionResult res;
	EntityID id = item->GetID();

	fxn(GetEntityPointer(), &res, id, &id, slot, 0);

	return std::pair < InventoryActionResult, std::shared_ptr<Entity>>(res, Entity::GetEntityByID(id));
}

std::pair<InventoryActionResult, std::shared_ptr<Entity>> Actor::UnequipItem(int slot)
{
	auto fxn = (void* (__thiscall*)(void*, InventoryActionResult*, int32_t, EntityID*))(Entity::GetVtableFxn(VTABLE_UNEQUIP_SLOT));
	InventoryActionResult res;
	EntityID id;
	fxn(GetEntityPointer(), &res, slot, &id);

	return std::pair < InventoryActionResult, std::shared_ptr<Entity>>(res, Entity::GetEntityByID(id));
}

bool Actor::TestFriendlyCrouched(Actor& tgt)
{
	// check if target is crouched or lower
	if (tgt.GetFieldString("posture") != "Stand")
	{
		// check if target is friendly
		if (GetTeamReaction(tgt) > 0)
		{
			// check if target is close enough
			Vector3 dir = tgt.GetLocation() - GetLocation();
			float dist = dir.distance();

			// game engine subtracts the sum of bounding box lengths
			// times 0.25 from the distance
			dist -= (GetBoundingBoxSum() + tgt.GetBoundingBoxSum())*0.25f;
			static const float sqrt6 = (float)sqrt(6.0);
			if (dist <= sqrt6)
			{
				return true;
			}
		}
		
	}
	return false;
}

std::shared_ptr<Entity> Actor::GetEquippedItem(int slot)
{
	if (slot < 0 || slot > 2)
		return Entity::GetEntityByID(0);
	return Entity::GetEntityByID(GetStruct()->slotequip[slot]);
}

void Actor::SwapFix()
{
	auto mystruct = GetStruct();

	EntityID temp = mystruct->slotequip[0];
	mystruct->slotequip[0] = mystruct->slotequip[1];
	mystruct->slotequip[1] = temp;
	mystruct->handselected = 1 - mystruct->handselected;

	auto fxn = (void(__thiscall*)(void*, int))(Entity::GetVtableFxn(VTABLE_SWITCH_HAND));
	fxn(GetEntityPointer(), 1 - mystruct->handselected);

}

Actor::ActorStructType* Actor::GetStruct()
{
	return (ActorStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_ACTOR_STRUCT);
}
