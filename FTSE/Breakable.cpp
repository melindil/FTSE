#include "Breakable.h"
#include "LuaHelper.h"
#include "AttributesTable.h"

Breakable::Breakable(uint16_t id)
	: Entity(id)
{
}

Breakable::Breakable(void * ptr)
	: Entity(ptr)
{
}

Breakable::~Breakable()
{
}

void Breakable::MakeLuaObject(lua_State * l)
{
	// Represent an entity as a table containing name and ID
// Can add other elements here for quick access (if needed)
// Otherwise, use the entity ID as the identifier to re-obtain
// the pointer from the entity table if we have to make any
// changes to it.

	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "BreakableMetaTable");
	lua_setmetatable(l, -2);

}

void Breakable::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "BreakableMetaTable");
	SetLuaSubclass(l);

	lua_pushstring(l, "Breakable");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "BreakableMetaTable");
}

int l_breakable_getattribute(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	std::string attrib = lua_tostring(l, 2);
	lua_pushinteger(l, e.GetAttribute(attrib));
	return 1;
}

int l_breakable_gethp(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetHP());
	return 1;
}
int l_breakable_mindamage(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMinDamage());
	return 1;
}
int l_breakable_maxdamage(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMaxDamage());
	return 1;
}

int l_breakable_etherealwhendead(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.isEtherealWhenDead());
	return 1;
}
int l_breakable_ismetal(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsMetal());
	return 1;
}
int l_breakable_primarydamagetype(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetPrimaryDamageType().c_str());
	return 1;
}
int l_breakable_secondarydamagetype(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetSecondaryDamageType().c_str());
	return 1;
}
int l_breakable_ammovariant(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetAmmoVariant().c_str());
	return 1;
}

int l_breakable_damageradius(lua_State* l)
{
	Breakable e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetDamageRadius());
	return 1;
}
void Breakable::SetLuaSubclass(lua_State * l)
{
	Entity::SetLuaSubclass(l);
	lua_pushcfunction(l, l_breakable_getattribute);
	lua_setfield(l , -2, "GetAttribute");
	lua_pushcfunction(l, l_breakable_gethp);
	lua_setfield(l, -2, "GetHP");
	lua_pushcfunction(l, l_breakable_etherealwhendead);
	lua_setfield(l, -2, "EtherealWhenDead");
	lua_pushcfunction(l, l_breakable_mindamage);
	lua_setfield(l, -2, "GetMinBaseDamage");
	lua_pushcfunction(l, l_breakable_maxdamage);
	lua_setfield(l, -2, "GetMaxBaseDamage");
	lua_pushcfunction(l, l_breakable_primarydamagetype);
	lua_setfield(l, -2, "GetPrimaryDamageType");
	lua_pushcfunction(l, l_breakable_secondarydamagetype);
	lua_setfield(l, -2, "GetSecondaryDamageType");
	lua_pushcfunction(l, l_breakable_ammovariant);
	lua_setfield(l, -2, "GetAmmoVariant");
	lua_pushcfunction(l, l_breakable_damageradius);
	lua_setfield(l, -2, "GetDamageRadius");
	lua_pushcfunction(l, l_breakable_ismetal);
	lua_setfield(l, -2, "IsMetal");
}

int32_t Breakable::GetAttribute(std::string const & name)
{
	int offset = AttributesTable::GetOffsetByName(name);
	if (std::string(name, 0, 4) == "tag_" || AttributesTable::GetGroupByName(name) == "otraits")
	{
		char result = GetStruct()->attributes[offset];
		return (int)result;
	}
	int result;
	memcpy(&result, GetStruct()->attributes + offset, sizeof(int32_t));
	return result;
}

int32_t Breakable::GetHP()
{
	return GetStruct()->actorstatus.hp;
}

bool Breakable::isEtherealWhenDead()
{
	return GetStruct()->etherealwhendead;
}

int32_t Breakable::GetMinDamage()
{
	return GetStruct()->mindamage;
}

int32_t Breakable::GetMaxDamage()
{
	return GetStruct()->maxdamage;
}

std::string Breakable::GetPrimaryDamageType()
{
	return Helpers::WcharToUTF8(GetStruct()->primarydamagetype);
}

std::string Breakable::GetSecondaryDamageType()
{
	return Helpers::WcharToUTF8(GetStruct()->secondarydamagetype);
}

std::string Breakable::GetAmmoVariant()
{
	return Helpers::WcharToUTF8(GetStruct()->ammovariant);
}

float Breakable::GetDamageRadius()
{
	return GetStruct()->damageradius;
}

bool Breakable::IsMetal()
{
	return GetStruct()->ismetal;
}


Breakable::BreakableStructType * Breakable::GetStruct()
{
	return (BreakableStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_BREAKABLE_STRUCT);
}
