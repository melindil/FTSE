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

#include "HookExecutor.h"
#include "Actor.h"
#include "World.h"
#include "Logger.h"
#include "FOTPerkTable.h"
#include "AttributesTable.h"
#include "CombatMessage.h"
#include "LuaHelper.h"
#include "DefaultStyle.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include "Helpers.h"
#include "Weapon.h"
#include "FOTString.h"

// Lua stubs
int l_replaceperk(lua_State* l)
{
	HookExecutor** he = (HookExecutor**)luaL_checkudata(l, 1, "HookExecutor");
	if (lua_istable(l, 2) && lua_isnumber(l, 3))
	{
		(*he)->ReplacePerk(l);
	}
	return 0;

}

int l_addlocalestring(lua_State* l)
{
	HookExecutor** he = (HookExecutor**)luaL_checkudata(l, 1, "HookExecutor");
	if (lua_isstring(l, 2) && lua_isstring(l, 3))
	{
		(*he)->AddLocaleString(lua_tostring(l, 2),lua_tostring(l,3));
	}
	return 0;
}

HookExecutor::HookExecutor(Logger* logger,std::string const& luaname)
	: logger_(logger), savedweapon_(NULL)
{
	lua_ = luaL_newstate();
	luaL_openlibs(lua_);
	if (luaL_dofile(lua_, luaname.c_str()))
	{
		*logger_ << "Error loading LUA scripts: " << lua_tostring(lua_, -1) << std::endl;
	}
	else
	{
		*logger_ << "Successfully started LUA engine." << std::endl;
	}

	logger->RegisterLUA(lua_);
	Actor::RegisterLua(lua_, logger);
	Weapon::RegisterLua(lua_, logger);
	World::RegisterLua(lua_, logger);
	DefaultStyle::RegisterLua(lua_);

	// Register HookExecutor functions for Lua
	HookExecutor** heptrptr = (HookExecutor**)lua_newuserdata(lua_, sizeof(HookExecutor*));
	*heptrptr = this;
	luaL_newmetatable(lua_, "HookExecutor");
	lua_pushcfunction(lua_, l_replaceperk);
	lua_setfield(lua_, -2, "ReplacePerk");
	lua_pushcfunction(lua_, l_addlocalestring);
	lua_setfield(lua_, -2, "AddLocaleString");
	lua_pushvalue(lua_, -1);
	lua_setfield(lua_, -2, "__index");
	lua_setmetatable(lua_, -2);
	lua_setglobal(lua_, "hookexecutor");
	
}

HookExecutor::~HookExecutor()
{
}

void HookExecutor::ReplacePerk(FOTPerkTableEntry* newperk, int entry)
{
	FOTPerkTableEntry* perkarray = (FOTPerkTableEntry*)DATA_PERK_TABLE;
	perkarray += entry;
	DWORD old_protect;
	VirtualProtect(perkarray, sizeof(FOTPerkTableEntry), PAGE_EXECUTE_READWRITE, &old_protect);
	memcpy(perkarray, newperk, sizeof(FOTPerkTableEntry));
	VirtualProtect(perkarray, sizeof(FOTPerkTableEntry), old_protect, &old_protect);
}

void HookExecutor::AddLocaleString(std::string const& key, std::string const& value)
{
	void* locale_dictionary = (void*)((uint32_t)DICT_GLOBAL_PTR);
	typedef void(__thiscall *fxntype)(void*, wchar_t**, wchar_t**);
	auto fxn = (fxntype)FXN_ADD_DICTIONARY;
	wchar_t* wkey = Helpers::UTF8ToWcharFOTHeap(key, 1);
	wchar_t* wvalue = Helpers::UTF8ToWcharFOTHeap(value, 1);
	(*fxn)(locale_dictionary, &wkey, &wvalue);
}

