#include "ScienceSwitch.h"

#include "LuaHelper.h"
#include "Helpers.h"

ScienceSwitch::ScienceSwitch(EntityID id)
	: Switch(id)
{
}

ScienceSwitch::ScienceSwitch(void * ptr)
	: Switch(ptr)
{
}

ScienceSwitch::~ScienceSwitch()
{
}

void ScienceSwitch::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ScienceSwitchMetaTable");
	lua_setmetatable(l, -2);
}

ScienceSwitch::ScienceSwitchStructType * ScienceSwitch::GetStruct()
{
	return (ScienceSwitchStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_SCIENCESWITCH_STRUCT);
}

void ScienceSwitch::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "ScienceSwitchMetaTable");
	Switch::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<ScienceSwitch, &ScienceSwitch::GetDifficulty>()));
	lua_setfield(l, -2, "GetDifficulty");

	lua_pushstring(l, "ScienceSwitch");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ScienceSwitchMetaTable");
}

int32_t ScienceSwitch::GetDifficulty()
{
	return GetStruct()->difficulty;
}
