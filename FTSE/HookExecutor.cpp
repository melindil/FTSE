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
#include "ui/WMainMenu.h"
using std::shared_ptr;

#include "EntityVtable.h"
#include "ZoneDefinition.h"
#include "FTImprover.h"
#include "GenericPatcher.h"
#include "Version.h"

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
		int dictionary = 3;	// We used to only write into misc dictionary
		if (lua_isinteger(l, 4))
		{
			dictionary = (int)lua_tointeger(l, 4);
		}
		(*he)->AddLocaleString(lua_tostring(l, 2),lua_tostring(l,3),dictionary);
	}
	return 0;
}
int l_getlocalestring(lua_State* l)
{
	HookExecutor** he = (HookExecutor**)luaL_checkudata(l, 1, "HookExecutor");
	if (lua_isstring(l, 2) && lua_isinteger(l, 3))
	{
		lua_pushstring(l, (*he)->GetLocaleString(lua_tostring(l, 2), (int)lua_tointeger(l, 3)).c_str());
	}
	else
	{
		lua_pushstring(l,"");
	}
	return 1;
}

int l_installvtablehook(lua_State* l)
{
	HookExecutor** he = (HookExecutor**)luaL_checkudata(l, 1, "HookExecutor");
	if (lua_isstring(l, 2) && lua_isinteger(l, 3) && lua_isfunction(l, 4))
	{
		(*he)->InstallVtableHook(lua_tostring(l, 2), (int)lua_tointeger(l, 3));
	}
	return 0;
}

int l_installhook(lua_State* l)
{
	HookExecutor** he = (HookExecutor**)luaL_checkudata(l, 1, "HookExecutor");
	if (lua_isstring(l, 2) && lua_isfunction(l, 3))
	{
		lua_getfield(l, LUA_REGISTRYINDEX, lua_tostring(l, 2));
		if (!lua_istable(l, -1))
		{
			lua_pop(l, 1);
			lua_newtable(l);
		}
		lua_pushvalue(l, 3);
		lua_rawseti(l, -2, lua_rawlen(l, -2) + 1);
		lua_setfield(l, LUA_REGISTRYINDEX, lua_tostring(l, 2));
	}
	return 0;

}

HookExecutor::HookExecutor(Logger* logger,std::string const& luaname)
	: logger_(logger), savedweapon_(NULL), saved_hits_(), loaded_sav_(false), locale_(logger)
{
	lua_ = luaL_newstate();
	luaL_openlibs(lua_);
	std::string setpathstring = "package.path=package.path .. ';./" + luaname + "/?.lua;./"+luaname+"/?/init.lua'";
	luaL_dostring(lua_, setpathstring.c_str());

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

	entity_vtable_ = std::make_shared<EntityVtable>(lua_, logger_);
	Entity::RegisterEntityVtable(entity_vtable_);

	SetupVtableHookTemplates();

	improver_.RegisterLua(lua_, logger_);

	// Register HookExecutor functions for Lua
	HookExecutor** heptrptr = (HookExecutor**)lua_newuserdata(lua_, sizeof(HookExecutor*));
	*heptrptr = this;
	luaL_newmetatable(lua_, "HookExecutor");
	lua_pushcfunction(lua_, l_replaceperk);
	lua_setfield(lua_, -2, "ReplacePerk");
	lua_pushcfunction(lua_, l_addlocalestring);
	lua_setfield(lua_, -2, "AddLocaleString");
	lua_pushcfunction(lua_, l_getlocalestring);
	lua_setfield(lua_, -2, "GetLocaleString");
	lua_pushcfunction(lua_, l_installvtablehook);
	lua_setfield(lua_, -2, "InstallVtableHook");
	lua_pushcfunction(lua_, l_installhook);
	lua_setfield(lua_, -2, "InstallHook");
	lua_pushvalue(lua_, -1);
	lua_setfield(lua_, -2, "__index");
	lua_setmetatable(lua_, -2);
	lua_setglobal(lua_, "hookexecutor");

	char* nochain = (char*)lua_newuserdata(lua_, 1);
	*nochain = 'N';
	lua_setglobal(lua_, "NO_CHAIN");

	lua_newtable(lua_);
	lua_setglobal(lua_, "FTSECustomStorage");
	lua_pushinteger(lua_, 1);
	lua_setglobal(lua_, "FTSECustomStorageMaxID");

	bool failed = luaL_dostring(lua_, "require \"FTSE\"");
	if (failed)
	{
		*logger_ << "Error loading LUA scripts: " << lua_tostring(lua_, -1) << std::endl;
	}
	else
	{
		*logger_ << "Successfully started LUA engine." << std::endl;
	}
}

HookExecutor::~HookExecutor()
{
}
void HookExecutor::InstallVtableHook(std::string const & classname, int idx)
{
	// Hook stub: converts __thiscall of Entity to __thiscall of HookExecutor, and adds
	//            the index of the hook as a parameter, so that we can use that to look
	//            up the correct Lua function in the Lua registry.

	// pop eax                 58
	// push ecx                51
	// push <idx>              68 xx xx xx xx
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
	memcpy(newhook + 3, &idx, sizeof(int));
	newhook[7] = 0x50;
	newhook[8] = 0xb9;
	void* ptr = this;
	memcpy(newhook + 9, &ptr, sizeof(void**));
	newhook[13] = 0xe9;

	size_t codeaddr = VtableHookTemplates_[idx];
	size_t relative_address = codeaddr - (((size_t)newhook) + 18) ;
	memcpy(newhook + 14, &relative_address, sizeof(size_t));

	// Now copy the hook to the vtable(s)
	if (!entity_vtable_->InstallVtableHook(classname, idx, (size_t)newhook))
	{
		// Nothing used the new hook function - delete it
		delete[] newhook;
	}

}
void HookExecutor::EntityConstructHook(void * entity)
{
	Entity::GetEntityByPointer(entity)->SetFTSEID(0);
}

void HookExecutor::EntitySerializeHook(void * entity, void * stream)
{
	auto ent = Entity::GetEntityByPointer(((char*)entity)-0x189);	// Caller already incremented by 0x47, plus the 0x142 from ClassType header
	char typ = *(((char*)stream) + 0x3f);
	if (typ == 2)
	{
		// Loading
		int64_t entid = 0;
		int64_t maxentid = 0;
		int32_t id_field = 0;
		auto fxn = (void(__thiscall *)(void*,char const*, int32_t*, int32_t, int, uint8_t))0x5f7890;	// EntityStreamWriteInteger
		(*fxn)(stream,"FTSE_IDlow", &id_field, 0, 3, 4);
		memcpy(&entid, &id_field, 3);
		id_field = 0;
		(*fxn)(stream,"FTSE_IDhigh", &id_field, 0, 3, 4);
		memcpy((((char*)&entid)+3), &id_field, 3);
		ent->SetFTSEID(entid);
		id_field = 0;
		(*fxn)(stream, "FTSE_MAXIDlow", &id_field, 0, 3, 4);
		memcpy(&maxentid, &id_field, 3);
		id_field = 0;
		(*fxn)(stream, "FTSE_MAXIDhigh", &id_field, 0, 3, 4);
		memcpy((((char*)&maxentid) + 3), &id_field, 3);
		if (maxentid > Entity::GetNextFTSEID())
		{
			Entity::ResetNextFTSEID(maxentid);
		}


		if (entid != 0)
		{
			auto fxn2 = (void(__thiscall *)(void*, char const*, wchar_t**, char const*, uint8_t))0x59b740;	// EntityStreamWriteStringType
			FOTString in(std::string(""));
			wchar_t* in2 = in.getraw();
			(*fxn2)(stream, "FTSE_Custom", &in2, "", 2);
			in.force_override(in2);
			std::string in3 = in.get();
			lua_getglobal(lua_, "FTSE_Deserialize");
			if (lua_isfunction(lua_, -1))
			{
				lua_pushstring(lua_, in3.c_str());
				if (lua_pcall(lua_, 1, 1, 0) == LUA_ERRRUN)
				{
					(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
				}
				else if (lua_istable(lua_, -1))
				{
					lua_getglobal(lua_, "FTSECustomStorage");
					lua_pushvalue(lua_, -2);
					lua_setfield(lua_, -2, std::to_string(entid).c_str());
					lua_pop(lua_, 1);
				}
				lua_pop(lua_, 1);
			}
			else
			{
				// The function is gone? Can't do anything
				lua_pop(lua_, 1);
			}
		}

//		(*logger_) << "Loaded entity " << ent->GetEntityVerboseString() << std::endl;

	}
	else if (typ == 3)
	{
		// Saving
		int64_t entid = ent->GetFTSEID();
//		(*logger_) << "Saving entity " << ent->GetEntityVerboseString() << std::endl;
		int32_t id_field = 0;
		memcpy(&id_field, &entid, 3);

		auto fxn = (void(__thiscall *)(void*, char const*, int32_t*, int32_t, int, uint8_t))0x5f7890;	// EntityStreamWriteInteger
		(*fxn)(stream,"FTSE_IDlow", &id_field, 0, 3, 4);
		memcpy(&id_field, (((char*)&entid)+3), 3);
		(*fxn)(stream,"FTSE_IDhigh", &id_field, 0, 3, 4);
		id_field = 0;
		int64_t maxentid = Entity::GetNextFTSEID();
		memcpy(&id_field, &maxentid, 3);
		(*fxn)(stream, "FTSE_MAXIDlow", &id_field, 0, 3, 4);
		memcpy(&id_field, (((char*)&maxentid) + 3), 3);
		(*fxn)(stream, "FTSE_MAXIDhigh", &id_field, 0, 3, 4);


		if (entid != 0)
		{
			// We need to use Lua to serialize
			lua_getglobal(lua_, "FTSE_Serialize");
			if (lua_isfunction(lua_, -1))
			{
				lua_getglobal(lua_, "FTSECustomStorage");
				lua_getfield(lua_, -1, std::to_string(entid).c_str());
				lua_remove(lua_, -2);
				if (lua_istable(lua_, -1))
				{
					if (lua_pcall(lua_, 1, 1, 0) == LUA_ERRRUN)
					{
						(*logger_) << "LUA error: " << lua_tostring(lua_, -1) << std::endl;
						lua_pop(lua_, 1);
					}
					FOTString out(std::string(lua_tostring(lua_, -1)));

					//(*logger_) << "Deserialized: " << out << std::endl;
					auto fxn2 = (void(__thiscall *)(void*, char const*, wchar_t**, char const*, uint8_t))0x59b740;	// EntityStreamWriteStringType
					wchar_t* out2 = out.getraw();
					(*fxn2)(stream, "FTSE_Custom", &out2, "", 2);
				}
				else
				{
					lua_pop(lua_, 2);
				}
			}
			else
			{
				lua_pop(lua_, 1);
			}
		}

	}
	else if (typ == 1)
	{
		// Entity editor?
		int64_t entid = 0;
		int32_t id_field = 0;
		auto fxn = (void(__thiscall *)(void*,char const*, int32_t*, int32_t, int, uint8_t))0x5f7890;	// EntityStreamWriteInteger
		(*fxn)(stream,"FTSE_IDlow", &id_field, 0, 3, 4);
		memcpy(&entid, &id_field, 3);
		id_field = 0;
		(*fxn)(stream,"FTSE_IDhigh", &id_field, 0, 3, 4);
		memcpy((((char*)&entid) + 3), &id_field, 3);
//		(*logger_) << "Editor Loaded entity with ID " << entid << std::endl;
		ent->SetFTSEID(entid);
	}
	else
	{
//		(*logger_) << "Unknown serialize type " << ((int)typ) << std::endl;
	}
}

bool HookExecutor::SpeechHook(EntityID player, EntityID npc, void* conversation)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnSpeechHook");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return false;
	}

	// Params:
	lua_newtable(lua_);

	// Player
	shared_ptr<Entity> p = Entity::GetEntityByID(player);
	p->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);

	// NPC
	shared_ptr<Entity> n = Entity::GetEntityByID(npc);
	n->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);

	// Conversation Name
	wchar_t** conversation_name_w = (wchar_t**)(((char*)conversation) + 1);
	std::string conversation_name = Helpers::WcharToUTF8(*conversation_name_w);
	lua_pushstring(lua_, conversation_name.c_str());
	lua_rawseti(lua_, -2, 3);

	// Call hooks in the list
	CallHookChain(lua_);

	// Return contains the speech object if we need to spawn a window
	bool ret = false;
	if (lua_istable(lua_, -1))
	{
		lua_setfield(lua_, LUA_REGISTRYINDEX, "SpeechObject");
		WFTSESpeech::Construct(conversation_name, player, npc, lua_);
		ret = true;
	}
	else
	{
		// Pop result
		lua_pop(lua_, 1);
	}

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

