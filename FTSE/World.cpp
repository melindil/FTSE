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

	auto fxn = (void(__thiscall*)(void*))FXN_WORLD_SETVARIABLE;
	fxn(&setvar);


}

int l_gettime(lua_State* l);
int l_getmissionvar(lua_State* l);
int l_getcampaignvar(lua_State* l); 
int l_setmissionvar(lua_State* l);
int l_setcampaignvar(lua_State* l);
int l_getplayer(lua_State* l);
int l_getsquad(lua_State* l);
int l_combatlog(lua_State* l);

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
	lua_pushcfunction(l, l_combatlog);
	lua_setfield(l, -2, "CombatLog");
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
	lua_getglobal(l, "MsecToDayHMS"); 
	if (lua_isfunction(l, -1))
	{
		lua_pushinteger(l, world->gameTime);
		lua_pushinteger(l, 1);
		lua_pcall(l, 2, 1, 0);
		lua_getglobal(l, "AddBaseToGameTime");
		lua_insert(l, -2);	// swap function and previous retrun
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

int l_combatlog(lua_State* l)
{
	int level =(int) lua_tointeger(l, 2);
	std::string txt = lua_tostring(l, 3);
	World::CombatLog(level, txt);
	return 0;
}

void World::CombatLog(int level, std::string const& txt)
{
	typedef void(__thiscall *fxntype)(void*, wchar_t**, uint32_t, uint32_t, uint32_t*);
	auto fxn = (fxntype) FXN_WORLD_COMBATLOG;

	auto wchar_txt = Helpers::UTF8ToWcharFOTHeap(txt, 0);
	uint32_t unknown_var4 = 0xffffffffu;
	(*fxn)(GetEntity(GetSquad()[0]), &wchar_txt, level, 0, &unknown_var4);
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

bool World::CheckBlocked(Vector3 source, Vector3 target)
{
	BlockSearch bs;
	memset(&bs, 0, sizeof(BlockSearch));
	bs.vtable = BLOCKSEARCH_VTABLE;
	*(BLOCKSEARCH_CTR_PTR)++;
	bs.ctr = *BLOCKSEARCH_CTR_PTR;

	// Calculate the points as done in FoT code: each coord is
	// first multiplied ed by 4 and made an int. pt3 has the highest X,Y,Z
	// values, and pt4 the smallest. Then, add one to each of the lowest
	// coords, and set pt2 to the nee highest and pt1 to the new lowest.
	for (int i = 0; i < 3; i++)
	{
		int32_t sp = (int32_t)(source.v[i] * 4.0f);
		int32_t tp = (int32_t)(target.v[i] * 4.0f);
		if (sp < tp)
		{
			bs.pt1[i] = sp + 1;
			bs.pt2[i] = tp;
			bs.pt3[i] = tp;
			bs.pt4[i] = sp;
		}
		else if (tp < sp)
		{
			bs.pt1[i] = tp + 1;
			bs.pt2[i] = sp;
			bs.pt3[i] = sp;
			bs.pt4[i] = tp;
		}
		else
		{
			bs.pt1[i] = bs.pt3[i] = bs.pt4[i] = tp;
			bs.pt2[i] = tp + 1;
		}
	}

	auto fxn = (void(__thiscall*)(void*, void*))(0x6e73e0);
	World::WorldFOTObject* world = World::GetGlobal();
	fxn(&world->level_object, &bs);
	return bs.blockedflags;
}