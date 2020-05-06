#include "Geiger.h"

#include "LuaHelper.h"

Geiger::Geiger(EntityID id) : Collectable(id)
{
}

Geiger::Geiger(void * ptr) : Collectable(ptr)
{
}

Geiger::~Geiger()
{
}

void Geiger::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "GeigerMetaTable");
	lua_setmetatable(l, -2);
}

void Geiger::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "GeigerMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushstring(l, "Geiger");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "GeigerMetaTable");

}
