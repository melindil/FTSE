#include "World.h"

#include "lua.hpp"
#include "LuaHelper.h"
#include "Logger.h"
#include <vector>
#include "Helpers.h"

#include "Actor.h"
#include "Collectable.h"

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

void World::AdvanceTime(int64_t msec)
{
	World::WorldFOTObject* world = World::GetGlobal();
	world->gameTime += msec;

	auto fxn = (void(__thiscall*)(void*))FXN_WORLD_ADVANCETIME;
	fxn(world);

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
int l_getallentities(lua_State* l);
int l_getentitiesbytag(lua_State* l);
int l_createentity(lua_State* l);
int l_advancetime(lua_State* l);

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
	lua_pushcfunction(l, l_getallentities);
	lua_setfield(l, -2, "GetAllEntities");
	lua_pushcfunction(l, l_getentitiesbytag);
	lua_setfield(l, -2, "GetEntitiesByTag");
	lua_pushcfunction(l, l_advancetime);
	lua_setfield(l, -2, "AdvanceTime");

	lua_pushcfunction(l, l_createentity);
	lua_setfield(l, -2, "CreateEntity");
	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setmetatable(l, -2);
	lua_setglobal(l, "world");
}

int l_getallentities(lua_State* l)
{
	std::vector<void*> entities = World::GetAllEntities();
	lua_newtable(l);
	for (unsigned int i=0;i<entities.size(); i++)
	{
		Entity::GetEntityByPointer(entities[i])->MakeLuaObject(l);
		lua_rawseti(l, -2, i + 1);
	}
	return 1;
}

int l_getentitiesbytag(lua_State* l)
{
	std::string tagname = lua_tostring(l, 2);
	std::vector<void*> entities = World::GetAllEntities();
	lua_newtable(l);
	int j = 1;
	for (auto entity : entities)
	{
		auto ent = Entity::GetEntityByPointer(entity);
		if (ent->GetTag() == tagname)
		{
			ent->MakeLuaObject(l);
			lua_rawseti(l, -2, j);
			j++;
		}
	}
	return 1;
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

int l_advancetime(lua_State* l)
{
	World::WorldFOTObject* world = World::GetGlobal();
	int64_t msec;
	msec = (int64_t)LuaHelper::GetTableInteger(l, 2, "day") * 86400000LL;
	msec += (int64_t)LuaHelper::GetTableInteger(l, 2, "hour") * 3600000LL;
	msec += (int64_t)LuaHelper::GetTableInteger(l, 2, "minute") * 60000LL;
	msec += (int64_t)LuaHelper::GetTableInteger(l, 2, "second") * 1000LL;
	msec += (int64_t)LuaHelper::GetTableInteger(l, 2, "msec");
	msec /= 3;

	World::AdvanceTime(msec);

	return 0;
}

int l_getplayer(lua_State* l)
{
	auto sqd = World::GetSquad();
	Entity::GetEntityByID(sqd[0])->MakeLuaObject(l);
	return 1;
}

int l_getsquad(lua_State* l)
{
	auto sqd = World::GetSquad();
	lua_newtable(l);
	for (uint32_t i = 0; i < sqd.size(); i++)
	{
		
		Entity::GetEntityByID(sqd[i])->MakeLuaObject(l);
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

int l_createentity(lua_State* l)
{
	int32_t count = 1;
	std::string entfile = lua_tostring(l, 2);
	if (lua_isinteger(l, 3))
	{
		count = (int32_t)lua_tointeger(l, 3);
	}

	World::CreateEntity(entfile, count)->MakeLuaObject(l);
	return 1;

}

void World::CombatLog(int level, std::string const& txt)
{
	typedef void(__thiscall *fxntype)(void*, wchar_t**, uint32_t, uint32_t, uint32_t*);
	auto fxn = (fxntype) FXN_WORLD_COMBATLOG;

	auto wchar_txt = Helpers::UTF8ToWcharFOTHeap(txt, 0);
	uint32_t unknown_var4 = 0xffffffffu;
	(*fxn)(GetEntity(GetSquad()[0]), &wchar_txt, level, 0, &unknown_var4);
}

bool World::IsLoaded()
{
	World::WorldFOTObject* world = World::GetGlobal();
	auto players = world->ptrPlayerList;

	// First check if no players at all
	if (players == world->ptrPlayerListEnd)
		return false;

	// Skip ahead to player 1 (typically the one that the player's squad is in), and see if it's OK
	players++;		// We want player[1]
	if (players == world->ptrPlayerListEnd)
		return false;

	// If the squad is empty (head node prev pointer points to itself), then we haven't finished loading yet
	// (things aren't finished setting up if this is the case)
	auto iter = players->players;
	if (iter == iter->prev) 
	{
		return false;
	}
	return true;
}

std::vector<EntityID> World::GetSquad()
{
	std::vector<EntityID> sqd;
	World::WorldFOTObject* world = World::GetGlobal();
	auto players = world->ptrPlayerList;
	players++;		// We want player[1]
	auto iter = players->players;
	auto iter_end = iter->prev;
	while (iter != iter_end)
	{
		iter = iter->next;		// yes, increment first - we skip the head node
		sqd.push_back(iter->entity_id);
		
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

std::shared_ptr<Entity> World::CreateEntity(std::string const & entityfile, int32_t count)
{
	wchar_t* filename = Helpers::UTF8ToWcharFOTHeap(entityfile, 1);
	FOTString fs(filename);
	EntityID id;

	auto fxn = (void*(*)(EntityID*, wchar_t**, int32_t))FXN_WORLD_MAKEENTITY;
	fxn(&id, &filename, -1);
	std::shared_ptr<Entity> ent = Entity::GetEntityByID(id);
	auto coll = std::dynamic_pointer_cast<Collectable>(ent);
	if (coll)
	{
		coll->OverrideCount(count);
	}
	return ent;
}

std::vector<void*> World::GetAllEntities()
{
	std::vector<void*> entities;
	World::WorldFOTObject* world = World::GetGlobal();
	for (EntityTable* entry = world->entityStart;
		entry != world->entityEnd;
		entry++)
	{
		if (entry->entityptr != world->dummyEntity)
		{
			entities.push_back(entry->entityptr);
		}
	}
	return entities;

}