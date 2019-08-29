#include "Ammo.h"
#include "LuaHelper.h"
#include "Helpers.h"

Ammo::Ammo(uint16_t id)
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

int l_ammo_gettype(lua_State* l)
{
	Ammo a(LuaHelper::GetEntityId(l));
	lua_pushstring(l, a.GetType().c_str());
	return 1;
}

int l_ammo_getvariant(lua_State* l)
{
	Ammo a(LuaHelper::GetEntityId(l));
	lua_pushstring(l, a.GetVariant().c_str());
	return 1;
}

int l_ammo_getprimarydamagetype(lua_State* l)
{
	Ammo a(LuaHelper::GetEntityId(l));
	lua_pushstring(l, a.GetPrimaryDamageType().c_str());
	return 1;
}

int l_ammo_getsecondarydamagetype(lua_State* l)
{
	Ammo a(LuaHelper::GetEntityId(l));
	lua_pushstring(l, a.GetSecondaryDamageType().c_str());
	return 1;
}

int l_ammo_getimpactfx(lua_State* l)
{
	Ammo a(LuaHelper::GetEntityId(l));
	lua_pushstring(l, a.GetImpactFX().c_str());
	return 1;
}

int l_ammo_getimpactsoundfx(lua_State* l)
{
	Ammo a(LuaHelper::GetEntityId(l));
	lua_pushstring(l, a.GetImpactSoundFX().c_str());
	return 1;
}

Ammo::AmmoStructType * Ammo::GetStruct()
{
	return (AmmoStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_AMMO_STRUCT);
}

void Ammo::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "AmmoMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, l_ammo_gettype);
	lua_setfield(l, -2, "GetType");
	lua_pushcfunction(l, l_ammo_getvariant);
	lua_setfield(l, -2, "GetVariant");
	lua_pushcfunction(l, l_ammo_getprimarydamagetype);
	lua_setfield(l, -2, "GetPrimaryDamageType");
	lua_pushcfunction(l, l_ammo_getsecondarydamagetype);
	lua_setfield(l, -2, "GetSecondaryDamageType");
	lua_pushcfunction(l, l_ammo_getimpactfx);
	lua_setfield(l, -2, "GetImpactFX");
	lua_pushcfunction(l, l_ammo_getimpactsoundfx);
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
