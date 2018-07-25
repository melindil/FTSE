#include "World.h"

#include "lua.hpp"
#include "Logger.h"
#include <vector>
#include "Helpers.h"

#include "Actor.h"

Logger* globallogger;
World::World()
{
}


World::~World()
{
}

World::WorldFOTObject* World::GetGlobal()
{
	return *(WorldFOTObject**)WORLD_GLOBAL_PTR;
}

void World::SetVariable(std::string const& key, std::string const& value, bool campaign)
{
#pragma pack(push,1)
	struct {
		uint32_t vtable;
		uint32_t unknown_0;
		char unknown_0a[3];
		wchar_t* key;
		wchar_t* value;
		bool campaign;
	} setvar;
#pragma pack(pop)

	setvar.vtable = 0x837b24;
	setvar.unknown_0 = 0;
	memset(setvar.unknown_0a, 0, 3);
	setvar.campaign = campaign;
	std::wstring wkey = Helpers::UTF8ToWchar(key);

	wchar_t** ptr;
	wchar_t** endptr;
	if (campaign)
	{
		uint32_t gbl = *(uint32_t*)World::CVAR_GLOBAL_PTR;
		ptr = *(wchar_t***)(gbl + 0x3b);
		endptr = *(wchar_t***)(gbl + 0x3f);
	}
	else
	{
		World::WorldFOTObject* world = World::GetGlobal();
		ptr = world->missionvar_key_start;
		endptr = world->missionvar_key_end;
	}
	setvar.key = nullptr;
	while (ptr != endptr)
	{
		if (wcscmp(*ptr, wkey.c_str()) == 0)
		{
			setvar.key = *ptr;
		}
		ptr++;
	}

	if (setvar.key == nullptr)
	{
		return;
	}
	wchar_t* wval = Helpers::UTF8ToWcharFOTHeap(value, 0);
	setvar.value = wval;

	DummyClass* c1 = (DummyClass*)&setvar;
	auto fxn = &DummyClass::SetVariable;
	uint32_t offset = FXN_WORLD_SETVARIABLE;
	memcpy(&fxn, &offset, 4);
	(c1->*fxn)();

}

void* World::GetEntity(uint16_t id)
{
	return GetGlobal()->entityStart[id].entityptr;
}
int l_gettime(lua_State* l);
int l_getmissionvar(lua_State* l);
int l_getcampaignvar(lua_State* l); 
int l_setmissionvar(lua_State* l);
int l_setcampaignvar(lua_State* l);
int l_getplayer(lua_State* l);
int l_getsquad(lua_State* l);

void World::RegisterLua(lua_State* l,Logger* logger)
{
	globallogger = logger;
	lua_newtable(l);
	luaL_newmetatable(l, "World");
	lua_pushcfunction(l, l_gettime);
	lua_setfield(l, -2, "GetTime");
	lua_pushcfunction(l, l_getmissionvar);
	lua_setfield(l, -2, "GetMissionVar");
	lua_pushcfunction(l, l_getcampaignvar);
	lua_setfield(l, -2, "GetCampaignVar");
	lua_pushcfunction(l, l_setmissionvar);
	lua_setfield(l, -2, "SetMissionVar");
	lua_pushcfunction(l, l_setcampaignvar);
	lua_setfield(l, -2, "SetCampaignVar");
	lua_pushcfunction(l, l_getplayer);
	lua_setfield(l, -2, "GetPlayer");
	lua_pushcfunction(l, l_getsquad);
	lua_setfield(l, -2, "GetSquad");
	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setmetatable(l, -2);
	lua_setglobal(l, "world");
}

int l_setmissionvar(lua_State* l)
{
	std::string key = lua_tostring(l, 2);
	std::string val = lua_tostring(l, 3);
	World::SetVariable(key, val, false);
	return 0;
}
int l_setcampaignvar(lua_State* l)
{
	std::string key = lua_tostring(l, 2);
	std::string val = lua_tostring(l, 3);
	World::SetVariable(key, val, true);
	return 0;
}

int l_getmissionvar(lua_State* l)
{
	std::string key = lua_tostring(l, 2);
	auto wkey = Helpers::UTF8ToWchar(key);

	World::WorldFOTObject* world = World::GetGlobal();
	wchar_t** ptr = world->missionvar_key_start;
	wchar_t** valptr = world->missionvar_val_start;
	while (ptr != world->missionvar_key_end)
	{
		if (wcscmp(*ptr, wkey.c_str()) == 0)
		{
			std::string val = Helpers::WcharToUTF8(*valptr);
			lua_pushstring(l, val.c_str());
			return 1;
		}
		ptr++;
		valptr++;
	}
	lua_pushstring(l, "");
	return 1;
}

int l_getcampaignvar(lua_State* l)
{
	std::string key = lua_tostring(l, 2);
	auto wkey = Helpers::UTF8ToWchar(key);

	uint32_t gbl = *(uint32_t*)World::CVAR_GLOBAL_PTR;
	wchar_t** ptr = *(wchar_t***)(gbl+0x3b);
	wchar_t** endptr = *(wchar_t***)(gbl + 0x3f);
	wchar_t** valptr = *(wchar_t***)(gbl + 0x2b);
	while (ptr != endptr)
	{
		if (wcscmp(*ptr, wkey.c_str()) == 0)
		{
			std::string val = Helpers::WcharToUTF8(*valptr);
			lua_pushstring(l, val.c_str());
			return 1;
		}
		ptr++;
		valptr++;
	}
	lua_pushstring(l, "");
	return 1;
}

int l_gettime(lua_State* l)
{
	World::WorldFOTObject* world = World::GetGlobal();
	lua_getglobal(l, "MsecToGametime"); 
	if (lua_isfunction(l, -1))
	{
		lua_pushinteger(l, world->gameTime*3);
		lua_pcall(l, 1, 1, 0);
	}
	else
	{
		lua_pop(l, 1);
		lua_newtable(l);
	}	
	return 1;
}

int l_getplayer(lua_State* l)
{
	auto sqd = World::GetSquad();
	Actor a(sqd[0]);
	a.MakeLuaObject(l);
	return 1;
}

int l_getsquad(lua_State* l)
{
	auto sqd = World::GetSquad();
	lua_newtable(l);
	for (uint32_t i = 0; i < sqd.size(); i++)
	{
		Actor a(sqd[i]);
		a.MakeLuaObject(l);
		lua_rawseti(l, -2, i + 1);
	}
	return 1;
}

std::vector<uint16_t> World::GetSquad()
{
	std::vector<uint16_t> sqd;
	World::WorldFOTObject* world = World::GetGlobal();
	auto players = world->ptrPlayerList;
	players++;		// We want player[1]

	auto iter = players->players;
	auto iter_end = iter->prev;
	while (iter != iter_end)
	{
		iter = iter->next;		// yes, increment first - we skip the head node
		uint32_t* ent = (uint32_t*)World::GetEntity(iter->entity_id);
		if (*ent == ACTOR_VTABLE)
		{
			sqd.push_back(iter->entity_id);
		}
	}
	return sqd;
}