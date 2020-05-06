#include "CaptureItem.h"

#include "LuaHelper.h"

CaptureItem::CaptureItem(EntityID id)
	: Entity(id)
{
}

CaptureItem::CaptureItem(void * ptr)
	: Entity(ptr)
{
}

CaptureItem::~CaptureItem()
{
}

void CaptureItem::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "CaptureItemMetaTable");
	lua_setmetatable(l, -2);
}

void CaptureItem::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "CaptureItemMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<CaptureItem, &CaptureItem::GetCaptureTeam>()));
	lua_setfield(l, -2, "GetCaptureTeam");
	lua_pushcfunction(l, (LuaHelper::THUNK<CaptureItem, &CaptureItem::GetInventoryItem>()));
	lua_setfield(l, -2, "GetInventoryItem");

	lua_pushstring(l, "CaptureItem");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "CaptureItemMetaTable");

}

int32_t CaptureItem::GetCaptureTeam()
{
	return GetStruct()->captureteam;
}

std::shared_ptr<Entity> CaptureItem::GetInventoryItem()
{
	return Entity::GetEntityByID(GetStruct()->captureitem);
}


CaptureItem::CaptureItemStructType * CaptureItem::GetStruct()
{
	return (CaptureItemStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_CAPTUREITEM_STRUCT);
}
