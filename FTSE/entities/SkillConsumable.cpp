#include "SkillConsumable.h"

#include "LuaHelper.h"
#include "Helpers.h"

SkillConsumable::SkillConsumable(EntityID id)
	: Collectable(id)
{
}

SkillConsumable::SkillConsumable(void * ptr)
	: Collectable(ptr)
{
}

SkillConsumable::~SkillConsumable()
{
}

void SkillConsumable::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "SkillConsumableMetaTable");
	lua_setmetatable(l, -2);
}

SkillConsumable::SkillConsumableStructType * SkillConsumable::GetStruct()
{
	return (SkillConsumableStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_SKILLCONSUMABLE_STRUCT);
}

void SkillConsumable::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "SkillConsumableMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<SkillConsumable, &SkillConsumable::GetTotalUses>()));
	lua_setfield(l, -2, "GetTotalUses");
	lua_pushcfunction(l, (LuaHelper::THUNK<SkillConsumable, &SkillConsumable::GetRemainingUses>()));
	lua_setfield(l, -2, "GetRemainingUses");

	lua_pushstring(l, "SkillConsumable");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "SkillConsumableMetaTable");
}

int32_t SkillConsumable::GetTotalUses()
{
	return GetStruct()->totaluses;
}
int32_t SkillConsumable::GetRemainingUses()
{
	return GetStruct()->remaining;
}

