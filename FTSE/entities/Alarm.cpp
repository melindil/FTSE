#include "Alarm.h"

#include "LuaHelper.h"
#include "Helpers.h"

Alarm::Alarm(EntityID id)
	: Usable(id)
{
}

Alarm::Alarm(void * ptr)
	: Usable(ptr)
{
}

Alarm::~Alarm()
{
}

void Alarm::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "AlarmMetaTable");
	lua_setmetatable(l, -2);
}

Alarm::AlarmStructType * Alarm::GetStruct()
{
	return (AlarmStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_ALARM_STRUCT);
}

void Alarm::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "AlarmMetaTable");
	Usable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Alarm, &Alarm::IsActivated>()));
	lua_setfield(l, -2, "IsActivated");

	lua_pushstring(l, "Alarm");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "AlarmMetaTable");
}

bool Alarm::IsActivated()
{
	return GetStruct()->isactivated;
}
