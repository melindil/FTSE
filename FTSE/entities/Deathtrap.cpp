#include "Deathtrap.h"

#include "LuaHelper.h"

Deathtrap::Deathtrap(EntityID id)
	: Entity(id)
{
}

Deathtrap::Deathtrap(void * ptr)
	: Entity(ptr)
{
}

Deathtrap::~Deathtrap()
{
}

void Deathtrap::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "DeathtrapMetaTable");
	lua_setmetatable(l, -2);
}

void Deathtrap::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "DeathtrapMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Deathtrap, &Deathtrap::IsRepeat>()));
	lua_setfield(l, -2, "IsRepeat");
	lua_pushcfunction(l, (LuaHelper::THUNK<Deathtrap, &Deathtrap::IsAnimated>()));
	lua_setfield(l, -2, "IsAnimated");
	lua_pushcfunction(l, (LuaHelper::THUNK<Deathtrap, &Deathtrap::IsScriptState>()));
	lua_setfield(l, -2, "IsScriptState");
	lua_pushcfunction(l, (LuaHelper::THUNK<Deathtrap, &Deathtrap::GetDamageType>()));
	lua_setfield(l, -2, "GetDamageType");
	lua_pushcfunction(l, (LuaHelper::THUNK<Deathtrap, &Deathtrap::GetDamage>()));
	lua_setfield(l, -2, "GetDamage");
	lua_pushcfunction(l, (LuaHelper::THUNK<Deathtrap, &Deathtrap::GetRepeatInterval>()));
	lua_setfield(l, -2, "GetRepeatInterval");
	lua_pushcfunction(l, (LuaHelper::THUNK<Deathtrap, &Deathtrap::GetNextActivateTime>()));
	lua_setfield(l, -2, "GetNextActivateTime");

	lua_pushstring(l, "Deathtrap");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "DeathtrapMetaTable");

}

bool Deathtrap::IsRepeat()
{
	return GetStruct()->repeat;
}

float Deathtrap::GetRepeatInterval()
{
	return GetStruct()->repeatinterval;
}

std::string Deathtrap::GetDamageType()
{
	return Helpers::WcharToUTF8(GetStruct()->damagetype);
}

int32_t Deathtrap::GetDamage()
{
	return GetStruct()->damage;
}

bool Deathtrap::IsAnimated()
{
	return GetStruct()->animated;
}

bool Deathtrap::IsScriptState()
{
	return GetStruct()->scriptstate;
}

float Deathtrap::GetNextActivateTime()
{
	return GetStruct()->nextactivatetime;
}

Deathtrap::DeathtrapStructType * Deathtrap::GetStruct()
{
	return (DeathtrapStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_DEATHTRAP_STRUCT);
}

