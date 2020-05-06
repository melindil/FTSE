#include "RotatingDoor.h"

#include "LuaHelper.h"

RotatingDoor::RotatingDoor(EntityID id) : Door(id)
{
}

RotatingDoor::RotatingDoor(void * ptr) : Door(ptr)
{
}

RotatingDoor::~RotatingDoor()
{
}

void RotatingDoor::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "RotatingDoorMetaTable");
	lua_setmetatable(l, -2);
}

void RotatingDoor::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "RotatingDoorMetaTable");
	Door::SetLuaSubclass(l);

	lua_pushstring(l, "RotatingDoor");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "RotatingDoorMetaTable");

}
