#include "Waypoint.h"

#include "LuaHelper.h"

Waypoint::Waypoint(EntityID id)
	: Entity(id)
{
}

Waypoint::Waypoint(void * ptr)
	: Entity(ptr)
{
}

Waypoint::~Waypoint()
{
}

void Waypoint::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "WaypointMetaTable");
	lua_setmetatable(l, -2);
}

void Waypoint::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "WaypointMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Waypoint, &Waypoint::GetNextWaypointList>()));
	lua_setfield(l, -2, "GetNextWaypointList");
	lua_pushcfunction(l, (LuaHelper::THUNK<Waypoint, &Waypoint::GetAction>()));
	lua_setfield(l, -2, "GetAction");
	lua_pushcfunction(l, (LuaHelper::THUNK<Waypoint, &Waypoint::GetPauseTime>()));
	lua_setfield(l, -2, "GetPauseTime");
	lua_pushcfunction(l, (LuaHelper::THUNK<Waypoint, &Waypoint::IsUseDirection>()));
	lua_setfield(l, -2, "IsUseDirection");
	lua_pushcfunction(l, (LuaHelper::THUNK<Waypoint, &Waypoint::IsRepeat>()));
	lua_setfield(l, -2, "IsRepeat");
	lua_pushcfunction(l, (LuaHelper::THUNK<Waypoint, &Waypoint::GetWaypointIndex>()));
	lua_setfield(l, -2, "GetWaypointIndex");

	lua_pushstring(l, "Waypoint");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "WaypointMetaTable");

}

Waypoint::WaypointStructType * Waypoint::GetStruct()
{
	return (WaypointStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_WAYPOINT_STRUCT);
}

float Waypoint::GetPauseTime()
{
	return GetStruct()->pausetime;
}

int32_t Waypoint::GetWaypointIndex()
{
	return GetStruct()->waypointindex;
}

std::string Waypoint::GetNextWaypointList()
{
	return Helpers::WcharToUTF8(GetStruct()->nextwaypointlist);
}

std::string Waypoint::GetAction()
{
	return Helpers::WcharToUTF8(GetStruct()->action);
}

bool Waypoint::IsRepeat()
{
	return GetStruct()->repeat;
}

bool Waypoint::IsUseDirection()
{
	return GetStruct()->usedirection;
}
