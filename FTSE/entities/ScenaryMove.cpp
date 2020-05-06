#include "ScenaryMove.h"

#include "LuaHelper.h"

ScenaryMove::ScenaryMove(EntityID id) : Entity(id)
{
}

ScenaryMove::ScenaryMove(void * ptr) : Entity(ptr)
{
}

ScenaryMove::~ScenaryMove()
{
}

void ScenaryMove::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ScenaryMoveMetaTable");
	lua_setmetatable(l, -2);
}

void ScenaryMove::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "ScenaryMoveMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushstring(l, "ScenaryMove");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ScenaryMoveMetaTable");

}