void HookExecutor::AddLocaleString(std::string const& key, std::string const& value, int dictionary)
{
	locale_.AddToDictionary((FOTLocale::LocaleDictionaries)dictionary, key, value);
}
std::string HookExecutor::GetLocaleString(std::string const& key, int dictionary)
{
	return locale_.GetDictionary((FOTLocale::LocaleDictionaries)dictionary, key);
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnRadiated");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	// Params - Entity
	lua_newtable(lua_);
	shared_ptr<Entity> e = Entity::GetEntityByPointer(entity);
	e->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);

	// Call hooks in the list
	CallHookChain(lua_);

	// Not expecting any return, so dump the nil we were (probably) given
	lua_pop(lua_, 1);

}

// Trigger for "long" (10 sec) game tick
void HookExecutor::LongTickTrigger(void* entity)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnLongTick");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	// Params - Entity
	lua_newtable(lua_);
	shared_ptr<Entity> e = Entity::GetEntityByPointer(entity);
	e->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);

	// Call hooks in the list
	CallHookChain(lua_);

	// Not expecting any return, so dump the nil we were (probably) given
	lua_pop(lua_, 1);

}

// Trigger for set variable
void HookExecutor::SetVariableTrigger(void* setvar)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnVariableChanged");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}	
	
	// Params: Key, Value, campaignvar
	lua_newtable(lua_);
	uint32_t ptr = (uint32_t)setvar;
	wchar_t* key = *(wchar_t**)(ptr + 0x0b);
	wchar_t* val = *(wchar_t**)(ptr + 0x0f);
	char campaignvar = *(char*)(ptr + 0x13);

	std::string convkey = Helpers::WcharToUTF8(key);
	std::string convval = Helpers::WcharToUTF8(val);
	lua_pushstring(lua_, convkey.c_str());
	lua_rawseti(lua_, -2, 1);
	lua_pushstring(lua_, convval.c_str());
	lua_rawseti(lua_, -2, 2);
	lua_pushboolean(lua_, campaignvar);
	lua_rawseti(lua_, -2, 3);

	// Call hooks in the list
	CallHookChain(lua_);

	// Not expecting any return, so dump the nil we were (probably) given
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


void HookExecutor::OnStart(std::shared_ptr<GenericPatcher> patcher)
{
	patcher_ = patcher;
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnStart");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
	}
	else
	{
		// params - none
		lua_newtable(lua_);

		// Call hooks in the list
		CallHookChain(lua_);

		// Not expecting any return, so dump the nil we were (probably) given
		lua_pop(lua_, 1);
	}

	// Initialize AttributesTable after OnStart, to pick up any perks
	// NOTE that this means OnStart shouldn't touch any entities (none are in memory yet anyway)
	AttributesTable::Initialize(logger_);
	ActorStatus_Initialize();
	improver_.UpdateMemoryLists();

}

void HookExecutor::DefaultStyleHandler()
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "DefaultStyleChanges");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
	}
	else
	{
		// Params: DefaultStyle
		lua_newtable(lua_);
		DefaultStyle d(logger_);
		d.MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);

		// Call hooks in the list
		CallHookChain(lua_);

		// Not expecting any return, so dump the nil we were (probably) given
		lua_pop(lua_, 1);
	}

	// Chain to OnLocaleLoad - new start point for DefaultStyle and locale are the same
	OnLocaleLoad();
}

void HookExecutor::OnLocaleLoad()
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnLocaleLoad");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
	}
	else
	{

		// params - none
		lua_newtable(lua_);

		// Call hooks in the list
		CallHookChain(lua_);

		// Not expecting any return, so dump the nil we were (probably) given
		lua_pop(lua_, 1);
	}

	wchar_t** version_app = (wchar_t**)0x8be4ac;
	char const* verstring = "1.27 + FTSE " FTSE_VERSION;
	wchar_t* unicode_verstring = Helpers::UTF8ToWcharFOTHeap(verstring, 2);
	DWORD old_protect;
	VirtualProtect(version_app, sizeof(wchar_t*), PAGE_EXECUTE_READWRITE, &old_protect);
	memcpy(version_app, &unicode_verstring, sizeof(wchar_t*));
	VirtualProtect(version_app, sizeof(wchar_t*), old_protect, &old_protect);
}

int HookExecutor::MsecTimerHook(uint64_t msec, uint32_t scale, void* target)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "MsecToDayHMS");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	// Params: msec and scale
	lua_newtable(lua_);
	lua_pushinteger(lua_, msec);
	lua_rawseti(lua_, -2, 1);
	lua_pushinteger(lua_, scale);
	lua_rawseti(lua_, -2, 2);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check return - expecting a table
	if (lua_istable(lua_, -1))
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "AddBaseToGameTime");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	// Params: DateTime table
	lua_newtable(lua_);	// for the param bundle

	// Build DateTime
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
	
	// Add DateTime to the bundle
	lua_rawseti(lua_, -2, 1);


	// Call hooks in the list
	CallHookChain(lua_);

	// Check return
	if (lua_istable(lua_, -1))
	{
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnChanceToHitCalc");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return ;
	}

	// Params: Attacker, Target, Weapon, hit chance table
	lua_newtable(lua_);

	Entity::GetEntityByPointer(attacker)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);

	Entity::GetEntityByPointer(target)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);

	if (savedweapon_ != NULL)
	{
		Entity::GetEntityByPointer(savedweapon_)->MakeLuaObject(lua_);
	}
	else
	{
		lua_pushnil(lua_);
	}
	lua_rawseti(lua_, -2, 3);

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
	lua_rawseti(lua_, -2, 4);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnChanceToCritical1");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return chance;
	}

	// Params: Attacker, Target, Weapon, location, chance
	lua_newtable(lua_);	
	Entity::GetEntityByPointer(attacker)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByPointer(target)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	Entity::GetEntityByPointer(weapon)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 3);

	std::string locutf8 = Helpers::WcharToUTF8(loc);
	lua_pushstring(lua_, locutf8.c_str());
	lua_rawseti(lua_, -2, 4);

	lua_pushinteger(lua_, chance);
	lua_rawseti(lua_, -2, 5);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
	if (lua_isinteger(lua_, -1))
	{
		chance = (int32_t)lua_tointeger(lua_, -1);
	}

	lua_pop(lua_, 1);
	return chance;
}

int32_t HookExecutor::OnChanceToCritical2(void* cmsg, int32_t chance)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnChanceToCritical2");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return chance;
	}

	// Params: Attacker, Target, Weapon, location, chance
	lua_newtable(lua_);

	CombatMessage* msg = (CombatMessage*)cmsg;
	Entity::GetEntityByID(msg->attacker)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByID(msg->target)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	Entity::GetEntityByID(msg->weapon)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 3);

	std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
	std::string aimlocstr = aimloc->get();
	lua_pushstring(lua_, aimlocstr.c_str());
	lua_rawseti(lua_, -2, 4);
	lua_pushinteger(lua_, chance);
	lua_rawseti(lua_, -2, 5);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
	if (lua_isinteger(lua_, -1))
	{
		chance = (int32_t)lua_tointeger(lua_, -1);
	}

	lua_pop(lua_, 1);
	return chance;

}


int32_t HookExecutor::OnCriticalEffect1(void* cmsg, int32_t roll)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnCriticalEffect1");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return roll;
	}
	return OnCriticalEffectImpl(cmsg, roll);
}
int32_t HookExecutor::OnCriticalEffect2(void* cmsg, int32_t roll)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnCriticalEffect2");
	if (lua_isnil(lua_, -1))
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnDamageCalc");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	// Params: attacker, target, weapon, location, damage, crittable, hits
	lua_newtable(lua_);
	CombatMessage* msg = (CombatMessage*)cmsg;
	Entity::GetEntityByID(msg->attacker)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByID(msg->target)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	Entity::GetEntityByID(msg->weapon)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 3);
	std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
	std::string aimlocstr = aimloc->get();
	lua_pushstring(lua_, aimlocstr.c_str());
	lua_rawseti(lua_, -2, 4);
	lua_pushinteger(lua_, msg->damage);
	lua_rawseti(lua_, -2, 5);
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
	lua_rawseti(lua_, -2, 6);
	lua_newtable(lua_);
	for (size_t i = 0; i < saved_hits_.size(); i++)
	{
		lua_pushinteger(lua_, saved_hits_[i]);
		lua_rawseti(lua_, -2, i+1);
	}
	lua_rawseti(lua_, -2, 7);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnInventoryAdd");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	// Params: receiver, item, quantity
	lua_newtable(lua_);
	int32_t item_id = *(int32_t*)item;
	Entity::GetEntityByPointer(receiver)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByID(item_id)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	lua_pushinteger(lua_, quantity);
	lua_rawseti(lua_, -2, 3);

	// Call hooks in the list
	CallHookChain(lua_);

	// Not expecting any return, so dump the nil we were (probably) given
	lua_pop(lua_, 1);

}

void HookExecutor::OnInventoryRemove(void * source, void * item, int32_t quantity)
{
	if (!World::IsLoaded()) return;
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnInventoryRemove");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	// Params: source, item, quantity
	lua_newtable(lua_);
	int32_t item_id = *(int32_t*)item;
	Entity::GetEntityByPointer(source)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByID(item_id)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	lua_pushinteger(lua_, quantity);
	lua_rawseti(lua_, -2, 3);

	// Call hooks in the list
	CallHookChain(lua_);

	// Not expecting any return, so dump the nil we were (probably) given
	lua_pop(lua_, 1);

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

	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnCheckUnequip");
	if (!lua_isnil(lua_, -1))
	{
		lua_newtable(lua_);
		swapactor->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);
		swapactor->GetEquippedItem(0)->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 2);
		lua_pushinteger(lua_, 0);
		lua_rawseti(lua_, -2, 3);
		CallHookChain(lua_);


		if (lua_isstring(lua_, -1))
		{
			retstruct->status = 1;
			std::string errmsg = lua_tostring(lua_, -1);
			retstruct->errstring = Helpers::UTF8ToWcharFOTHeap(errmsg, 1);
			lua_pop(lua_, 1);
			return returnptr;
		}
		lua_pop(lua_, 1);

		lua_getfield(lua_, LUA_REGISTRYINDEX, "OnCheckUnequip");
		lua_newtable(lua_);
		swapactor->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 1);
		swapactor->GetEquippedItem(1)->MakeLuaObject(lua_);
		lua_rawseti(lua_, -2, 2);
		lua_pushinteger(lua_, 1);
		lua_rawseti(lua_, -2, 3);
		CallHookChain(lua_);

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

