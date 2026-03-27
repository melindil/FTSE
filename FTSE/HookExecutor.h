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

#include <fstream>
#include <Windows.h>

#include <vector>
#include <string>

#include "FOTPerkTable.h"

#include "lua.hpp"
#include "EntityVtable.h"
#include "ControllerCommandStruct.h"
#include "FTImprover.h"
#include "FOTLocale.h"

#include "ui/WFTSESpeech.h"

class Logger;
class WMainMenu;
class GenericPatcher;
class HookExecutor
{
public:
	HookExecutor(Logger* logger,std::string const& luaname);
	~HookExecutor();

	void TeamPlayerTrigger(void* entity);
	void IsRadiated(void* entity);
	void LongTickTrigger(void* entity);
	void SetVariableTrigger(void* SetVariableObj);
	void DefaultStyleHandler();
	void OnStart(std::shared_ptr<GenericPatcher> patcher);
	void ReplacePerk(FOTPerkTableEntry* newstat, int entry);
	void ReplacePerk(lua_State* l);
	void AddLocaleString(std::string const& key, std::string const& value, int dictionary);
	std::string GetLocaleString(std::string const& key, int dictionary);
	int MsecTimerHook(uint64_t msec, uint32_t scale, void* target);
	int AddBaseTime(void* target);
	void OnChanceToHitCalc(void* attacker, void* target, void* weapon, void* chance, wchar_t* loc);
	void OnChanceToHitCalc_SaveWeapon(void* weapon);
	uint32_t OnBurstAttack(void* cmsg, void* arraystart, void* arrayend);
	uint32_t OnConeAttack(void* cmsg, void* arraystart, void* arrayend);
	uint32_t OnSprayAttack(void* cmsg, void* arraystart, void* arrayend);
	uint32_t OnRadialAttack(void* cmsg, void* arraystart, void* arrayend);
	uint32_t OnAreaAttack(void* cmsg, void* arraystart, void* arrayend);
	int OnStraightAttack(void* cmsg);
	int OnProjectileAttack(uint32_t hit, void* cmsg);
	void OnLocaleLoad();
	int32_t OnChanceToCritical1(void* attacker, void* target, void* weapon, int32_t chance, wchar_t* loc);
	int32_t OnChanceToCritical2(void* cmsg, int32_t chance);
	int32_t OnCriticalEffect1(void* cmsg, int32_t roll);
	int32_t OnCriticalEffect2(void* cmsg, int32_t roll);
	void OnDamageCalcSaveHit(int32_t damage);
	void OnDamageCalc(void* cmsg);
	void OnInventoryAdd(void* receiver, void* item, int32_t quantity);
	void OnInventoryRemove(void* source, void* item, int32_t quantity);
	int8_t OnCheckItemAllowed(void* actor, void* item);
	void OnEquip(void* equipper, void* item, int slot);
	wchar_t* OnCheckEquip(void* equipper, void* item, int slot);
	wchar_t* OnCheckUnequip(void* equipper, void* item, int slot);
	void OnUnequip(void* equipper, void* item, int slot);
	void* SwapFix(void* swapper, void* returnstruct);
	void OnMissionLoad(wchar_t** filename);

	void MainMenuHook(WMainMenu* mainwindow);

	uint32_t ArmourSpriteHook(wchar_t** dest, wchar_t** sex, wchar_t** race, wchar_t** armour);
	uint32_t SoundSpriteHook(wchar_t** dest, void* ent);

	void InstallVtableHook(std::string const& classname, int idx);

	void EntityConstructHook(void* entity);
	void EntitySerializeHook(void* entity, void* stream);

	bool SpeechHook(EntityID player, EntityID npc, void* speechnode);

private:

	uint32_t MultiTargetAttack(void* cmsg, void* astart, void* aend, bool area = false);
	int32_t OnCriticalEffectImpl(void* cmsg, int32_t roll);
	void SetupVtableHookTemplates();

	void CallHookChain(lua_State* l);
	void CallHookChainRet2(lua_State* l);

	static const uint32_t DATA_PERK_TABLE = 0x8a4500;

	Logger* logger_;
	lua_State* lua_;

	FTImprover improver_;
	FOTLocale locale_;
	std::shared_ptr<GenericPatcher> patcher_;

