#include "SentryAI.h"

#include "LuaHelper.h"

SentryAI::SentryAI(EntityID id)
	: Entity(id)
{
}

SentryAI::SentryAI(void * ptr)
	: Entity(ptr)
{
}

SentryAI::~SentryAI()
{
}

void SentryAI::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "SentryAIMetaTable");
	lua_setmetatable(l, -2);
}

void SentryAI::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "SentryAIMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<SentryAI, &SentryAI::GetCurrentTarget>()));
	lua_setfield(l, -2, "GetCurrentTarget");
	lua_pushcfunction(l, (LuaHelper::THUNK<SentryAI, &SentryAI::GetUserTarget>()));
	lua_setfield(l, -2, "GetUserTarget");
	lua_pushcfunction(l, (LuaHelper::THUNK<SentryAI, &SentryAI::GetRepathTime>()));
	lua_setfield(l, -2, "GetRepathTime");
	lua_pushcfunction(l, (LuaHelper::THUNK<SentryAI, &SentryAI::GetAimLocation>()));
	lua_setfield(l, -2, "GetAimLocation");
	lua_pushcfunction(l, (LuaHelper::THUNK<SentryAI, &SentryAI::GetAccuracy>()));
	lua_setfield(l, -2, "GetAccuracy");
	lua_pushcfunction(l, (LuaHelper::THUNK<SentryAI, &SentryAI::GetMode>()));
	lua_setfield(l, -2, "GetMode");

	lua_pushstring(l, "SentryAI");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "SentryAIMetaTable");

}

SentryAI::SentryAIStructType * SentryAI::GetStruct()
{
	return (SentryAIStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_SENTRYAI_STRUCT);
}

float SentryAI::GetRepathTime()
{
	return GetStruct()->repathtime;
}

int32_t SentryAI::GetMode()
{
	return GetStruct()->mode;
}

int32_t SentryAI::GetAccuracy()
{
	return GetStruct()->accuracy;
}

int32_t SentryAI::GetAimLocation()
{
	return GetStruct()->location;
}

std::shared_ptr<Entity> SentryAI::GetCurrentTarget()
{
	return Entity::GetEntityByID(GetStruct()->currenttarget);
}

std::shared_ptr<Entity> SentryAI::GetUserTarget()
{
	return Entity::GetEntityByID(GetStruct()->usertarget);
}
