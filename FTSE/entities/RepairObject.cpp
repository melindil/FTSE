#include "RepairObject.h"

#include "LuaHelper.h"
#include "Helpers.h"

RepairObject::RepairObject(EntityID id)
	: Collectable(id)
{
}

RepairObject::RepairObject(void * ptr)
	: Collectable(ptr)
{
}

RepairObject::~RepairObject()
{
}

void RepairObject::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "RepairObjectMetaTable");
	lua_setmetatable(l, -2);
}

int32_t RepairObject::GetDifficulty()
{
	return GetStruct()->difficulty;
}

RepairObject::RepairObjectStructType * RepairObject::GetStruct()
{
	return (RepairObjectStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_REPAIROBJECT_STRUCT);
}

void RepairObject::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "RepairObjectMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<RepairObject, &RepairObject::GetDifficulty>()));
	lua_setfield(l, -2, "GetDifficulty");

	lua_pushstring(l, "RepairObject");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "RepairObjectMetaTable");
}
