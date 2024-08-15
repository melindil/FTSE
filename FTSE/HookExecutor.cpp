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
#include "ActorStatus.h"
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
#include "Ammo.h"
#include "Trap.h"
#include "VehicleWeapon.h"
#include "Breakable.h"
#include "StateBreakable.h"
#include "GenericItem.h"
#include "SpawnPoint.h"
#include "Keys.h"
#include "Radiation.h"
#include "Armour.h"
#include "Book.h"
#include "Holodisk.h"
#include "Lockpick.h"
#include "SkillConsumable.h"
#include "VehicleController.h"
#include "Alarm.h"
#include "Switch.h"
#include "ScienceSwitch.h"
#include "Geiger.h"
#include "Sneak.h"
#include "TrapTrigger.h"
#include "Container.h"
#include "Light.h"
#include "Scenary.h"
#include "ScenaryMove.h"
#include "ScenarySpawn.h"
#include "StateScenary.h"
#include "Deathtrap.h"
#include "Powernode.h"
#include "RepairObject.h"
#include "CaptureInvItem.h"
#include "CaptureItem.h"
#include "Door.h"
#include "RotatingDoor.h"
#include "EffectSpawn.h"
#include "Waypoint.h"
#include "SentryAI.h"
#include "CombatFX.h"
#include "BaseAI.h"
#include "Controller.h"
#include "Consumable.h"
#include "Inventory.h"
#include "Vehicle.h"
#include "FOTString.h"
#include <memory>
using std::shared_ptr;

#include "EntityVtable.h"
#include "ZoneDefinition.h"

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

int l_installvtablehook(lua_State* l)
{
	HookExecutor** he = (HookExecutor**)luaL_checkudata(l, 1, "HookExecutor");
	if (lua_isstring(l, 2) && lua_isinteger(l, 3) && lua_isfunction(l, 4))
	{
		uint64_t ref = (*he)->InstallVtableHook(lua_tostring(l, 2), (int)lua_tointeger(l, 3));
		lua_pushvalue(l, 4);
		lua_rawseti(l, LUA_REGISTRYINDEX, ref);
	}
	return 0;
}

HookExecutor::HookExecutor(Logger* logger,std::string const& luaname)
	: logger_(logger), savedweapon_(NULL), saved_hits_()
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
	Entity::RegisterLua(lua_, logger_);
	Actor::RegisterLua(lua_, logger_);
	Weapon::RegisterLua(lua_, logger_);
	Ammo::RegisterLua(lua_, logger_);
	Trap::RegisterLua(lua_, logger_);
	VehicleWeapon::RegisterLua(lua_, logger_);
	Breakable::RegisterLua(lua_, logger_);
	StateBreakable::RegisterLua(lua_, logger_);
	GenericItem::RegisterLua(lua_, logger_);
	SpawnPoint::RegisterLua(lua_, logger_);
	Keys::RegisterLua(lua_, logger_);
	Radiation::RegisterLua(lua_, logger_);
	Armour::RegisterLua(lua_, logger_);
	Book::RegisterLua(lua_, logger_);
	Holodisk::RegisterLua(lua_, logger_);
	Lockpick::RegisterLua(lua_, logger_);
	SkillConsumable::RegisterLua(lua_, logger_);
	VehicleController::RegisterLua(lua_, logger_);
	Alarm::RegisterLua(lua_, logger_);
	Switch::RegisterLua(lua_, logger_);
	ScienceSwitch::RegisterLua(lua_, logger_);
	Geiger::RegisterLua(lua_, logger_);
	Sneak::RegisterLua(lua_, logger_);
	TrapTrigger::RegisterLua(lua_, logger_);
	Container::RegisterLua(lua_, logger_);
	Light::RegisterLua(lua_, logger_);
	Scenary::RegisterLua(lua_, logger_);
	ScenaryMove::RegisterLua(lua_, logger_);
	ScenarySpawn::RegisterLua(lua_, logger_);
	StateScenary::RegisterLua(lua_, logger_);
	Deathtrap::RegisterLua(lua_, logger_);
	Powernode::RegisterLua(lua_, logger_);
	RepairObject::RegisterLua(lua_, logger_);
	CaptureInvItem::RegisterLua(lua_, logger_);
	CaptureItem::RegisterLua(lua_, logger_);
	Door::RegisterLua(lua_, logger_);
	RotatingDoor::RegisterLua(lua_, logger_);
	EffectSpawn::RegisterLua(lua_, logger_);
	Waypoint::RegisterLua(lua_, logger_);
	SentryAI::RegisterLua(lua_, logger_);
	CombatFX::RegisterLua(lua_, logger_);
	BaseAI::RegisterLua(lua_, logger_);
	Controller::RegisterLua(lua_, logger_);
	Consumable::RegisterLua(lua_, logger_);
	Inventory::RegisterLua(lua_, logger_);
	World::RegisterLua(lua_, logger_);
	DefaultStyle::RegisterLua(lua_);
	Vehicle::RegisterLua(lua_, logger_);

	entity_vtable_ = std::make_shared<EntityVtable>(lua_);
	Entity::RegisterEntityVtable(entity_vtable_);

	SetupVtableHookTemplates();

	// Register HookExecutor functions for Lua
	HookExecutor** heptrptr = (HookExecutor**)lua_newuserdata(lua_, sizeof(HookExecutor*));
	*heptrptr = this;
	luaL_newmetatable(lua_, "HookExecutor");
	lua_pushcfunction(lua_, l_replaceperk);
	lua_setfield(lua_, -2, "ReplacePerk");
	lua_pushcfunction(lua_, l_addlocalestring);
	lua_setfield(lua_, -2, "AddLocaleString");
	lua_pushcfunction(lua_, l_installvtablehook);
	lua_setfield(lua_, -2, "InstallVtableHook");
	lua_pushvalue(lua_, -1);
	lua_setfield(lua_, -2, "__index");
	lua_setmetatable(lua_, -2);
	lua_setglobal(lua_, "hookexecutor");
	
}

HookExecutor::~HookExecutor()
{
}
uint64_t HookExecutor::InstallVtableHook(std::string const & classname, int idx)
{
	// Hook stub: converts __thiscall of Entity to __thiscall of HookExecutor, and adds
	//            the address of the hook as a parameter, so that we can use that to look
	//            up the correct Lua function in the Lua registry.

	// pop eax                 58
	// push ecx                51
	// push <newhook>          68 xx xx xx xx
	// push eax                50
	// mov ecx,<HookExecutor>  b9 xx xx xx xx
	// jmp <fxnaddr-rel>       e9 xx xx xx xx
	static const int HOOKSIZE = 18;

	// Allocate memory for the hook stub
	unsigned char* newhook = new unsigned char[HOOKSIZE];


	// Change execute privilege for hook stub
	DWORD oldprotect;
	VirtualProtect(newhook, HOOKSIZE, PAGE_EXECUTE_READWRITE, &oldprotect);

	// build the stub code
	newhook[0] = 0x58;
	newhook[1] = 0x51;
	newhook[2] = 0x68;
	memcpy(newhook + 3, &newhook, sizeof(unsigned char**));
	newhook[7] = 0x50;
	newhook[8] = 0xb9;
	void* ptr = this;
	memcpy(newhook + 9, &ptr, sizeof(void**));
	newhook[13] = 0xe9;

	size_t codeaddr = VtableHookTemplates_[idx];
	size_t relative_address = codeaddr - (((size_t)newhook) + 18) ;
	memcpy(newhook + 14, &relative_address, sizeof(size_t));

	// Now copy the hook to the vtable(s)
	entity_vtable_->InstallVtableHook(classname, idx, (size_t)newhook);

	uint64_t ret = (uint64_t)((size_t)newhook) | 0xca11f75e00000000LLU;
	return ret;
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
		shared_ptr<Entity> e = Entity::GetEntityByPointer(entity);
		e->MakeLuaObject(lua_);
		if (lua_pcall(lua_, 1, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
		}

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
		shared_ptr<Entity> e = Entity::GetEntityByPointer(entity);
		e->MakeLuaObject(lua_);
		if (lua_pcall(lua_, 1, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
		}

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
		if (lua_pcall(lua_, 3, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
		}

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
		if (lua_pcall(lua_, 0, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
		}
	}
	else
		lua_pop(lua_, 1);
	// Initialize AttributesTable after OnStart, to pick up any perks
	// NOTE that this means OnStart shouldn't touch any entities (none are in memory yet anyway)
	AttributesTable::Initialize(logger_);
	ActorStatus_Initialize();

}

void HookExecutor::DefaultStyleConstructed(void* style)
{
	lua_getglobal(lua_, "DefaultStyleChanges");
	if (lua_isfunction(lua_, -1))
	{
		DefaultStyle d(style, logger_);
		d.MakeLuaObject(lua_);
		if (lua_pcall(lua_, 1, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
		}
	}
	else
		lua_pop(lua_, 1);
}
void HookExecutor::OnLocaleLoad()
{
	lua_getglobal(lua_, "OnLocaleLoad");
	if (lua_isfunction(lua_, -1))
	{
		if (lua_pcall(lua_, 0, 0, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
		}
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
		if (lua_pcall(lua_, 2, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
			return 0;
		}
		//lua_pcall(lua_, 2, 1, 0);

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
		lua_pop(lua_, 1);
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
		if (lua_pcall(lua_, 1, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
			return 0;
		}

		datetime->year = LuaHelper::GetTableInteger(lua_, -1, "year");
		datetime->month = LuaHelper::GetTableInteger(lua_, -1, "month");
		datetime->days = LuaHelper::GetTableInteger(lua_, -1, "day");
		datetime->hour = LuaHelper::GetTableInteger(lua_, -1, "hour");
		datetime->minute = LuaHelper::GetTableInteger(lua_, -1, "minute");
		datetime->second = LuaHelper::GetTableInteger(lua_, -1, "second");
		datetime->msec = LuaHelper::GetTableInteger(lua_, -1, "msec");
		lua_pop(lua_, 1);
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

	Entity::GetEntityByPointer(attacker)->MakeLuaObject(lua_);
	Entity::GetEntityByPointer(target)->MakeLuaObject(lua_);
	if (savedweapon_ != NULL)
	{
		Entity::GetEntityByPointer(savedweapon_)->MakeLuaObject(lua_);
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
		lua_pop(lua_, 1);
		return;
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
		lua_pop(lua_, 1);
		return chance;
	}
	Entity::GetEntityByPointer(attacker)->MakeLuaObject(lua_);
	Entity::GetEntityByPointer(target)->MakeLuaObject(lua_);
	Entity::GetEntityByPointer(weapon)->MakeLuaObject(lua_);

	std::string locutf8 = Helpers::WcharToUTF8(loc);
	lua_pushstring(lua_, locutf8.c_str());

	lua_pushinteger(lua_, chance);

	if (lua_pcall(lua_, 5, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return chance;
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
		lua_pop(lua_, 1);
		return chance;
	}
	CombatMessage* msg = (CombatMessage*)cmsg;
	Entity::GetEntityByID(msg->attacker)->MakeLuaObject(lua_);
	Entity::GetEntityByID(msg->target)->MakeLuaObject(lua_);
	Entity::GetEntityByID(msg->weapon)->MakeLuaObject(lua_);

	std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
	std::string aimlocstr = aimloc->get();
	lua_pushstring(lua_, aimlocstr.c_str());
	lua_pushinteger(lua_, chance);

	if (lua_pcall(lua_, 5, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return chance;
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
		lua_pop(lua_, 1);
		return roll;
	}
	return OnCriticalEffectImpl(cmsg, roll);
}
int32_t HookExecutor::OnCriticalEffect2(void* cmsg, int32_t roll)
{
	lua_getglobal(lua_, "OnCriticalEffect2");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return roll;
	}
	return OnCriticalEffectImpl(cmsg, roll);
}

void HookExecutor::OnDamageCalcSaveHit(int32_t damage)
{
	saved_hits_.push_back(damage);
}

void HookExecutor::OnDamageCalc(void * cmsg)
{
	lua_getglobal(lua_, "OnDamageCalc");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		saved_hits_.clear();
		return;
	}
	CombatMessage* msg = (CombatMessage*)cmsg;
	Entity::GetEntityByID(msg->attacker)->MakeLuaObject(lua_);
	Entity::GetEntityByID(msg->target)->MakeLuaObject(lua_);
	Entity::GetEntityByID(msg->weapon)->MakeLuaObject(lua_);
	std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
	std::string aimlocstr = aimloc->get();
	lua_pushstring(lua_, aimlocstr.c_str());
	lua_pushinteger(lua_, msg->damage);
	lua_newtable(lua_);
	if (msg->critflags & 1)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "bypassdefenses");
	}
	if (msg->critflags & 2)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "wickedhit");
	}
	if (msg->critflags & 4)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "knockdown");
	}
	if (msg->critflags & 8)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "injure");
	}
	if (msg->critflags & 16)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "disarmright");
	}
	if (msg->critflags & 32)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "disarmleft");
	}
	if (msg->critflags & 64)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "knockout");
	}
	if (msg->critflags & 128)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "breakweapon");
	}
	if (msg->critflags & 256)
	{
		lua_pushboolean(lua_, true);
		lua_setfield(lua_, -2, "tornapart");
	}
	lua_newtable(lua_);
	for (size_t i = 0; i < saved_hits_.size(); i++)
	{
		lua_pushinteger(lua_, saved_hits_[i]);
		lua_rawseti(lua_, -2, i+1);
	}
	if (lua_pcall(lua_, 7, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return;
	}
	if (lua_isinteger(lua_, -1))
	{
		msg->damage = (int32_t)lua_tointeger(lua_, -1);
	}
	lua_pop(lua_, 1);
	saved_hits_.clear();
}

