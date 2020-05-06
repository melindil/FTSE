#include "TrapTrigger.h"

#include "LuaHelper.h"

TrapTrigger::TrapTrigger(EntityID id) : Collectable(id)
{
}

TrapTrigger::TrapTrigger(void * ptr) : Collectable(ptr)
{
}

TrapTrigger::~TrapTrigger()
{
}

void TrapTrigger::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "TrapTriggerMetaTable");
	lua_setmetatable(l, -2);
}

void TrapTrigger::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "TrapTriggerMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushstring(l, "TrapTrigger");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "TrapTriggerMetaTable");

}

