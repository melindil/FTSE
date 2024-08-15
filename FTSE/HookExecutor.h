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

class Logger;

class HookExecutor
{
public:
	HookExecutor(Logger* logger,std::string const& luaname);
	~HookExecutor();

	void TeamPlayerTrigger(void* entity);
	void IsRadiated(void* entity);
	void LongTickTrigger(void* entity);
	void SetVariableTrigger(void* SetVariableObj);
	void DefaultStyleConstructed(void* style);
	void OnStart();
	void ReplacePerk(FOTPerkTableEntry* newstat, int entry);
	void ReplacePerk(lua_State* l);
	void AddLocaleString(std::string const& key, std::string const& value);
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

	uint64_t InstallVtableHook(std::string const& classname, int idx);

private:

	uint32_t MultiTargetAttack(void* cmsg, void* astart, void* aend, bool area = false);
	int32_t OnCriticalEffectImpl(void* cmsg, int32_t roll);
	void SetupVtableHookTemplates();

	static const uint32_t DATA_PERK_TABLE = 0x8a4500;
	static const uint32_t DICT_GLOBAL_PTR = 0x8bd8f4;
	static const uint32_t FXN_ADD_DICTIONARY = 0x703260;

	Logger* logger_;
	lua_State* lua_;

	std::shared_ptr<EntityVtable> entity_vtable_;
	size_t VtableHookTemplates_[536];

	// Some hooks need a saved state
	void* savedweapon_;
	std::vector<int> saved_hits_;

	// Vtable hook templates
	void vtable_hook_template_noop(size_t hook_handle, Entity* ent)
	{
		logger_->Log("Unimplemented Vtable hook called - expect a crash soon!");
	}