void HookExecutor::OnInventoryAdd(void * receiver, void * item, int32_t quantity)
{
	if (!World::IsLoaded()) return;
	lua_getglobal(lua_, "OnInventoryAdd");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}
	int32_t item_id = *(int32_t*)item;
	Entity::GetEntityByPointer(receiver)->MakeLuaObject(lua_);
	Entity::GetEntityByID(item_id)->MakeLuaObject(lua_);
	lua_pushinteger(lua_, quantity);
	if (lua_pcall(lua_, 3, 0, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
	}

}

void HookExecutor::OnInventoryRemove(void * source, void * item, int32_t quantity)
{
	if (!World::IsLoaded()) return;
	lua_getglobal(lua_, "OnInventoryRemove");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}
	int32_t item_id = *(int32_t*)item;
	Entity::GetEntityByPointer(source)->MakeLuaObject(lua_);
	Entity::GetEntityByID(item_id)->MakeLuaObject(lua_);
	lua_pushinteger(lua_, quantity);
	if (lua_pcall(lua_, 3, 0, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
	}

}

void* HookExecutor::SwapFix(void* swapper, void* returnptr)
{
	InventoryActionResult* retstruct = static_cast<InventoryActionResult*>(returnptr);
	std::shared_ptr<Entity> swapentity = Entity::GetEntityByPointer(swapper);
	std::shared_ptr<Actor> swapactor = std::dynamic_pointer_cast<Actor>(swapentity);
	retstruct->status = 0;
	retstruct->unk_entity_1 = 0;
	retstruct->unk2 = -1;
	retstruct->unk3 = -1;
	retstruct->unk_entity_4 = 0;
	retstruct->unk_5 = 0;

	if (!swapactor)
	{
		retstruct->errstring = Helpers::UTF8ToWcharFOTHeap("", 1);
		return returnptr;
	}

	if (swapactor->GetEquippedItem(0)->GetVtable() == VehicleController::VTABLE ||
		swapactor->GetEquippedItem(0)->GetVtable() == VehicleWeapon::VTABLE ||
		swapactor->GetEquippedItem(1)->GetVtable() == VehicleController::VTABLE ||
		swapactor->GetEquippedItem(1)->GetVtable() == VehicleWeapon::VTABLE)
	{
		retstruct->status = 1;
		retstruct->errstring = Helpers::UTF8ToWcharFOTHeap("", 1);
		return returnptr;
	}
	lua_getglobal(lua_, "OnCheckUnequip");
	if (lua_isfunction(lua_, -1))
	{
		swapactor->MakeLuaObject(lua_);
		swapactor->GetEquippedItem(0)->MakeLuaObject(lua_);
		lua_pushinteger(lua_, 0);
		if (lua_pcall(lua_, 3, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
			return returnptr;
		}

		if (lua_isstring(lua_, -1))
		{
			retstruct->status = 1;
			std::string errmsg = lua_tostring(lua_, -1);
			retstruct->errstring = Helpers::UTF8ToWcharFOTHeap(errmsg, 1);
			lua_pop(lua_, 1);
			return returnptr;
		}
		lua_pop(lua_, 1);
		lua_getglobal(lua_, "OnCheckUnequip");
		swapactor->MakeLuaObject(lua_);
		swapactor->GetEquippedItem(1)->MakeLuaObject(lua_);
		lua_pushinteger(lua_, 1);
		if (lua_pcall(lua_, 3, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
			lua_pop(lua_, 1);
			return returnptr;
		}

		if (lua_isstring(lua_, -1))
		{
			retstruct->status = 1;
			std::string errmsg = lua_tostring(lua_, -1);
			retstruct->errstring = Helpers::UTF8ToWcharFOTHeap(errmsg, 1);
			lua_pop(lua_, 1);
			return returnptr;
		}
		lua_pop(lua_, 1);

	}
	else
	{
		lua_pop(lua_, 1);
	}

	swapactor->SwapFix();
	retstruct->errstring = Helpers::UTF8ToWcharFOTHeap("", 1);

	return returnptr;
}

int32_t HookExecutor::OnCriticalEffectImpl(void* cmsg, int32_t roll)
{
	CombatMessage* msg = (CombatMessage*)cmsg;
	Entity::GetEntityByID(msg->attacker)->MakeLuaObject(lua_);
	Entity::GetEntityByID(msg->target)->MakeLuaObject(lua_);
	Entity::GetEntityByID(msg->weapon)->MakeLuaObject(lua_);

	std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
	std::string aimlocstr = aimloc->get();
	lua_pushstring(lua_, aimlocstr.c_str());
	lua_pushinteger(lua_, roll);

	if (lua_pcall(lua_, 5, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return 0;
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
	uint32_t tgt = msg->target;
	if (tgt == 0)
	{
		// For now, don't hook untargeted shot - will be treated as a burst
		lua_pop(lua_, 1);
		return 0;
	}
	uint32_t* tgtptr = &tgt;

	uint32_t ret = MultiTargetAttack(cmsg, &tgtptr, (&tgtptr)+1);
	if (ret == 2)	// 2 is a miss
	{
		// Zero hit counter
		msg->numshots = 0;

		Entity::GetEntityByID(msg->target)->ShotAtMissed(cmsg);
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
	if (msg->target.id == 0)
	{
		lua_pop(lua_, 1);
		return 0;
	}

	auto attacker = Entity::GetEntityByID(msg->attacker);
	attacker->MakeLuaObject(lua_);
	auto weapon = Entity::GetEntityByID(msg->weapon);

	lua_pushinteger(lua_, msg->numshots);
	lua_newtable(lua_);

	auto target = Entity::GetEntityByID(msg->target);
	Vector3 dir(target->GetLocation() - attacker->GetLocation());
	float dist = dir.distance();

	lua_newtable(lua_);

	target->MakeLuaObject(lua_);
	lua_setfield(lua_, -2, "actor");
	lua_pushinteger(lua_, ht & 0xff);
	lua_setfield(lua_, -2, "hit_chance");
	lua_pushnumber(lua_, dist);
	lua_setfield(lua_, -2, "distance");
	lua_pushboolean(lua_, true);
	lua_setfield(lua_, -2, "intended");
	lua_rawseti(lua_, -2, 1);

	weapon->MakeLuaObject(lua_);
	if (lua_pcall(lua_, 4, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return 0;
	}
	struct restype
	{
		uint32_t id;
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
		auto hit_target = Entity::GetEntityByID(elem.id);

		CombatMessage cm;
		memcpy(&cm, msg, sizeof(CombatMessage));
		cm.target = hit_target->GetID();
		cm.numshots = elem.hits;
		Vector3 loc = hit_target->GetLocation();
		cm.target_pos = loc;
		cm.target_pos.v[0] += elem.deltax;
		cm.target_pos.v[2] += elem.deltaz;
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

	auto attacker = Entity::GetEntityByID(msg->attacker);
	Vector3 center(msg->attacker_pos);
	auto weapon = Entity::GetEntityByID(msg->weapon);
	Vector3 aim(Vector3(msg->target_pos) - attacker->GetLocation());

	attacker->MakeLuaObject(lua_);
	lua_pushinteger(lua_, msg->numshots);
	lua_newtable(lua_);

	uint16_t** ptr = (uint16_t**)astart;
	int array_idx = 1;
	int ret = 2;
	while (ptr != (uint16_t**)aend)
	{
		auto target = Entity::GetEntityByIDBase(*(*ptr));
		if (target->isAlive())
		{
			if (area)
			{
				// check if explosion is blocked
				Vector3 tgtloc = target->GetLocation();
				tgtloc.v[1] += target->GetHeight()*0.9f;
				if (World::CheckBlocked(center, tgtloc))
				{
					ptr++;
					continue;
				}
			}
			ChanceToHit cth;
			std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
			
			void* loc = (void*)msg->target_pos.v;

			// NOTE: The chance to hit routine will decrement refcount on the location string passed to it!
			// Increment the ref counter so that memory isn't freed before FOTString destructs
			aimloc->incref();

			(*FOTChanceHit)(&cth,
				attacker->GetEntityPointer(),
				target->GetEntityPointer(),
				loc,
				weapon->GetEntityPointer(),
				aimloc->getraw());


			if (area || cth.ineligible_flags == 0)	
			{
				if (!area && 
					target->GetVtable() == Actor::VTABLE && 
					attacker->GetVtable() == Actor::VTABLE &&
					std::dynamic_pointer_cast<Actor>(attacker)->TestFriendlyCrouched(
						*std::dynamic_pointer_cast<Actor>(target)))
				{
					// friendly is crouched in front, shoot over them
					// (remove them from eligible target list)
					//(*logger_) << "Target " << tgt.GetActorName() << " saved by crouch test" << std::endl;
					ptr++;
					continue;
				}
				bool intended = (target->GetID() == msg->target);
				float dist;
				float angle;
				if (area)
				{
					
					Vector3 dir(target->GetLocation() - center);
					dist = dir.distance();
					angle = 0;
				}
				else
				{
					Vector3 dir(target->GetLocation() - attacker->GetLocation());
					dist = dir.distance();
					angle = Vector3::angle(aim, dir);
				}
				
				lua_newtable(lua_);

				target->MakeLuaObject(lua_);
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
	weapon->MakeLuaObject(lua_);
	if (lua_pcall(lua_, 4, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return 0;
	}
	struct restype
	{
		uint32_t id;
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
		auto a = Entity::GetEntityByID(elem.id);

		CombatMessage cm;
		memcpy(&cm, msg, sizeof(CombatMessage));
		cm.target = a->GetID();
		cm.numshots = elem.hits;
		Vector3 loc = a->GetLocation();
		cm.target_pos = loc;
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
			uint32_t vtable_tgt = a->GetVtableFxn(0x4a8);
			auto ApplyHit = (void (__thiscall*)(void*, CombatMessage*))(vtable_tgt);
			ApplyHit(a->GetEntityPointer(), &cm);
		}
		else
		{
			(*FOTApplyHits)(&cm, elem.mult);
		}

		
	}

	return ret;

}

int8_t HookExecutor::OnCheckItemAllowed(void* actor, void* item)
{
	if (!World::IsLoaded()) return -1;
	int8_t ret = -1;		// -1 = perform the normal check
	lua_getglobal(lua_, "OnCheckItemAllowed");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return ret;
	}
	Entity::GetEntityByPointer(actor)->MakeLuaObject(lua_);
	Entity::GetEntityByPointer(item)->MakeLuaObject(lua_);

	if (lua_pcall(lua_, 2, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return ret;
	}

	if (lua_isboolean(lua_, -1))
	{
		ret = (lua_toboolean(lua_, -1)) ? 1 : 0;
	}
	lua_pop(lua_, 1);
	return ret;

}

void HookExecutor::OnEquip(void* equipper, void* item, int slot)
{
	if (!World::IsLoaded()) return;
	lua_getglobal(lua_, "OnEquip");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	Entity::GetEntityByPointer(equipper)->MakeLuaObject(lua_);
	Entity::GetEntityByID(*(EntityID*)item)->MakeLuaObject(lua_);
	lua_pushinteger(lua_, slot);
	
	if (lua_pcall(lua_, 3, 0, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
	}
}

wchar_t* HookExecutor::OnCheckEquip(void* equipper, void* item, int slot)
{
	if (!World::IsLoaded()) return nullptr;
	lua_getglobal(lua_, "OnCheckEquip");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return nullptr;
	}

	Entity::GetEntityByPointer(equipper)->MakeLuaObject(lua_);
	Entity::GetEntityByID(*(EntityID*)item)->MakeLuaObject(lua_);
	lua_pushinteger(lua_, slot);

	if (lua_pcall(lua_, 3, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return nullptr;
	}

	if (lua_isstring(lua_, -1))
	{
		std::string errmsg = lua_tostring(lua_, -1);
		auto wchar_txt = Helpers::UTF8ToWcharFOTHeap(errmsg, 1);
		lua_pop(lua_, 1);
		return wchar_txt;
	}
	lua_pop(lua_, 1);

	return nullptr;

}

void HookExecutor::OnUnequip(void* equipper, void* item, int slot)
{
	if (!World::IsLoaded()) return;
	lua_getglobal(lua_, "OnUnequip");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	Entity::GetEntityByPointer(equipper)->MakeLuaObject(lua_);
	Entity::GetEntityByPointer(item)->MakeLuaObject(lua_);
	lua_pushinteger(lua_, slot);

	if (lua_pcall(lua_, 3, 0, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
	}
}

wchar_t* HookExecutor::OnCheckUnequip(void* equipper, void* item, int slot)
{
	if (!World::IsLoaded()) return nullptr;
	lua_getglobal(lua_, "OnCheckUnequip");
	if (!lua_isfunction(lua_, -1))
	{
		lua_pop(lua_, 1);
		return nullptr;
	}

	Entity::GetEntityByPointer(equipper)->MakeLuaObject(lua_);
	Entity::GetEntityByID(*(EntityID*)item)->MakeLuaObject(lua_);
	lua_pushinteger(lua_, slot);

	if (lua_pcall(lua_, 3, 1, 0) == LUA_ERRRUN)
	{
		(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
		lua_pop(lua_, 1);
		return nullptr;
	}

	if (lua_isstring(lua_, -1))
	{
		std::string errmsg = lua_tostring(lua_, -1);
		auto wchar_txt = Helpers::UTF8ToWcharFOTHeap(errmsg, 1);
		lua_pop(lua_, 1);
		return wchar_txt;
	}
	lua_pop(lua_, 1);
	return nullptr;

}

template<typename T>
size_t constexpr ConvertFunction2(T fxn)
{
	size_t* ret = reinterpret_cast<size_t*>(&fxn);
	return *ret;
}

void HookExecutor::SetupVtableHookTemplates()
{
	// See the template definitions in EntityVtable.h for naming convention
	VtableHookTemplates_[0] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<RGBColor, Vector3*, float>);								// RGBColor* (*GetRenderLightLevel)(struct Entity *, struct RGBColor *, struct LocationXYZFloat *, float));
	VtableHookTemplates_[1] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*RenderObjectMaybe)(struct Entity *, void *, int *);
	VtableHookTemplates_[2] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);																// void (*InitDefaultEntityFields)(struct Entity *));
	VtableHookTemplates_[3] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<wchar_t**, bool>);												// bool (*SetAnimationType)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[4] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<wchar_t**, bool>);												// bool (*UpdateSprite)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[5] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);																// void (*Vtable5NoOp)(struct Entity *));
	VtableHookTemplates_[6] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);																// void (*InitEntityAfterSpawn)(struct Entity *));
	VtableHookTemplates_[7] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);																// void (*PrepareEntity)(struct Entity *));
	VtableHookTemplates_[8] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);																// void (*EraseInventory)(struct Entity *));
	VtableHookTemplates_[9] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);																// void (*MakeFreshCharacter)(struct Entity *));
	VtableHookTemplates_[10] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*DestructEntity)(struct Entity *));
	VtableHookTemplates_[11] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*SetDoFade)(struct Entity *));
	VtableHookTemplates_[12] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*AliasToDestructEntity)(struct Entity *));
	VtableHookTemplates_[13] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*SomethingWithBoundingBoxAndCollision)(struct Entity *));
	VtableHookTemplates_[14] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*FlagCaptureFunction1)(struct Entity *));
	VtableHookTemplates_[15] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*AttachNewFileToActor)(struct Entity *, void *));
	VtableHookTemplates_[16] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*AttachNewFileToActor2)(struct Entity *));
	VtableHookTemplates_[17] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*SomethingNewSpawnRelated)(struct Entity *));
	VtableHookTemplates_[18] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*Vtable18EquipRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[19] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*AdjustSpriteOffsetForRotatingDoor)(struct Entity *));
	VtableHookTemplates_[20] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*FinishedAnimationSequenceMaybe)(struct Entity *));
	VtableHookTemplates_[21] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable21NoOp)(struct Entity *));
	VtableHookTemplates_[22] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable22NoOp)(struct Entity *));
	VtableHookTemplates_[23] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable23NoOp)(struct Entity *));
	VtableHookTemplates_[24] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // float (*RenderRelated)(struct Entity *, void *, int *));
	VtableHookTemplates_[25] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*CheckThinkTimers)(struct Entity *));
	VtableHookTemplates_[26] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*DoEngineThink)(struct Entity *));
	VtableHookTemplates_[27] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*DoGameThink)(struct Entity *));
	VtableHookTemplates_[28] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SoundRelated1)(struct Entity *, int));
	VtableHookTemplates_[29] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);													// FOTString ** (*GetSoundType)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[30] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetRicochetSoundCounters)(struct Entity *, int));
	VtableHookTemplates_[31] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<Vector3>);													// int* (*SetParameterToZero)(struct Entity *, int *));
	VtableHookTemplates_[32] = ConvertFunction2(&HookExecutor::vtable_hook_template_X32);																// void (*InitLightParameters)(struct Entity *, float *));
	VtableHookTemplates_[33] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetAttached)(struct Entity *, struct EntityID));
	VtableHookTemplates_[34] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t**>);													// void (*SetCustomName)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[35] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString ** (*GetCustomName)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[36] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString ** (*GetDisplayName)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[37] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString ** (*GetHealthStatusString)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[38] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString ** (*Vtable38NoOpEmptyStringRet)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[39] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);													// FOTString ** (*GetUseActionName)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[40] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);														// bool (*CanBeHeldInHand)(struct Entity *));
	VtableHookTemplates_[41] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsTimerMine)(struct Entity *));
	VtableHookTemplates_[42] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsTriggerTrap)(struct Entity *));
	VtableHookTemplates_[43] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsCollectable)(struct Entity *));
	VtableHookTemplates_[44] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsSingleUseItem)(struct Entity *));
	VtableHookTemplates_[45] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsSkillConsumable)(struct Entity *));
	VtableHookTemplates_[46] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsArmour)(struct Entity *));
	VtableHookTemplates_[47] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsWeapon)(struct Entity *));
	VtableHookTemplates_[48] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsAmmo)(struct Entity *));
	VtableHookTemplates_[49] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsKey)(struct Entity *));
	VtableHookTemplates_[50] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsContainer)(struct Entity *));
	VtableHookTemplates_[51] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsUnarmedAttack)(struct Entity *));
	VtableHookTemplates_[52] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsMeleeAttack)(struct Entity *));
	VtableHookTemplates_[53] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsBaseAI)(struct Entity *));
	VtableHookTemplates_[54] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsController)(struct Entity *));
	VtableHookTemplates_[55] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsActor)(struct Entity *));
	VtableHookTemplates_[56] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsActorUnk20Set)(struct Entity *));
	VtableHookTemplates_[57] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsLight)(struct Entity *));
	VtableHookTemplates_[58] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsVehicle)(struct Entity *));
	VtableHookTemplates_[59] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsVehicleController)(struct Entity *));
	VtableHookTemplates_[60] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsVehicleWeapon)(struct Entity *));
	VtableHookTemplates_[61] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*CanBeActivated)(struct Entity *));
	VtableHookTemplates_[62] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsDeadOrDestroyedMaybe)(struct Entity *));
	VtableHookTemplates_[63] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsAttachedBaseAIMaybe)(struct Entity *));
	VtableHookTemplates_[64] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsLiving)(struct Entity *));
	VtableHookTemplates_[65] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsTrap)(struct Entity *));
	VtableHookTemplates_[66] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);														// bool (*IsSentryAI)(struct Entity *));
	VtableHookTemplates_[67] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsGeiger)(struct Entity *));
	VtableHookTemplates_[68] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsWaypoint)(struct Entity *));
	VtableHookTemplates_[69] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsSpawnpoint)(struct Entity *));
	VtableHookTemplates_[70] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsAlarm)(struct Entity *));
	VtableHookTemplates_[71] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsCombatFX)(struct Entity *));
	VtableHookTemplates_[72] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsSwitch)(struct Entity *));
	VtableHookTemplates_[73] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsDoor)(struct Entity *));
	VtableHookTemplates_[74] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsGunTurret)(struct Entity *));
	VtableHookTemplates_[75] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsBrainJar)(struct Entity *));
	VtableHookTemplates_[76] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsBreakableType)(struct Entity *));
	VtableHookTemplates_[77] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsCaptureFlag)(struct Entity *));
	VtableHookTemplates_[78] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsTrapDoor)(struct Entity *));
	VtableHookTemplates_[79] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsCyborgGeneral)(struct Entity *));
	VtableHookTemplates_[80] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsPopupTurret)(struct Entity *));
	VtableHookTemplates_[81] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsNotCaptureInvItem)(struct Entity *));
	VtableHookTemplates_[82] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);													// FOTString * (*RetTypeStringForCarryableEntity)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[83] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, EntityID, EntityID>);					// UseItemResponse * (*MaybeUseEntity)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID));
	VtableHookTemplates_[84] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<EntityID, int>);												// EntityID * (*SplitEntity)(struct Entity *, struct EntityID *, dword));
	VtableHookTemplates_[85] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<EntityID, int, bool>);											// bool (*MergeEntities)(struct Entity *, struct EntityID, dword));
	VtableHookTemplates_[86] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);												// bool (*MaybeCheckingIfMergeableAndMatching)(struct Entity *, struct EntityID));
	VtableHookTemplates_[87] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);														// bool (*CanBeRemoved)(struct Entity *));
	VtableHookTemplates_[88] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetCanBeRemovedFlag)(struct Entity *, bool));
	VtableHookTemplates_[89] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetBeenAppliedFlag)(struct Entity *, bool));
	VtableHookTemplates_[90] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);														// bool (*IsUnique)(struct Entity *));
	VtableHookTemplates_[91] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetIsUniqueFlag)(struct Entity *, bool));
	VtableHookTemplates_[92] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<AttributesTable*, bool>);										// bool (*MeetRequirementsCheck)(struct Entity *, struct AttributesArray *));
	VtableHookTemplates_[93] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop);	// Stats * (*GetItemSPECIALRequired)(struct Entity *));   // (Maybe editor only?)
	VtableHookTemplates_[94] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<EntityID, int>);												// void (*ApplyEquip)(struct Entity *, struct EntityID, int));
	VtableHookTemplates_[95] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<EntityID, int>);												// void (*UnApply)(struct Entity *, struct EntityID, dword));
	VtableHookTemplates_[96] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetTrapHolder)(struct Entity *, struct EntityID));
	VtableHookTemplates_[97] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*ClearHolder)(struct Entity *, struct EntityID));
	VtableHookTemplates_[98] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetCount)(struct Entity *));
	VtableHookTemplates_[99] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetCountNonLootable)(struct Entity *));
	VtableHookTemplates_[100] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsLootable)(struct Entity *));
	VtableHookTemplates_[101] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetUnk25EntityID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[102] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetValueBaseOfItems)(struct Entity *));
	VtableHookTemplates_[103] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetCollectableRankReq)(struct Entity *));
	VtableHookTemplates_[104] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetWeight)(struct Entity *));
	VtableHookTemplates_[105] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetCount)(struct Entity *, int));
	VtableHookTemplates_[106] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<int, int>);													// int (*SetNonLootableCount)(struct Entity *, int));
	VtableHookTemplates_[107] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BreakEntity)(struct Entity *));
	VtableHookTemplates_[108] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsBroken)(struct Entity *));
	VtableHookTemplates_[109] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*RepairEntity)(struct Entity *));
	VtableHookTemplates_[110] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetTrapTotalDifficulty)(struct Entity *));
	VtableHookTemplates_[111] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable111Controller)(struct Entity *));
	VtableHookTemplates_[112] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// bool (*Vtable112Controller)(struct Entity *, struct EntityID));
	VtableHookTemplates_[113] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// bool (*Vtable113Controller)(struct Entity *, struct EntityID));
	VtableHookTemplates_[114] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerMoveTypePriority)(struct Entity *, dword));
	VtableHookTemplates_[115] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<ZoneDefinition*>);											// void (*SetControllerWorkingZone)(struct Entity *, struct ZoneDefinition *));
	VtableHookTemplates_[116] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<ZoneDefinition*>);											// void (*SetControllerRetreatZone)(struct Entity *, struct ZoneDefinition *));
	VtableHookTemplates_[117] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*SetControllerMoveTargetTag)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[118] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerAggression)(struct Entity *, int));
	VtableHookTemplates_[119] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerCommunicate)(struct Entity *, int));
	VtableHookTemplates_[120] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerMobility)(struct Entity *, int));
	VtableHookTemplates_[121] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<bool, int>);													// void (*SetControllerRandomMove)(struct Entity *, bool, byte));
	VtableHookTemplates_[122] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetControllerMoveTypePriority)(struct Entity *));
	VtableHookTemplates_[123] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<ZoneDefinition>);											// void (*GetControllerWorkingZone)(struct Entity *, struct ZoneDefinition *));
	VtableHookTemplates_[124] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<ZoneDefinition>);											// void (*GetControllerRetreatZone)(struct Entity *, struct ZoneDefinition *));
	VtableHookTemplates_[125] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetControllerMoveTargetTag)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[126] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetControllerAggression)(struct Entity *));
	VtableHookTemplates_[127] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetControllerCommunicate)(struct Entity *));
	VtableHookTemplates_[128] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetControllerMobility)(struct Entity *));
	VtableHookTemplates_[129] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*GetControllerRandomMove)(struct Entity *));
	VtableHookTemplates_[130] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetControllerType)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[131] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*SetControllerType)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[132] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*DeactivateController)(struct Entity *));
	VtableHookTemplates_[133] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*SetControllerWaypointModeAndMoveTargetTag)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[134] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*SetControllerStationary)(struct Entity *));
	VtableHookTemplates_[135] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*SetControllerToPerimeterZone)(struct Entity *));
	VtableHookTemplates_[136] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerToFollow)(struct Entity *, int));
	VtableHookTemplates_[137] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*SetControllerToFlee)(struct Entity *));
	VtableHookTemplates_[138] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*ControllerDoSomethingWithUnitList)(struct Entity *));
	VtableHookTemplates_[139] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*SetControllerUnknown4String)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[140] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetControllerUnknown4String)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[141] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*ControllerWakeUpUnitList)(struct Entity *, int));
	VtableHookTemplates_[142] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<EntityID, int>);												// void (*ControllerIssueTauntForEntity)(struct Entity *, struct EntityID, int));
	VtableHookTemplates_[143] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*ControllerWaypointMaybeTimingRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[144] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<wchar_t*, EntityID>);										// FOTString * (*ControllerGetUnitWaypointActionString)(struct Entity *, struct FOTString *, struct EntityID));
	VtableHookTemplates_[145] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*ControllerWaypointCheckingRepeatFlag)(struct Entity *, struct EntityID));
	VtableHookTemplates_[146] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<EntityID, Vector3*, float>);									// float (*ControllerMoveRelated)(struct Entity *, struct EntityID, struct LocationXYZFloat *));
	VtableHookTemplates_[147] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<EntityID, EntityID, bool>);									// EntityID * (*Vtable147Controller)(struct Entity *, struct EntityID *, struct EntityID, bool));
	VtableHookTemplates_[148] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<EntityID, wchar_t*>);										// EntityID * (*Vtable148Controller)(struct Entity *, struct EntityID *, struct EntityID, struct FOTString *));
	VtableHookTemplates_[149] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<EntityID, int, float>);										// float (*ControllerGetFromUnknownList2)(struct Entity *, struct EntityID, int));
	VtableHookTemplates_[150] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable150Controller)(struct Entity *));
	VtableHookTemplates_[151] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<int, int, float>);											// float (*Vtable151NoOpReturnFloat0)(struct Entity *, int, int));
	VtableHookTemplates_[152] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<int, int, float>);										// float (*Vtable152NoOpReturnFloat0)(struct Entity *, int, int));
	VtableHookTemplates_[153] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<EntityID, Vector3*, float>);									// float (*ControllerMoreMoveRelated)(struct Entity *, struct EntityID, struct LocationXYZFloat *));
	VtableHookTemplates_[154] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<Vector3, EntityID>);										// LocationXYZFloat * (*ControllerWaypointStuff)(struct Entity *, struct LocationXYZFloat *, struct EntityID));
	VtableHookTemplates_[155] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, int64_t>);										// longlong (*ControllerSomethingAboutTookDamage)(struct Entity *, struct EntityID));
	VtableHookTemplates_[156] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<EntityID, Vector3*>);										// void (*Vtable156Controller)(struct Entity *, struct EntityID, struct LocationXYZFloat *));
	VtableHookTemplates_[157] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r<EntityID, Vector3*, bool, bool>);								// bool (*ControllerUsesWorkingZoneAndMoveType)(struct Entity *, struct EntityID, struct LocationXYZFloat *, bool));
	VtableHookTemplates_[158] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BaseAINeedsToSwitchTargetMaybe)(struct Entity *));
	VtableHookTemplates_[159] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*BaseAIEngineThinkRelated)(struct Entity *, int *));
	VtableHookTemplates_[160] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetBaseAIController)(struct Entity *, struct EntityID));
	VtableHookTemplates_[161] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetBaseAIController)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[162] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*Vtable162BaseAI)(struct Entity *, int *));
	VtableHookTemplates_[163] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetBaseAINature)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[164] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*StopMovement)(struct Entity *, bool));
	VtableHookTemplates_[165] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*BaseAIGetCurrentTarget)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[166] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BaseAIHasATargetMaybe)(struct Entity *));
	VtableHookTemplates_[167] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsSleeping)(struct Entity *));
	VtableHookTemplates_[168] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetSleepingFlag)(struct Entity *, bool));
	VtableHookTemplates_[169] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<float>);													// void (*SetBaseAIUnknown7)(struct Entity *, float));
	VtableHookTemplates_[170] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetBaseAIUnknown7ToRealTimeTimerValue)(struct Entity *, int));
	VtableHookTemplates_[171] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*BaseAIStopMovementIfHighestDesireSet)(struct Entity *));
	VtableHookTemplates_[172] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // bool (*BaseAIMaybeOpenDoorRelated)(struct Entity *, struct BaseAIEntityPlus4Struct *));
	VtableHookTemplates_[173] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*Vtable173BaseAI)(struct Entity *, struct BaseAIEntityPlus4Struct *));
	VtableHookTemplates_[174] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*Vtable174BaseAI)(struct Entity *, struct EntityID));
	VtableHookTemplates_[175] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetBaseAITauntTypeString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[176] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetBaseAIHighestDesireRoundedUp)(struct Entity *));
	VtableHookTemplates_[177] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*ClearSentryAITargets)(struct Entity *));
	VtableHookTemplates_[178] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetSentryMode)(struct Entity *, int));
	VtableHookTemplates_[179] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetSentryMode)(struct Entity *));
	VtableHookTemplates_[180] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetSentryAccuracyTrigger)(struct Entity *, int));
	VtableHookTemplates_[181] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetSentryAccuracyTrigger)(struct Entity *));
	VtableHookTemplates_[182] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetSentryLocation)(struct Entity *, int));
	VtableHookTemplates_[183] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetSentryLocation)(struct Entity *));
	VtableHookTemplates_[184] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<EntityID, int>);												// void (*SetSentryUserTarget)(struct Entity *, struct EntityID, int));
	VtableHookTemplates_[185] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SentryAIMaybeCheckingTargets)(struct Entity *, struct EntityID));
	VtableHookTemplates_[186] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // ActorSPECIALCopyAndFields * (*GetActorCopyofSPECIALStats)(struct Entity *));
	VtableHookTemplates_[187] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*CopySomeActorFields)(struct Entity *, struct ActorSPECIALCopyAndFields *));
	VtableHookTemplates_[188] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // bool (*CopySomeActorFields2)(struct Entity *, struct ActorSPECIALCopyAndFields *, void *));
	VtableHookTemplates_[189] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // bool (*Vtable189Actor)(struct Entity *, void *, void *, void *));
	VtableHookTemplates_[190] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetActorStartIndex)(struct Entity *));
	VtableHookTemplates_[191] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetActorStartIndex)(struct Entity *, int));
	VtableHookTemplates_[192] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetActorFractionOfHitPointsLost)(struct Entity *));
	VtableHookTemplates_[193] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*CheckBestHPRatioIfUseHealingItemMaybe)(struct Entity *));
	VtableHookTemplates_[194] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*ActorHasItemToPermBoostStats)(struct Entity *));
	VtableHookTemplates_[195] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*ActorHasItemToPermBoostStats2)(struct Entity *));
	VtableHookTemplates_[196] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetActorPoisonPointsTimes15Cap90)(struct Entity *));
	VtableHookTemplates_[197] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*ActorLooksLikeNoOp)(struct Entity *));
	VtableHookTemplates_[198] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetActorRadPointsDivided10Cap90)(struct Entity *));
	VtableHookTemplates_[199] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*ActorCheckItemsForRadHealing)(struct Entity *));
	VtableHookTemplates_[200] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<int, int>);													// void (*MakeConsumableGiveRadiation)(struct Entity *, int, int));
	VtableHookTemplates_[201] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<int, int>);												// void (*MakeConsumableGivePoison)(struct Entity *, int, int));
	VtableHookTemplates_[202] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*ApplyBonusesToAttachedActor)(struct Entity *));
	VtableHookTemplates_[203] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*GetExpectedHPRatioIfConsumableUsed)(struct Entity *, struct EntityID));
	VtableHookTemplates_[204] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*DoesConsumablePermBoostSPEAL)(struct Entity *, struct EntityID));
	VtableHookTemplates_[205] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*Vtable205NoOpRetZeroFloat)(struct Entity *));
	VtableHookTemplates_[206] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*GetRatioOfRadPointHealingForConsumable)(struct Entity *, struct EntityID));
	VtableHookTemplates_[207] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable207ActorMaybeCheckForPlayerCharacter)(struct Entity *));
	VtableHookTemplates_[208] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetRankString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[209] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetActorRankID)(struct Entity *));
	VtableHookTemplates_[210] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetRanksGivenEntityHasAboveThis)(struct Entity *, struct EntityID));
	VtableHookTemplates_[211] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetReputationString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[212] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetCappedReputation)(struct Entity *));
	VtableHookTemplates_[213] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetMissionsCom)(struct Entity *));
	VtableHookTemplates_[214] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*IncrementMissionsCom)(struct Entity *, int));
	VtableHookTemplates_[215] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*AddToReputation)(struct Entity *, int));
	VtableHookTemplates_[216] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsGlowing)(struct Entity *));
	VtableHookTemplates_[217] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // int (*GetActorAge)(struct Entity *, struct Traits *));
	VtableHookTemplates_[218] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<wchar_t**, wchar_t**>);										// void (*SetCurrentActionStringMaybe)(struct Entity *, struct FOTString **, struct FOTString **));
	VtableHookTemplates_[219] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetCurrentActionString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[220] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsIdle)(struct Entity *));
	VtableHookTemplates_[221] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsMoving_WalkRunClimbLadderOrDriving)(struct Entity *));
	VtableHookTemplates_[222] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsClimbing)(struct Entity *));
	VtableHookTemplates_[223] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsIdleOrWalkRun)(struct Entity *));
	VtableHookTemplates_[224] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*HasFallen)(struct Entity *));
	VtableHookTemplates_[225] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable225ThinkRelated)(struct Entity *));
	VtableHookTemplates_[226] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanMove)(struct Entity *));
	VtableHookTemplates_[227] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanRun)(struct Entity *));
	VtableHookTemplates_[228] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);													// void (*AdjustStanceDownward)(struct Entity *, bool));
	VtableHookTemplates_[229] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);													// void (*AdjustStanceUpward)(struct Entity *, bool));
	VtableHookTemplates_[230] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsHumanoidOrDog)(struct Entity *));
	VtableHookTemplates_[231] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*MaxMovementSpeedMaybe)(struct Entity *));
	VtableHookTemplates_[232] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsEncumbered)(struct Entity *));
	VtableHookTemplates_[233] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsStanding)(struct Entity *));
	VtableHookTemplates_[234] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsCrouching)(struct Entity *));
	VtableHookTemplates_[235] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsProne)(struct Entity *));
	VtableHookTemplates_[236] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetSelectedStance)(struct Entity *));
	VtableHookTemplates_[237] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*ActoSetSelectedStance)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[238] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsHidden)(struct Entity *));
	VtableHookTemplates_[239] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetPostureString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[240] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);													// void (*SetSneakState)(struct Entity *, bool));
	VtableHookTemplates_[241] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsBurrowed)(struct Entity *));
	VtableHookTemplates_[242] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);													// void (*SetBurrowingState)(struct Entity *, bool));
	VtableHookTemplates_[243] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<wchar_t**, int, bool>);										// bool (*RollAgainstStat)(struct Entity *, struct FOTString *, int));
	VtableHookTemplates_[244] = ConvertFunction2(&HookExecutor::vtable_hook_template_4r1<InventoryActionResult, wchar_t**, EntityID, EntityID>);		// UseItemResponse * (*ApplySkillToTargetEntity)(struct Entity *, struct UseItemResponse *, struct FOTString *, struct EntityID, struct EntityID));
	VtableHookTemplates_[245] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSkillStringBasedOnUnk25c)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[246] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetActorUnk2EntityID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[247] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetAdjustedPilotSkill)(struct Entity *));
	VtableHookTemplates_[248] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<wchar_t**, bool>);											// bool (*DoesSkillStringMatchHelperItem)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[249] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<wchar_t**, bool>);											// bool (*IsValidTargetForSkill)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[250] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<EntityID, EntityID, bool>);									// bool (*IsValidTargetForDefaultAction)(struct Entity *, struct EntityID, struct EntityID));
	VtableHookTemplates_[251] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsGeneral)(struct Entity *));
	VtableHookTemplates_[252] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsRecruitMaster)(struct Entity *));
	VtableHookTemplates_[253] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsQuartermaster)(struct Entity *));
	VtableHookTemplates_[254] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanBarterWith)(struct Entity *));
	VtableHookTemplates_[255] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BartersWithBOSScript)(struct Entity *));
	VtableHookTemplates_[256] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanGambleWith)(struct Entity *));
	VtableHookTemplates_[257] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetRepairObjectDifficulty)(struct Entity *));
	VtableHookTemplates_[258] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<float, float>);												// float (*GetScaledKnockdownChance)(struct Entity *, float));
	VtableHookTemplates_[259] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*ActorKnockoutRelated)(struct Entity *));
	VtableHookTemplates_[260] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, int, int>);							// UseItemResponse * (*ActorInjureLocation)(struct Entity *, struct UseItemResponse *, int, int));
	VtableHookTemplates_[261] = ConvertFunction2(&HookExecutor::vtable_hook_template_4r1<InventoryActionResult, int, int, int>);					// UseItemResponse * (*ActorKnockout)(struct Entity *, struct UseItemResponse *, int, int, int));
	VtableHookTemplates_[262] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, int, int>);						// UseItemResponse * (*ActorDisarm)(struct Entity *, struct UseItemResponse *, int, int));
	VtableHookTemplates_[263] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, int, int>);						// UseItemResponse * (*ActorBreakWeaponMaybe)(struct Entity *, struct UseItemResponse *, int, int));
	VtableHookTemplates_[264] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, int>);							// UseItemResponse * (*ActorCriticalMissHitSelfMaybe)(struct Entity *, struct UseItemResponse *, int));
	VtableHookTemplates_[265] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, int>);							// UseItemResponse * (*ActorCriticalMissHitSelfMaybe2)(struct Entity *, struct UseItemResponse *, int));
	VtableHookTemplates_[266] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*StunActor)(struct Entity *, int));
	VtableHookTemplates_[267] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>);												// bool (*IsRightLegInjured)(struct Entity *, bool));
	VtableHookTemplates_[268] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>);												// bool (*IsLeftLegInjured)(struct Entity *, bool));
	VtableHookTemplates_[269] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsRightArmInjured)(struct Entity *, bool));
	VtableHookTemplates_[270] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsLeftArmInjured)(struct Entity *, bool));
	VtableHookTemplates_[271] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsBlinded)(struct Entity *, bool));
	VtableHookTemplates_[272] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsWinded)(struct Entity *, bool));
	VtableHookTemplates_[273] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsImmobilized)(struct Entity *, bool));
	VtableHookTemplates_[274] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsConcussed)(struct Entity *, bool));
	VtableHookTemplates_[275] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsStunned)(struct Entity *, bool));
	VtableHookTemplates_[276] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsUnconscious)(struct Entity *));
	VtableHookTemplates_[277] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsBandagedStatus)(struct Entity *));
	VtableHookTemplates_[278] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<Vector3*, int>); 											// int (*GetActorMovementCostToLocation)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[279] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsActorUnk23SetPlusSomeOtherFieldFalse)(struct Entity *));
	VtableHookTemplates_[280] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Move)(struct Entity *));
	VtableHookTemplates_[281] = ConvertFunction2(&HookExecutor::vtable_hook_template_5<Vector3*, int, float, float, bool>);							// void (*SetDestination)(struct Entity *, struct LocationXYZFloat *, int, float, float, bool));
	VtableHookTemplates_[282] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<Vector3>);												// LocationXYZFloat * (*GetLocationBasedOnUnk23)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[283] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*ActorGetUnk24bEntityID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[284] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<Vector3*, bool, bool>);										// bool (*ActorSomethingTouchSearch)(struct Entity *, struct LocationXYZFloat *, bool));
	VtableHookTemplates_[285] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<Vector3*, bool, bool>);									// bool (*ActorSomethingTouchSearch2)(struct Entity *, struct LocationXYZFloat *, bool));
	VtableHookTemplates_[286] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<Vector3*, bool, bool>);									// bool (*ActorSomethingTouchSearch3)(struct Entity *, struct LocationXYZFloat *, bool));
	VtableHookTemplates_[287] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable287NoOpRet0)(struct Entity *));
	VtableHookTemplates_[288] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsLegalTarget)(struct Entity *));
	VtableHookTemplates_[289] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*ActorIsNotLegalTargetAndUnk1Set)(struct Entity *));
	VtableHookTemplates_[290] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsVehicleDisabled)(struct Entity *));
	VtableHookTemplates_[291] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, EntityID, int>);						// UseItemResponse * (*ActorPickupItem)(struct Entity *, struct UseItemResponse *, struct EntityID, int));
	VtableHookTemplates_[292] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable292Actor)(struct Entity *));
	VtableHookTemplates_[293] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<InventoryActionResult>);									// UseItemResponse * (*ReturnUseItemResponseType0x40)(struct Entity *, struct UseItemResponse *));
	VtableHookTemplates_[294] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<ChanceToHit, EntityID, bool>);								// HitChanceStruct * (*GetSomeKindOfFlagForMouseOverTarget)(struct Entity *, struct HitChanceStruct *, struct EntityID, bool));
	VtableHookTemplates_[295] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<ChanceToHit, Vector3*>);									// HitChanceStruct * (*GetSomeKindOfFlagForMouseOverTarget2)(struct Entity *, struct HitChanceStruct *, struct LocationXYZFloat *));
	VtableHookTemplates_[296] = ConvertFunction2(&HookExecutor::vtable_hook_template_4r1<ChanceToHit, EntityID, int, int>);							// HitChanceStruct * (*BeginAttackAnEntity)(struct Entity *, struct HitChanceStruct *, struct EntityID, int, int));
	VtableHookTemplates_[297] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<ChanceToHit, Vector3*>);									// HitChanceStruct * (*BeginAttackALocation)(struct Entity *, struct HitChanceStruct *, struct LocationXYZFloat *));
	VtableHookTemplates_[298] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<CombatMessage*>);											// void (*ApplyAttackResultToEntity)(struct Entity *, struct CombatMessage *));
	VtableHookTemplates_[299] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<CombatMessage*>);											// void (*ShotAtMissed)(struct Entity *, struct CombatMessage *));
	VtableHookTemplates_[300] = ConvertFunction2(&HookExecutor::vtable_hook_template_6<EntityID, int, int, int, wchar_t*, int>);						// void (*ApplyDamage)(struct Entity *, struct EntityID, int, int, int, struct FOTString *, int));
	VtableHookTemplates_[301] = ConvertFunction2(&HookExecutor::vtable_hook_template_4<EntityID, int, int, wchar_t*>);								// void (*EntityWasKilled)(struct Entity *, struct EntityID, int, int, struct FOTString *));
	VtableHookTemplates_[302] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*IncrementKillCount)(struct Entity *, struct EntityID));
	VtableHookTemplates_[303] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*CopyActorKillList)(struct Entity *, void *));
	VtableHookTemplates_[304] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable304NoOpRet0)(struct Entity *));
	VtableHookTemplates_[305] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<AnimOffset*>);												// void (*ResolveCombatMessageForAttacker)(struct Entity *, AnimOffset *));
	VtableHookTemplates_[306] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetIDOfEquippedItemInActiveSlot)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[307] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetEntityIDOfNonEquippedWeapon)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[308] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<InventoryActionResult>);									// UseItemResponse * (*ReloadActiveWeaponWithAnyAmmo)(struct Entity *, struct UseItemResponse *));
	VtableHookTemplates_[309] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, int, EntityID>);						// UseItemResponse * (*ReloadWeaponWithSelectedAmmo)(struct Entity *, struct UseItemResponse *, int, struct EntityID));
	VtableHookTemplates_[310] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*ActorCanAttackWithActiveWeapon)(struct Entity *));
	VtableHookTemplates_[311] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable311ActorCheckingLastTimeRun)(struct Entity *));
	VtableHookTemplates_[312] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*InitActorCombatMessages)(struct Entity *));
	VtableHookTemplates_[313] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*LevelUp)(struct Entity *, int));
	VtableHookTemplates_[314] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetCombatMessage2TargetID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[315] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<EntityID, EntityID, int>);									// int (*GetPlayerBuyPriceForItem)(struct Entity *, struct EntityID, struct EntityID));
	VtableHookTemplates_[316] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<EntityID, EntityID, int>);								// int (*GetPlayerSellBackPriceForItem)(struct Entity *, struct EntityID, struct EntityID));
	VtableHookTemplates_[317] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetCountOfMatchingItems)(struct Entity *, struct EntityID));
	VtableHookTemplates_[318] = ConvertFunction2(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);		// UseItemResponse * (*EquipItem)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[319] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, int, EntityID>);					// UseItemResponse * (*RemoveItem)(struct Entity *, struct UseItemResponse *, int, struct EntityID *));
	VtableHookTemplates_[320] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, EntityID, EntityID>);				// UseItemResponse * (*RemoveItemIfEquipped)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *));
	VtableHookTemplates_[321] = ConvertFunction2(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);	// UseItemResponse * (*AddToInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[322] = ConvertFunction2(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);	// UseItemResponse * (*RemoveFromInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[323] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, int, EntityID>);					// UseItemResponse * (*UnequipItemFromSlotAndPutInInventory)(struct Entity *, struct UseItemResponse *, int, struct EntityID *));
	VtableHookTemplates_[324] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, EntityID, EntityID>);				// UseItemResponse * (*UnequipItemByIDAndReturnToInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *));
	VtableHookTemplates_[325] = ConvertFunction2(&HookExecutor::vtable_hook_template_4r2<InventoryActionResult, EntityID, EntityID, int>);			// UseItemResponse * (*EquipItemFromInventoryToSlot)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int));
	VtableHookTemplates_[326] = ConvertFunction2(&HookExecutor::vtable_hook_template_4r2<InventoryActionResult, EntityID, EntityID, int>);		// UseItemResponse * (*RemoveFromInventory2)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int));
	VtableHookTemplates_[327] = ConvertFunction2(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);	// UseItemResponse * (*ActorSomethingRelatedToPickup)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[328] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, int, EntityID>);					// UseItemResponse * (*RemoveItemFromSlot2)(struct Entity *, struct UseItemResponse *, int, struct EntityID *));
	VtableHookTemplates_[329] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, EntityID, EntityID>);				// UseItemResponse * (*RemoveItemIfEquipped2)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *));
	VtableHookTemplates_[330] = ConvertFunction2(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);	// UseItemResponse * (*PickupItemAndEquipInSlot)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[331] = ConvertFunction2(&HookExecutor::vtable_hook_template_4r2<InventoryActionResult, EntityID, EntityID, int>);		// UseItemResponse * (*RemoveItemFromEquippedThenInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int));
	VtableHookTemplates_[332] = ConvertFunction2(&HookExecutor::vtable_hook_template_4r2<InventoryActionResult, EntityID, EntityID, int>);		// UseItemResponse * (*RemoveItemFromInventoryThenEquipped)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int));
	VtableHookTemplates_[333] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, int, EntityID>);					// UseItemResponse * (*Vtable333SomethingForApplyingItem)(struct Entity *, struct UseItemResponse *, int, struct EntityID *));
	VtableHookTemplates_[334] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetActiveWeaponHand)(struct Entity *, char));
	VtableHookTemplates_[335] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*ToggleActiveHand)(struct Entity *));
	VtableHookTemplates_[336] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*CheckIfTargetInReachableRange)(struct Entity *, struct EntityID));
	VtableHookTemplates_[337] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<uint8_t>);													// byte (*GetSelectedHand)(struct Entity *));
	VtableHookTemplates_[338] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<uint8_t>);													// byte (*GetUnselectedHand)(struct Entity *));
	VtableHookTemplates_[339] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<EntityID, int>);											// EntityID * (*GetIDofEquippedItemInSlot)(struct Entity *, struct EntityID *, int));
	VtableHookTemplates_[340] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, EntityID>);							// UseItemResponse * (*Vtable340InventoryCombineRelatedMaybe)(struct Entity *, struct UseItemResponse *, struct EntityID));
	VtableHookTemplates_[341] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<wchar_t*, bool>);											// bool (*PlayerEquippedItemWithTagName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[342] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*IsItemIDEquipped)(struct Entity *, struct EntityID));
	VtableHookTemplates_[343] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<wchar_t*, bool>);											// bool (*ContainsItemWithTagName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[344] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*ContainsItemByID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[345] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<wchar_t*, bool>);											// bool (*EquippedOrContainsItemWithTagName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[346] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*FindIfEquippedOrCarriedItemByID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[347] = ConvertFunction2(&HookExecutor::vtable_hook_template_4r1<InventoryActionResult, int, int, uint8_t>);					// UseItemResponse * (*Vtable347ActorSomethingAboutUsingItem)(struct Entity *, struct UseItemResponse *, word, word, byte));
	VtableHookTemplates_[348] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, EntityID>);							// UseItemResponse * (*CanUseItem)(struct Entity *, struct UseItemResponse *, struct EntityID));
	VtableHookTemplates_[349] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, EntityID>);						// UseItemResponse * (*CanReachInteractable)(struct Entity *, struct UseItemResponse *, struct EntityID));
	VtableHookTemplates_[350] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSizeStringMaybe)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[351] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<AttributesTable*>);										// AttributesArray * (*GetCurrentStats)(struct Entity *));
	VtableHookTemplates_[352] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<AttributesTable*>);										// AttributesArray * (*GetCopyStats)(struct Entity *));
	VtableHookTemplates_[353] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<AttributesTable*>);										// AttributesArray * (*GetTempStats)(struct Entity *));
	VtableHookTemplates_[354] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<AttributesTable*>);										// AttributesArray * (*GetBaseStats)(struct Entity *));
	VtableHookTemplates_[355] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<AttributesTable*>);											// void (*CopyAttributesToBaseStats)(struct Entity *, struct AttributesArray *));
	VtableHookTemplates_[356] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<ActorStatus*>);											// ActorStatus * (*GetActorStatus)(struct Entity *));
	VtableHookTemplates_[357] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<ActorStatus*>);												// void (*CopyActorStatus)(struct Entity *, struct ActorStatus *));
	VtableHookTemplates_[358] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<float>);													// void (*AdjustAP)(struct Entity *, float));
	VtableHookTemplates_[359] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*AdjustAP2)(struct Entity *, int));
	VtableHookTemplates_[360] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetCurrentAP)(struct Entity *));
	VtableHookTemplates_[361] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetAPAvailablePercentage)(struct Entity *));
	VtableHookTemplates_[362] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*ActorUnconsciousAPHandling)(struct Entity *, bool));
	VtableHookTemplates_[363] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsMale)(struct Entity *));
	VtableHookTemplates_[364] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsFemale)(struct Entity *));
	VtableHookTemplates_[365] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSexString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[366] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetEntityCharacterTypeString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[367] = ConvertFunction2(&HookExecutor::vtable_hook_template_3<AttributesTable*, int, float>);								// void (*AddTempBonuses)(struct Entity *, struct AttributesArray *, dword, float));
	VtableHookTemplates_[368] = ConvertFunction2(&HookExecutor::vtable_hook_template_3<AttributesTable*, int, float>);							// void (*RemoveTempBonuses)(struct Entity *, struct AttributesArray *, dword, float));
	VtableHookTemplates_[369] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r<int, int, wchar_t**, int>);									// int (*IncreaseAttributeByRange)(struct Entity *, int, int, struct FOTString *));
	VtableHookTemplates_[370] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<int, bool>);												// bool (*GrantXP)(struct Entity *, int));
	VtableHookTemplates_[371] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetNPCLevel)(struct Entity *));
	VtableHookTemplates_[372] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*HandleXPForNewPlayerCharacterMaybe)(struct Entity *));
	VtableHookTemplates_[373] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*GiveXPForLevelUp)(struct Entity *));
	VtableHookTemplates_[374] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetExpectedLevelForMyXP)(struct Entity *));
	VtableHookTemplates_[375] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*XPAndValueRelatedCalc)(struct Entity *));
	VtableHookTemplates_[376] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<float>);														// void (*RadiateEntity)(struct Entity *, float));
	VtableHookTemplates_[377] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<float>);														// void (*PoisonEntity)(struct Entity *, flost));
	VtableHookTemplates_[378] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<float>);													// void (*AddOverdosePoints)(struct Entity *, float));
	VtableHookTemplates_[379] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<float>);													// void (*MaybeHealOverTime)(struct Entity *, float));
	VtableHookTemplates_[380] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsAlwaysFriend)(struct Entity *));
	VtableHookTemplates_[381] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*AdjustActorStatusBasedOnEffectChangeEtc)(struct Entity *, dword *, float));
	VtableHookTemplates_[382] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetHP)(struct Entity *));
	VtableHookTemplates_[383] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*HasAddiction)(struct Entity *));
	VtableHookTemplates_[384] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CheckOverdosed)(struct Entity *));
	VtableHookTemplates_[385] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsInWithdrawal)(struct Entity *));
	VtableHookTemplates_[386] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsRadiated)(struct Entity *));
	VtableHookTemplates_[387] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsPoisoned)(struct Entity *));
	VtableHookTemplates_[388] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsDrunk)(struct Entity *));
	VtableHookTemplates_[389] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetWaypointPauseTime)(struct Entity *));
	VtableHookTemplates_[390] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetWaypointIndex)(struct Entity *));
	VtableHookTemplates_[391] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*AlwaysReturnMaxInt)(struct Entity *));
	VtableHookTemplates_[392] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetNextWaypointList)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[393] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetWaypointActionString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[394] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*GetRepeat)(struct Entity *));
	VtableHookTemplates_[395] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*GetWaypointUseDirection)(struct Entity *));
	VtableHookTemplates_[396] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetWaypointIndex)(struct Entity *, int));
	VtableHookTemplates_[397] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<Vector3>);												// LocationXYZFloat * (*GetActorTargetLocationMaybe)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[398] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<Vector3*>);													// void (*SetActorTargetLocationMaybe)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[399] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<Vector3>);												// LocationXYZFloat * (*GetAttackDir)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[400] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetGUISlot)(struct Entity *, int));
	VtableHookTemplates_[401] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetGUISlot)(struct Entity *));
	VtableHookTemplates_[402] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<Inventory::InventoryStructType>);							// Inventory::ClassType * (*GetInventoryList)(struct Entity *, struct Inventory::ClassType *));
	VtableHookTemplates_[403] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetInventoryID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[404] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetInventoryID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[405] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*ClearInventoryObject)(struct Entity *));
	VtableHookTemplates_[406] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetUnk22EntityID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[407] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*ResetUnk22Entity)(struct Entity *, struct EntityID));
	VtableHookTemplates_[408] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*InitUnkFields)(struct Entity *, bool));
	VtableHookTemplates_[409] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetMinDmg)(struct Entity *, struct EntityID));
	VtableHookTemplates_[410] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetMaxDmg)(struct Entity *, struct EntityID));
	VtableHookTemplates_[411] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetWeaponModeByIndex)(struct Entity *, int));
	VtableHookTemplates_[412] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetNumWeaponModes)(struct Entity *));
	VtableHookTemplates_[413] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop);		// WeaponMode * (*GetActiveWeaponMode)(struct Entity *));
	VtableHookTemplates_[414] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>);												// void (*SetNextWeaponMode)(struct Entity *, bool));
	VtableHookTemplates_[415] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<bool, bool>);												// bool (*SetWeaponCalledShot)(struct Entity *, bool));
	VtableHookTemplates_[416] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*WeaponCanUseCalledShot)(struct Entity *));
	VtableHookTemplates_[417] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<int, bool>);												// bool (*CanWeaponAndAttackerUseCalledShot)(struct Entity *, dword));
	VtableHookTemplates_[418] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*UseAmmoForCurrentMode)(struct Entity *));
	VtableHookTemplates_[419] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*HaveEnoughAmmoAndCanShoot)(struct Entity *));
	VtableHookTemplates_[420] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*WeaponHasNonNormalPrimaryDamageTypeMaybe)(struct Entity *));
	VtableHookTemplates_[421] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*ExplodeHeldWeaponMaybe)(struct Entity *));
	VtableHookTemplates_[422] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetAPCost)(struct Entity *, struct EntityID));
	VtableHookTemplates_[423] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, EntityID, EntityID>);				// UseItemResponse * (*WeaponLoadingAmmoRelated)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID));
	VtableHookTemplates_[424] = ConvertFunction2(&HookExecutor::vtable_hook_template_3r1<EntityID, EntityID, EntityID>);								// EntityID * (*MaybeUnloadAmmoFromWeapon)(struct Entity *, struct EntityID *, struct EntityID, struct EntityID));
	VtableHookTemplates_[425] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetAmmoTypeString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[426] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetWeaponPerkString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[427] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetWeaponDamageRadius)(struct Entity *));
	VtableHookTemplates_[428] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetImpactFX)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[429] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSoundFX)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[430] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetProjectileName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[431] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetPrimaryDamageType)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[432] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSecondaryDamageType)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[433] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetAmmoVariantString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[434] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetAmmoOrUsesLeft)(struct Entity *));
	VtableHookTemplates_[435] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);													// int (*GetMaxUsesOrAmmoCapacity)(struct Entity *));
	VtableHookTemplates_[436] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*WeaponIsDestroyUser)(struct Entity *));
	VtableHookTemplates_[437] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r1<EntityID, Inventory::InventoryStructType*>);				// EntityID * (*FindSuitableAmmoInInventory)(struct Entity *, struct EntityID *, struct Inventory::ClassType *));
	VtableHookTemplates_[438] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID*, bool>);											// bool (*ContainsEntity)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[439] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetEffectiveWeaponRangeByRangeType)(struct Entity *));
	VtableHookTemplates_[440] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*GetRange)(struct Entity *, struct EntityID));
	VtableHookTemplates_[441] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetMinRange)(struct Entity *));
	VtableHookTemplates_[442] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetAverageDamage)(struct Entity *));
	VtableHookTemplates_[443] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanWeaponAOE)(struct Entity *));
	VtableHookTemplates_[444] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanWeaponNotAOE)(struct Entity *));
	VtableHookTemplates_[445] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<int, EntityID, bool>);										// bool (*CanFireWithRemainingAP)(struct Entity *, int, struct EntityID));
	VtableHookTemplates_[446] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsActivated)(struct Entity *));
	VtableHookTemplates_[447] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetFrequency)(struct Entity *));
	VtableHookTemplates_[448] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<EntityID, EntityID>);										// void (*ActivateTrapRelated)(struct Entity *, struct EntityID, struct EntityID));
	VtableHookTemplates_[449] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetTrapTimeToDetonation)(struct Entity *));
	VtableHookTemplates_[450] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<int, int>);													// void (*SetTrapFrequencyCode)(struct Entity *, int, int));
	VtableHookTemplates_[451] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*DetonateTrap)(struct Entity *));
	VtableHookTemplates_[452] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*HasActiveTrap)(struct Entity *));
	VtableHookTemplates_[453] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*TrapRelatedEitherDisarmOrTrigger)(struct Entity *, struct EntityID));
	VtableHookTemplates_[454] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*SetContainedTrap)(struct Entity *, struct EntityID));
	VtableHookTemplates_[455] = ConvertFunction2(&HookExecutor::vtable_hook_template_3<EntityID, EntityID, bool>);									// void (*PlaceTrapMaybe)(struct Entity *, struct EntityID, struct EntityID, bool));
	VtableHookTemplates_[456] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*DisarmTrap)(struct Entity *));
	VtableHookTemplates_[457] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// bool (*ActivateOrBreakAlarm)(struct Entity *, struct EntityID));
	VtableHookTemplates_[458] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsDoorOpen)(struct Entity *));
	VtableHookTemplates_[459] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsDoorClosed)(struct Entity *));
	VtableHookTemplates_[460] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsLocked)(struct Entity *));
	VtableHookTemplates_[461] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*CanDoorOpen)(struct Entity *, struct EntityID));
	VtableHookTemplates_[462] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*Vtable462DoorRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[463] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<CombatMessage*, int>);										// void (*SetupCombatFXCombatMessageStruct)(struct Entity *, struct CombatMessage *, int));
	VtableHookTemplates_[464] = ConvertFunction2(&HookExecutor::vtable_hook_template_6<int, int, int, int, int, int>);									// void (*Vtable464NoOp)(struct Entity *, int, int, int, int, int, int));
	VtableHookTemplates_[465] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<CombatMessage*>);											// void (*Vtable465CombatFX)(struct Entity *, struct CombatMessage *));
	VtableHookTemplates_[466] = ConvertFunction2(&HookExecutor::vtable_hook_template_4<int, int, int, int>);										// void (*Vtable466NoOp)(struct Entity *, int, int, int, int));
	VtableHookTemplates_[467] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<Vector3*>);													// void (*Vtable467CombatFX)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[468] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable468CombatFX)(struct Entity *));
	VtableHookTemplates_[469] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<Vector3*>);												// void (*Vtable469CombatFX)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[470] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<CombatMessage*>);											// void (*CombatFXBloodSprayRelated)(struct Entity *, struct CombatMessage *));
	VtableHookTemplates_[471] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<CombatMessage*, int>);										// void (*CombatFXBloodTrailRelated)(struct Entity *, struct CombatMessage *, int));
	VtableHookTemplates_[472] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*CombatFXBloodPoolRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[473] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<Vector3*, int>);											// void (*SetupScenaryMove)(struct Entity *, struct LocationXYZFloat *, int));
	VtableHookTemplates_[474] = ConvertFunction2(&HookExecutor::vtable_hook_template_6<int, int, int, int, int, int>);							// void (*Vtable474NoOp)(struct Entity *, int, int, int, int, int, int));
	VtableHookTemplates_[475] = ConvertFunction2(&HookExecutor::vtable_hook_template_6<int, int, int, int, int, int>);							// void (*Vtable475NoOp)(struct Entity *, int, int, int, int, int, int));
	VtableHookTemplates_[476] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<int, int, float>);											// float (*Vtable476ReturnFloatZero)(struct Entity *, int, int));
	VtableHookTemplates_[477] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<int, Vector3*, float>);										// float (*Vtable477ActorMaybeWeaponImpactKnockdownRelated)(struct Entity *, int, struct LocationXYZFloat *));
	VtableHookTemplates_[478] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable478NoOp)(struct Entity *));
	VtableHookTemplates_[479] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable479ReturnFalse)(struct Entity *));
	VtableHookTemplates_[480] = ConvertFunction2(&HookExecutor::vtable_hook_template_3<int, int, int>);												// void (*Vtable480DestructsStuff)(struct Entity *, int, int, int));
	VtableHookTemplates_[481] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsVehicleEmpty)(struct Entity *));
	VtableHookTemplates_[482] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*IsThisEntityTheDriver)(struct Entity *, struct EntityID));
	VtableHookTemplates_[483] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*IsThisEntityTheGunner)(struct Entity *, struct EntityID));
	VtableHookTemplates_[484] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*IsThisEntityInVehicle)(struct Entity *, struct EntityID));
	VtableHookTemplates_[485] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetVehicleDriver)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[486] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityIDVector>);											// void (*GetVehiclePassengerVector)(struct Entity *, struct SpecialVectorToPointers *));
	VtableHookTemplates_[487] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // bool (*VehicleBoundingBoxStuff1)(struct Entity *, struct BoundingBoxIntVehicleSpecial *));
	VtableHookTemplates_[488] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsInReverse)(struct Entity *));
	VtableHookTemplates_[489] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<EntityID, Vector3*>);										// bool (*VehiclePassengerDisembarkAllowed)(struct Entity *, struct EntityID, struct LocationXYZFloat *));
	VtableHookTemplates_[490] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<Vector3>);												// LocationXYZFloat * (*SetExitPoint)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[491] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsNotDeactivated)(struct Entity *));
	VtableHookTemplates_[492] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*ActivateActor)(struct Entity *));
	VtableHookTemplates_[493] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*DeactivateActor)(struct Entity *));
	VtableHookTemplates_[494] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*Vtable494ActorEffectsRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[495] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*RemoveEffectID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[496] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*ApplyEffectsWithGivenTagName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[497] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BoundingBox2dSet)(struct Entity *));
	VtableHookTemplates_[498] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<EntityIDVector>);											// void (*GetListOfContainedEntities)(struct Entity *, struct SpecialVectorToPointers *));
	VtableHookTemplates_[499] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<wchar_t**, ZoneDefinition*, bool>);							// bool (*ScriptConditionCheckMaybe)(struct Entity *, struct FOTString **, ZoneDefinition *));
	VtableHookTemplates_[500] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*KillEntity)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[501] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetActivationState)(struct Entity *, int));
	VtableHookTemplates_[502] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsInActiveState)(struct Entity *));
	VtableHookTemplates_[503] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsEnabledForScriptEvents)(struct Entity *));
	VtableHookTemplates_[504] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);													// void (*SetLockedState)(struct Entity *, bool));
	VtableHookTemplates_[505] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*Vtable505ActorCallsWorldForSomething)(struct Entity *));
	VtableHookTemplates_[506] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetRandomSpeechIDString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[507] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetClickSpeechIDString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[508] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*SetRandomSpeechIDString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[509] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*SetClickSpeechIDString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[510] = ConvertFunction2(&HookExecutor::vtable_hook_template_X510); // void (*ActOnControllerCommand)(struct Entity *, struct ControllerCommandStruct *));
	VtableHookTemplates_[511] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable511ActorVehicle)(struct Entity *));
	VtableHookTemplates_[512] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetHavingTurn)(struct Entity *, bool));
	VtableHookTemplates_[513] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsHavingTurn)(struct Entity *));
	VtableHookTemplates_[514] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetOverwatch)(struct Entity *, bool));
	VtableHookTemplates_[515] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsOverwatch)(struct Entity *));
	VtableHookTemplates_[516] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*ResetBonusAC)(struct Entity *));
	VtableHookTemplates_[517] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void (*SetBonusACForHTHEvade)(struct Entity *));
	VtableHookTemplates_[518] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<uint8_t>);													// byte (*GetUnk23)(struct Entity *));
	VtableHookTemplates_[519] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<int, bool>);												// bool (*Vtable519EntityESSRelated)(struct Entity *, int));
	VtableHookTemplates_[520] = ConvertFunction2(&HookExecutor::vtable_hook_template_1r<int, bool>);												// bool (*DoSomethingWithHP)(struct Entity *, dword));
	VtableHookTemplates_[521] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void (*Vtable521ALLSomethingWithAllFields)(struct Entity *, int *));
	VtableHookTemplates_[522] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*Vtable522EntityDunno)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[523] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // classtype_vtable * (*GetClassType)(struct Entity *));
	VtableHookTemplates_[524] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<uint8_t>);													// Entity * (*ResetAndOrDestruct)(struct Entity *, byte));
	VtableHookTemplates_[525] = ConvertFunction2(&HookExecutor::vtable_hook_template_0r<bool>);													// bool(*IsSomethingMDCRelated)(struct Actor *));
	VtableHookTemplates_[526] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // void(*ActorPostureRelated)(struct Actor *, int *));
	VtableHookTemplates_[527] = ConvertFunction2(&HookExecutor::vtable_hook_template_noop); // float * (*RenderUnconscious)(struct Actor *, float *, int, float, bool, bool));
	VtableHookTemplates_[528] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<int, int, float>);											// float(*Vtable528NoOp)(struct Actor *, int, int));
	VtableHookTemplates_[529] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void(*AdjustRadLevelFromRadPoints)(struct Actor *));
	VtableHookTemplates_[530] = ConvertFunction2(&HookExecutor::vtable_hook_template_1<int>);														// void(*ApplyRadLevelPenalties)(struct Actor *, int));
	VtableHookTemplates_[531] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void(*UndoExistingRadLevelPenalties)(struct Entity *));
	VtableHookTemplates_[532] = ConvertFunction2(&HookExecutor::vtable_hook_template_2<float, bool>);												// void(*PoisonEntity2)(struct Entity *, float, bool));
	VtableHookTemplates_[533] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void(*ApplyPoisonDamage)(struct Actor *));
	VtableHookTemplates_[534] = ConvertFunction2(&HookExecutor::vtable_hook_template_0);															// void(*ApplyOverdoseDamage)(struct Actor *));
	VtableHookTemplates_[535] = ConvertFunction2(&HookExecutor::vtable_hook_template_2r<int, int, int>);												// int(*ContestedSkillRollMaybe)(struct Actor *, int, int));
}