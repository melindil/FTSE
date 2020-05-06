#include "Trap.h"

#include "LuaHelper.h"

Trap::Trap(EntityID id)
	: Collectable(id)
{
}

Trap::Trap(void * ptr)
	: Collectable(ptr)
{
}

Trap::~Trap()
{
}

int32_t Trap::GetSkillRoll()
{
	return GetStruct()->skillroll;
}

std::shared_ptr<Entity> Trap::GetOwner()
{
	return Entity::GetEntityByID(GetStruct()->owner);
}

std::shared_ptr<Entity> Trap::GetHolder()
{
	return Entity::GetEntityByID(GetStruct()->holder);
}

bool Trap::IsActivated()
{
	return GetStruct()->activated;
}

bool Trap::IsHidden()
{
	return GetStruct()->hidden;
}

uint64_t Trap::GetDetonationTime()
{
	return GetStruct()->detonationtime;
}

float Trap::GetFrequency()
{
	return GetStruct()->frequency;
}

bool Trap::IsReusable()
{
	return GetStruct()->reusable;
}

int32_t Trap::GetDifficulty()
{
	return GetStruct()->difficulty;
}

int32_t Trap::GetCriticalChance()
{
	return GetStruct()->critchance;
}

int32_t Trap::GetMinBaseDamage()
{
	return GetStruct()->mindmg;
}

int32_t Trap::GetMaxBaseDamage()
{
	return GetStruct()->maxdmg;
}

std::string Trap::GetPrimaryDamageType()
{
	return Helpers::WcharToUTF8(GetStruct()->primarydamagetype);
}

std::string Trap::GetSecondaryDamageType()
{
	return Helpers::WcharToUTF8(GetStruct()->secondarydamagetype);
}

std::string Trap::GetAmmoVariant()
{
	return Helpers::WcharToUTF8(GetStruct()->ammovariant);
}

float Trap::GetDamageRadius()
{
	return GetStruct()->damageradius;
}

std::string Trap::GetImpactFX()
{
	return Helpers::WcharToUTF8(GetStruct()->impactfx);
}

Trap::TrapStructType * Trap::GetStruct()
{
	return (TrapStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_TRAP_STRUCT);;
}

void Trap::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "TrapMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetOwner>()));
	lua_setfield(l, -2, "GetOwner");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetHolder>()));
	lua_setfield(l, -2, "GetHolder");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetSkillRoll>()));
	lua_setfield(l, -2, "GetSkillRoll");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::IsActivated>()));
	lua_setfield(l, -2, "IsActivated");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::IsHidden>()));
	lua_setfield(l, -2, "IsHidden");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetDetonationTime>()));
	lua_setfield(l, -2, "GetDetonationTime");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetFrequency>()));
	lua_setfield(l, -2, "GetFrequency");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::IsReusable>()));
	lua_setfield(l, -2, "IsReusable");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetDifficulty>()));
	lua_setfield(l, -2, "GetDifficulty");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetCriticalChance>()));
	lua_setfield(l, -2, "GetCriticalChance");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetMinBaseDamage>()));
	lua_setfield(l, -2, "GetMinBaseDamage");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetMaxBaseDamage>()));
	lua_setfield(l, -2, "GetMaxBaseDamage");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetPrimaryDamageType>()));
	lua_setfield(l, -2, "GetPrimaryDamageType");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetSecondaryDamageType>()));
	lua_setfield(l, -2, "GetSecondaryDamageType");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetAmmoVariant>()));
	lua_setfield(l, -2, "GetAmmoVariant");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetDamageRadius>()));
	lua_setfield(l, -2, "GetDamageRadius");
	lua_pushcfunction(l, (LuaHelper::THUNK<Trap, &Trap::GetImpactFX>()));
	lua_setfield(l, -2, "GetImpactFX");

	lua_pushstring(l, "Trap");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "TrapMetaTable");
}

void Trap::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "TrapMetaTable");
	lua_setmetatable(l, -2);
}