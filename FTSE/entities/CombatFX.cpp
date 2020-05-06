#include "CombatFX.h"

#include "LuaHelper.h"

CombatFX::CombatFX(EntityID id)
	: Entity(id)
{
}

CombatFX::CombatFX(void * ptr)
	: Entity(ptr)
{
}

CombatFX::~CombatFX()
{
}

void CombatFX::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "CombatFXMetaTable");
	lua_setmetatable(l, -2);
}

void CombatFX::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "CombatFXMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<CombatFX, &CombatFX::GetAimpoint>()));
	lua_setfield(l, -2, "GetAimpoint");
	lua_pushcfunction(l, (LuaHelper::THUNK<CombatFX, &CombatFX::GetLastFrame>()));
	lua_setfield(l, -2, "GetLastFrame");
	lua_pushcfunction(l, (LuaHelper::THUNK<CombatFX, &CombatFX::GetNumFlames>()));
	lua_setfield(l, -2, "GetNumFlames");
	lua_pushcfunction(l, (LuaHelper::THUNK<CombatFX, &CombatFX::GetType>()));
	lua_setfield(l, -2, "GetType");

	lua_pushstring(l, "CombatFX");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "CombatFXMetaTable");

}

CombatFX::CombatFXStructType * CombatFX::GetStruct()
{
	return (CombatFXStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_COMBATFX_STRUCT);
}

int32_t CombatFX::GetType()
{
	return GetStruct()->type;
}

Vector3 CombatFX::GetAimpoint()
{
	return Vector3(GetStruct()->aimloc);
}

int32_t CombatFX::GetNumFlames()
{
	return GetStruct()->numflames;
}

int32_t CombatFX::GetLastFrame()
{
	return GetStruct()->lastframe;
}