void HookExecutor::OnMissionLoad(wchar_t** filename_w)
{
	std::string filename = Helpers::WcharToUTF8(*filename_w);
	if (std::string(filename, filename.length() - 3) == "sav")
	{
		// loaded a .sav file, next mission load is not a first visit
		loaded_sav_ = true;

		// Reset FTSE custom storage here - consider moving to "level unload" if that is hooked in the future
		Entity::ResetNextFTSEID(1);
		lua_newtable(lua_);
		lua_setglobal(lua_, "FTSECustomStorage");
		return;
	}

	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnMissionLoad");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}
	// Params: map filename, first visit flag
	lua_newtable(lua_);
	lua_pushstring(lua_, filename.c_str());
	lua_rawseti(lua_, -2, 1);
	lua_pushboolean(lua_, !loaded_sav_);
	lua_rawseti(lua_, -2, 2);
	CallHookChain(lua_);

	// Not expecting any return, so dump the nil we were (probably) given
	lua_pop(lua_, 1);

	// Reset the .sav flag for next map load
	loaded_sav_ = false;

}

void HookExecutor::MainMenuHook(WMainMenu * mainwindow)
{
	if (patcher_->IsFlagSet("FTSE_MAINMENU"))
		mainwindow->PrepAddedButtons(patcher_);
}

uint32_t HookExecutor::ArmourSpriteHook(wchar_t ** dest, wchar_t ** sex, wchar_t ** race, wchar_t ** armour)
{
	if (!improver_.IsArmourReplaceEnabled())
	{
		return 0;
	}

	improver_.GetArmourSpriteName(dest, sex, race, armour);
	return 0x62b6e1;

}

uint32_t HookExecutor::SoundSpriteHook(wchar_t ** dest, void * ent)
{
	if (!improver_.IsArmourReplaceEnabled())
	{
		return 0;
	}

	improver_.GetSoundPrefixName(dest, Entity::GetEntityByPointer(ent).get());
	return 0x560fdf;
}

int32_t HookExecutor::OnCriticalEffectImpl(void* cmsg, int32_t roll)
{
	// Param bundle
	lua_newtable(lua_);
	CombatMessage* msg = (CombatMessage*)cmsg;
	Entity::GetEntityByID(msg->attacker)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByID(msg->target)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	Entity::GetEntityByID(msg->weapon)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 3);

	std::unique_ptr<FOTString> aimloc = msg->GetAimedLocation();
	std::string aimlocstr = aimloc->get();
	lua_pushstring(lua_, aimlocstr.c_str());
	lua_rawseti(lua_, -2, 4);
	lua_pushinteger(lua_, roll);
	lua_rawseti(lua_, -2, 5);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result	
	if (lua_isinteger(lua_, -1))
	{
		roll = (int32_t)lua_tointeger(lua_, -1);
		lua_pop(lua_, 1);
		return roll;
	}
	else if (lua_istable(lua_, -1))
	{

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

	lua_pop(lua_, 1);
	return roll;

}

uint32_t HookExecutor::OnBurstAttack(void* cmsg, void* astart, void* aend)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnBurstAttack");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend);
}

uint32_t HookExecutor::OnConeAttack(void* cmsg, void* astart, void* aend)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnConeAttack");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend);
}

uint32_t HookExecutor::OnSprayAttack(void* cmsg, void* astart, void* aend)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnSprayAttack");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend);
}

uint32_t HookExecutor::OnRadialAttack(void* cmsg, void* astart, void* aend)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnRadialAttack");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend);
}

uint32_t HookExecutor::OnAreaAttack(void* cmsg, void* astart, void* aend)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnAreaAttack");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return 0;
	}

	return MultiTargetAttack(cmsg, astart, aend, true);
}


int HookExecutor::OnStraightAttack(void* cmsg)
{
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnStraightAttack");
	if (lua_isnil(lua_, -1))
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnProjectileAttack");
	if (lua_isnil(lua_, -1))
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

	// Params
	lua_newtable(lua_);
	auto attacker = Entity::GetEntityByID(msg->attacker);
	attacker->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	auto weapon = Entity::GetEntityByID(msg->weapon);

	lua_pushinteger(lua_, msg->numshots);
	lua_rawseti(lua_, -2, 2);

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
	lua_rawseti(lua_, -2, 3);

	weapon->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 4);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
	if (lua_istable(lua_, -1))
	{
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
			auto CFxPopulate = (void(__thiscall *)(void*, void*, void*))(vtable[0x73c / 4]);
			char c = 0;
			CFxPopulate(cfx, &cm, &c);

		}

		return 1;
	}

	lua_pop(lua_, 1);
	return 0;
}

uint32_t HookExecutor::MultiTargetAttack(void* cmsg, void* astart, void* aend, bool area)
{
	// Setup param bundle
	lua_newtable(lua_);
	
	auto FOTChanceHit = (void(*)(ChanceToHit*, void*, void*, void*, void*, wchar_t*))(0x615e80);
	auto FOTApplyHits = (int(*)(CombatMessage*, float))(0x613230);
	CombatMessage* msg = (CombatMessage*)cmsg;

	auto attacker = Entity::GetEntityByID(msg->attacker);
	Vector3 center(msg->attacker_pos);
	auto weapon = Entity::GetEntityByID(msg->weapon);
	Vector3 aim(Vector3(msg->target_pos) - attacker->GetLocation());

	attacker->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	lua_pushinteger(lua_, msg->numshots);
	lua_rawseti(lua_, -2, 2);
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
	lua_rawseti(lua_, -2, 3);

	weapon->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 4);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
	if (lua_istable(lua_, -1))
	{
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
				auto ApplyHit = (void(__thiscall*)(void*, CombatMessage*))(vtable_tgt);
				ApplyHit(a->GetEntityPointer(), &cm);
			}
			else
			{
				(*FOTApplyHits)(&cm, elem.mult);
			}


		}

		return ret;
	}
	lua_pop(lua_, 1);
	return 0;
}

int8_t HookExecutor::OnCheckItemAllowed(void* actor, void* item)
{
	if (!World::IsLoaded()) return -1;
	int8_t ret = -1;		// -1 = perform the normal check

	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnCheckItemAllowed");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return ret;
	}

	// Set up params
	lua_newtable(lua_);
	Entity::GetEntityByPointer(actor)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByPointer(item)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnEquip");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	// Set up params
	lua_newtable(lua_);
	Entity::GetEntityByPointer(equipper)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByID(*(EntityID*)item)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	lua_pushinteger(lua_, slot);
	lua_rawseti(lua_, -2, 3);

	// Call hooks in the list
	CallHookChain(lua_);

	// No return expected - dump the nil
	lua_pop(lua_, 1);
}

wchar_t* HookExecutor::OnCheckEquip(void* equipper, void* item, int slot)
{
	if (!World::IsLoaded()) return nullptr;
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnCheckEquip");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return nullptr;
	}

	// Set up params
	lua_newtable(lua_);
	Entity::GetEntityByPointer(equipper)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByID(*(EntityID*)item)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	lua_pushinteger(lua_, slot);
	lua_rawseti(lua_, -2, 3);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
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
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnUnequip");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return;
	}

	// Set up params
	lua_newtable(lua_);
	Entity::GetEntityByPointer(equipper)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByPointer(item)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	lua_pushinteger(lua_, slot);
	lua_rawseti(lua_, -2, 3);

	// Call hooks in the list
	CallHookChain(lua_);

	// No return expected - dump the nil
	lua_pop(lua_, 1);
}

wchar_t* HookExecutor::OnCheckUnequip(void* equipper, void* item, int slot)
{
	if (!World::IsLoaded()) return nullptr;
	// Get hook list - exit if empty
	lua_getfield(lua_, LUA_REGISTRYINDEX, "OnCheckUnequip");
	if (lua_isnil(lua_, -1))
	{
		lua_pop(lua_, 1);
		return nullptr;
	}

	// Set up params
	lua_newtable(lua_);
	Entity::GetEntityByPointer(equipper)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 1);
	Entity::GetEntityByID(*(EntityID*)item)->MakeLuaObject(lua_);
	lua_rawseti(lua_, -2, 2);
	lua_pushinteger(lua_, slot);
	lua_rawseti(lua_, -2, 3);

	// Call hooks in the list
	CallHookChain(lua_);

	// Check result
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

