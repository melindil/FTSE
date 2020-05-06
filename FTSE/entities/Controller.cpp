#include "Controller.h"

#include "LuaHelper.h"

Controller::Controller(EntityID id)
	: Entity(id)
{
}

Controller::Controller(void * ptr)
	: Entity(ptr)
{
}

Controller::~Controller()
{
}

void Controller::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ControllerMetaTable");
	lua_setmetatable(l, -2);
}

void Controller::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "ControllerMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetControllerDir>()));
	lua_setfield(l, -2, "GetControllerDir");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetStartWaypoint>()));
	lua_setfield(l, -2, "GetStartWaypoint");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetFollowTarget>()));
	lua_setfield(l, -2, "GetFollowTarget");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetControllerType>()));
	lua_setfield(l, -2, "GetControllerType");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetMoveTargetTag>()));
	lua_setfield(l, -2, "GetMoveTargetTag");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetWorkingZone>()));
	lua_setfield(l, -2, "GetWorkingZone");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetRetreatZone>()));
	lua_setfield(l, -2, "GetRetreatZone");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetMoveTypePriority>()));
	lua_setfield(l, -2, "GetMoveTypePriority");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetAggression>()));
	lua_setfield(l, -2, "GetAggression");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetCommunicate>()));
	lua_setfield(l, -2, "GetCommunicate");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::GetMobility>()));
	lua_setfield(l, -2, "GetMobility");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::IsGroupMove>()));
	lua_setfield(l, -2, "IsGroupMove");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::IsRandMove>()));
	lua_setfield(l, -2, "IsRandMove");
	lua_pushcfunction(l, (LuaHelper::THUNK<Controller, &Controller::IsNeverLeaveZone>()));
	lua_setfield(l, -2, "IsNeverLeaveZone");

	lua_pushstring(l, "Controller");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ControllerMetaTable");

}

Controller::ControllerStructType * Controller::GetStruct()
{
	return (ControllerStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_CONTROLLER_STRUCT);
}

std::string Controller::GetControllerType()
{
	return Helpers::WcharToUTF8(GetStruct()->controllertype);
}

std::string Controller::GetMoveTargetTag()
{
	return Helpers::WcharToUTF8(GetStruct()->movetargettag);
}

int32_t Controller::GetMoveTypePriority()
{
	return GetStruct()->movetypepriority;
}

bool Controller::IsGroupMove()
{
	return GetStruct()->groupmove;
}

bool Controller::IsRandMove()
{
	return GetStruct()->randmove;
}

std::string Controller::GetWorkingZone()
{
	return Helpers::WcharToUTF8(GetStruct()->workingzone);
}

std::string Controller::GetRetreatZone()
{
	return Helpers::WcharToUTF8(GetStruct()->retreatzone);
}

int32_t Controller::GetAggression()
{
	return GetStruct()->aggression;
}

int32_t Controller::GetCommunicate()
{
	return GetStruct()->communicate;
}

int32_t Controller::GetMobility()
{
	return GetStruct()->mobility;
}

std::shared_ptr<Entity> Controller::GetStartWaypoint()
{
	return Entity::GetEntityByID(GetStruct()->startwaypoint);
}

std::shared_ptr<Entity> Controller::GetFollowTarget()
{
	return Entity::GetEntityByID(GetStruct()->followtarget);
}

bool Controller::IsNeverLeaveZone()
{
	return GetStruct()->neverleavezone;
}

Vector3 Controller::GetControllerDir()
{
	return Vector3(GetStruct()->dir);
}
