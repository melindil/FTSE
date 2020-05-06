#include "CaptureInvItem.h"

#include "LuaHelper.h"

CaptureInvItem::CaptureInvItem(EntityID id) : Collectable(id)
{
}

CaptureInvItem::CaptureInvItem(void * ptr) : Collectable(ptr)
{
}

CaptureInvItem::~CaptureInvItem()
{
}

void CaptureInvItem::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "CaptureInvItemMetaTable");
	lua_setmetatable(l, -2);
}

void CaptureInvItem::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "CaptureInvItemMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushstring(l, "CaptureInvItem");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "CaptureInvItemMetaTable");

}

