#include "Scenary.h"

#include "LuaHelper.h"

Scenary::Scenary(EntityID id) : Entity(id)
{
}

Scenary::Scenary(void * ptr) : Entity(ptr)
{
}

Scenary::~Scenary()
{
}

void Scenary::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ScenaryMetaTable");
	lua_setmetatable(l, -2);
}

void Scenary::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "ScenaryMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushstring(l, "Scenary");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ScenaryMetaTable");

}

