#include "Ammo.h"
#include "LuaHelper.h"
#include "Helpers.h"

Ammo::Ammo(EntityID id)
	: Collectable(id)
{
}

Ammo::Ammo(void * ptr)
	: Collectable(ptr)
{
}

Ammo::~Ammo()
{
}

void Ammo::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "AmmoMetaTable");
	lua_setmetatable(l, -2);
}

Ammo::AmmoStructType * Ammo::GetStruct()
{
	return (AmmoStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_AMMO_STRUCT);
}

void Ammo::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "AmmoMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Ammo, &Ammo::GetType>()));
	lua_setfield(l, -2, "GetType");
	lua_pushcfunction(l, (LuaHelper::THUNK<Ammo, &Ammo::GetVariant>()));
	lua_setfield(l, -2, "GetVariant");
	lua_pushcfunction(l, (LuaHelper::THUNK<Ammo, &Ammo::GetPrimaryDamageType>()));
	lua_setfield(l, -2, "GetPrimaryDamageType");
	lua_pushcfunction(l, (LuaHelper::THUNK<Ammo, &Ammo::GetSecondaryDamageType>()));
	lua_setfield(l, -2, "GetSecondaryDamageType");
	lua_pushcfunction(l, (LuaHelper::THUNK<Ammo, &Ammo::GetImpactFX>()));
	lua_setfield(l, -2, "GetImpactFX");
	lua_pushcfunction(l, (LuaHelper::THUNK<Ammo, &Ammo::GetImpactSoundFX>()));
	lua_setfield(l, -2, "GetImpactSoundFX");

	lua_pushstring(l, "Ammo");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "AmmoMetaTable");
}

std::string Ammo::GetType()
{
	return Helpers::WcharToUTF8(GetStruct()->type);
}

std::string Ammo::GetVariant()
{
	return Helpers::WcharToUTF8(GetStruct()->variant);
}

std::string Ammo::GetPrimaryDamageType()
{
	return Helpers::WcharToUTF8(GetStruct()->primarydamagetype);
}

std::string Ammo::GetSecondaryDamageType()
{
	return Helpers::WcharToUTF8(GetStruct()->secondarydamagetype);
}

std::string Ammo::GetImpactFX()
{
	return Helpers::WcharToUTF8(GetStruct()->impactfx);
}

std::string Ammo::GetImpactSoundFX()
{
	return Helpers::WcharToUTF8(GetStruct()->impactsoundfx);
}
