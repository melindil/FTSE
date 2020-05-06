#include "Light.h"

#include "LuaHelper.h"

Light::Light(EntityID id)
	: Entity(id)
{
}

Light::Light(void * ptr)
	: Entity(ptr)
{
}

Light::~Light()
{
}

void Light::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "LightMetaTable");
	lua_setmetatable(l, -2);
}

int l_light_color1(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Light>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		Vector3 col = entity->GetColor1();
		lua_newtable(l);
		lua_pushnumber(l, col.v[0]);
		lua_setfield(l, -2, "r");
		lua_pushnumber(l, col.v[1]);
		lua_setfield(l, -2, "g");
		lua_pushnumber(l, col.v[2]);
		lua_setfield(l, -2, "b");
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_light_color2(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Light>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		Vector3 col = entity->GetColor2();
		lua_newtable(l);
		lua_pushnumber(l, col.v[0]);
		lua_setfield(l, -2, "r");
		lua_pushnumber(l, col.v[1]);
		lua_setfield(l, -2, "g");
		lua_pushnumber(l, col.v[2]);
		lua_setfield(l, -2, "b");
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

void Light::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "LightMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, l_light_color1);
	lua_setfield(l, -2, "GetColor1");
	lua_pushcfunction(l, l_light_color2);
	lua_setfield(l, -2, "GetColor2");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::GetRateChange>()));
	lua_setfield(l, -2, "GetRateChange");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::GetArc>()));
	lua_setfield(l, -2, "GetArc");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::GetIntensityStringPos>()));
	lua_setfield(l, -2, "GetIntensityStringPos");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::GetIntensityString>()));
	lua_setfield(l, -2, "GetIntensityString");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::GetRadius>()));
	lua_setfield(l, -2, "GetRadius");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::GetHighBound>()));
	lua_setfield(l, -2, "GetHighBound");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::GetLowBound>()));
	lua_setfield(l, -2, "GetLowBound");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::GetFogAlpha>()));
	lua_setfield(l, -2, "GetFogAlpha");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::OnOffSwitch>()));
	lua_setfield(l, -2, "OnOffSwitch");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::IsNightOnly>()));
	lua_setfield(l, -2, "IsNightOnly");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::IsRepeating>()));
	lua_setfield(l, -2, "IsRepeating");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::IsUseColor2>()));
	lua_setfield(l, -2, "IsUseColor2");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::IsUseArc>()));
	lua_setfield(l, -2, "IsUseArc");
	lua_pushcfunction(l, (LuaHelper::THUNK<Light, &Light::IsUseFog>()));
	lua_setfield(l, -2, "IsUseFog");

	lua_pushstring(l, "Light");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "LightMetaTable");

}

Light::LightStructType * Light::GetStruct()
{
	return (LightStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_LIGHT_STRUCT);
}


bool Light::OnOffSwitch()
{
	return GetStruct()->onoffswitch;
}

bool Light::IsNightOnly()
{
	return GetStruct()->nightonly;
}

float Light::GetRadius()
{
	return GetStruct()->radius;
}

int32_t Light::GetRateChange()
{
	return GetStruct()->ratechange;
}

Vector3 Light::GetColor1()
{
	return Vector3(GetStruct()->color1);
}

std::string Light::GetIntensityString()
{
	return Helpers::WcharToUTF8(GetStruct()->intensitystring);
}

bool Light::IsRepeating()
{
	return GetStruct()->repeating;
}

bool Light::IsUseColor2()
{
	return GetStruct()->usecolor2;
}

Vector3 Light::GetColor2()
{
	return Vector3(GetStruct()->color2);
}

bool Light::IsUseArc()
{
	return GetStruct()->usearc;
}

int32_t Light::GetArc()
{
	return GetStruct()->arc;
}

float Light::GetHighBound()
{
	return GetStruct()->highbound;
}

float Light::GetLowBound()
{
	return GetStruct()->lowbound;
}

bool Light::IsUseFog()
{
	return GetStruct()->usefog;
}

float Light::GetFogAlpha()
{
	return GetStruct()->fogalpha;
}

int32_t Light::GetIntensityStringPos()
{
	return GetStruct()->intensitypos;
}
