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
#include <memory>
#include "FOTString.h"
#include "ZoneDefinition.h"
#include "CombatMessage.h"
#include "AttributesTable.h"
#include "ActorStatus.h"
#include "Weapon.h"
#include "Inventory.h"

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

	// *************************************************************
	// VTABLE CALL/HOOK CONVERSIONS
	// *************************************************************

	template<typename PARAM>
	static PARAM Retrieve(lua_State* l, int index);
	
	template<typename PARAM>
	static PARAM RetrieveFromTable(lua_State* l, int index, std::string const& field)
	{
		lua_getfield(l, index, field.c_str());
		PARAM ret = LuaHelper::Retrieve<PARAM>(l, -1);
		lua_pop(l, 1);
		return ret;
	}

	template<>
	static int Retrieve<int>(lua_State* l, int index)
	{
		if (lua_isinteger(l, index))
			return static_cast<int>(lua_tointeger(l, index));
		return 0;
	}
	template<>
	static int64_t Retrieve<int64_t>(lua_State* l, int index)
	{
		if (lua_isinteger(l, index))
			return lua_tointeger(l, index);
		return 0;
	}
	template<>
	static uint8_t Retrieve<uint8_t>(lua_State* l, int index)
	{
		if (lua_isinteger(l, index))
			return static_cast<uint8_t>(lua_tointeger(l, index));
		return 0;
	}
	template<>
	static float Retrieve<float>(lua_State* l, int index)
	{
		if (lua_isnumber(l, index))
			return static_cast<float>(lua_tonumber(l, index));
		return 0;
	}
	template<>
	static bool Retrieve<bool>(lua_State* l, int index)
	{
		if (lua_isboolean(l, index))
			return static_cast<bool>(lua_toboolean(l, index));
		return 0;
	}
	template<>
	static EntityID Retrieve<EntityID>(lua_State* l, int index)
	{
		if (lua_istable(l, index))
		{
			return LuaHelper::GetEntityId(l, index);
		}
		return EntityID();
	}
	template<>
	static EntityID* Retrieve<EntityID*>(lua_State* l, int index)
	{
		if (lua_istable(l, index))
		{
			return new EntityID(LuaHelper::GetEntityId(l, index));
		}
		return new EntityID();
	}

	template<>
	static Vector3* Retrieve<Vector3*>(lua_State* l, int index)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		if (lua_istable(l, index))
		{
			x = LuaHelper::GetTableFloat(l, index, "x");
			y = LuaHelper::GetTableFloat(l, index, "y");
			z = LuaHelper::GetTableFloat(l, index, "z");
		}
		return new Vector3(x, y, z);

	}

	template<>
	static wchar_t** Retrieve<wchar_t**>(lua_State* l, int index)
	{
		if (lua_isstring(l, index))
		{
			FOTString fstr(std::string(lua_tostring(l, index)));
			fstr.incref();
			wchar_t** ret = new wchar_t*;
			*ret = fstr.getraw();
			return ret;
		}
		FOTString fstr("");
		wchar_t** ret = new wchar_t*;
		*ret = (wchar_t*)0x8af7e4;
		return ret;
	}
	template<>
	static wchar_t* Retrieve<wchar_t*>(lua_State* l, int index)
	{
		// If we pass wchar_t* to a function, we expect the callee to
		// decrement the refcount of the string. So we need to double-
		// increment the refcount (FOTString destructor will reduce it
		// by one here, and Release will reduce it again).
		if (lua_isstring(l, index))
		{
			FOTString fstr(std::string(lua_tostring(l, index)));
			fstr.incref();
			fstr.incref();
			return fstr.getraw();
		}
		FOTString fstr("");
		fstr.incref();
		fstr.incref();
		return fstr.getraw();
	}

	template<>
	static Vector3 Retrieve<Vector3>(lua_State* l, int index)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		if (lua_istable(l, index))
		{
			x = LuaHelper::GetTableFloat(l, index, "x");
			y = LuaHelper::GetTableFloat(l, index, "y");
			z = LuaHelper::GetTableFloat(l, index, "z");
		}
		return Vector3(x, y, z);
	}
	template<>
	static RGBColor Retrieve<RGBColor>(lua_State* l, int index)
	{
		float r = 0;
		float g = 0;
		float b = 0;
		if (lua_istable(l, index))
		{
			r = LuaHelper::GetTableFloat(l, index, "r");
			g = LuaHelper::GetTableFloat(l, index, "g");
			b = LuaHelper::GetTableFloat(l, index, "b");
		}
		return RGBColor(r, g, b);
	}
	template<>
	static ZoneDefinition* Retrieve<ZoneDefinition*>(lua_State* l, int index)
	{
		ZoneDefinition* zd = new ZoneDefinition();
		if (lua_istable(l, index))
		{
			zd->bound_top = RetrieveFromTable<Vector3>(l, index, "bound_top");
			zd->bound_bottom = RetrieveFromTable<Vector3>(l, index, "bound_bottom");
			zd->zonecolor = RetrieveFromTable<RGBColor>(l, index, "zonecolor");
			zd->zonename = RetrieveFromTable<wchar_t*>(l, index, "zonename");
			zd->soundzone = GetTableBool(l, index, "soundzone");
			zd->unk = GetTableBool(l, index, "unk");
		}
		return zd;
	}
	template<>
	static ZoneDefinition Retrieve<ZoneDefinition>(lua_State* l, int index)
	{
		ZoneDefinition zd;
		if (lua_istable(l, index))
		{
			zd.bound_top = RetrieveFromTable<Vector3>(l, index, "bound_top");
			zd.bound_bottom = RetrieveFromTable<Vector3>(l, index, "bound_bottom");
			zd.zonecolor = RetrieveFromTable<RGBColor>(l, index, "zonecolor");
			zd.zonename = RetrieveFromTable<wchar_t*>(l, index, "zonename");
			zd.soundzone = GetTableBool(l, index, "soundzone");
			zd.unk = GetTableBool(l, index, "unk");
		}
		return zd;
	}

	template<>
	static AttributesTable* Retrieve<AttributesTable*>(lua_State* l, int index)
	{
		// Create and init attributes table
		AttributesTable* ret = new AttributesTable();
		auto fxn = (void(__thiscall*)(void*))0x608d30;
		fxn(ret);

		// Now process all fields in the attributes table
		if (!lua_istable(l, index))
		{
			// someone did something wrong, bail out
			return ret;
		}
		lua_pushvalue(l, index);	// copy reference for the table
		lua_pushnil(l);
		while (lua_next(l, -2))
		{
			std::string key = lua_tostring(l, -2);
			int32_t value = (int32_t)lua_tointeger(l, -1);

			char* ptr = (char*)ret + AttributesTable::GetOffsetByName(key);
			if (std::string(key, 0, 4) == "tag_" || AttributesTable::GetGroupByName(key) == "otraits")
			{
				memcpy(ptr, &value, 1);
			}
			else
			{
				memcpy(ptr, &value, 4);
			}

			// pop the value, the key stays for next loop
			lua_pop(l, 1);

		}
		lua_pop(l, 1);	// pop the duplicate table

		return ret;
	}

	template<>
	static ActorStatus* Retrieve<ActorStatus*>(lua_State* l, int index)
	{
		ActorStatus* ret = new ActorStatus();
		ret->vtable = ActorStatus::VTABLE;
		if (lua_istable(l, index))
		{
			ret->hp = GetTableInteger(l, index, "hp");
			ret->bandaged = GetTableInteger(l, index, "bandaged");
			ret->ap = GetTableFloat(l, index, "ap");
			ret->radpoints = GetTableInteger(l, index, "radpoints");
			ret->radlevel = GetTableInteger(l, index, "radlevel");
			ret->poisonpoints = GetTableInteger(l, index, "poisonpoints");
			ret->poisonlevel = GetTableInteger(l, index, "poisonlevel");
			ret->overdosepoints = GetTableInteger(l, index, "overdosepoints");
			ret->goneuplevel = GetTableBool(l, index, "goneuplevel");

		}
		return ret;
	}

	template<>
	static CombatMessage* Retrieve<CombatMessage*>(lua_State* l, int index)
	{
		CombatMessage* ret = new CombatMessage();
		ret->vtable = CombatMessage::VTABLE;

		if (lua_istable(l, index))
		{
			ret->attacker = RetrieveFromTable<EntityID>(l, index, "attacker");
			ret->target = RetrieveFromTable<EntityID>(l, index, "target");
			ret->weapon = RetrieveFromTable<EntityID>(l, index, "weapon");
			ret->unk1 = GetTableInteger(l, index, "unk1");
			ret->attacker_pos = RetrieveFromTable<Vector3>(l, index, "attacker_pos");
			ret->target_pos = RetrieveFromTable<Vector3>(l, index, "target_pos");
			ret->damage = GetTableInteger(l, index, "damage");
			ret->critflags = GetTableInteger(l, index, "critflags");
			ret->unk3 = GetTableInteger(l, index, "unk3");
			ret->unk4 = GetTableInteger(l, index, "unk4");
			ret->aimedlocation = GetTableInteger(l, index, "aimedlocation");
			ret->numshots = GetTableInteger(l, index, "numshots");
			ret->unk5 = GetTableInteger(l, index, "unk5");
			ret->arraystart =0;
			ret->arrayend = 0;
			ret->arrayptr = 0;

		}
		return ret;
	}
	template<>
	static InventoryActionResult Retrieve<InventoryActionResult>(lua_State* l, int index)
	{
		InventoryActionResult ret;
		if (lua_istable(l, index))
		{
			ret.status = GetTableInteger(l, index, "status");
			ret.unk_entity_1 = RetrieveFromTable<EntityID>(l, index, "unk_entity_1");
			ret.unk2 = GetTableInteger(l, index, "unk2");
			ret.unk3 = GetTableInteger(l, index, "unk3");

			lua_getfield(l, index, "errstring");
			FOTString str(Retrieve<wchar_t*>(l, -1));
			ret.errstring = str.getraw();
			lua_pop(l, 1);

			ret.unk_entity_4 = RetrieveFromTable<EntityID>(l, index, "unk_entity_4");
			ret.unk_5 = GetTableInteger(l, index, "unk_5");
		}
		return ret;

	}

	template<>
	static AnimOffset* Retrieve<AnimOffset*>(lua_State* l, int index)
	{
		AnimOffset* ret = new AnimOffset();
		if (lua_istable(l, index))
		{
			ret->v[0] = LuaHelper::GetTableInteger(l, index, "x");
			ret->v[1] = LuaHelper::GetTableInteger(l, index, "y");
			ret->v[2] = LuaHelper::GetTableInteger(l, index, "z");
		}
		return ret;
	}

	template<>
	static Inventory::InventoryStructType* Retrieve<Inventory::InventoryStructType*>(lua_State* l, int index)
	{
		Inventory::InventoryStructType* ret = new Inventory::InventoryStructType;
		ret->unk1 = 0;
		ret->numitems = lua_rawlen(l, index);
		Inventory::InventoryFOTLinkedList* head = new Inventory::InventoryFOTLinkedList;
		ret->itemlist = head;
		Inventory::InventoryFOTLinkedList* prev = head;
		Inventory::InventoryFOTLinkedList* curr;

		for (int i = 0; i < ret->numitems; i++)
		{
			curr = new Inventory::InventoryFOTLinkedList;
			curr->prev = prev;
			prev->next = curr;

			lua_rawgeti(l, index, i + 1);

			curr->entity_id = LuaHelper::GetEntityId(l, -1);
			lua_pop(l, 1);

			prev = curr;

		}
		prev->next = head;
		head->prev = prev;
		return ret;
	}

	template<>
	static Inventory::InventoryStructType Retrieve<Inventory::InventoryStructType>(lua_State* l, int index)
	{
		const size_t FXN_FOTHEAPALLOC = 0x6c4dd0;
		char* (*FOTHeapAlloc)(size_t) = (char* (*)(size_t))FXN_FOTHEAPALLOC;
		
		Inventory::InventoryStructType ret;
		ret.unk1 = 0;
		ret.numitems = lua_rawlen(l, index);
		Inventory::InventoryFOTLinkedList* head = (Inventory::InventoryFOTLinkedList*)FOTHeapAlloc(sizeof(void*));
		ret.itemlist = head;
		Inventory::InventoryFOTLinkedList* prev = head;
		Inventory::InventoryFOTLinkedList* curr;

		for (int i = 0; i < ret.numitems; i++)
		{
			curr = (Inventory::InventoryFOTLinkedList*)FOTHeapAlloc(sizeof(void*));
			curr->prev = prev;
			prev->next = curr;

			lua_rawgeti(l, index, i + 1);

			curr->entity_id = LuaHelper::GetEntityId(l, -1);
			lua_pop(l, 1);

			prev = curr;

		}
		prev->next = head;
		head->prev = prev;
		return ret;
	}

	template<>
	static EntityIDVector Retrieve<EntityIDVector>(lua_State* l, int index)
	{
		EntityIDVector ret;
		ret.unk = 0;
		int items = lua_rawlen(l, index);
		const size_t FXN_FOTHEAPALLOC = 0x6c4dd0;
		char* (*FOTHeapAlloc)(size_t) = (char* (*)(size_t))FXN_FOTHEAPALLOC;
		ret.start = (EntityID*)FOTHeapAlloc(items * sizeof(EntityID));
		ret.end = ret.start + items;
		ret.cap = ret.end;
		for (int i = 0; i < items; i++)
		{
			lua_rawgeti(l, index, i + 1);
			ret.start[i] = Retrieve<EntityID>(l, -1);
			lua_pop(l, 1);
		}
		return ret;
	}
	template<>
	static ChanceToHit Retrieve<ChanceToHit>(lua_State* l, int index)
	{
		ChanceToHit ret;
		if (lua_istable(l, index))
		{
			ret.hit_chance = LuaHelper::GetTableInteger(l, index, "hit_chance");
			ret.raw_chance = LuaHelper::GetTableInteger(l, index, "raw_chance");
			ret.ineligible_flags = LuaHelper::GetTableInteger(l, index, "ineligible_flags");
			ret.unk2 = LuaHelper::GetTableInteger(l, index, "unk2");
		}
		return ret;
	}


	template<typename PARAM>
	static void Return(lua_State* l, PARAM const& param);

	template<>
	static void Return<AttributesTable*>(lua_State* l, AttributesTable* const& param)
	{
		lua_newtable(l);

		int ct;
		size_t offset = AttributesTable::OFFSET_STATS;
		for (ct = 0; ct < 7; ct++)
		{
			int32_t val = *(int32_t*)((char*)param + offset);
			if ( val != 0)
			{
				lua_pushinteger(l, val);
				lua_setfield(l, -2, AttributesTable::GetNameByOffset(offset).c_str());
			}
			offset += 4;
		}
		offset = AttributesTable::OFFSET_TRAITS;
		for (ct = 0; ct < 11; ct++)
		{
			int32_t val = *(int32_t*)((char*)param + offset);
			if (val != 0)
			{
				lua_pushinteger(l, val);
				lua_setfield(l, -2, AttributesTable::GetNameByOffset(offset).c_str());
			}
			offset += 4;
		}
		offset = AttributesTable::OFFSET_DERIVED;
		for (ct = 0; ct <26; ct++)
		{
			int32_t val = *(int32_t*)((char*)param + offset);
			if (val != 0)
			{
				lua_pushinteger(l, val);
				lua_setfield(l, -2, AttributesTable::GetNameByOffset(offset).c_str());
			}
			offset += 4;
		}
		offset = AttributesTable::OFFSET_SKILLS;
		for (ct = 0; ct < 18; ct++)
		{
			int32_t val = *(int32_t*)((char*)param + offset);
			if (val != 0)
			{
				lua_pushinteger(l, val);
				lua_setfield(l, -2, AttributesTable::GetNameByOffset(offset).c_str());
			}
			offset += 4;
		}
		for (ct = 0; ct < 18; ct++)
		{
			uint8_t val = *(uint8_t*)((char*)param + offset);
			if (val != 0)
			{
				lua_pushinteger(l, val);
				lua_setfield(l, -2, AttributesTable::GetNameByOffset(offset).c_str());
			}
			offset += 1;
		}
		offset = AttributesTable::OFFSET_OTRAITS;
		for (ct = 0; ct < 31; ct++)
		{
			uint8_t val = *(uint8_t*)((char*)param + offset);
			if (val != 0)
			{
				lua_pushinteger(l, val);
				lua_setfield(l, -2, AttributesTable::GetNameByOffset(offset).c_str());
			}
			offset += 1;
		}
		offset = AttributesTable::OFFSET_PERKS;
		for (ct = 0; ct < 111; ct++)
		{
			int32_t val = *(int32_t*)((char*)param + offset);
			if (val != 0)
			{
				lua_pushinteger(l, val);
				lua_setfield(l, -2, AttributesTable::GetNameByOffset(offset).c_str());
			}
			offset += 4;
		}
		offset = AttributesTable::OFFSET_CHEMS;
		for (ct = 0; ct < 10; ct++)
		{
			int32_t val = *(int32_t*)((char*)param + offset);
			if (val != 0)
			{
				lua_pushinteger(l, val);
				lua_setfield(l, -2, AttributesTable::GetNameByOffset(offset).c_str());
			}
			offset += 4;
		}

	}
	template<>
	static void Return<int>(lua_State* l, int const& param)
	{
		lua_pushinteger(l, param);
	}
	template<>
	static void Return<int64_t>(lua_State* l, int64_t const& param)
	{
		lua_pushinteger(l, param);
	}
	template<>
	static void Return<float>(lua_State* l, float const& param)
	{
		lua_pushnumber(l, param);
	}
	template<>
	static void Return<bool>(lua_State* l, bool const& param)
	{
		lua_pushboolean(l, param);
	}
	template<>
	static void Return<uint8_t>(lua_State* l, uint8_t const& param)
	{
		lua_pushinteger(l, param);
	}
	template<>
	static void Return<EntityID>(lua_State* l, EntityID const& param)
	{
		std::shared_ptr<Entity> e = Entity::GetEntityByID(param);
		e->MakeLuaObject(l);
	}
	template<>
	static void Return<EntityID*>(lua_State* l, EntityID* const& param)
	{
		std::shared_ptr<Entity> e = Entity::GetEntityByID(*param);
		e->MakeLuaObject(l);
	}
	template<>
	static void Return<Inventory::InventoryStructType*>(lua_State* l, Inventory::InventoryStructType* const& param)
	{
		lua_newtable(l);
		Inventory::InventoryFOTLinkedList* head = param->itemlist;
		Inventory::InventoryFOTLinkedList* curr = head->next;
		Inventory::InventoryFOTLinkedList* temp;
		int ctr = 1;
		while (curr != head)
		{
			Entity::GetEntityByID(curr->entity_id)->MakeLuaObject(l);
			lua_rawseti(l, -2, ctr);
			temp = curr->next;
			delete curr;
			curr = temp;
			ctr++;
		}
	}

	template<>
	static void Return<wchar_t*>(lua_State* l,wchar_t* const& param)
	{
		FOTString fstr(param);
		lua_pushstring(l, fstr.get().c_str());
	}
	template<>
	static void Return<wchar_t**>(lua_State* l, wchar_t** const& param)
	{
		lua_pushstring(l, Helpers::WcharToUTF8(*param).c_str());
	}
	template<>
	static void Return<FOTString>(lua_State* l, FOTString const& param)
	{
		lua_pushstring(l, param.get().c_str());
	}

	template<>
	static void Return<Vector3>(lua_State* l, Vector3 const& param)
	{
		lua_newtable(l);
		lua_pushnumber(l, param.v[0]);
		lua_setfield(l, -2, "x");
		lua_pushnumber(l, param.v[1]);
		lua_setfield(l, -2, "y");
		lua_pushnumber(l, param.v[2]);
		lua_setfield(l, -2, "z");
	}
	template<>
	static void Return<AnimOffset*>(lua_State* l, AnimOffset* const& param)
	{
		lua_newtable(l);
		lua_pushinteger(l, param->v[0]);
		lua_setfield(l, -2, "x");
		lua_pushinteger(l, param->v[1]);
		lua_setfield(l, -2, "y");
		lua_pushinteger(l, param->v[2]);
		lua_setfield(l, -2, "z");
	}
	template<>
	static void Return<Vector3*>(lua_State* l, Vector3* const& param)
	{
		lua_newtable(l);
		lua_pushnumber(l, param->v[0]);
		lua_setfield(l, -2, "x");
		lua_pushnumber(l, param->v[1]);
		lua_setfield(l, -2, "y");
		lua_pushnumber(l, param->v[2]);
		lua_setfield(l, -2, "z");
	}
	template<>
	static void Return< RGBColor>(lua_State* l, RGBColor const& param)
	{
		lua_newtable(l);
		lua_pushnumber(l, param.v[0]);
		lua_setfield(l, -2, "r");
		lua_pushnumber(l, param.v[1]);
		lua_setfield(l, -2, "g");
		lua_pushnumber(l, param.v[2]);
		lua_setfield(l, -2, "b");
	}

	template<>
	static void Return<CombatMessage*>(lua_State* l, CombatMessage* const& param)
	{
		lua_newtable(l);
		Entity::GetEntityByID(param->attacker)->MakeLuaObject(l);
		lua_setfield(l, -2, "attacker");
		Entity::GetEntityByID(param->target)->MakeLuaObject(l);
		lua_setfield(l, -2, "target");
		Entity::GetEntityByID(param->weapon)->MakeLuaObject(l);
		lua_setfield(l, -2, "weapon");
		lua_pushinteger(l, param->unk1);
		lua_setfield(l, -2, "unk1");
		Return<Vector3>(l, param->attacker_pos);
		lua_setfield(l, -2, "attacker_pos");
		Return<Vector3>(l, param->target_pos);
		lua_setfield(l, -2, "target_pos");
		lua_pushinteger(l, param->damage);
		lua_setfield(l, -2, "damage");
		lua_pushinteger(l, param->critflags);
		lua_setfield(l, -2, "critflags");
		lua_pushinteger(l, param->unk3);
		lua_setfield(l, -2, "unk3");
		lua_pushinteger(l, param->unk4);
		lua_setfield(l, -2, "unk4");
		lua_pushinteger(l, param->aimedlocation);
		lua_setfield(l, -2, "aimedlocation");
		lua_pushinteger(l, param->numshots);
		lua_setfield(l, -2, "numshots");
		lua_pushinteger(l, param->unk5);
		lua_setfield(l, -2, "unk5");

	}
	template<>
	static void Return< InventoryActionResult>(lua_State* l, InventoryActionResult const& param)
	{
		lua_newtable(l);
		lua_pushinteger(l, param.status);
		lua_setfield(l, -2, "status");
		Entity::GetEntityByID(param.unk_entity_1)->MakeLuaObject(l);
		lua_setfield(l, -2, "unk_entity_1");
		lua_pushinteger(l, param.unk2);
		lua_setfield(l, -2, "unk2");
		lua_pushinteger(l, param.unk3);
		lua_setfield(l, -2, "unk3");
		FOTString fstr(param.errstring);
		lua_pushstring(l, fstr.get().c_str());
		lua_setfield(l, -2, "errstring");
		Entity::GetEntityByID(param.unk_entity_4)->MakeLuaObject(l);
		lua_setfield(l, -2, "unk_entity_4");
		lua_pushinteger(l, param.unk_5);
		lua_setfield(l, -2, "unk_5");
	}
	template<>
	static void Return<ZoneDefinition>(lua_State* l, ZoneDefinition const& param)
	{
		lua_newtable(l);
		
		Return<Vector3>(l, param.bound_top);
		lua_setfield(l, -2, "bound_top");
		Return<Vector3>(l, param.bound_bottom);
		lua_setfield(l, -2, "bound_bottom");
		Return<RGBColor>(l, param.zonecolor);
		lua_setfield(l, -2, "zonecolor");
		Return<wchar_t*>(l, param.zonename);
		lua_setfield(l, -2, "zonename");
		Return<bool>(l, param.soundzone);
		lua_setfield(l, -2, "soundzone");
		Return<bool>(l, param.unk);
		lua_setfield(l, -2, "unk");
	}
	template<>
	static void Return<ZoneDefinition*>(lua_State* l, ZoneDefinition* const& param)
	{
		lua_newtable(l);

		Return<Vector3>(l, param->bound_top);
		lua_setfield(l, -2, "bound_top");
		Return<Vector3>(l, param->bound_bottom);
		lua_setfield(l, -2, "bound_bottom");
		Return<RGBColor>(l, param->zonecolor);
		lua_setfield(l, -2, "zonecolor");
		Return<wchar_t*>(l, param->zonename);
		lua_setfield(l, -2, "zonename");
		Return<bool>(l, param->soundzone);
		lua_setfield(l, -2, "soundzone");
		Return<bool>(l, param->unk);
		lua_setfield(l, -2, "unk");
	}
	template<>
	static void Return<ChanceToHit>(lua_State* l, ChanceToHit const& param)
	{
		lua_newtable(l);
		lua_pushinteger(l, param.hit_chance);
		lua_setfield(l, -2, "hit_chance");
		lua_pushinteger(l, param.raw_chance);
		lua_setfield(l, -2, "raw_chance");
		lua_pushinteger(l, param.ineligible_flags);
		lua_setfield(l, -2, "ineligible_flags");
		lua_pushinteger(l, param.unk2);
		lua_setfield(l, -2, "unk2");
	}

	template<>
	static void Return<EntityIDVector>(lua_State* l, EntityIDVector const& param)
	{
		lua_newtable(l);
		EntityID* id = param.start;
		int ctr = 1;
		while (id != nullptr && id <= param.end)
		{
			Entity::GetEntityByID(*id)->MakeLuaObject(l);
			lua_rawseti(l, -2, ctr);
			id++; 
			ctr++;
		}
	}

	template<>
	static void Return<ActorStatus*>(lua_State* l, ActorStatus* const& param)
	{
		lua_newtable(l);
		lua_pushinteger(l, param->hp);
		lua_setfield(l, -2, "hp");
		lua_pushinteger(l, param->bandaged);
		lua_setfield(l, -2, "bandaged");
		lua_pushnumber(l, param->ap);
		lua_setfield(l, -2, "ap");
		lua_pushinteger(l, param->radpoints);
		lua_setfield(l, -2, "radpoints");
		lua_pushinteger(l, param->radlevel);
		lua_setfield(l, -2, "radlevel");
		lua_pushinteger(l, param->poisonpoints);
		lua_setfield(l, -2, "poisonpoints");
		lua_pushinteger(l, param->poisonlevel);
		lua_setfield(l, -2, "poisonlevel");
		lua_pushinteger(l, param->overdosepoints);
		lua_setfield(l, -2, "overdosepoints");
		lua_pushboolean(l, param->goneuplevel);
		lua_setfield(l, -2, "goneuplevel");
	}

	template<>
	static void Return<Weapon::WeaponModeStructType*>(lua_State* l, Weapon::WeaponModeStructType* const& param)
	{
		lua_newtable(l);
		lua_pushboolean(l, param->enabled);
		lua_setfield(l, -2, "enabled");
		lua_pushstring(l, Helpers::WcharToUTF8(param->modename).c_str());
		lua_setfield(l, -2, "modename");
		lua_pushstring(l, Helpers::WcharToUTF8(param->weaponskill).c_str());
		lua_setfield(l, -2, "weaponskill");
		lua_pushstring(l, Helpers::WcharToUTF8(param->animation).c_str());
		lua_setfield(l, -2, "animation");
		lua_pushstring(l, Helpers::WcharToUTF8(param->rangetype).c_str());
		lua_setfield(l, -2, "rangetype");
		lua_pushnumber(l, param->maxrange);
		lua_setfield(l, -2, "maxrange");
		lua_pushstring(l, Helpers::WcharToUTF8(param->spreadtype).c_str());
		lua_setfield(l, -2, "spreadtype");
		lua_pushinteger(l, param->accuracybonus);
		lua_setfield(l, -2, "accuracybonus");
		lua_pushnumber(l, param->damagemult);
		lua_setfield(l, -2, "damagemult");
		lua_pushinteger(l, param->ammousage);
		lua_setfield(l, -2, "ammousage");
		lua_pushstring(l, Helpers::WcharToUTF8(param->actioncosttype).c_str());
		lua_setfield(l, -2, "actioncosttype");
		lua_pushboolean(l, param->cancallshot);
		lua_setfield(l, -2, "cancallshot");
		lua_pushinteger(l, param->critbonus);
		lua_setfield(l, -2, "critbonus");
		lua_pushnumber(l, param->knockovermult);
		lua_setfield(l, -2, "knockovermult");
		lua_pushstring(l, Helpers::WcharToUTF8(param->impactfx).c_str());
		lua_setfield(l, -2, "impactfx");
		lua_pushstring(l, Helpers::WcharToUTF8(param->soundfx).c_str());
		lua_setfield(l, -2, "soundfx");
		lua_pushboolean(l, param->hasprojectile);
		lua_setfield(l, -2, "hasprojectile");
		lua_pushboolean(l, param->destroyonuse);
		lua_setfield(l, -2, "destroyonuse");
		lua_pushstring(l, Helpers::WcharToUTF8(param->lightfx).c_str());
		lua_setfield(l, -2, "lightfx");
	}

	template<>
	static void Return<Inventory::InventoryStructType>(lua_State* l, Inventory::InventoryStructType const& param)
	{
		lua_newtable(l);
		Inventory::InventoryFOTLinkedList* head = param.itemlist;
		Inventory::InventoryFOTLinkedList* curr = head->next;
		Inventory::InventoryFOTLinkedList* temp;
		auto FOTFreeFxn = (void(__cdecl *)(void*))0x6c4e50;
		int ctr = 1;
		while (curr != head)
		{
			Entity::GetEntityByID(curr->entity_id)->MakeLuaObject(l);
			lua_rawseti(l, -2, ctr);
			temp = curr->next;
			FOTFreeFxn(reinterpret_cast<void*>(curr));
			delete curr;
			curr = temp;
			ctr++;
		}
		FOTFreeFxn(reinterpret_cast<void*>(head));
	}

	template<typename T>
	static void Release(T a)
	{

	}

	template<>
	static void Release(wchar_t* a)
	{
		FOTString fstr(a);
	}
	template<>
	static void Release(wchar_t** a)
	{
		FOTString fstr(*a);
		delete a;
	}
	template<>
	static void Release(Vector3* a)
	{
		delete a;
	}
	template<>
	static void Release(EntityID* a)
	{
		delete a;
	}
	template<>
	static void Release(AttributesTable* a)
	{
		delete a;
	}
	static void Release(AnimOffset* a)
	{
		delete a;
	}
	template<>
	static void Release(CombatMessage* a)
	{
		delete a;
	}
	template<>
	static void Release(ActorStatus* a)
	{
		delete a;
	}

	template<>
	static void Release(ZoneDefinition* a)
	{
		// Retrieve for ZoneDefinition creates a wchar_t* string with an initial refcount of 3,
		// since it assumes the called function will reduce the refcount by 1, and with the
		// reductions in Retrieve and Release, we expect the count to go to 0.  However, for
		// ZoneDefinition, the called function doesn't decrement refcount for the bundled string.
		// So do an extra decrement here.
		FOTString str(a->zonename);
		str.decref();

		// Now delete the main ZoneDefinition pointer we created in Retrieve.
		delete a;
	}

	template<>
	static void Release(Inventory::InventoryStructType* a)
	{
		Inventory::InventoryFOTLinkedList* head = a->itemlist;
		Inventory::InventoryFOTLinkedList* curr = head->next;
		Inventory::InventoryFOTLinkedList* temp;
		while (curr != head)
		{
			temp = curr->next;
			delete curr;
			curr = temp;
		}
		delete head;
		delete a;
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

