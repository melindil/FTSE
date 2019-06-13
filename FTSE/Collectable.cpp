#include "Collectable.h"

#include "AttributesTable.h"

Collectable::Collectable(void * ptr)
	: Entity(ptr)
{
}

Collectable::Collectable(uint16_t id)
	: Entity(id)
{
}

Collectable::~Collectable()
{
}

float Collectable::GetWeightPerUnit()
{
	return GetCollectableStruct()->weight;
}

float Collectable::GetWeightTotal()
{
	return GetCollectableStruct()->weight *
		(GetCollectableStruct()->count + GetCollectableStruct()->countnonlootable);
}

int Collectable::GetCount()
{
	return GetCollectableStruct()->count;
}

int Collectable::GetCountNonLootable()
{
	return GetCollectableStruct()->countnonlootable;
}

int Collectable::GetValuePerUnit()
{
	return GetCollectableStruct()->valuebase;
}

int Collectable::GetValueTotal()
{
	return GetCollectableStruct()->valuebase *
		(GetCollectableStruct()->count + GetCollectableStruct()->countnonlootable);
}

bool Collectable::IsUnique()
{
	return GetCollectableStruct()->unique;
}

bool Collectable::CanBeRemoved()
{
	return GetCollectableStruct()->canberemoved;
}

bool Collectable::BeenApplied()
{
	return GetCollectableStruct()->beenapplied;
}

bool Collectable::Lootable()
{
	return GetCollectableStruct()->lootable;
}

int Collectable::GetRequiredAttribute(std::string const & name)
{
	int offset = AttributesTable::GetOffsetByName(name);
	if (std::string(name, 0, 4) == "tag_" || AttributesTable::GetGroupByName(name) == "otraits")
	{
		char result = GetCollectableStruct()->required_attributes[offset];
		return (int)result;
	}
	int result;
	memcpy(&result, GetCollectableStruct()->required_attributes + offset, sizeof(int32_t));
	return result;
}

int Collectable::GetEffectAttribute(std::string const & name)
{
	int offset = AttributesTable::GetOffsetByName(name);
	if (std::string(name, 0, 4) == "tag_" || AttributesTable::GetGroupByName(name) == "otraits")
	{
		char result = GetCollectableStruct()->effect_attributes[offset];
		return (int)result;
	}
	int result;
	memcpy(&result, GetCollectableStruct()->effect_attributes + offset, sizeof(int32_t));
	return result;
}

Collectable::CollectableStructType* Collectable::GetCollectableStruct()
{
	char* ptr = ((char*)GetEntityPointer()) + OFFSET_STRUCT;
	return reinterpret_cast<CollectableStructType*>(ptr);
}

int l_collectable_getrequired(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
        std::string attrib = lua_tostring(l, 2);
	Collectable e(id);
	lua_pushinteger(l, e.GetRequiredAttribute(attrib));
	return 1;
}

int l_collectable_geteffect(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
        std::string attrib = lua_tostring(l, 2);
	Collectable e(id);
	lua_pushinteger(l, e.GetEffectAttribute(attrib));
	return 1;
}
int l_collectable_getunitweight(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushnumber(l, e.GetWeightPerUnit());
	return 1;
}
int l_collectable_gettotalweight(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushnumber(l, e.GetWeightTotal());
	return 1;
}
int l_collectable_getcount(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushinteger(l, e.GetCount());
	return 1;
}
int l_collectable_getcountnonloot(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushinteger(l, e.GetCountNonLootable());
	return 1;
}

int l_collectable_getunitvalue(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushinteger(l, e.GetValuePerUnit());
	return 1;
}

int l_collectable_gettotalvalue(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushinteger(l, e.GetValueTotal());
	return 1;
}

int l_collectable_isunique(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushboolean(l, e.IsUnique());
	return 1;
}

int l_collectable_canberemoved(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushboolean(l, e.CanBeRemoved());
	return 1;
}

int l_collectable_lootable(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushboolean(l, e.Lootable());
	return 1;
}

int l_collectable_beenapplied(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Collectable e(id);
	lua_pushboolean(l, e.BeenApplied());
	return 1;
}

/*
Table of functions

lua_pushcfunction(l, l_collectable_getrequired);
lua_setfield(l, -2, "GetRequiredAttribute");
lua_pushcfunction(l, l_collectable_geteffect);
lua_setfield(l, -2, "GetEffectAttribute");
lua_pushcfunction(l, l_collectable_getrequired);
lua_setfield(l, -2, "GetRequiredAttribute");
lua_pushcfunction(l, l_collectable_getunitweight);
lua_setfield(l, -2, "GetWeightPerUnit");
lua_pushcfunction(l, l_collectable_gettotalweight);
lua_setfield(l, -2, "GetWeightTotal");
lua_pushcfunction(l, l_collectable_getunitvalue);
lua_setfield(l, -2, "GetValuePerUnit");
lua_pushcfunction(l, l_collectable_gettotalvalue);
lua_setfield(l, -2, "GetValueTotal");
lua_pushcfunction(l, l_collectable_getcount);
lua_setfield(l, -2, "GetCount");
lua_pushcfunction(l, l_collectable_getcountnonloot);
lua_setfield(l, -2, "GetCountNonLootable");
lua_pushcfunction(l, l_collectable_isunique);
lua_setfield(l, -2, "IsUnique");
lua_pushcfunction(l, l_collectable_canberemoved);
lua_setfield(l, -2, "CanBeRemoved");
lua_pushcfunction(l, l_collectable_lootable);
lua_setfield(l, -2, "Lootable");
lua_pushcfunction(l, l_collectable_beenapplied);
lua_setfield(l, -2, "BeenApplied");

*/