void HookExecutor::CallHookChain(lua_State* l)
{
	// Assume stack -2 is the chain of hook functions. and stack -1 is an object containing
	// all parameters for the call
	int object_stack_loc = lua_gettop(l);
	int hook_stack_loc = object_stack_loc - 1;
	size_t param_count = lua_rawlen(l, object_stack_loc);

	if (!lua_istable(l, hook_stack_loc))
	{
		// Something's wrong - either we don't have any hook functions, or the stack
		// is corrupted. Pop the hook and param objects and bail out with a nil return
		lua_pop(l, 2);
		lua_pushnil(l);
		return;
	}

	// loop through hook functions
	for (size_t i = 1; i <= lua_rawlen(l, hook_stack_loc); i++)
	{
		// Get function onto stack
		lua_rawgeti(l, hook_stack_loc, i);

		// Get parameters onto stack
		for (size_t j = 1; j <= param_count; j++)
		{
			lua_rawgeti(l, object_stack_loc, j);
		}

		// Call this hook function
		if (lua_pcall(l, param_count, 1, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(l, -1) << std::endl;
			lua_pop(l, 1);
		}

		else if (!lua_isnil(l, -1))
		{
			// remove the hook and params objects, and just keep the return value in Lua stack
			lua_remove(l, object_stack_loc);
			lua_remove(l, hook_stack_loc);
			return;
		}
		else
		{
			// Remove the nil and go around the loop again
			lua_pop(l, 1);
		}
	}

	// All hook functions called and none returned any value, return a nil to the caller
	lua_pop(l, 2);
	lua_pushnil(l);
}

void HookExecutor::CallHookChainRet2(lua_State* l)
{
	// Assume stack -2 is the chain of hook functions. and stack -1 is an object containing
	// all parameters for the call
	int object_stack_loc = lua_gettop(l);
	int hook_stack_loc = object_stack_loc - 1;
	size_t param_count = lua_rawlen(l, object_stack_loc);

	if (!lua_istable(l, hook_stack_loc))
	{
		// Something's wrong - either we don't have any hook functions, or the stack
		// is corrupted. Pop the hook and param objects and bail out with a nil return
		lua_pop(l, 2);
		lua_pushnil(l);
		return;
	}

	// loop through hook functions
	for (size_t i = 1; i <= lua_rawlen(l, hook_stack_loc); i++)
	{
		// Get function onto stack
		lua_rawgeti(l, hook_stack_loc, i);

		// Get parameters onto stack
		for (size_t j = 1; j <= param_count; j++)
		{
			lua_rawgeti(l, object_stack_loc, j);
		}

		// Call this hook function
		if (lua_pcall(l, param_count, 2, 0) == LUA_ERRRUN)
		{
			(*logger_) << "LUA error: " << lua_tostring(l, -1) << std::endl;
			lua_pop(l, 1);
		}

		else if (!lua_isnil(l, -1) || !lua_isnil(l, -2))
		{
			// remove the hook and params objects, and just keep the return value in Lua stack
			lua_remove(l, object_stack_loc);
			lua_remove(l, hook_stack_loc);
			return;
		}
		else
		{

			// Remove the nil and go around the loop again
			lua_pop(l, 2);
		}
	}

	// All hook functions called and none returned any value, return a nil to the caller
	lua_pop(l, 2);
	lua_pushnil(l);
	lua_pushnil(l);
}

void HookExecutor::SetupVtableHookTemplates()
{
	// See the template definitions in EntityVtable.h for naming convention
	VtableHookTemplates_[0] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<RGBColor, Vector3*, float>);								// RGBColor* (*GetRenderLightLevel)(struct Entity *, struct RGBColor *, struct LocationXYZFloat *, float));
	VtableHookTemplates_[1] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*RenderObjectMaybe)(struct Entity *, void *, int *);
	VtableHookTemplates_[2] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);																// void (*InitDefaultEntityFields)(struct Entity *));
	VtableHookTemplates_[3] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<wchar_t**, bool>);												// bool (*SetAnimationType)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[4] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<wchar_t**, bool>);												// bool (*UpdateSprite)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[5] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);																// void (*Vtable5NoOp)(struct Entity *));
	VtableHookTemplates_[6] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);																// void (*InitEntityAfterSpawn)(struct Entity *));
	VtableHookTemplates_[7] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);																// void (*PrepareEntity)(struct Entity *));
	VtableHookTemplates_[8] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);																// void (*EraseInventory)(struct Entity *));
	VtableHookTemplates_[9] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);																// void (*MakeFreshCharacter)(struct Entity *));
	VtableHookTemplates_[10] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*DestructEntity)(struct Entity *));
	VtableHookTemplates_[11] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*SetDoFade)(struct Entity *));
	VtableHookTemplates_[12] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*AliasToDestructEntity)(struct Entity *));
	VtableHookTemplates_[13] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*SomethingWithBoundingBoxAndCollision)(struct Entity *));
	VtableHookTemplates_[14] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*FlagCaptureFunction1)(struct Entity *));
	VtableHookTemplates_[15] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*AttachNewFileToActor)(struct Entity *, void *));
	VtableHookTemplates_[16] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*AttachNewFileToActor2)(struct Entity *));
	VtableHookTemplates_[17] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*SomethingNewSpawnRelated)(struct Entity *));
	VtableHookTemplates_[18] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*Vtable18EquipRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[19] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*AdjustSpriteOffsetForRotatingDoor)(struct Entity *));
	VtableHookTemplates_[20] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*FinishedAnimationSequenceMaybe)(struct Entity *));
	VtableHookTemplates_[21] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable21NoOp)(struct Entity *));
	VtableHookTemplates_[22] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable22NoOp)(struct Entity *));
	VtableHookTemplates_[23] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable23NoOp)(struct Entity *));
	VtableHookTemplates_[24] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // float (*RenderRelated)(struct Entity *, void *, int *));
	VtableHookTemplates_[25] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*CheckThinkTimers)(struct Entity *));
	VtableHookTemplates_[26] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*DoEngineThink)(struct Entity *));
	VtableHookTemplates_[27] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*DoGameThink)(struct Entity *));
	VtableHookTemplates_[28] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SoundRelated1)(struct Entity *, int));
	VtableHookTemplates_[29] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);													// FOTString ** (*GetSoundType)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[30] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetRicochetSoundCounters)(struct Entity *, int));
	VtableHookTemplates_[31] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<Vector3>);													// int* (*SetParameterToZero)(struct Entity *, int *));
	VtableHookTemplates_[32] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_X32);																// void (*InitLightParameters)(struct Entity *, float *));
	VtableHookTemplates_[33] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetAttached)(struct Entity *, struct EntityID));
	VtableHookTemplates_[34] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t**>);													// void (*SetCustomName)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[35] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString ** (*GetCustomName)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[36] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString ** (*GetDisplayName)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[37] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString ** (*GetHealthStatusString)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[38] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString ** (*Vtable38NoOpEmptyStringRet)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[39] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);													// FOTString ** (*GetUseActionName)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[40] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);														// bool (*CanBeHeldInHand)(struct Entity *));
	VtableHookTemplates_[41] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsTimerMine)(struct Entity *));
	VtableHookTemplates_[42] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsTriggerTrap)(struct Entity *));
	VtableHookTemplates_[43] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsCollectable)(struct Entity *));
	VtableHookTemplates_[44] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsSingleUseItem)(struct Entity *));
	VtableHookTemplates_[45] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsSkillConsumable)(struct Entity *));
	VtableHookTemplates_[46] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsArmour)(struct Entity *));
	VtableHookTemplates_[47] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsWeapon)(struct Entity *));
	VtableHookTemplates_[48] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsAmmo)(struct Entity *));
	VtableHookTemplates_[49] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsKey)(struct Entity *));
	VtableHookTemplates_[50] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsContainer)(struct Entity *));
	VtableHookTemplates_[51] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsUnarmedAttack)(struct Entity *));
	VtableHookTemplates_[52] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsMeleeAttack)(struct Entity *));
	VtableHookTemplates_[53] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsBaseAI)(struct Entity *));
	VtableHookTemplates_[54] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsController)(struct Entity *));
	VtableHookTemplates_[55] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsActor)(struct Entity *));
	VtableHookTemplates_[56] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsActorUnk20Set)(struct Entity *));
	VtableHookTemplates_[57] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsLight)(struct Entity *));
	VtableHookTemplates_[58] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsVehicle)(struct Entity *));
	VtableHookTemplates_[59] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsVehicleController)(struct Entity *));
	VtableHookTemplates_[60] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsVehicleWeapon)(struct Entity *));
	VtableHookTemplates_[61] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*CanBeActivated)(struct Entity *));
	VtableHookTemplates_[62] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsDeadOrDestroyedMaybe)(struct Entity *));
	VtableHookTemplates_[63] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsAttachedBaseAIMaybe)(struct Entity *));
	VtableHookTemplates_[64] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsLiving)(struct Entity *));
	VtableHookTemplates_[65] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsTrap)(struct Entity *));
	VtableHookTemplates_[66] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);														// bool (*IsSentryAI)(struct Entity *));
	VtableHookTemplates_[67] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsGeiger)(struct Entity *));
	VtableHookTemplates_[68] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsWaypoint)(struct Entity *));
	VtableHookTemplates_[69] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsSpawnpoint)(struct Entity *));
	VtableHookTemplates_[70] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsAlarm)(struct Entity *));
	VtableHookTemplates_[71] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsCombatFX)(struct Entity *));
	VtableHookTemplates_[72] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsSwitch)(struct Entity *));
	VtableHookTemplates_[73] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsDoor)(struct Entity *));
	VtableHookTemplates_[74] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsGunTurret)(struct Entity *));
	VtableHookTemplates_[75] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsBrainJar)(struct Entity *));
	VtableHookTemplates_[76] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsBreakableType)(struct Entity *));
	VtableHookTemplates_[77] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsCaptureFlag)(struct Entity *));
	VtableHookTemplates_[78] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsTrapDoor)(struct Entity *));
	VtableHookTemplates_[79] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsCyborgGeneral)(struct Entity *));
	VtableHookTemplates_[80] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsPopupTurret)(struct Entity *));
	VtableHookTemplates_[81] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsNotCaptureInvItem)(struct Entity *));
	VtableHookTemplates_[82] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);													// FOTString * (*RetTypeStringForCarryableEntity)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[83] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, EntityID, EntityID>);					// UseItemResponse * (*MaybeUseEntity)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID));
	VtableHookTemplates_[84] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<EntityID, int>);												// EntityID * (*SplitEntity)(struct Entity *, struct EntityID *, dword));
	VtableHookTemplates_[85] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<EntityID, int, bool>);											// bool (*MergeEntities)(struct Entity *, struct EntityID, dword));
	VtableHookTemplates_[86] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);												// bool (*MaybeCheckingIfMergeableAndMatching)(struct Entity *, struct EntityID));
	VtableHookTemplates_[87] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);														// bool (*CanBeRemoved)(struct Entity *));
	VtableHookTemplates_[88] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetCanBeRemovedFlag)(struct Entity *, bool));
	VtableHookTemplates_[89] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetBeenAppliedFlag)(struct Entity *, bool));
	VtableHookTemplates_[90] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);														// bool (*IsUnique)(struct Entity *));
	VtableHookTemplates_[91] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetIsUniqueFlag)(struct Entity *, bool));
	VtableHookTemplates_[92] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<AttributesTable*, bool>);										// bool (*MeetRequirementsCheck)(struct Entity *, struct AttributesArray *));
	VtableHookTemplates_[93] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop);	// Stats * (*GetItemSPECIALRequired)(struct Entity *));   // (Maybe editor only?)
	VtableHookTemplates_[94] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<EntityID, int>);												// void (*ApplyEquip)(struct Entity *, struct EntityID, int));
	VtableHookTemplates_[95] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<EntityID, int>);												// void (*UnApply)(struct Entity *, struct EntityID, dword));
	VtableHookTemplates_[96] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetTrapHolder)(struct Entity *, struct EntityID));
	VtableHookTemplates_[97] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*ClearHolder)(struct Entity *, struct EntityID));
	VtableHookTemplates_[98] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetCount)(struct Entity *));
	VtableHookTemplates_[99] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetCountNonLootable)(struct Entity *));
	VtableHookTemplates_[100] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsLootable)(struct Entity *));
	VtableHookTemplates_[101] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetUnk25EntityID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[102] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetValueBaseOfItems)(struct Entity *));
	VtableHookTemplates_[103] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetCollectableRankReq)(struct Entity *));
	VtableHookTemplates_[104] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetWeight)(struct Entity *));
	VtableHookTemplates_[105] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetCount)(struct Entity *, int));
	VtableHookTemplates_[106] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<int, int>);													// int (*SetNonLootableCount)(struct Entity *, int));
	VtableHookTemplates_[107] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BreakEntity)(struct Entity *));
	VtableHookTemplates_[108] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsBroken)(struct Entity *));
	VtableHookTemplates_[109] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*RepairEntity)(struct Entity *));
	VtableHookTemplates_[110] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetTrapTotalDifficulty)(struct Entity *));
	VtableHookTemplates_[111] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable111Controller)(struct Entity *));
	VtableHookTemplates_[112] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// bool (*Vtable112Controller)(struct Entity *, struct EntityID));
	VtableHookTemplates_[113] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// bool (*Vtable113Controller)(struct Entity *, struct EntityID));
	VtableHookTemplates_[114] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerMoveTypePriority)(struct Entity *, dword));
	VtableHookTemplates_[115] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<ZoneDefinition*>);											// void (*SetControllerWorkingZone)(struct Entity *, struct ZoneDefinition *));
	VtableHookTemplates_[116] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<ZoneDefinition*>);											// void (*SetControllerRetreatZone)(struct Entity *, struct ZoneDefinition *));
	VtableHookTemplates_[117] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*SetControllerMoveTargetTag)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[118] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerAggression)(struct Entity *, int));
	VtableHookTemplates_[119] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerCommunicate)(struct Entity *, int));
	VtableHookTemplates_[120] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerMobility)(struct Entity *, int));
	VtableHookTemplates_[121] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<bool, int>);													// void (*SetControllerRandomMove)(struct Entity *, bool, byte));
	VtableHookTemplates_[122] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetControllerMoveTypePriority)(struct Entity *));
	VtableHookTemplates_[123] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<ZoneDefinition>);											// void (*GetControllerWorkingZone)(struct Entity *, struct ZoneDefinition *));
	VtableHookTemplates_[124] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<ZoneDefinition>);											// void (*GetControllerRetreatZone)(struct Entity *, struct ZoneDefinition *));
	VtableHookTemplates_[125] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetControllerMoveTargetTag)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[126] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetControllerAggression)(struct Entity *));
	VtableHookTemplates_[127] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetControllerCommunicate)(struct Entity *));
	VtableHookTemplates_[128] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetControllerMobility)(struct Entity *));
	VtableHookTemplates_[129] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*GetControllerRandomMove)(struct Entity *));
	VtableHookTemplates_[130] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetControllerType)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[131] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*SetControllerType)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[132] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*DeactivateController)(struct Entity *));
	VtableHookTemplates_[133] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*SetControllerWaypointModeAndMoveTargetTag)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[134] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*SetControllerStationary)(struct Entity *));
	VtableHookTemplates_[135] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*SetControllerToPerimeterZone)(struct Entity *));
	VtableHookTemplates_[136] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetControllerToFollow)(struct Entity *, int));
	VtableHookTemplates_[137] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*SetControllerToFlee)(struct Entity *));
	VtableHookTemplates_[138] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*ControllerDoSomethingWithUnitList)(struct Entity *));
	VtableHookTemplates_[139] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*SetControllerUnknown4String)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[140] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetControllerUnknown4String)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[141] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*ControllerWakeUpUnitList)(struct Entity *, int));
	VtableHookTemplates_[142] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<EntityID, int>);												// void (*ControllerIssueTauntForEntity)(struct Entity *, struct EntityID, int));
	VtableHookTemplates_[143] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*ControllerWaypointMaybeTimingRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[144] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<wchar_t*, EntityID>);										// FOTString * (*ControllerGetUnitWaypointActionString)(struct Entity *, struct FOTString *, struct EntityID));
	VtableHookTemplates_[145] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*ControllerWaypointCheckingRepeatFlag)(struct Entity *, struct EntityID));
	VtableHookTemplates_[146] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<EntityID, Vector3*, float>);									// float (*ControllerMoveRelated)(struct Entity *, struct EntityID, struct LocationXYZFloat *));
	VtableHookTemplates_[147] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<EntityID, EntityID, bool>);									// EntityID * (*Vtable147Controller)(struct Entity *, struct EntityID *, struct EntityID, bool));
	VtableHookTemplates_[148] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<EntityID, wchar_t*>);										// EntityID * (*Vtable148Controller)(struct Entity *, struct EntityID *, struct EntityID, struct FOTString *));
	VtableHookTemplates_[149] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<EntityID, int, float>);										// float (*ControllerGetFromUnknownList2)(struct Entity *, struct EntityID, int));
	VtableHookTemplates_[150] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable150Controller)(struct Entity *));
	VtableHookTemplates_[151] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<int, int, float>);											// float (*Vtable151NoOpReturnFloat0)(struct Entity *, int, int));
	VtableHookTemplates_[152] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<int, int, float>);										// float (*Vtable152NoOpReturnFloat0)(struct Entity *, int, int));
	VtableHookTemplates_[153] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<EntityID, Vector3*, float>);									// float (*ControllerMoreMoveRelated)(struct Entity *, struct EntityID, struct LocationXYZFloat *));
	VtableHookTemplates_[154] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<Vector3, EntityID>);										// LocationXYZFloat * (*ControllerWaypointStuff)(struct Entity *, struct LocationXYZFloat *, struct EntityID));
	VtableHookTemplates_[155] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, int64_t>);										// longlong (*ControllerSomethingAboutTookDamage)(struct Entity *, struct EntityID));
	VtableHookTemplates_[156] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<EntityID, Vector3*>);										// void (*Vtable156Controller)(struct Entity *, struct EntityID, struct LocationXYZFloat *));
	VtableHookTemplates_[157] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r<EntityID, Vector3*, bool, bool>);								// bool (*ControllerUsesWorkingZoneAndMoveType)(struct Entity *, struct EntityID, struct LocationXYZFloat *, bool));
	VtableHookTemplates_[158] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BaseAINeedsToSwitchTargetMaybe)(struct Entity *));
	VtableHookTemplates_[159] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*BaseAIEngineThinkRelated)(struct Entity *, int *));
	VtableHookTemplates_[160] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetBaseAIController)(struct Entity *, struct EntityID));
	VtableHookTemplates_[161] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetBaseAIController)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[162] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*Vtable162BaseAI)(struct Entity *, int *));
	VtableHookTemplates_[163] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetBaseAINature)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[164] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*StopMovement)(struct Entity *, bool));
	VtableHookTemplates_[165] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*BaseAIGetCurrentTarget)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[166] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BaseAIHasATargetMaybe)(struct Entity *));
	VtableHookTemplates_[167] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsSleeping)(struct Entity *));
	VtableHookTemplates_[168] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetSleepingFlag)(struct Entity *, bool));
	VtableHookTemplates_[169] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<float>);													// void (*SetBaseAIUnknown7)(struct Entity *, float));
	VtableHookTemplates_[170] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetBaseAIUnknown7ToRealTimeTimerValue)(struct Entity *, int));
	VtableHookTemplates_[171] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*BaseAIStopMovementIfHighestDesireSet)(struct Entity *));
	VtableHookTemplates_[172] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // bool (*BaseAIMaybeOpenDoorRelated)(struct Entity *, struct BaseAIEntityPlus4Struct *));
	VtableHookTemplates_[173] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*Vtable173BaseAI)(struct Entity *, struct BaseAIEntityPlus4Struct *));
	VtableHookTemplates_[174] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*Vtable174BaseAI)(struct Entity *, struct EntityID));
	VtableHookTemplates_[175] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetBaseAITauntTypeString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[176] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetBaseAIHighestDesireRoundedUp)(struct Entity *));
	VtableHookTemplates_[177] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*ClearSentryAITargets)(struct Entity *));
	VtableHookTemplates_[178] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetSentryMode)(struct Entity *, int));
	VtableHookTemplates_[179] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetSentryMode)(struct Entity *));
	VtableHookTemplates_[180] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetSentryAccuracyTrigger)(struct Entity *, int));
	VtableHookTemplates_[181] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetSentryAccuracyTrigger)(struct Entity *));
	VtableHookTemplates_[182] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetSentryLocation)(struct Entity *, int));
	VtableHookTemplates_[183] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetSentryLocation)(struct Entity *));
	VtableHookTemplates_[184] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<EntityID, int>);												// void (*SetSentryUserTarget)(struct Entity *, struct EntityID, int));
	VtableHookTemplates_[185] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SentryAIMaybeCheckingTargets)(struct Entity *, struct EntityID));
	VtableHookTemplates_[186] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // ActorSPECIALCopyAndFields * (*GetActorCopyofSPECIALStats)(struct Entity *));
	VtableHookTemplates_[187] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*CopySomeActorFields)(struct Entity *, struct ActorSPECIALCopyAndFields *));
	VtableHookTemplates_[188] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // bool (*CopySomeActorFields2)(struct Entity *, struct ActorSPECIALCopyAndFields *, void *));
	VtableHookTemplates_[189] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // bool (*Vtable189Actor)(struct Entity *, void *, void *, void *));
	VtableHookTemplates_[190] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetActorStartIndex)(struct Entity *));
	VtableHookTemplates_[191] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetActorStartIndex)(struct Entity *, int));
	VtableHookTemplates_[192] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetActorFractionOfHitPointsLost)(struct Entity *));
	VtableHookTemplates_[193] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*CheckBestHPRatioIfUseHealingItemMaybe)(struct Entity *));
	VtableHookTemplates_[194] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*ActorHasItemToPermBoostStats)(struct Entity *));
	VtableHookTemplates_[195] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*ActorHasItemToPermBoostStats2)(struct Entity *));
	VtableHookTemplates_[196] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetActorPoisonPointsTimes15Cap90)(struct Entity *));
	VtableHookTemplates_[197] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*ActorLooksLikeNoOp)(struct Entity *));
	VtableHookTemplates_[198] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetActorRadPointsDivided10Cap90)(struct Entity *));
	VtableHookTemplates_[199] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*ActorCheckItemsForRadHealing)(struct Entity *));
	VtableHookTemplates_[200] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<int, int>);													// void (*MakeConsumableGiveRadiation)(struct Entity *, int, int));
	VtableHookTemplates_[201] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<int, int>);												// void (*MakeConsumableGivePoison)(struct Entity *, int, int));
	VtableHookTemplates_[202] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*ApplyBonusesToAttachedActor)(struct Entity *));
	VtableHookTemplates_[203] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*GetExpectedHPRatioIfConsumableUsed)(struct Entity *, struct EntityID));
	VtableHookTemplates_[204] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*DoesConsumablePermBoostSPEAL)(struct Entity *, struct EntityID));
	VtableHookTemplates_[205] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*Vtable205NoOpRetZeroFloat)(struct Entity *));
	VtableHookTemplates_[206] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*GetRatioOfRadPointHealingForConsumable)(struct Entity *, struct EntityID));
	VtableHookTemplates_[207] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable207ActorMaybeCheckForPlayerCharacter)(struct Entity *));
	VtableHookTemplates_[208] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetRankString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[209] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetActorRankID)(struct Entity *));
	VtableHookTemplates_[210] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetRanksGivenEntityHasAboveThis)(struct Entity *, struct EntityID));
	VtableHookTemplates_[211] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetReputationString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[212] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetCappedReputation)(struct Entity *));
	VtableHookTemplates_[213] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetMissionsCom)(struct Entity *));
	VtableHookTemplates_[214] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*IncrementMissionsCom)(struct Entity *, int));
	VtableHookTemplates_[215] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*AddToReputation)(struct Entity *, int));
	VtableHookTemplates_[216] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsGlowing)(struct Entity *));
	VtableHookTemplates_[217] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // int (*GetActorAge)(struct Entity *, struct Traits *));
	VtableHookTemplates_[218] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<wchar_t**, wchar_t**>);										// void (*SetCurrentActionStringMaybe)(struct Entity *, struct FOTString **, struct FOTString **));
	VtableHookTemplates_[219] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetCurrentActionString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[220] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsIdle)(struct Entity *));
	VtableHookTemplates_[221] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsMoving_WalkRunClimbLadderOrDriving)(struct Entity *));
	VtableHookTemplates_[222] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsClimbing)(struct Entity *));
	VtableHookTemplates_[223] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsIdleOrWalkRun)(struct Entity *));
	VtableHookTemplates_[224] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*HasFallen)(struct Entity *));
	VtableHookTemplates_[225] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable225ThinkRelated)(struct Entity *));
	VtableHookTemplates_[226] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanMove)(struct Entity *));
	VtableHookTemplates_[227] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanRun)(struct Entity *));
	VtableHookTemplates_[228] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);													// void (*AdjustStanceDownward)(struct Entity *, bool));
	VtableHookTemplates_[229] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);													// void (*AdjustStanceUpward)(struct Entity *, bool));
	VtableHookTemplates_[230] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsHumanoidOrDog)(struct Entity *));
	VtableHookTemplates_[231] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*MaxMovementSpeedMaybe)(struct Entity *));
	VtableHookTemplates_[232] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsEncumbered)(struct Entity *));
	VtableHookTemplates_[233] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsStanding)(struct Entity *));
	VtableHookTemplates_[234] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsCrouching)(struct Entity *));
	VtableHookTemplates_[235] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsProne)(struct Entity *));
	VtableHookTemplates_[236] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetSelectedStance)(struct Entity *));
	VtableHookTemplates_[237] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*ActoSetSelectedStance)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[238] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsHidden)(struct Entity *));
	VtableHookTemplates_[239] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetPostureString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[240] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);													// void (*SetSneakState)(struct Entity *, bool));
	VtableHookTemplates_[241] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsBurrowed)(struct Entity *));
	VtableHookTemplates_[242] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);													// void (*SetBurrowingState)(struct Entity *, bool));
	VtableHookTemplates_[243] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<wchar_t**, int, bool>);										// bool (*RollAgainstStat)(struct Entity *, struct FOTString *, int));
	VtableHookTemplates_[244] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4r1<InventoryActionResult, wchar_t**, EntityID, EntityID>);		// UseItemResponse * (*ApplySkillToTargetEntity)(struct Entity *, struct UseItemResponse *, struct FOTString *, struct EntityID, struct EntityID));
	VtableHookTemplates_[245] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSkillStringBasedOnUnk25c)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[246] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetActorUnk2EntityID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[247] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetAdjustedPilotSkill)(struct Entity *));
	VtableHookTemplates_[248] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<wchar_t**, bool>);											// bool (*DoesSkillStringMatchHelperItem)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[249] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<wchar_t**, bool>);											// bool (*IsValidTargetForSkill)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[250] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<EntityID, EntityID, bool>);									// bool (*IsValidTargetForDefaultAction)(struct Entity *, struct EntityID, struct EntityID));
	VtableHookTemplates_[251] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsGeneral)(struct Entity *));
	VtableHookTemplates_[252] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsRecruitMaster)(struct Entity *));
	VtableHookTemplates_[253] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsQuartermaster)(struct Entity *));
	VtableHookTemplates_[254] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanBarterWith)(struct Entity *));
	VtableHookTemplates_[255] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BartersWithBOSScript)(struct Entity *));
	VtableHookTemplates_[256] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanGambleWith)(struct Entity *));
	VtableHookTemplates_[257] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetRepairObjectDifficulty)(struct Entity *));
	VtableHookTemplates_[258] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<float, float>);												// float (*GetScaledKnockdownChance)(struct Entity *, float));
	VtableHookTemplates_[259] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*ActorKnockoutRelated)(struct Entity *));
	VtableHookTemplates_[260] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, int, int>);							// UseItemResponse * (*ActorInjureLocation)(struct Entity *, struct UseItemResponse *, int, int));
	VtableHookTemplates_[261] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4r1<InventoryActionResult, int, int, int>);					// UseItemResponse * (*ActorKnockout)(struct Entity *, struct UseItemResponse *, int, int, int));
	VtableHookTemplates_[262] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, int, int>);						// UseItemResponse * (*ActorDisarm)(struct Entity *, struct UseItemResponse *, int, int));
	VtableHookTemplates_[263] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, int, int>);						// UseItemResponse * (*ActorBreakWeaponMaybe)(struct Entity *, struct UseItemResponse *, int, int));
	VtableHookTemplates_[264] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, int>);							// UseItemResponse * (*ActorCriticalMissHitSelfMaybe)(struct Entity *, struct UseItemResponse *, int));
	VtableHookTemplates_[265] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, int>);							// UseItemResponse * (*ActorCriticalMissHitSelfMaybe2)(struct Entity *, struct UseItemResponse *, int));
	VtableHookTemplates_[266] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*StunActor)(struct Entity *, int));
	VtableHookTemplates_[267] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>);												// bool (*IsRightLegInjured)(struct Entity *, bool));
	VtableHookTemplates_[268] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>);												// bool (*IsLeftLegInjured)(struct Entity *, bool));
	VtableHookTemplates_[269] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsRightArmInjured)(struct Entity *, bool));
	VtableHookTemplates_[270] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsLeftArmInjured)(struct Entity *, bool));
	VtableHookTemplates_[271] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsBlinded)(struct Entity *, bool));
	VtableHookTemplates_[272] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsWinded)(struct Entity *, bool));
	VtableHookTemplates_[273] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsImmobilized)(struct Entity *, bool));
	VtableHookTemplates_[274] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsConcussed)(struct Entity *, bool));
	VtableHookTemplates_[275] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>); 											// bool (*IsStunned)(struct Entity *, bool));
	VtableHookTemplates_[276] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsUnconscious)(struct Entity *));
	VtableHookTemplates_[277] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsBandagedStatus)(struct Entity *));
	VtableHookTemplates_[278] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<Vector3*, int>); 											// int (*GetActorMovementCostToLocation)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[279] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>); 													// bool (*IsActorUnk23SetPlusSomeOtherFieldFalse)(struct Entity *));
	VtableHookTemplates_[280] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Move)(struct Entity *));
	VtableHookTemplates_[281] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_5<Vector3*, int, float, float, bool>);							// void (*SetDestination)(struct Entity *, struct LocationXYZFloat *, int, float, float, bool));
	VtableHookTemplates_[282] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<Vector3>);												// LocationXYZFloat * (*GetLocationBasedOnUnk23)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[283] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*ActorGetUnk24bEntityID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[284] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<Vector3*, bool, bool>);										// bool (*ActorSomethingTouchSearch)(struct Entity *, struct LocationXYZFloat *, bool));
	VtableHookTemplates_[285] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<Vector3*, bool, bool>);									// bool (*ActorSomethingTouchSearch2)(struct Entity *, struct LocationXYZFloat *, bool));
	VtableHookTemplates_[286] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<Vector3*, bool, bool>);									// bool (*ActorSomethingTouchSearch3)(struct Entity *, struct LocationXYZFloat *, bool));
	VtableHookTemplates_[287] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable287NoOpRet0)(struct Entity *));
	VtableHookTemplates_[288] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsLegalTarget)(struct Entity *));
	VtableHookTemplates_[289] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*ActorIsNotLegalTargetAndUnk1Set)(struct Entity *));
	VtableHookTemplates_[290] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsVehicleDisabled)(struct Entity *));
	VtableHookTemplates_[291] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, EntityID, int>);						// UseItemResponse * (*ActorPickupItem)(struct Entity *, struct UseItemResponse *, struct EntityID, int));
	VtableHookTemplates_[292] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable292Actor)(struct Entity *));
	VtableHookTemplates_[293] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<InventoryActionResult>);									// UseItemResponse * (*ReturnUseItemResponseType0x40)(struct Entity *, struct UseItemResponse *));
	VtableHookTemplates_[294] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<ChanceToHit, EntityID, bool>);								// HitChanceStruct * (*GetSomeKindOfFlagForMouseOverTarget)(struct Entity *, struct HitChanceStruct *, struct EntityID, bool));
	VtableHookTemplates_[295] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<ChanceToHit, Vector3*>);									// HitChanceStruct * (*GetSomeKindOfFlagForMouseOverTarget2)(struct Entity *, struct HitChanceStruct *, struct LocationXYZFloat *));
	VtableHookTemplates_[296] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4r1<ChanceToHit, EntityID, int, int>);							// HitChanceStruct * (*BeginAttackAnEntity)(struct Entity *, struct HitChanceStruct *, struct EntityID, int, int));
	VtableHookTemplates_[297] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<ChanceToHit, Vector3*>);									// HitChanceStruct * (*BeginAttackALocation)(struct Entity *, struct HitChanceStruct *, struct LocationXYZFloat *));
	VtableHookTemplates_[298] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<CombatMessage*>);											// void (*ApplyAttackResultToEntity)(struct Entity *, struct CombatMessage *));
	VtableHookTemplates_[299] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<CombatMessage*>);											// void (*ShotAtMissed)(struct Entity *, struct CombatMessage *));
	VtableHookTemplates_[300] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_6<EntityID, int, int, int, wchar_t*, int>);						// void (*ApplyDamage)(struct Entity *, struct EntityID, int, int, int, struct FOTString *, int));
	VtableHookTemplates_[301] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4<EntityID, int, int, wchar_t*>);								// void (*EntityWasKilled)(struct Entity *, struct EntityID, int, int, struct FOTString *));
	VtableHookTemplates_[302] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*IncrementKillCount)(struct Entity *, struct EntityID));
	VtableHookTemplates_[303] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*CopyActorKillList)(struct Entity *, void *));
	VtableHookTemplates_[304] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable304NoOpRet0)(struct Entity *));
	VtableHookTemplates_[305] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<AnimOffset*>);												// void (*ResolveCombatMessageForAttacker)(struct Entity *, AnimOffset *));
	VtableHookTemplates_[306] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetIDOfEquippedItemInActiveSlot)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[307] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetEntityIDOfNonEquippedWeapon)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[308] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<InventoryActionResult>);									// UseItemResponse * (*ReloadActiveWeaponWithAnyAmmo)(struct Entity *, struct UseItemResponse *));
	VtableHookTemplates_[309] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, int, EntityID>);						// UseItemResponse * (*ReloadWeaponWithSelectedAmmo)(struct Entity *, struct UseItemResponse *, int, struct EntityID));
	VtableHookTemplates_[310] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*ActorCanAttackWithActiveWeapon)(struct Entity *));
	VtableHookTemplates_[311] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable311ActorCheckingLastTimeRun)(struct Entity *));
	VtableHookTemplates_[312] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*InitActorCombatMessages)(struct Entity *));
	VtableHookTemplates_[313] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*LevelUp)(struct Entity *, int));
	VtableHookTemplates_[314] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetCombatMessage2TargetID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[315] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<EntityID, EntityID, int>);									// int (*GetPlayerBuyPriceForItem)(struct Entity *, struct EntityID, struct EntityID));
	VtableHookTemplates_[316] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<EntityID, EntityID, int>);								// int (*GetPlayerSellBackPriceForItem)(struct Entity *, struct EntityID, struct EntityID));
	VtableHookTemplates_[317] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetCountOfMatchingItems)(struct Entity *, struct EntityID));
	VtableHookTemplates_[318] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);		// UseItemResponse * (*EquipItem)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[319] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, int, EntityID>);					// UseItemResponse * (*RemoveItem)(struct Entity *, struct UseItemResponse *, int, struct EntityID *));
	VtableHookTemplates_[320] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, EntityID, EntityID>);				// UseItemResponse * (*RemoveItemIfEquipped)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *));
	VtableHookTemplates_[321] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);	// UseItemResponse * (*AddToInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[322] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);	// UseItemResponse * (*RemoveFromInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[323] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, int, EntityID>);					// UseItemResponse * (*UnequipItemFromSlotAndPutInInventory)(struct Entity *, struct UseItemResponse *, int, struct EntityID *));
	VtableHookTemplates_[324] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, EntityID, EntityID>);				// UseItemResponse * (*UnequipItemByIDAndReturnToInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *));
	VtableHookTemplates_[325] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4r2<InventoryActionResult, EntityID, EntityID, int>);			// UseItemResponse * (*EquipItemFromInventoryToSlot)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int));
	VtableHookTemplates_[326] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4r2<InventoryActionResult, EntityID, EntityID, int>);		// UseItemResponse * (*RemoveFromInventory2)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int));
	VtableHookTemplates_[327] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);	// UseItemResponse * (*ActorSomethingRelatedToPickup)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[328] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, int, EntityID>);					// UseItemResponse * (*RemoveItemFromSlot2)(struct Entity *, struct UseItemResponse *, int, struct EntityID *));
	VtableHookTemplates_[329] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, EntityID, EntityID>);				// UseItemResponse * (*RemoveItemIfEquipped2)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *));
	VtableHookTemplates_[330] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_5r2<InventoryActionResult, EntityID, EntityID, int, int>);	// UseItemResponse * (*PickupItemAndEquipInSlot)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int));
	VtableHookTemplates_[331] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4r2<InventoryActionResult, EntityID, EntityID, int>);		// UseItemResponse * (*RemoveItemFromEquippedThenInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int));
	VtableHookTemplates_[332] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4r2<InventoryActionResult, EntityID, EntityID, int>);		// UseItemResponse * (*RemoveItemFromInventoryThenEquipped)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int));
	VtableHookTemplates_[333] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r2<InventoryActionResult, int, EntityID>);					// UseItemResponse * (*Vtable333SomethingForApplyingItem)(struct Entity *, struct UseItemResponse *, int, struct EntityID *));
	VtableHookTemplates_[334] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetActiveWeaponHand)(struct Entity *, char));
	VtableHookTemplates_[335] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*ToggleActiveHand)(struct Entity *));
	VtableHookTemplates_[336] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*CheckIfTargetInReachableRange)(struct Entity *, struct EntityID));
	VtableHookTemplates_[337] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<uint8_t>);													// byte (*GetSelectedHand)(struct Entity *));
	VtableHookTemplates_[338] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<uint8_t>);													// byte (*GetUnselectedHand)(struct Entity *));
	VtableHookTemplates_[339] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<EntityID, int>);											// EntityID * (*GetIDofEquippedItemInSlot)(struct Entity *, struct EntityID *, int));
	VtableHookTemplates_[340] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, EntityID>);							// UseItemResponse * (*Vtable340InventoryCombineRelatedMaybe)(struct Entity *, struct UseItemResponse *, struct EntityID));
	VtableHookTemplates_[341] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<wchar_t*, bool>);											// bool (*PlayerEquippedItemWithTagName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[342] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*IsItemIDEquipped)(struct Entity *, struct EntityID));
	VtableHookTemplates_[343] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<wchar_t*, bool>);											// bool (*ContainsItemWithTagName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[344] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*ContainsItemByID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[345] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<wchar_t*, bool>);											// bool (*EquippedOrContainsItemWithTagName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[346] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*FindIfEquippedOrCarriedItemByID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[347] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4r1<InventoryActionResult, int, int, uint8_t>);					// UseItemResponse * (*Vtable347ActorSomethingAboutUsingItem)(struct Entity *, struct UseItemResponse *, word, word, byte));
	VtableHookTemplates_[348] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, EntityID>);							// UseItemResponse * (*CanUseItem)(struct Entity *, struct UseItemResponse *, struct EntityID));
	VtableHookTemplates_[349] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<InventoryActionResult, EntityID>);						// UseItemResponse * (*CanReachInteractable)(struct Entity *, struct UseItemResponse *, struct EntityID));
	VtableHookTemplates_[350] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSizeStringMaybe)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[351] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<AttributesTable*>);										// AttributesArray * (*GetCurrentStats)(struct Entity *));
	VtableHookTemplates_[352] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<AttributesTable*>);										// AttributesArray * (*GetCopyStats)(struct Entity *));
	VtableHookTemplates_[353] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<AttributesTable*>);										// AttributesArray * (*GetTempStats)(struct Entity *));
	VtableHookTemplates_[354] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<AttributesTable*>);										// AttributesArray * (*GetBaseStats)(struct Entity *));
	VtableHookTemplates_[355] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<AttributesTable*>);											// void (*CopyAttributesToBaseStats)(struct Entity *, struct AttributesArray *));
	VtableHookTemplates_[356] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<ActorStatus*>);											// ActorStatus * (*GetActorStatus)(struct Entity *));
	VtableHookTemplates_[357] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<ActorStatus*>);												// void (*CopyActorStatus)(struct Entity *, struct ActorStatus *));
	VtableHookTemplates_[358] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<float>);													// void (*AdjustAP)(struct Entity *, float));
	VtableHookTemplates_[359] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*AdjustAP2)(struct Entity *, int));
	VtableHookTemplates_[360] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetCurrentAP)(struct Entity *));
	VtableHookTemplates_[361] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetAPAvailablePercentage)(struct Entity *));
	VtableHookTemplates_[362] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*ActorUnconsciousAPHandling)(struct Entity *, bool));
	VtableHookTemplates_[363] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsMale)(struct Entity *));
	VtableHookTemplates_[364] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsFemale)(struct Entity *));
	VtableHookTemplates_[365] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSexString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[366] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetEntityCharacterTypeString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[367] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3<AttributesTable*, int, float>);								// void (*AddTempBonuses)(struct Entity *, struct AttributesArray *, dword, float));
	VtableHookTemplates_[368] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3<AttributesTable*, int, float>);							// void (*RemoveTempBonuses)(struct Entity *, struct AttributesArray *, dword, float));
	VtableHookTemplates_[369] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r<int, int, wchar_t**, int>);									// int (*IncreaseAttributeByRange)(struct Entity *, int, int, struct FOTString *));
	VtableHookTemplates_[370] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<int, bool>);												// bool (*GrantXP)(struct Entity *, int));
	VtableHookTemplates_[371] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetNPCLevel)(struct Entity *));
	VtableHookTemplates_[372] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*HandleXPForNewPlayerCharacterMaybe)(struct Entity *));
	VtableHookTemplates_[373] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*GiveXPForLevelUp)(struct Entity *));
	VtableHookTemplates_[374] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetExpectedLevelForMyXP)(struct Entity *));
	VtableHookTemplates_[375] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*XPAndValueRelatedCalc)(struct Entity *));
	VtableHookTemplates_[376] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<float>);														// void (*RadiateEntity)(struct Entity *, float));
	VtableHookTemplates_[377] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<float>);														// void (*PoisonEntity)(struct Entity *, flost));
	VtableHookTemplates_[378] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<float>);													// void (*AddOverdosePoints)(struct Entity *, float));
	VtableHookTemplates_[379] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<float>);													// void (*MaybeHealOverTime)(struct Entity *, float));
	VtableHookTemplates_[380] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsAlwaysFriend)(struct Entity *));
	VtableHookTemplates_[381] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*AdjustActorStatusBasedOnEffectChangeEtc)(struct Entity *, dword *, float));
	VtableHookTemplates_[382] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetHP)(struct Entity *));
	VtableHookTemplates_[383] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*HasAddiction)(struct Entity *));
	VtableHookTemplates_[384] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CheckOverdosed)(struct Entity *));
	VtableHookTemplates_[385] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsInWithdrawal)(struct Entity *));
	VtableHookTemplates_[386] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsRadiated)(struct Entity *));
	VtableHookTemplates_[387] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsPoisoned)(struct Entity *));
	VtableHookTemplates_[388] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsDrunk)(struct Entity *));
	VtableHookTemplates_[389] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetWaypointPauseTime)(struct Entity *));
	VtableHookTemplates_[390] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetWaypointIndex)(struct Entity *));
	VtableHookTemplates_[391] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*AlwaysReturnMaxInt)(struct Entity *));
	VtableHookTemplates_[392] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetNextWaypointList)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[393] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetWaypointActionString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[394] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*GetRepeat)(struct Entity *));
	VtableHookTemplates_[395] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*GetWaypointUseDirection)(struct Entity *));
	VtableHookTemplates_[396] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetWaypointIndex)(struct Entity *, int));
	VtableHookTemplates_[397] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<Vector3>);												// LocationXYZFloat * (*GetActorTargetLocationMaybe)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[398] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<Vector3*>);													// void (*SetActorTargetLocationMaybe)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[399] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<Vector3>);												// LocationXYZFloat * (*GetAttackDir)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[400] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetGUISlot)(struct Entity *, int));
	VtableHookTemplates_[401] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetGUISlot)(struct Entity *));
	VtableHookTemplates_[402] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<Inventory::InventoryStructType>);							// Inventory::ClassType * (*GetInventoryList)(struct Entity *, struct Inventory::ClassType *));
	VtableHookTemplates_[403] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetInventoryID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[404] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*SetInventoryID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[405] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*ClearInventoryObject)(struct Entity *));
	VtableHookTemplates_[406] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetUnk22EntityID)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[407] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*ResetUnk22Entity)(struct Entity *, struct EntityID));
	VtableHookTemplates_[408] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*InitUnkFields)(struct Entity *, bool));
	VtableHookTemplates_[409] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetMinDmg)(struct Entity *, struct EntityID));
	VtableHookTemplates_[410] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetMaxDmg)(struct Entity *, struct EntityID));
	VtableHookTemplates_[411] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetWeaponModeByIndex)(struct Entity *, int));
	VtableHookTemplates_[412] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetNumWeaponModes)(struct Entity *));
	VtableHookTemplates_[413] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop);		// WeaponMode * (*GetActiveWeaponMode)(struct Entity *));
	VtableHookTemplates_[414] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>);												// void (*SetNextWeaponMode)(struct Entity *, bool));
	VtableHookTemplates_[415] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<bool, bool>);												// bool (*SetWeaponCalledShot)(struct Entity *, bool));
	VtableHookTemplates_[416] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*WeaponCanUseCalledShot)(struct Entity *));
	VtableHookTemplates_[417] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<int, bool>);												// bool (*CanWeaponAndAttackerUseCalledShot)(struct Entity *, dword));
	VtableHookTemplates_[418] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*UseAmmoForCurrentMode)(struct Entity *));
	VtableHookTemplates_[419] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*HaveEnoughAmmoAndCanShoot)(struct Entity *));
	VtableHookTemplates_[420] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*WeaponHasNonNormalPrimaryDamageTypeMaybe)(struct Entity *));
	VtableHookTemplates_[421] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*ExplodeHeldWeaponMaybe)(struct Entity *));
	VtableHookTemplates_[422] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, int>);											// int (*GetAPCost)(struct Entity *, struct EntityID));
	VtableHookTemplates_[423] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<InventoryActionResult, EntityID, EntityID>);				// UseItemResponse * (*WeaponLoadingAmmoRelated)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID));
	VtableHookTemplates_[424] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3r1<EntityID, EntityID, EntityID>);								// EntityID * (*MaybeUnloadAmmoFromWeapon)(struct Entity *, struct EntityID *, struct EntityID, struct EntityID));
	VtableHookTemplates_[425] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetAmmoTypeString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[426] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetWeaponPerkString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[427] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetWeaponDamageRadius)(struct Entity *));
	VtableHookTemplates_[428] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetImpactFX)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[429] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSoundFX)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[430] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetProjectileName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[431] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetPrimaryDamageType)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[432] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetSecondaryDamageType)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[433] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetAmmoVariantString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[434] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetAmmoOrUsesLeft)(struct Entity *));
	VtableHookTemplates_[435] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);													// int (*GetMaxUsesOrAmmoCapacity)(struct Entity *));
	VtableHookTemplates_[436] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*WeaponIsDestroyUser)(struct Entity *));
	VtableHookTemplates_[437] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r1<EntityID, Inventory::InventoryStructType*>);				// EntityID * (*FindSuitableAmmoInInventory)(struct Entity *, struct EntityID *, struct Inventory::ClassType *));
	VtableHookTemplates_[438] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID*, bool>);											// bool (*ContainsEntity)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[439] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetEffectiveWeaponRangeByRangeType)(struct Entity *));
	VtableHookTemplates_[440] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, float>);											// float (*GetRange)(struct Entity *, struct EntityID));
	VtableHookTemplates_[441] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetMinRange)(struct Entity *));
	VtableHookTemplates_[442] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetAverageDamage)(struct Entity *));
	VtableHookTemplates_[443] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanWeaponAOE)(struct Entity *));
	VtableHookTemplates_[444] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*CanWeaponNotAOE)(struct Entity *));
	VtableHookTemplates_[445] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<int, EntityID, bool>);										// bool (*CanFireWithRemainingAP)(struct Entity *, int, struct EntityID));
	VtableHookTemplates_[446] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsActivated)(struct Entity *));
	VtableHookTemplates_[447] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<int>);														// int (*GetFrequency)(struct Entity *));
	VtableHookTemplates_[448] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<EntityID, EntityID>);										// void (*ActivateTrapRelated)(struct Entity *, struct EntityID, struct EntityID));
	VtableHookTemplates_[449] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<float>);													// float (*GetTrapTimeToDetonation)(struct Entity *));
	VtableHookTemplates_[450] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<EntityID, int>);													// void (*SetTrapFrequencyCode)(struct Entity *, EntityID, int));
	VtableHookTemplates_[451] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*DetonateTrap)(struct Entity *));
	VtableHookTemplates_[452] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*HasActiveTrap)(struct Entity *));
	VtableHookTemplates_[453] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*TrapRelatedEitherDisarmOrTrigger)(struct Entity *, struct EntityID));
	VtableHookTemplates_[454] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*SetContainedTrap)(struct Entity *, struct EntityID));
	VtableHookTemplates_[455] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3<EntityID, EntityID, bool>);									// void (*PlaceTrapMaybe)(struct Entity *, struct EntityID, struct EntityID, bool));
	VtableHookTemplates_[456] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*DisarmTrap)(struct Entity *));
	VtableHookTemplates_[457] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// bool (*ActivateOrBreakAlarm)(struct Entity *, struct EntityID));
	VtableHookTemplates_[458] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsDoorOpen)(struct Entity *));
	VtableHookTemplates_[459] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsDoorClosed)(struct Entity *));
	VtableHookTemplates_[460] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsLocked)(struct Entity *));
	VtableHookTemplates_[461] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*CanDoorOpen)(struct Entity *, struct EntityID));
	VtableHookTemplates_[462] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*Vtable462DoorRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[463] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<CombatMessage*, int>);										// void (*SetupCombatFXCombatMessageStruct)(struct Entity *, struct CombatMessage *, int));
	VtableHookTemplates_[464] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_6<int, int, int, int, int, int>);									// void (*Vtable464NoOp)(struct Entity *, int, int, int, int, int, int));
	VtableHookTemplates_[465] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<CombatMessage*>);											// void (*Vtable465CombatFX)(struct Entity *, struct CombatMessage *));
	VtableHookTemplates_[466] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_4<int, int, int, int>);										// void (*Vtable466NoOp)(struct Entity *, int, int, int, int));
	VtableHookTemplates_[467] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<Vector3*>);													// void (*Vtable467CombatFX)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[468] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable468CombatFX)(struct Entity *));
	VtableHookTemplates_[469] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<Vector3*>);												// void (*Vtable469CombatFX)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[470] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<CombatMessage*>);											// void (*CombatFXBloodSprayRelated)(struct Entity *, struct CombatMessage *));
	VtableHookTemplates_[471] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<CombatMessage*, int>);										// void (*CombatFXBloodTrailRelated)(struct Entity *, struct CombatMessage *, int));
	VtableHookTemplates_[472] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*CombatFXBloodPoolRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[473] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<Vector3*, int>);											// void (*SetupScenaryMove)(struct Entity *, struct LocationXYZFloat *, int));
	VtableHookTemplates_[474] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_6<int, int, int, int, int, int>);							// void (*Vtable474NoOp)(struct Entity *, int, int, int, int, int, int));
	VtableHookTemplates_[475] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_6<int, int, int, int, int, int>);							// void (*Vtable475NoOp)(struct Entity *, int, int, int, int, int, int));
	VtableHookTemplates_[476] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<int, int, float>);											// float (*Vtable476ReturnFloatZero)(struct Entity *, int, int));
	VtableHookTemplates_[477] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<int, Vector3*, float>);										// float (*Vtable477ActorMaybeWeaponImpactKnockdownRelated)(struct Entity *, int, struct LocationXYZFloat *));
	VtableHookTemplates_[478] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable478NoOp)(struct Entity *));
	VtableHookTemplates_[479] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable479ReturnFalse)(struct Entity *));
	VtableHookTemplates_[480] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_3<int, int, int>);												// void (*Vtable480DestructsStuff)(struct Entity *, int, int, int));
	VtableHookTemplates_[481] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsVehicleEmpty)(struct Entity *));
	VtableHookTemplates_[482] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*IsThisEntityTheDriver)(struct Entity *, struct EntityID));
	VtableHookTemplates_[483] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*IsThisEntityTheGunner)(struct Entity *, struct EntityID));
	VtableHookTemplates_[484] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<EntityID, bool>);											// bool (*IsThisEntityInVehicle)(struct Entity *, struct EntityID));
	VtableHookTemplates_[485] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityID>);												// EntityID * (*GetVehicleDriver)(struct Entity *, struct EntityID *));
	VtableHookTemplates_[486] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityIDVector>);											// void (*GetVehiclePassengerVector)(struct Entity *, struct SpecialVectorToPointers *));
	VtableHookTemplates_[487] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // bool (*VehicleBoundingBoxStuff1)(struct Entity *, struct BoundingBoxIntVehicleSpecial *));
	VtableHookTemplates_[488] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsInReverse)(struct Entity *));
	VtableHookTemplates_[489] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<EntityID, Vector3*>);										// bool (*VehiclePassengerDisembarkAllowed)(struct Entity *, struct EntityID, struct LocationXYZFloat *));
	VtableHookTemplates_[490] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<Vector3>);												// LocationXYZFloat * (*SetExitPoint)(struct Entity *, struct LocationXYZFloat *));
	VtableHookTemplates_[491] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsNotDeactivated)(struct Entity *));
	VtableHookTemplates_[492] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*ActivateActor)(struct Entity *));
	VtableHookTemplates_[493] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*DeactivateActor)(struct Entity *));
	VtableHookTemplates_[494] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*Vtable494ActorEffectsRelated)(struct Entity *, struct EntityID));
	VtableHookTemplates_[495] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<EntityID>);													// void (*RemoveEffectID)(struct Entity *, struct EntityID));
	VtableHookTemplates_[496] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*ApplyEffectsWithGivenTagName)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[497] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*BoundingBox2dSet)(struct Entity *));
	VtableHookTemplates_[498] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<EntityIDVector>);											// void (*GetListOfContainedEntities)(struct Entity *, struct SpecialVectorToPointers *));
	VtableHookTemplates_[499] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<wchar_t**, ZoneDefinition*, bool>);							// bool (*ScriptConditionCheckMaybe)(struct Entity *, struct FOTString **, ZoneDefinition *));
	VtableHookTemplates_[500] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*KillEntity)(struct Entity *, struct FOTString **));
	VtableHookTemplates_[501] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void (*SetActivationState)(struct Entity *, int));
	VtableHookTemplates_[502] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsInActiveState)(struct Entity *));
	VtableHookTemplates_[503] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsEnabledForScriptEvents)(struct Entity *));
	VtableHookTemplates_[504] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);													// void (*SetLockedState)(struct Entity *, bool));
	VtableHookTemplates_[505] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*Vtable505ActorCallsWorldForSomething)(struct Entity *));
	VtableHookTemplates_[506] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetRandomSpeechIDString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[507] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r1<wchar_t*>);												// FOTString * (*GetClickSpeechIDString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[508] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*SetRandomSpeechIDString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[509] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t**>);												// void (*SetClickSpeechIDString)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[510] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_X510); // void (*ActOnControllerCommand)(struct Entity *, struct ControllerCommandStruct *));
	VtableHookTemplates_[511] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*Vtable511ActorVehicle)(struct Entity *));
	VtableHookTemplates_[512] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetHavingTurn)(struct Entity *, bool));
	VtableHookTemplates_[513] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsHavingTurn)(struct Entity *));
	VtableHookTemplates_[514] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<bool>);														// void (*SetOverwatch)(struct Entity *, bool));
	VtableHookTemplates_[515] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool (*IsOverwatch)(struct Entity *));
	VtableHookTemplates_[516] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*ResetBonusAC)(struct Entity *));
	VtableHookTemplates_[517] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void (*SetBonusACForHTHEvade)(struct Entity *));
	VtableHookTemplates_[518] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<uint8_t>);													// byte (*GetUnk23)(struct Entity *));
	VtableHookTemplates_[519] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<int, bool>);												// bool (*Vtable519EntityESSRelated)(struct Entity *, int));
	VtableHookTemplates_[520] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1r<int, bool>);												// bool (*DoSomethingWithHP)(struct Entity *, dword));
	VtableHookTemplates_[521] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void (*Vtable521ALLSomethingWithAllFields)(struct Entity *, int *));
	VtableHookTemplates_[522] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<wchar_t*>);													// void (*Vtable522EntityDunno)(struct Entity *, struct FOTString *));
	VtableHookTemplates_[523] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // classtype_vtable * (*GetClassType)(struct Entity *));
	VtableHookTemplates_[524] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<uint8_t>);													// Entity * (*ResetAndOrDestruct)(struct Entity *, byte));
	VtableHookTemplates_[525] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0r<bool>);													// bool(*IsSomethingMDCRelated)(struct Actor *));
	VtableHookTemplates_[526] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // void(*ActorPostureRelated)(struct Actor *, int *));
	VtableHookTemplates_[527] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_noop); // float * (*RenderUnconscious)(struct Actor *, float *, int, float, bool, bool));
	VtableHookTemplates_[528] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<int, int, float>);											// float(*Vtable528NoOp)(struct Actor *, int, int));
	VtableHookTemplates_[529] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void(*AdjustRadLevelFromRadPoints)(struct Actor *));
	VtableHookTemplates_[530] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_1<int>);														// void(*ApplyRadLevelPenalties)(struct Actor *, int));
	VtableHookTemplates_[531] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void(*UndoExistingRadLevelPenalties)(struct Entity *));
	VtableHookTemplates_[532] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2<float, bool>);												// void(*PoisonEntity2)(struct Entity *, float, bool));
	VtableHookTemplates_[533] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void(*ApplyPoisonDamage)(struct Actor *));
	VtableHookTemplates_[534] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_0);															// void(*ApplyOverdoseDamage)(struct Actor *));
	VtableHookTemplates_[535] = Helpers::ConvertFunction(&HookExecutor::vtable_hook_template_2r<int, int, int>);												// int(*ContestedSkillRollMaybe)(struct Actor *, int, int));
}