	void vtable_hook_template_0(size_t hook_handle, void* entaddr)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);
		if (lua_pcall(lua_, 1, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}
	template<typename RET>
	RET vtable_hook_template_0r(size_t hook_handle, void* entaddr)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		if (lua_pcall(lua_, 1, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		RET ret = LuaHelper::Retrieve<RET>(lua_, -1);
		lua_pop(lua_, 1);
		return ret;
	}

	template<typename PARAM1>
	void vtable_hook_template_1(size_t hook_handle, void* entaddr,PARAM1 param1)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);
		
		LuaHelper::Return<PARAM1>(lua_, param1);

		if (lua_pcall(lua_, 2, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}
	template<typename PARAM1,typename RET>
	RET vtable_hook_template_1r(size_t hook_handle, void* entaddr, PARAM1 param1)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM1>(lua_, param1);

		if (lua_pcall(lua_, 2, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		RET ret = LuaHelper::Retrieve<RET>(lua_, -1);
		lua_pop(lua_, 1);
		return ret;

	}
	template<typename PARAM1>
	PARAM1* vtable_hook_template_1r1(size_t hook_handle, void* entaddr, PARAM1* param1)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		//LuaHelper::Return<PARAM1>(lua_, param1);

		if (lua_pcall(lua_, 1, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -1);
		lua_pop(lua_, 1);
		return param1;

	}
	template<typename PARAM1,typename PARAM2>
	void vtable_hook_template_2(size_t hook_handle, void* entaddr, PARAM1 param1,PARAM2 param2)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM1>(lua_, param1);
		LuaHelper::Return<PARAM2>(lua_, param2);

		if (lua_pcall(lua_, 3, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}
	template<typename PARAM1, typename PARAM2,typename RET>
	RET vtable_hook_template_2r(size_t hook_handle, void* entaddr, PARAM1 param1,PARAM2 param2)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM1>(lua_, param1);
		LuaHelper::Return<PARAM2>(lua_, param2);

		if (lua_pcall(lua_, 3, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		RET ret = LuaHelper::Retrieve<RET>(lua_, -1);
		lua_pop(lua_, 1);
		return ret;

	}
	template<typename PARAM1,typename PARAM2>
	PARAM1* vtable_hook_template_2r1(size_t hook_handle, void* entaddr, PARAM1* param1,PARAM2 param2)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM2>(lua_, param2);

		if (lua_pcall(lua_, 2, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -1);
		lua_pop(lua_, 1);
		return param1;

	}
	template<typename PARAM1, typename PARAM2,typename PARAM3>
	void vtable_hook_template_3(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2,PARAM3 param3)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM1>(lua_, param1);
		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM3>(lua_, param3);

		if (lua_pcall(lua_, 4, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename RET>
	RET vtable_hook_template_3r(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2, PARAM3 param3)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM1>(lua_, param1);
		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM3>(lua_, param3);

		if (lua_pcall(lua_, 4, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		RET ret = LuaHelper::Retrieve<RET>(lua_, -1);
		lua_pop(lua_, 1);
		return ret;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3>
	PARAM1* vtable_hook_template_3r1(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3 param3)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM3>(lua_, param3);

		if (lua_pcall(lua_, 3, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -1);
		lua_pop(lua_, 1);
		return param1;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3,typename PARAM4>
	void vtable_hook_template_4(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2, PARAM3 param3,PARAM4 param4)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM1>(lua_, param1);
		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM3>(lua_, param3);
		LuaHelper::Return<PARAM4>(lua_, param4);

		if (lua_pcall(lua_, 5, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
	PARAM1* vtable_hook_template_4r1(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3 param3, PARAM4 param4)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM3>(lua_, param3);
		LuaHelper::Return<PARAM4>(lua_, param4);

		if (lua_pcall(lua_, 4, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -1);
		lua_pop(lua_, 1);
		return param1;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4,typename PARAM5>
	void vtable_hook_template_5(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4,PARAM5 param5)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM1>(lua_, param1);
		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM3>(lua_, param3);
		LuaHelper::Return<PARAM4>(lua_, param4);
		LuaHelper::Return<PARAM5>(lua_, param5);

		if (lua_pcall(lua_, 6, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4, typename PARAM5,typename PARAM6>
	void vtable_hook_template_6(size_t hook_handle, void* entaddr, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5, PARAM6 param6)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM1>(lua_, param1);
		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM3>(lua_, param3);
		LuaHelper::Return<PARAM4>(lua_, param4);
		LuaHelper::Return<PARAM5>(lua_, param5);
		LuaHelper::Return<PARAM6>(lua_, param6);

		if (lua_pcall(lua_, 7, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}
	template<typename PARAM1, typename PARAM2, typename PARAM3>
	PARAM1* vtable_hook_template_3r2(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3* param3)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM2>(lua_, param2);

		if (lua_pcall(lua_, 2, 2, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -2);
		*param3 = LuaHelper::Retrieve<PARAM3>(lua_, -1);
		lua_pop(lua_, 2);
		return param1;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3,typename PARAM4>
	PARAM1* vtable_hook_template_4r2(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3* param3,PARAM4 param4)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM4>(lua_, param4);

		if (lua_pcall(lua_, 3, 2, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -2);
		*param3 = LuaHelper::Retrieve<PARAM3>(lua_, -1);
		lua_pop(lua_, 2);
		return param1;

	}
	template<typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4,typename PARAM5>
	PARAM1* vtable_hook_template_5r2(size_t hook_handle, void* entaddr, PARAM1* param1, PARAM2 param2, PARAM3* param3, PARAM4 param4,PARAM5 param5)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		LuaHelper::Return<PARAM2>(lua_, param2);
		LuaHelper::Return<PARAM4>(lua_, param4);
		LuaHelper::Return<PARAM5>(lua_, param5);

		if (lua_pcall(lua_, 4, 2, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}

		*param1 = LuaHelper::Retrieve<PARAM1>(lua_, -2);
		*param3 = LuaHelper::Retrieve<PARAM3>(lua_, -1);
		lua_pop(lua_, 2);
		return param1;

	}

	void vtable_hook_template_X32(size_t hook_handle, void* entaddr, float* param1)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

		lua_newtable(lua_);
		
		for (int i = 0; i < 12; i++)
		{
			lua_pushnumber(lua_, param1[i]);
			lua_rawseti(lua_, -2, i + 1);
		}

		if (lua_pcall(lua_, 2, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}
	void vtable_hook_template_X510(size_t hook_handle, void* entaddr, ControllerCommandStruct* param1)
	{
		uint64_t lua_fxn_ref = hook_handle | 0xca11f75e00000000LLU;
		lua_rawgeti(lua_, LUA_REGISTRYINDEX, lua_fxn_ref);
		Entity::GetEntityByPointer(entaddr)->MakeLuaObject(lua_);

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

		if (lua_pcall(lua_, 2, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);

		}
	}};