void HookExecutor::ReplacePerk(lua_State* l)
{
	*logger_ << "Lua called ReplacePerk" << std::endl;

	int entry = (int)lua_tointeger(l, 3);
	*logger_ << "Entry " << entry << std::endl;
	FOTPerkTableEntry newperk;

	newperk.perkShortName = LuaHelper::GetPermTableString(l, 2, "name");

	newperk.reqLevel = LuaHelper::GetTableInteger(l, 2, "minlevel");
	*logger_ << "Perk min level: " << newperk.reqLevel << std::endl;
	newperk.maxLevels = LuaHelper::GetTableInteger(l, 2, "maxperktaken");
	*logger_ << "Perk num levels: " << newperk.maxLevels << std::endl;

	newperk.perkBonusStat = LuaHelper::GetPermTableString(l, 2, "bonusstat");
	*logger_ << "BonusStat: " << newperk.perkBonusStat << std::endl;
	newperk.perkBonusAmt = LuaHelper::GetTableInteger(l, 2, "bonusamt");
	*logger_ << "BonusAmt: " << newperk.perkBonusAmt << std::endl;

	newperk.perkReqStat1 = LuaHelper::GetPermTableString(l, 2, "requiredstat1");
	*logger_ << "Perk required stat 1: " << newperk.perkReqStat1 << std::endl;
	newperk.perkReqAmt1 = LuaHelper::GetTableInteger(l, 2, "requiredamt1");
	*logger_ << "Perk required amt 1: " << newperk.perkReqAmt1 << std::endl;

	newperk.perkAndOrFlag = LuaHelper::GetTableInteger(l, 2, "and_or_flag");
	*logger_ << "Perk And/Or: " << newperk.perkAndOrFlag << std::endl;

	newperk.perkReqStat2 = LuaHelper::GetPermTableString(l, 2, "requiredstat2");
	*logger_ << "Perk required stat 2: " << newperk.perkReqStat2 << std::endl;
	newperk.perkReqAmt2 = LuaHelper::GetTableInteger(l, 2, "requiredamt2");
	*logger_ << "Perk required amt 2: " << newperk.perkReqAmt2 << std::endl;

	newperk.perkReqST = LuaHelper::GetTableInteger(l, 2, "requiredST");
	newperk.perkReqPE = LuaHelper::GetTableInteger(l, 2, "requiredPE");
	newperk.perkReqEN = LuaHelper::GetTableInteger(l, 2, "requiredEN");
	newperk.perkReqCH = LuaHelper::GetTableInteger(l, 2, "requiredCH");
	newperk.perkReqIN = LuaHelper::GetTableInteger(l, 2, "requiredIN");
	newperk.perkReqAG = LuaHelper::GetTableInteger(l, 2, "requiredAG");
	newperk.perkReqLK = LuaHelper::GetTableInteger(l, 2, "requiredLK");
	*logger_ << "Perk required ST: " << newperk.perkReqST << std::endl;
	*logger_ << "Perk required PE: " << newperk.perkReqPE << std::endl;
	*logger_ << "Perk required EN: " << newperk.perkReqEN << std::endl;
	*logger_ << "Perk required CH: " << newperk.perkReqCH << std::endl;
	*logger_ << "Perk required IN: " << newperk.perkReqIN << std::endl;
	*logger_ << "Perk required AG: " << newperk.perkReqAG << std::endl;
	*logger_ << "Perk required LK: " << newperk.perkReqLK << std::endl;

	newperk.perkRestrictedRace = LuaHelper::GetPermTableString(l, 2, "prohibitedRace");
	*logger_ << "Perk prohibited race: " << newperk.perkRestrictedRace << std::endl;
	newperk.perkRequiredRace = LuaHelper::GetPermTableString(l, 2, "requiredRace");
	*logger_ << "Perk required race: " << newperk.perkRequiredRace << std::endl;

	*logger_ << "Adding perk " << newperk.perkShortName << std::endl;
	ReplacePerk(&newperk, entry);

}

// Testing trigger - fires every 10 seconds when an entity has Team Player bonus.
// Added this trigger since I was familiar with the code in that area.
void HookExecutor::TeamPlayerTrigger(void* entity)
{

}

