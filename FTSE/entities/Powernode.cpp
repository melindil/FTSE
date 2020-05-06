#include "Powernode.h"

#include "LuaHelper.h"
#include "Helpers.h"

Powernode::Powernode(EntityID id)
	: Usable(id)
{
}

Powernode::Powernode(void * ptr)
	: Usable(ptr)
{
}

Powernode::~Powernode()
{
}

void Powernode::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "PowernodeMetaTable");
	lua_setmetatable(l, -2);
}

Powernode::PowernodeStructType * Powernode::GetStruct()
{
	return (PowernodeStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_POWERNODE_STRUCT);
}

void Powernode::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "PowernodeMetaTable");
	Usable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Powernode, &Powernode::GetHealRate>()));
	lua_setfield(l, -2, "GetHealRate");
	lua_pushcfunction(l, (LuaHelper::THUNK<Powernode, &Powernode::GetDifficulty>()));
	lua_setfield(l, -2, "GetDifficulty");
	lua_pushcfunction(l, (LuaHelper::THUNK<Powernode, &Powernode::GetArmorClass>()));
	lua_setfield(l, -2, "GetArmorClass");
	lua_pushcfunction(l, (LuaHelper::THUNK<Powernode, &Powernode::GetMaxHP>()));
	lua_setfield(l, -2, "GetMaxHP");
	lua_pushcfunction(l, (LuaHelper::THUNK<Powernode, &Powernode::GetCurrentHP>()));
	lua_setfield(l, -2, "GetCurrentHP");
	lua_pushcfunction(l, (LuaHelper::THUNK<Powernode, &Powernode::GetLastHealTime>()));
	lua_setfield(l, -2, "GetLastHealTime");
	lua_pushcfunction(l, (LuaHelper::THUNK<Powernode, &Powernode::GetCurrentAction>()));
	lua_setfield(l, -2, "GetCurrentAction");

	lua_pushstring(l, "Powernode");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "PowernodeMetaTable");
}

uint64_t Powernode::GetLastHealTime()
{
	return GetStruct()->lasthealtime;
}

int32_t Powernode::GetCurrentHP()
{
	return int32_t(GetStruct()->currenthp);
}

int32_t Powernode::GetMaxHP()
{
	return int32_t(GetStruct()->maxhp);
}

std::string Powernode::GetCurrentAction()
{
	return Helpers::WcharToUTF8(GetStruct()->currentaction);
}

int32_t Powernode::GetArmorClass()
{
	return GetStruct()->armorclass;
}

float Powernode::GetHealRate()
{
	return GetStruct()->healrate;
}

int32_t Powernode::GetDifficulty()
{
	return GetStruct()->difficulty;
}
