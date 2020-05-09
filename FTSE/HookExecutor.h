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

private:

	uint32_t MultiTargetAttack(void* cmsg, void* astart, void* aend, bool area = false);
	int32_t OnCriticalEffectImpl(void* cmsg, int32_t roll);

	static const uint32_t DATA_PERK_TABLE = 0x8a4500;
	static const uint32_t DICT_GLOBAL_PTR = 0x8bd8f4;
	static const uint32_t FXN_ADD_DICTIONARY = 0x703260;

	Logger* logger_;
	lua_State* lua_;

	// Some hooks need a saved state
	void* savedweapon_;
	std::vector<int> saved_hits_;

};