	std::shared_ptr<EntityVtable> entity_vtable_;
	size_t VtableHookTemplates_[536];

	// Some hooks need a saved state
	void* savedweapon_;
	std::vector<int> saved_hits_;
	bool loaded_sav_;

	// Vtable hook templates
	void vtable_hook_template_noop(size_t hook_handle, Entity* ent)
	{
		logger_->Log("Unimplemented Vtable hook called - expect a crash soon!");
	}

	void vtable_hook_template_0(size_t hook_handle, void* entaddr)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle*sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
	template<typename RET>
	RET vtable_hook_template_0r(size_t hook_handle, void* entaddr)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		RET ret = LuaHelper::Retrieve<RET>(lua_, -1);
		lua_pop(lua_, 1);
		return ret;
	}

	template<typename PARAM1>
	void vtable_hook_template_1(size_t hook_handle, void* entaddr,PARAM1 param1)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
	template<typename PARAM1,typename RET>
	RET vtable_hook_template_1r(size_t hook_handle, void* entaddr, PARAM1 param1)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		RET ret = LuaHelper::Retrieve<RET>(lua_, -1);
		lua_settop(lua_, 0);
		return ret;

	}
	template<typename PARAM1>
	PARAM1* vtable_hook_template_1r1(size_t hook_handle, void* entaddr, PARAM1* param1)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -1);
		lua_settop(lua_, 0);
		return param1;

	}
	template<typename PARAM1,typename PARAM2>
	void vtable_hook_template_2(size_t hook_handle, void* entaddr, PARAM1 param1,PARAM2 param2)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 3);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
	template<typename PARAM1, typename PARAM2,typename RET>
	RET vtable_hook_template_2r(size_t hook_handle, void* entaddr, PARAM1 param1,PARAM2 param2)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 3);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		RET ret = LuaHelper::Retrieve<RET>(lua_, -1);
		lua_settop(lua_, 0);
		return ret;

	}
	template<typename PARAM1,typename PARAM2>
	PARAM1* vtable_hook_template_2r1(size_t hook_handle, void* entaddr, PARAM1* param1,PARAM2 param2)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 2);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -1);
		lua_settop(lua_, 0);
		return param1;

	}
	template<typename PARAM1, typename PARAM2,typename PARAM3>
	void vtable_hook_template_3(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2,PARAM3 param3)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 3);
		LuaHelper::Return<PARAM3>(lua_, param3);
		lua_rawseti(lua_, -2, 4);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename RET>
	RET vtable_hook_template_3r(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2, PARAM3 param3)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 3);
		LuaHelper::Return<PARAM3>(lua_, param3);
		lua_rawseti(lua_, -2, 4);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		RET ret = LuaHelper::Retrieve<RET>(lua_, -1);
		lua_settop(lua_, 0);
		return ret;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3>
	PARAM1* vtable_hook_template_3r1(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3 param3)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM3>(lua_, param3);
		lua_rawseti(lua_, -2, 3);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -1);
		lua_settop(lua_, 0);
		return param1;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3,typename PARAM4>
	void vtable_hook_template_4(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2, PARAM3 param3,PARAM4 param4)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 3);
		LuaHelper::Return<PARAM3>(lua_, param3);
		lua_rawseti(lua_, -2, 4);
		LuaHelper::Return<PARAM4>(lua_, param4);
		lua_rawseti(lua_, -2, 5);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
	PARAM1* vtable_hook_template_4r1(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3 param3, PARAM4 param4)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM3>(lua_, param3);
		lua_rawseti(lua_, -2, 3);
		LuaHelper::Return<PARAM4>(lua_, param4);
		lua_rawseti(lua_, -2, 4);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -1);
		lua_settop(lua_, 0);
		return param1;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4,typename PARAM5>
	void vtable_hook_template_5(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4,PARAM5 param5)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 3);
		LuaHelper::Return<PARAM3>(lua_, param3);
		lua_rawseti(lua_, -2, 4);
		LuaHelper::Return<PARAM4>(lua_, param4);
		lua_rawseti(lua_, -2, 5);
		LuaHelper::Return<PARAM5>(lua_, param5);
		lua_rawseti(lua_, -2, 6);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4, typename PARAM5,typename PARAM6>
	void vtable_hook_template_6(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM1>(lua_, param1);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 3);
		LuaHelper::Return<PARAM3>(lua_, param3);
		lua_rawseti(lua_, -2, 4);
		LuaHelper::Return<PARAM4>(lua_, param4);
		lua_rawseti(lua_, -2, 5);
		LuaHelper::Return<PARAM5>(lua_, param5);
		lua_rawseti(lua_, -2, 6);
		LuaHelper::Return<PARAM6>(lua_, param6);
		lua_rawseti(lua_, -2, 7);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
	template<typename PARAM1, typename PARAM2, typename PARAM3>
	PARAM1* vtable_hook_template_3r2(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3* param3)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 2);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChainRet2(lua_);
		if (lua_isnil(lua_, -1) && lua_isnil(lua_, -2))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -2);
		*param3 = LuaHelper::Retrieve<PARAM3>(lua_, -1);
		lua_settop(lua_, 0);
		return param1;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3,typename PARAM4>
	PARAM1* vtable_hook_template_4r2(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3* param3,PARAM4 param4)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM4>(lua_, param4);
		lua_rawseti(lua_, -2, 3);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChainRet2(lua_);
		if (lua_isnil(lua_, -1) && lua_isnil(lua_, -2))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -2);
		*param3 = LuaHelper::Retrieve<PARAM3>(lua_, -1);
		lua_settop(lua_, 0);
		return param1;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4,typename PARAM5>
	PARAM1* vtable_hook_template_5r2(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3* param3, PARAM4 param4,PARAM5 param5)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		LuaHelper::Return<PARAM2>(lua_, param2);
		lua_rawseti(lua_, -2, 2);
		LuaHelper::Return<PARAM4>(lua_, param4);
		lua_rawseti(lua_, -2, 3);
		LuaHelper::Return<PARAM5>(lua_, param5);
		lua_rawseti(lua_, -2, 4);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChainRet2(lua_);
		if (lua_isnil(lua_, -1) && lua_isnil(lua_, -2))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -2);
		*param3 = LuaHelper::Retrieve<PARAM3>(lua_, -1);
		lua_settop(lua_, 0);
		return param1;

	}

	void vtable_hook_template_X32(size_t hook_handle, void* entaddr, float* param1)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		lua_newtable(lua_);
		
		for (int i = 0; i < 12; i++)
		{
			lua_pushnumber(lua_, param1[i]);
			lua_rawseti(lua_, -2, i + 1);
		}
		lua_rawseti(lua_, -2, 2);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
	void vtable_hook_template_X510(size_t hook_handle, void* entaddr, ControllerCommandStruct* param1)
	{
		auto e = Entity::GetEntityByPointer(entaddr);
		uint64_t lua_fxn_ref = ((hook_handle * sizeof(size_t)) + e->GetVtable()) | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		lua_newtable(lua_);
		e->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		lua_newtable(lua_);

		lua_pushinteger(lua_, param1->type);
		lua_setfield(lua_, -2, "type");
		Entity::GetEntityByID(param1->entity_id_1)->MakeLuaObject(lua_);
		lua_setfield(lua_, -2, "entity1");
		Entity::GetEntityByID(param1->entity_id_2)->MakeLuaObject(lua_);
		lua_setfield(lua_, -2, "entity2");
		lua_pushinteger(lua_, param1->param);
		lua_setfield(lua_, -2, "param");
		LuaHelper::Return<Vector3>(lua_, param1->loc);
		lua_setfield(lua_, -2, "loc");

		lua_rawseti(lua_, -2, 2);

		lua_pushvalue(lua_, -1);
		lua_insert(lua_, 1);

		CallHookChain(lua_);
		if (lua_isnil(lua_, -1))
		{
			lua_settop(lua_, 1);
			for (size_t i = 1; i <= lua_rawlen(lua_, 1); i++)
			{
				lua_rawgeti(lua_, 1, i);
				if (i == 1) lua_pushinteger(lua_, hook_handle);
			}
			lua_remove(lua_, 1);
			e->CallOrigVtable(lua_, hook_handle);
		}
		lua_settop(lua_, 0);
	}
};

