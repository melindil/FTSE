#include "Keys.h"

#include "LuaHelper.h"

Keys::Keys(EntityID id) : GenericItem(id)
{
}

Keys::Keys(void * ptr) : GenericItem(ptr)
{
}

Keys::~Keys()
{
}

void Keys::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "KeysItemMetaTable");
	lua_setmetatable(l, -2);
}

void Keys::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "KeysMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushstring(l, "Keys");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "KeysItemMetaTable");

}


