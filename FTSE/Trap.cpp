#include "Trap.h"

#include "LuaHelper.h"

Trap::Trap(uint16_t id)
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

int16_t Trap::GetOwnerID()
{
	return GetStruct()->ownerid;
}

int16_t Trap::GetHolderID()
{
	return GetStruct()->holderid;
}

bool Trap::IsActivated()
{
	return GetStruct()->activated;
}

bool Trap::IsHidden()
{
	return GetStruct()->hidden;
}

uint32_t Trap::GetDetonationTime1()
{
	return GetStruct()->detonationtime;
}

uint32_t Trap::GetDetonationTime2()
{
	return GetStruct()->detonationtime2;
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

int l_trap_getowner(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	Entity::GetEntityByID(e.GetOwnerID())->MakeLuaObject(l);
	return 1;
}
int l_trap_getholder(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	Entity::GetEntityByID(e.GetHolderID())->MakeLuaObject(l);
	return 1;
}

int l_trap_getskillroll(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetSkillRoll());
	return 1;
}

int l_trap_isactivated(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsActivated());
	return 1;
}

int l_trap_ishidden(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsHidden());
	return 1;
}

int l_trap_getdetonationtime1(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetDetonationTime1());
	return 1;
}
int l_trap_getdetonationtime2(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetDetonationTime2());
	return 1;
}

int l_trap_getfrequency(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetFrequency());
	return 1;
}

int l_trap_isreusable(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsReusable());
	return 1;
}

int l_trap_getdifficulty(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetDifficulty());
	return 1;
}

int l_trap_getcriticalchance(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetCriticalChance());
	return 1;
}

int l_trap_getminbasedamage(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMinBaseDamage());
	return 1;
}

int l_trap_getmaxbasedamage(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMaxBaseDamage());
	return 1;
}

int l_trap_getprimarydamagetype(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetPrimaryDamageType().c_str());
	return 1;
}

int l_trap_getsecondarydamagetype(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetSecondaryDamageType().c_str());
	return 1;
}

int l_trap_getammovariant(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetAmmoVariant().c_str());
	return 1;
}
int l_trap_getdamageradius(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetDamageRadius());
	return 1;
}

int l_trap_getimpactfx(lua_State* l)
{
	Trap e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetImpactFX().c_str());
	return 1;
}

void Trap::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "TrapMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, l_trap_getowner);
	lua_setfield(l, -2, "GetOwner");
	lua_pushcfunction(l, l_trap_getholder);
	lua_setfield(l, -2, "GetHolder");
	lua_pushcfunction(l, l_trap_getskillroll);
	lua_setfield(l, -2, "GetSkillRoll");
	lua_pushcfunction(l, l_trap_isactivated);
	lua_setfield(l, -2, "IsActivated");
	lua_pushcfunction(l, l_trap_ishidden);
	lua_setfield(l, -2, "IsHidden");
	lua_pushcfunction(l, l_trap_getdetonationtime1);
	lua_setfield(l, -2, "GetDetonationTime1");
	lua_pushcfunction(l, l_trap_getdetonationtime2);
	lua_setfield(l, -2, "GetDetonationTime2");
	lua_pushcfunction(l, l_trap_getfrequency);
	lua_setfield(l, -2, "GetFrequency");
	lua_pushcfunction(l, l_trap_isreusable);
	lua_setfield(l, -2, "IsReusable");
	lua_pushcfunction(l, l_trap_getdifficulty);
	lua_setfield(l, -2, "GetDifficulty");
	lua_pushcfunction(l, l_trap_getcriticalchance);
	lua_setfield(l, -2, "GetCriticalChance");
	lua_pushcfunction(l, l_trap_getminbasedamage);
	lua_setfield(l, -2, "GetMinBaseDamage");
	lua_pushcfunction(l, l_trap_getmaxbasedamage);
	lua_setfield(l, -2, "GetMaxBaseDamage");
	lua_pushcfunction(l, l_trap_getprimarydamagetype);
	lua_setfield(l, -2, "GetPrimaryDamageType");
	lua_pushcfunction(l, l_trap_getsecondarydamagetype);
	lua_setfield(l, -2, "GetSecondaryDamageType");
	lua_pushcfunction(l, l_trap_getammovariant);
	lua_setfield(l, -2, "GetAmmoVariant");
	lua_pushcfunction(l, l_trap_getdamageradius);
	lua_setfield(l, -2, "GetDamageRadius");
	lua_pushcfunction(l, l_trap_getimpactfx);
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