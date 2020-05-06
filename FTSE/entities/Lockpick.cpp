#include "Lockpick.h"

#include "LuaHelper.h"
#include "Helpers.h"

Lockpick::Lockpick(EntityID id)
	: Collectable(id)
{
}

Lockpick::Lockpick(void * ptr)
	: Collectable(ptr)
{
}

Lockpick::~Lockpick()
{
}

void Lockpick::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "LockpickMetaTable");
	lua_setmetatable(l, -2);
}

Lockpick::LockpickStructType * Lockpick::GetStruct()
{
	return (LockpickStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_LOCKPICK_STRUCT);
}

void Lockpick::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "LockpickMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Lockpick, &Lockpick::IsElectronic>()));
	lua_setfield(l, -2, "IsElectronic");

	lua_pushstring(l, "Lockpick");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "LockpickMetaTable");
}

bool Lockpick::IsElectronic()
{
	return GetStruct()->electronic;
}

