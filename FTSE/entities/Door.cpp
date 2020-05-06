#include "Door.h"

#include "LuaHelper.h"
#include "Helpers.h"

Door::Door(EntityID id)
	: Usable(id)
{
}

Door::Door(void * ptr)
	: Usable(ptr)
{
}

Door::~Door()
{
}

void Door::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "DoorMetaTable");
	lua_setmetatable(l, -2);
}


Door::DoorStructType * Door::GetStruct()
{
	return (DoorStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_DOOR_STRUCT);
}

void Door::SetLuaSubclass(lua_State * l)
{
	Usable::SetLuaSubclass(l);
	lua_pushboolean(l, true);
	lua_setfield(l, -2, "isDoor");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::GetSounds>()));
	lua_setfield(l, -2, "GetSounds");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::GetSensorRange>()));
	lua_setfield(l, -2, "GetSensorRange");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::GetChanceUnlock>()));
	lua_setfield(l, -2, "GetChanceUnlock");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::GetTimeOpen>()));
	lua_setfield(l, -2, "GetTimeOpen");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::IsLaserDoor>()));
	lua_setfield(l, -2, "IsLaserDoor");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::IsAutoclose>()));
	lua_setfield(l, -2, "IsAutoclose");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::CanBreak>()));
	lua_setfield(l, -2, "CanBreak");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::IsSensorOpen>()));
	lua_setfield(l, -2, "IsSensorOpen");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::IsTrapDoor>()));
	lua_setfield(l, -2, "IsTrapDoor");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::IsOpen>()));
	lua_setfield(l, -2, "IsOpen");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::IsOpenOutwards>()));
	lua_setfield(l, -2, "IsOpenOutwards");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::IsLocked>()));
	lua_setfield(l, -2, "IsLocked");
	lua_pushcfunction(l, (LuaHelper::THUNK<Door, &Door::IsLockable>()));
	lua_setfield(l, -2, "IsLockable");
}

void Door::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "DoorMetaTable");
	SetLuaSubclass(l);

	lua_pushstring(l, "Door");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "DoorMetaTable");
}

bool Door::IsOpen()
{
	return GetStruct()->isopen;
}

bool Door::IsOpenOutwards()
{
	return GetStruct()->openoutwards;
}

bool Door::IsLocked()
{
	return GetStruct()->islocked;
}

bool Door::IsLockable()
{
	return GetStruct()->islockable;
}

int32_t Door::GetChanceUnlock()
{
	return GetStruct()->chanceunlock;
}

bool Door::IsAutoclose()
{
	return GetStruct()->autoclose;
}

int32_t Door::GetTimeOpen()
{
	return GetStruct()->timeopen;
}

bool Door::CanBreak()
{
	return GetStruct()->canbreak;
}

std::string Door::GetSounds()
{
	return Helpers::WcharToUTF8(GetStruct()->sounds);
}

bool Door::IsSensorOpen()
{
	return GetStruct()->sensoropen;
}

float Door::GetSensorRange()
{
	return GetStruct()->sensorrange;
}

bool Door::IsTrapDoor()
{
	return GetStruct()->trapdoor;
}

bool Door::IsLaserDoor()
{
	return GetStruct()->laserdoor;
}