// Trigger for Hulk Smash start - character is irradiated

void HookExecutor::IsRadiated(void* entity)
{
	lua_getglobal(lua_, "OnRadiated");
	if (lua_isfunction(lua_, -1))
	{
		Actor e(entity);
		e.MakeLuaObject(lua_);
		lua_pcall(lua_, 1, 0, 0);

	}
	else
		lua_pop(lua_, 1);
}

// Trigger for "long" (10 sec) game tick
void HookExecutor::LongTickTrigger(void* entity)
{
	lua_getglobal(lua_, "OnLongTick");
	if (lua_isfunction(lua_, -1))
	{
		Actor e(entity);
		e.MakeLuaObject(lua_);
		lua_pcall(lua_, 1, 0, 0);

	}
	else
		lua_pop(lua_, 1);

}

// Trigger for set variable
void HookExecutor::SetVariableTrigger(void* setvar)
{
	lua_getglobal(lua_, "OnVariableChanged");
	if (lua_isfunction(lua_, -1))
	{
		uint32_t ptr = (uint32_t)setvar;
		wchar_t* key = *(wchar_t**)(ptr + 0x0b);
		wchar_t* val = *(wchar_t**)(ptr + 0x0f);
		char campaignvar = *(char*)(ptr + 0x13);

		std::string convkey = Helpers::WcharToUTF8(key);
		std::string convval = Helpers::WcharToUTF8(val);
		lua_pushstring(lua_, convkey.c_str());
		lua_pushstring(lua_, convval.c_str());
		lua_pushboolean(lua_, campaignvar);
		lua_pcall(lua_, 3, 0, 0);

	}
	else
		lua_pop(lua_, 1);
}
/*bool HookExecutor::EntityHasPerk(void* entity, int perknum)
{
	DWORD* perkptr = (DWORD*)
		((char*)entity
			+ ENTITY_OFFSET_ATTRIBUTES	// start of standard attributes
			+ AttributesTable::OFFSET_PERKS);				// start of perk table

	return perkptr[perknum] != 0;
}*/

/*DWORD HookExecutor::GetEntityTempPerkValue(void* entity, int perknum)
{
	DWORD* perkptr = (DWORD*)
		((char*)entity
			+ ENTITY_OFFSET_TEMP_ATTRIBUTES	// start of standard attributes
			+ AttributesTable::OFFSET_PERKS);				// start of perk table
	return perkptr[perknum];
}*/

/*void HookExecutor::SetEntityTempPerkValue(void* entity, int perknum, DWORD value)
{
	DWORD* perkptr = (DWORD*)
		((char*)entity
			+ ENTITY_OFFSET_TEMP_ATTRIBUTES	// start of standard attributes
			+ AttributesTable::OFFSET_PERKS);				// start of perk table

	perkptr[perknum] = value;
}*/


void HookExecutor::OnStart()
{

	lua_getglobal(lua_, "OnStart");
	if (lua_isfunction(lua_, -1))
	{
		lua_pcall(lua_, 0, 0, 0);
	}
	else
		lua_pop(lua_, 1);
	// Initialize AttributesTable after OnStart, to pick up any perks
	// NOTE that this means OnStart shouldn't touch any entities (none are in memory yet anyway)
	AttributesTable::Initialize(logger_);

}

void HookExecutor::DefaultStyleConstructed(void* style)
{
	lua_getglobal(lua_, "DefaultStyleChanges");
	if (lua_isfunction(lua_, -1))
	{
		DefaultStyle d(style, logger_);
		d.MakeLuaObject(lua_);
		lua_pcall(lua_, 1, 0, 0);
	}
	else
		lua_pop(lua_, 1);
}
void HookExecutor::OnLocaleLoad()
{
	lua_getglobal(lua_, "OnLocaleLoad");
	if (lua_isfunction(lua_, -1))
	{
		lua_pcall(lua_, 0, 0, 0);
	}
	else
		lua_pop(lua_, 1);
}

