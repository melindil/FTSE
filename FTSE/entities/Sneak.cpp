#include "Sneak.h"

#include "LuaHelper.h"

Sneak::Sneak(EntityID id) : Entity(id)
{
}

Sneak::Sneak(void * ptr) : Entity(ptr)
{
}

Sneak::~Sneak()
{
}

void Sneak::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "SneakMetaTable");
	lua_setmetatable(l, -2);
}

void Sneak::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "SneakMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushstring(l, "Sneak");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "SneakMetaTable");

}


