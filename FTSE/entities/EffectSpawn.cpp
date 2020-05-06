#include "EffectSpawn.h"

#include "LuaHelper.h"

EffectSpawn::EffectSpawn(EntityID id)
	: Entity(id)
{
}

EffectSpawn::EffectSpawn(void * ptr)
	: Entity(ptr)
{
}

EffectSpawn::~EffectSpawn()
{
}

void EffectSpawn::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "EffectSpawnMetaTable");
	lua_setmetatable(l, -2);
}

void EffectSpawn::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "EffectSpawnMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<EffectSpawn, &EffectSpawn::GetSprite>()));
	lua_setfield(l, -2, "GetSprite");
	lua_pushcfunction(l, (LuaHelper::THUNK<EffectSpawn, &EffectSpawn::GetSequence>()));
	lua_setfield(l, -2, "GetSequence");
	lua_pushcfunction(l, (LuaHelper::THUNK<EffectSpawn, &EffectSpawn::GetLightAttach>()));
	lua_setfield(l, -2, "GetLightAttach");
	lua_pushcfunction(l, (LuaHelper::THUNK<EffectSpawn, &EffectSpawn::GetWavSound>()));
	lua_setfield(l, -2, "GetWavSound");
	lua_pushcfunction(l, (LuaHelper::THUNK<EffectSpawn, &EffectSpawn::GetRandomDelay>()));
	lua_setfield(l, -2, "GetRandomDelay");
	lua_pushcfunction(l, (LuaHelper::THUNK<EffectSpawn, &EffectSpawn::IsOn>()));
	lua_setfield(l, -2, "IsOn");

	lua_pushstring(l, "EffectSpawn");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "EffectSpawnMetaTable");

}

EffectSpawn::EffectSpawnStructType * EffectSpawn::GetStruct()
{
	return (EffectSpawnStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_EFFECTSPAWN_STRUCT);
}


std::string EffectSpawn::GetSprite()
{
	return Helpers::WcharToUTF8(GetStruct()->sprite);
}

std::string EffectSpawn::GetSequence()
{
	return Helpers::WcharToUTF8(GetStruct()->sequence);
}

std::string EffectSpawn::GetLightAttach()
{
	return Helpers::WcharToUTF8(GetStruct()->lightattach);
}

std::string EffectSpawn::GetWavSound()
{
	return Helpers::WcharToUTF8(GetStruct()->wavsound);
}

int32_t EffectSpawn::GetRandomDelay()
{
	return GetStruct()->randomdelay;
}

bool EffectSpawn::IsOn()
{
	return GetStruct()->on;
}
