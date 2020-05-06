#include "ScenarySpawn.h"

#include "LuaHelper.h"

ScenarySpawn::ScenarySpawn(EntityID id)
	: Entity(id)
{
}

ScenarySpawn::ScenarySpawn(void * ptr)
	: Entity(ptr)
{
}

ScenarySpawn::~ScenarySpawn()
{
}

void ScenarySpawn::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ScenarySpawnMetaTable");
	lua_setmetatable(l, -2);
}

void ScenarySpawn::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "ScenarySpawnMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<ScenarySpawn, &ScenarySpawn::GetSpeed>()));
	lua_setfield(l, -2, "GetSpeed");
	lua_pushcfunction(l, (LuaHelper::THUNK<ScenarySpawn, &ScenarySpawn::GetFrequency>()));
	lua_setfield(l, -2, "GetFrequency");
	lua_pushcfunction(l, (LuaHelper::THUNK<ScenarySpawn, &ScenarySpawn::GetMaxSpawn>()));
	lua_setfield(l, -2, "GetMaxSpawn");
	lua_pushcfunction(l, (LuaHelper::THUNK<ScenarySpawn, &ScenarySpawn::GetSpawnSprite>()));
	lua_setfield(l, -2, "GetSpawnSprite");

	lua_pushstring(l, "ScenarySpawn");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ScenarySpawnMetaTable");

}

ScenarySpawn::ScenarySpawnStructType * ScenarySpawn::GetStruct()
{
	return (ScenarySpawnStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_SCENARYSPAWN_STRUCT);
}

float ScenarySpawn::GetSpeed()
{
	return GetStruct()->speed;
}

float ScenarySpawn::GetFrequency()
{
	return GetStruct()->frequency;
}

std::string ScenarySpawn::GetSpawnSprite()
{
	return Helpers::WcharToUTF8(GetStruct()->sprite);
}

int32_t ScenarySpawn::GetMaxSpawn()
{
	return GetStruct()->maxspawn;
}