int HookExecutor::MsecTimerHook(uint64_t msec, uint32_t scale, void* target)
{
	lua_getglobal(lua_, "MsecToDayHMS");
	if (lua_isfunction(lua_, -1))
	{
		lua_pushinteger(lua_, msec);
		lua_pushinteger(lua_, scale);
		lua_pcall(lua_, 2, 1, 0);

		struct _datetime {
			uint64_t days;
			uint16_t hour;
			uint16_t minute;
			uint16_t second;
			uint16_t msec;
			uint16_t year;
			uint16_t month;
		} *datetime = (_datetime*)target;
		datetime->year = 0;
		datetime->month = 0;
		datetime->days = LuaHelper::GetTableInteger(lua_, -1, "day");
		datetime->hour = LuaHelper::GetTableInteger(lua_, -1, "hour");
		datetime->minute = LuaHelper::GetTableInteger(lua_, -1, "minute");
		datetime->second = LuaHelper::GetTableInteger(lua_, -1, "second");
		datetime->msec = LuaHelper::GetTableInteger(lua_, -1, "msec");
		return 1;
	}
	lua_pop(lua_, 1);
	return 0;
}

int HookExecutor::AddBaseTime(void* target)
{
	lua_getglobal(lua_, "AddBaseToGameTime");
	if (lua_isfunction(lua_, -1))
	{
		struct _datetime {
			uint64_t days;
			uint16_t hour;
			uint16_t minute;
			uint16_t second;
			uint16_t msec;
			uint16_t year;
			uint16_t month;
		} *datetime = (_datetime*)target;
		lua_newtable(lua_);
		lua_pushinteger(lua_, datetime->days);
		lua_setfield(lua_, -2, "day");
		lua_pushinteger(lua_, datetime->hour);
		lua_setfield(lua_, -2, "hour");
		lua_pushinteger(lua_, datetime->minute);
		lua_setfield(lua_, -2, "minute");
		lua_pushinteger(lua_, datetime->second);
		lua_setfield(lua_, -2, "second");
		lua_pushinteger(lua_, datetime->msec);
		lua_setfield(lua_, -2, "msec");
		lua_pushinteger(lua_, datetime->year);
		lua_setfield(lua_, -2, "year");
		lua_pushinteger(lua_, datetime->month);
		lua_setfield(lua_, -2, "month");
		lua_pcall(lua_, 1, 1, 0);

		datetime->year = LuaHelper::GetTableInteger(lua_, -1, "year");
		datetime->month = LuaHelper::GetTableInteger(lua_, -1, "month");
		datetime->days = LuaHelper::GetTableInteger(lua_, -1, "day");
		datetime->hour = LuaHelper::GetTableInteger(lua_, -1, "hour");
		datetime->minute = LuaHelper::GetTableInteger(lua_, -1, "minute");
		datetime->second = LuaHelper::GetTableInteger(lua_, -1, "second");
		datetime->msec = LuaHelper::GetTableInteger(lua_, -1, "msec");
		return 1;
	}
	lua_pop(lua_, 1);
	return 0;

}

