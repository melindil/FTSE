#include "Armour.h"

#include "LuaHelper.h"

Armour::Armour(EntityID id) : Collectable(id)
{
}

Armour::Armour(void * ptr) : Collectable(ptr)
{
}

Armour::~Armour()
{
}

void Armour::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ArmourMetaTable");
	lua_setmetatable(l, -2);
}

void Armour::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "ArmourMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushstring(l, "Armour");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ArmourMetaTable");

}

