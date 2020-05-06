#include "BaseAI.h"

#include "LuaHelper.h"

BaseAI::BaseAI(EntityID id)
	: Entity(id)
{
}

BaseAI::BaseAI(void * ptr)
	: Entity(ptr)
{
}

BaseAI::~BaseAI()
{
}

void BaseAI::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "BaseAIMetaTable");
	lua_setmetatable(l, -2);
}

void BaseAI::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "BaseAIMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::GetCurrentTarget>()));
	lua_setfield(l, -2, "GetCurrentTarget");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::GetController>()));
	lua_setfield(l, -2, "GetController");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::GetAction>()));
	lua_setfield(l, -2, "GetAction");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::GetNature>()));
	lua_setfield(l, -2, "GetNature");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::GetTactic>()));
	lua_setfield(l, -2, "GetTactic");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::GetItemTag>()));
	lua_setfield(l, -2, "GetItemTag");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::GetTaunts>()));
	lua_setfield(l, -2, "GetTaunts");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::UsesCover>()));
	lua_setfield(l, -2, "UsesCover");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::UsesSneak>()));
	lua_setfield(l, -2, "UsesSneak");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::IsSleeping>()));
	lua_setfield(l, -2, "IsSleeping");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::IsSuppressed>()));
	lua_setfield(l, -2, "IsSuppressed");
	lua_pushcfunction(l, (LuaHelper::THUNK<BaseAI, &BaseAI::IsAttemptSuppressed>()));
	lua_setfield(l, -2, "IsAttemptSuppressed");

	lua_pushstring(l, "BaseAI");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "BaseAIMetaTable");

}

BaseAI::BaseAIStructType * BaseAI::GetStruct()
{
	return (BaseAIStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_BASEAI_STRUCT);
}

std::string BaseAI::GetAction()
{
	return Helpers::WcharToUTF8(GetStruct()->currentaction);
}

std::string BaseAI::GetNature()
{
	return Helpers::WcharToUTF8(GetStruct()->nature);
}

std::string BaseAI::GetTactic()
{
	return Helpers::WcharToUTF8(GetStruct()->tactic);
}

bool BaseAI::UsesCover()
{
	return GetStruct()->usecover;
}

bool BaseAI::UsesSneak()
{
	return GetStruct()->usesneak;
}

bool BaseAI::IsSleeping()
{
	return GetStruct()->sleeping;
}

std::string BaseAI::GetItemTag()
{
	return Helpers::WcharToUTF8(GetStruct()->itemtag);
}

std::string BaseAI::GetTaunts()
{
	return Helpers::WcharToUTF8(GetStruct()->taunts);
}

std::shared_ptr<Entity> BaseAI::GetCurrentTarget()
{
	return Entity::GetEntityByID(GetStruct()->currenttarget);
}

std::shared_ptr<Entity> BaseAI::GetController()
{
	return Entity::GetEntityByID(GetStruct()->controller);
}

bool BaseAI::IsSuppressed()
{
	return GetStruct()->suppressed;
}

bool BaseAI::IsAttemptSuppressed()
{
	return GetStruct()->attemptsuppressed;
}