void HookExecutor::OnChanceToHitCalc(void* attacker, void* target, void* weapon, void* chancevptr, wchar_t* loc)
{
	lua_getglobal(lua_, "OnChanceToHitCalc");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	Actor atk(attacker);
	atk.MakeLuaObject(lua_);
	Actor tgt(target);
	tgt.MakeLuaObject(lua_);
	if (savedweapon_ != NULL)
	{
		Weapon(savedweapon_).MakeLuaObject(lua_);
	}
	else
	{
		lua_pushnil(lua_);
	}
	std::string utf8loc = Helpers::WcharToUTF8(loc);
	lua_newtable(lua_);
	ChanceToHit* cth = (ChanceToHit*)chancevptr;
	lua_pushinteger(lua_, cth->hit_chance);
	lua_setfield(lua_, -2, "hit_chance");
	lua_pushinteger(lua_, cth->raw_chance);
	lua_setfield(lua_, -2, "raw_chance");
	lua_pushinteger(lua_, cth->ineligible_flags);
	lua_setfield(lua_, -2, "ineligible_flags");
	lua_pushstring(lua_, utf8loc.c_str());
	lua_setfield(lua_, -2, "location");

	if (lua_pcall(lua_, 4, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
	}

	if (lua_istable(lua_, -1))
	{
		cth->hit_chance = LuaHelper::GetTableInteger(lua_, -1, "hit_chance");
		cth->raw_chance = LuaHelper::GetTableInteger(lua_, -1, "raw_chance");
		cth->ineligible_flags = LuaHelper::GetTableInteger(lua_, -1, "ineligible_flags");
	}
	lua_pop(lua_, 1);
	
}

void HookExecutor::OnChanceToHitCalc_SaveWeapon(void* weapon)
{
	savedweapon_ = weapon;
}

int32_t HookExecutor::OnChanceToCritical1(void* attacker, void* target, void* weapon, int32_t chance, wchar_t* loc)
{
	lua_getglobal(lua_, "OnChanceToCritical1");
	if (!lua_isfunction(lua_, -1))
	{
		return chance;
	}
	Actor atk(attacker);
	atk.MakeLuaObject(lua_);
	Actor tgt(target);
	tgt.MakeLuaObject(lua_);
	
	Weapon(weapon).MakeLuaObject(lua_);
	std::string locutf8 = Helpers::WcharToUTF8(loc);
	lua_pushstring(lua_, locutf8.c_str());

	lua_pushinteger(lua_, chance);

	if (lua_pcall(lua_, 5, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
	}
	chance =(int32_t) lua_tointeger(lua_, -1);

	lua_pop(lua_, 1);
	return chance;
}

int32_t HookExecutor::OnChanceToCritical2(void* cmsg, int32_t chance)
{
	lua_getglobal(lua_, "OnChanceToCritical2");
	if (!lua_isfunction(lua_, -1))
	{
		return chance;
	}
	CombatMessage* msg = (CombatMessage*)cmsg;
	Actor attacker(msg->attacker);
	Actor target(msg->target);
	attacker.MakeLuaObject(lua_);
	target.MakeLuaObject(lua_);

	Weapon wpn(msg->weapon);
	wpn.MakeLuaObject(lua_);

	std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
	std::string aimlocstr = aimloc->get();
	lua_pushstring(lua_, aimlocstr.c_str());
	lua_pushinteger(lua_, chance);

	if (lua_pcall(lua_, 5, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
	}
	chance = (int32_t)lua_tointeger(lua_, -1);

	lua_pop(lua_, 1);
	return chance;

}


int32_t HookExecutor::OnCriticalEffect1(void* cmsg, int32_t roll)
{
	lua_getglobal(lua_, "OnCriticalEffect1");
	if (!lua_isfunction(lua_, -1))
	{
		return roll;
	}
	return OnCriticalEffectImpl(cmsg, roll);
}
int32_t HookExecutor::OnCriticalEffect2(void* cmsg, int32_t roll)
{
	lua_getglobal(lua_, "OnCriticalEffect2");
	if (!lua_isfunction(lua_, -1))
	{
		return roll;
	}
	return OnCriticalEffectImpl(cmsg, roll);
}

int32_t HookExecutor::OnCriticalEffectImpl(void* cmsg, int32_t roll)
{
	CombatMessage* msg = (CombatMessage*)cmsg;
	Actor attacker(msg->attacker);
	Actor target(msg->target);
	attacker.MakeLuaObject(lua_);
	target.MakeLuaObject(lua_);
	Weapon wpn(msg->weapon);
	wpn.MakeLuaObject(lua_);

	std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
	std::string aimlocstr = aimloc->get();
	lua_pushstring(lua_, aimlocstr.c_str());
	lua_pushinteger(lua_, roll);

	if (lua_pcall(lua_, 5, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
	}
	
	if (lua_isinteger(lua_, -1))
	{
		roll = (int32_t)lua_tointeger(lua_, -1);
		lua_pop(lua_, 1);
		return roll;
	}

	// TODO: Parse return table
	if (LuaHelper::GetTableBool(lua_, -1, "bypassdefenses"))
	{
		msg->critflags |= 1;
	}
	if (LuaHelper::GetTableBool(lua_, -1, "wickedhit"))
	{
		msg->critflags |= 2;
	}
	if (LuaHelper::GetTableBool(lua_, -1, "knockdown"))
	{
		msg->critflags |= 4;
	}
	if (LuaHelper::GetTableBool(lua_, -1, "injure"))
	{
		msg->critflags |= 8;
	}
	if (LuaHelper::GetTableBool(lua_, -1, "disarmright"))
	{
		msg->critflags |= 16;
	}
	if (LuaHelper::GetTableBool(lua_, -1, "disarmleft"))
	{
		msg->critflags |= 32;
	}
	if (LuaHelper::GetTableBool(lua_, -1, "knockout"))
	{
		msg->critflags |= 64;
	}
	if (LuaHelper::GetTableBool(lua_, -1, "breakweapon"))
	{
		msg->critflags |= 128;
	}
	if (LuaHelper::GetTableBool(lua_, -1, "tornapart"))
	{
		msg->critflags |= 256;
	}
	lua_pop(lua_, 1);
	return 0;
}

uint32_t HookExecutor::OnBurstAttack(void* cmsg, void* astart, void* aend)
{
	lua_getglobal(lua_, "OnBurstAttack");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend);
}

uint32_t HookExecutor::OnConeAttack(void* cmsg, void* astart, void* aend)
{
	lua_getglobal(lua_, "OnConeAttack");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend);
}

uint32_t HookExecutor::OnSprayAttack(void* cmsg, void* astart, void* aend)
{
	lua_getglobal(lua_, "OnSprayAttack");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend);
}

uint32_t HookExecutor::OnRadialAttack(void* cmsg, void* astart, void* aend)
{
	lua_getglobal(lua_, "OnRadialAttack");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend);
}

