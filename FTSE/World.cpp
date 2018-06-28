#include "World.h"

#include "lua.hpp"
#include "Logger.h"

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

void* World::GetEntity(uint16_t id)
{
	return GetGlobal()->entityStart[id].entityptr;
}
int l_gettime(lua_State* l);
void World::RegisterLua(lua_State* l,Logger* logger)
{
	globallogger = logger;
	lua_newtable(l);
	luaL_newmetatable(l, "World");
	lua_pushcfunction(l, l_gettime);
	lua_setfield(l, -2, "GetTime");
	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setmetatable(l, -2);
	lua_setglobal(l, "world");
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