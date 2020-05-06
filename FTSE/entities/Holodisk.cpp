#include "Holodisk.h"

#include "LuaHelper.h"
#include "Helpers.h"

Holodisk::Holodisk(EntityID id)
	: Collectable(id)
{
}

Holodisk::Holodisk(void * ptr)
	: Collectable(ptr)
{
}

Holodisk::~Holodisk()
{
}

void Holodisk::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "HolodiskMetaTable");
	lua_setmetatable(l, -2);
}

Holodisk::HolodiskStructType * Holodisk::GetStruct()
{
	return (HolodiskStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_HOLODISK_STRUCT);
}

void Holodisk::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "HolodiskMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Holodisk, &Holodisk::GetText>()));
	lua_setfield(l, -2, "GetText");

	lua_pushstring(l, "Holodisk");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "HolodiskMetaTable");
}

std::string Holodisk::GetText()
{
	return Helpers::WcharToUTF8(GetStruct()->text);
}