uint32_t HookExecutor::OnAreaAttack(void* cmsg, void* astart, void* aend)
{
	lua_getglobal(lua_, "OnAreaAttack");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend, true);
}

int HookExecutor::OnStraightAttack(void* cmsg)
{
	lua_getglobal(lua_, "OnStraightAttack");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}
	CombatMessage* msg = (CombatMessage*)cmsg;
	uint16_t tgt = msg->target;
	if (tgt == 0)
	{
		// For now, don't hook untargeted shot - will be treated as a burst
		lua_pop(lua_, 1);
		return 0;
	}
	uint16_t* tgtptr = &tgt;

	uint32_t ret = MultiTargetAttack(cmsg, &tgtptr, (&tgtptr)+1);
	if (ret == 2)	// 2 is a miss
	{
		// Zero hit counter
		msg->numshots = 0;

		Actor tgt(msg->target);
		tgt.ShotAtMissed(cmsg);
	}
	return 1;
}

int HookExecutor::OnProjectileAttack(uint32_t ht,void* cmsg)
{
	lua_getglobal(lua_, "OnProjectileAttack");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}
	CombatMessage* msg = (CombatMessage*)cmsg;
	if (msg->target == 0)
	{
		lua_pop(lua_, 1);
		return 0;
	}

	Actor attacker(msg->attacker);
	Weapon wpn(msg->weapon);

	attacker.MakeLuaObject(lua_);
	lua_pushinteger(lua_, msg->numshots);
	lua_newtable(lua_);

	Actor tgt(msg->target);
	Vector3 dir(tgt.GetLocation() - attacker.GetLocation());
	float dist = dir.distance();

	lua_newtable(lua_);

	tgt.MakeLuaObject(lua_);
	lua_setfield(lua_, -2, "actor");
	lua_pushinteger(lua_, ht & 0xff);
	lua_setfield(lua_, -2, "hit_chance");
	lua_pushnumber(lua_, dist);
	lua_setfield(lua_, -2, "distance");
	lua_pushboolean(lua_, true);
	lua_setfield(lua_, -2, "intended");
	lua_rawseti(lua_, -2, 1);

	wpn.MakeLuaObject(lua_);
	if (lua_pcall(lua_, 4, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
	}
	struct restype
	{
		uint16_t id;
		uint32_t hits;
		float mult;
		float deltax;
		float deltaz;
	};
	std::vector<restype> results;
	lua_pushnil(lua_);
	while (lua_next(lua_, -2))
	{
		restype res;
		res.id = LuaHelper::GetTableInteger(lua_, -1, "id");
		res.hits = LuaHelper::GetTableInteger(lua_, -1, "hits");
		res.mult = LuaHelper::GetTableFloat(lua_, -1, "mult");
		res.deltax = LuaHelper::GetTableFloat(lua_, -1, "deltax");
		res.deltaz = LuaHelper::GetTableFloat(lua_, -1, "deltaz");
		if (res.mult == 0.0f)
		{
			res.mult = 1.0f; // default value
		}
		results.push_back(res);
		lua_pop(lua_, 1);
	}
	lua_pop(lua_, 1);

	for (auto elem : results)
	{
		Actor a(elem.id);

		CombatMessage cm;
		memcpy(&cm, msg, sizeof(CombatMessage));
		cm.target = a.GetID();
		cm.target_2 = a.GetID();
		cm.target_flags = a.GetFlags();
		cm.numshots = elem.hits;
		Vector3 loc = a.GetLocation();
		cm.target_x = loc.v[0] + elem.deltax;
		cm.target_y = loc.v[1];
		cm.target_z = loc.v[2] + elem.deltaz;
		uint32_t arry = 0;
		cm.arraystart = cm.arrayend = cm.arrayptr = (uint32_t)(&arry);

		auto CombatFXConstructor = (void* (*)())0x593520;
		void* cfx = CombatFXConstructor();
		size_t* vtable = *((size_t**)cfx);
		auto CFxPopulate = (void(__thiscall *)(void*, void*, void*))(vtable[0x73c/4]);
		char c = 0;
		CFxPopulate(cfx, &cm, &c);

	}

	return 1;
}

