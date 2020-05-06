#include "StateScenary.h"

#include "LuaHelper.h"

StateScenary::StateScenary(EntityID id)
	: Entity(id)
{
}

StateScenary::StateScenary(void * ptr)
	: Entity(ptr)
{
}

StateScenary::~StateScenary()
{
}

void StateScenary::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "StateScenaryMetaTable");
	lua_setmetatable(l, -2);
}

void StateScenary::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "StateScenaryMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<StateScenary, &StateScenary::IsOn>()));
	lua_setfield(l, -2, "IsOn");

	lua_pushstring(l, "StateScenary");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "StateScenaryMetaTable");

}

StateScenary::StateScenaryStructType * StateScenary::GetStruct()
{
	return (StateScenaryStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_STATESCENARY_STRUCT);
}

bool StateScenary::IsOn()
{
	return GetStruct()->on;
}
