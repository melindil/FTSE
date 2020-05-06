#include "Switch.h"

#include "LuaHelper.h"
#include "Helpers.h"

Switch::Switch(EntityID id)
	: Usable(id)
{
}

Switch::Switch(void * ptr)
	: Usable(ptr)
{
}

Switch::~Switch()
{
}

void Switch::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "SwitchMetaTable");
	lua_setmetatable(l, -2);
}

Switch::SwitchStructType * Switch::GetStruct()
{
	return (SwitchStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_SWITCH_STRUCT);
}

void Switch::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "SwitchMetaTable");
	SetLuaSubclass(l);
	
	lua_pushstring(l, "Switch");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "SwitchMetaTable");
}

bool Switch::IsOn()
{
	return GetStruct()->on;
}
bool Switch::IsBrainExtractor()
{
	return GetStruct()->brainextractor;
}
std::string Switch::GetKeyTagName()
{
	return Helpers::WcharToUTF8(GetStruct()->keytagname);
}

void Switch::SetLuaSubclass(lua_State * l)
{
	Usable::SetLuaSubclass(l);
	lua_pushboolean(l, true);
	lua_setfield(l, -2, "isSwitch");

	lua_pushcfunction(l, (LuaHelper::THUNK<Switch, &Switch::IsOn>()));
	lua_setfield(l, -2, "IsOn");
	lua_pushcfunction(l, (LuaHelper::THUNK<Switch, &Switch::IsBrainExtractor>()));
	lua_setfield(l, -2, "IsBrainExtractor");
	lua_pushcfunction(l, (LuaHelper::THUNK<Switch, &Switch::GetKeyTagName>()));
	lua_setfield(l, -2, "GetKeyTagName");

}