uint32_t HookExecutor::MultiTargetAttack(void* cmsg, void* astart, void* aend, bool area)
{
	auto FOTChanceHit = (void(*)(ChanceToHit*, void*, void*, void*, void*, wchar_t*))(0x615e80);
	auto FOTApplyHits = (int(*)(CombatMessage*, float))(0x613230);
	CombatMessage* msg = (CombatMessage*)cmsg;

	Actor attacker(msg->attacker);
	Vector3 center(&msg->attacker_x);
	Weapon wpn(msg->weapon);
	Vector3 aim(Vector3(&msg->target_x) - attacker.GetLocation());

	attacker.MakeLuaObject(lua_);
	lua_pushinteger(lua_, msg->numshots);
	lua_newtable(lua_);

	uint16_t** ptr = (uint16_t**)astart;
	int array_idx = 1;
	int ret = 2;
	while (ptr != (uint16_t**)aend)
	{
		Actor tgt(*(*ptr));
		if (tgt.isAlive())
		{
			if (area)
			{
				// check if explosion is blocked
				Vector3 tgtloc = tgt.GetLocation();
				tgtloc.v[1] += tgt.GetHeight()*0.9f;
				if (World::CheckBlocked(center, tgtloc))
				{
					(*logger_) << "Target " << tgt.GetEntityName() << " blocked" << std::endl;
					ptr++;
					continue;
				}
				(*logger_) << "Target " << tgt.GetEntityName() << " eligible" << std::endl;
			}
			ChanceToHit cth;
			std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
			
			void* weapon = Actor(msg->weapon).GetEntityPointer();
			void* loc = &msg->target_x;

			// NOTE: The chance to hit routine will decrement refcount on the location string passed to it!
			// Increment the ref counter so that memory isn't freed before FOTString destructs
			aimloc->incref();

			(*FOTChanceHit)(&cth,
				attacker.GetEntityPointer(),
				tgt.GetEntityPointer(),
				loc,
				weapon,
				aimloc->getraw());


			if (area || cth.ineligible_flags == 0)	
			{
				if (!area && tgt.GetVtable() == Actor::VTABLE && attacker.TestFriendlyCrouched(tgt))
				{
					// friendly is crouched in front, shoot over them
					// (remove them from eligible target list)
					//(*logger_) << "Target " << tgt.GetActorName() << " saved by crouch test" << std::endl;
					ptr++;
					continue;
				}
				bool intended = (tgt.GetID() == msg->target);
				float dist;
				float angle;
				if (area)
				{
					
					Vector3 dir(tgt.GetLocation() - center);
					dist = dir.distance();
					angle = 0;
				}
				else
				{
					Vector3 dir(tgt.GetLocation() -attacker.GetLocation());
					dist = dir.distance();
					angle = Vector3::angle(aim, dir);
				}
				
				lua_newtable(lua_);

				tgt.MakeLuaObject(lua_);
				lua_setfield(lua_, -2, "actor");
				lua_pushinteger(lua_, cth.hit_chance);
				lua_setfield(lua_, -2, "hit_chance");
				lua_pushnumber(lua_, dist);
				lua_setfield(lua_, -2, "distance");
				lua_pushnumber(lua_, angle);
				lua_setfield(lua_, -2, "angle");
				lua_pushboolean(lua_, intended);
				lua_setfield(lua_, -2, "intended");
				lua_rawseti(lua_, -2, array_idx);

				array_idx++;
			}

		}
		ptr++;
	}
	wpn.MakeLuaObject(lua_);
	if (lua_pcall(lua_, 4, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
	}
	struct restype
	{
		uint16_t id;
		uint32_t hits;
		float mult;
	};
	std::vector<restype> results;
	lua_pushnil(lua_);
	while (lua_next(lua_, -2))
	{
		restype res;
		res.id = LuaHelper::GetTableInteger(lua_, -1, "id");
		res.hits = LuaHelper::GetTableInteger(lua_, -1, "hits");
		res.mult = LuaHelper::GetTableFloat(lua_, -1, "mult");
		if (res.mult == 0.0f)
		{
			res.mult = 1.0f; // default value
		}
		results.push_back(res);
		lua_pop(lua_, 1);
	}
	lua_pop(lua_, 1);

	for (auto elem : results)
	{
		if (elem.hits == 0)
			continue;

		ret = 1; // flag to Asm code that a hit occurred
		Actor a(elem.id);
		(*logger_) << "Target " << elem.id << " " << a.GetEntityName()
			<< " hits " << elem.hits << "x" << elem.mult << std::endl;

		CombatMessage cm;
		memcpy(&cm, msg, sizeof(CombatMessage));
		cm.target = a.GetID();
		cm.target_2 = a.GetID();
		cm.target_flags = a.GetFlags();
		cm.numshots = elem.hits;
		Vector3 loc = a.GetLocation();
		cm.target_x = loc.v[0];
		cm.target_y = loc.v[1];
		cm.target_z = loc.v[2];
		uint32_t arry = 0;
		cm.arraystart = cm.arrayend = cm.arrayptr = (uint32_t)(&arry);

		if (area)
		{
			// Calling the normal hit routine for area attack would recurse infinitely
			// Call the hit functions manually
			auto CalcDamage = (void(*)(CombatMessage*, float))(0x612170);
			CalcDamage(&cm, elem.mult);
			auto SpecialEffects = (void(*)(CombatMessage*))(0x619cb0);
			SpecialEffects(&cm);
			uint32_t vtable_tgt = a.GetVtableFxn(0x4a8);
			auto ApplyHit = (void (__thiscall*)(void*, CombatMessage*))(vtable_tgt);
			ApplyHit(a.GetEntityPointer(), &cm);
		}
		else
		{
			(*FOTApplyHits)(&cm, elem.mult);
		}

		
	}

	return ret;

}