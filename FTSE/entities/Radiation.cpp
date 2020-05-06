#include "Radiation.h"
#include "LuaHelper.h"

Radiation::Radiation(EntityID id)
	: Entity(id)
{
}

Radiation::Radiation(void * ptr)
	: Entity(ptr)
{
}

Radiation::~Radiation()
{
}

void Radiation::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "RadiationMetaTable");
	lua_setmetatable(l, -2);
}

void Radiation::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "RadiationMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Radiation, &Radiation::GetRadius>()));
	lua_setfield(l, -2, "GetRadius");
	lua_pushcfunction(l, (LuaHelper::THUNK<Radiation, &Radiation::GetRadiationLevel>()));
	lua_setfield(l, -2, "GetRadiationLevel");

	lua_pushstring(l, "Radiation");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "RadiationMetaTable");

}

float Radiation::GetRadius()
{
	return GetStruct()->radius;
}
float Radiation::GetRadiationLevel()
{
	return GetStruct()->radiationlevel;
}

Radiation::RadiationStructType * Radiation::GetStruct()
{
	return (RadiationStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_RADIATION_STRUCT);
}
