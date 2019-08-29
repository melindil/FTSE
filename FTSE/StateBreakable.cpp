#include "StateBreakable.h"
#include "LuaHelper.h"

StateBreakable::StateBreakable(uint16_t id)
	: Breakable(id)
{
}

StateBreakable::StateBreakable(void * ptr)
	: Breakable(ptr)
{
}

StateBreakable::~StateBreakable()
{
}

void StateBreakable::MakeLuaObject(lua_State * l)
{
	// Represent an entity as a table containing name and ID
// Can add other elements here for quick access (if needed)
// Otherwise, use the entity ID as the identifier to re-obtain
// the pointer from the entity table if we have to make any
// changes to it.

	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "StateBreakableMetaTable");
	lua_setmetatable(l, -2);

}

int l_statebreakable_ison(lua_State* l)
{
	StateBreakable e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsOn());
	return 1;
}

void StateBreakable::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "StateBreakableMetaTable");

	Breakable::SetLuaSubclass(l);

	lua_pushcfunction(l, l_statebreakable_ison);
	lua_setfield(l, -2, "On");

	lua_pushstring(l, "StateBreakable");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "StateBreakableMetaTable");
}

bool StateBreakable::IsOn()
{
	return *(bool*)(((uint32_t)GetEntityPointer()) + OFFSET_STATEBREAKABLE_STRUCT);
}
