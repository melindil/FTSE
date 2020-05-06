#include "GenericItem.h"

#include "LuaHelper.h"

GenericItem::GenericItem(EntityID id) : Collectable(id)
{
}

GenericItem::GenericItem(void * ptr) : Collectable(ptr)
{
}

GenericItem::~GenericItem()
{
}

void GenericItem::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "GenericItemMetaTable");
	lua_setmetatable(l, -2);
}

void GenericItem::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "GenericItemMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushstring(l, "GenericItem");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "GenericItemMetaTable");

}


