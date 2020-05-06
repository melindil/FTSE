#include "SpawnPoint.h"
#include "LuaHelper.h"

SpawnPoint::SpawnPoint(EntityID id)
	: Entity(id)
{
}

SpawnPoint::SpawnPoint(void * ptr)
	: Entity(ptr)
{
}

SpawnPoint::~SpawnPoint()
{
}

void SpawnPoint::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "SpawnPointMetaTable");
	lua_setmetatable(l, -2);
}

void SpawnPoint::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "SpawnPointMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<SpawnPoint, &SpawnPoint::GetDesiredTeam>()));
	lua_setfield(l, -2, "GetDesiredTeam");
	lua_pushcfunction(l, (LuaHelper::THUNK<SpawnPoint, &SpawnPoint::GetSpawnCounter>()));
	lua_setfield(l, -2, "GetSpawnCounter");
	lua_pushcfunction(l, (LuaHelper::THUNK<SpawnPoint, &SpawnPoint::GetSpawnType>()));
	lua_setfield(l, -2, "GetSpawnType");
	lua_pushcfunction(l, (LuaHelper::THUNK<SpawnPoint, &SpawnPoint::GetMainDudeOnly>()));
	lua_setfield(l, -2, "GetMainDudeOnly");

	lua_pushstring(l, "SpawnPoint");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "SpawnPointMetaTable");

}

int32_t SpawnPoint::GetDesiredTeam()
{
	return GetStruct()->desiredteam;
}

int32_t SpawnPoint::GetSpawnCounter()
{
	return GetStruct()->spawncounter;
}

std::string SpawnPoint::GetSpawnType()
{
	return Helpers::WcharToUTF8(GetStruct()->spawntype);
}

bool SpawnPoint::GetMainDudeOnly()
{
	return GetStruct()->maindudeonly;
}

SpawnPoint::SpawnPointStructType * SpawnPoint::GetStruct()
{
	return (SpawnPointStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_SPAWNPOINT_STRUCT);
}
