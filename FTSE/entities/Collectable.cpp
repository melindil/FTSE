#include "Collectable.h"
#include "Helpers.h"
#include "LuaHelper.h"
#include "AttributesTable.h"

Collectable::Collectable(void * ptr)
	: Entity(ptr)
{
}

Collectable::Collectable(EntityID id)
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
		GetCountTotal();
}

int Collectable::GetCount()
{
	return GetCollectableStruct()->count;
}

int Collectable::GetCountNonLootable()
{
	return GetCollectableStruct()->countnonlootable;
}

int Collectable::GetCountTotal()
{
	return GetCount() + GetCountNonLootable();
}

int Collectable::GetValuePerUnit()
{
	return GetCollectableStruct()->valuebase;
}

int Collectable::GetValueTotal()
{
	return GetCollectableStruct()->valuebase * GetCountTotal();
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

void Collectable::OverrideCount(int newcount)
{
	GetCollectableStruct()->count = newcount;
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

int l_collectable_getrequired(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Collectable>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		std::string attrib = lua_tostring(l, 2);
		lua_pushinteger(l, entity->GetRequiredAttribute(attrib));
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

int l_collectable_geteffect(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Collectable>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		std::string attrib = lua_tostring(l, 2);
		lua_pushinteger(l, entity->GetEffectAttribute(attrib));
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

void Collectable::SetLuaSubclass(lua_State * l)
{
	Entity::SetLuaSubclass(l);
	lua_pushboolean(l, true);
	lua_setfield(l, -2, "isCollectable");

	lua_pushcfunction(l, l_collectable_getrequired);
	lua_setfield(l, -2, "GetRequiredAttribute");
	lua_pushcfunction(l, l_collectable_geteffect);
	lua_setfield(l, -2, "GetEffectAttribute");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::GetWeightPerUnit>()));
	lua_setfield(l, -2, "GetWeightPerUnit");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::GetWeightTotal>()));
	lua_setfield(l, -2, "GetWeightTotal");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::GetValuePerUnit>()));
	lua_setfield(l, -2, "GetValuePerUnit");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::GetValueTotal>()));
	lua_setfield(l, -2, "GetValueTotal");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::GetCount>()));
	lua_setfield(l, -2, "GetCount");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::GetCountNonLootable>()));
	lua_setfield(l, -2, "GetCountNonLootable");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::GetCountTotal>()));
	lua_setfield(l, -2, "GetCountTotal");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::IsUnique>()));
	lua_setfield(l, -2, "IsUnique");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::CanBeRemoved>()));
	lua_setfield(l, -2, "CanBeRemoved");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::Lootable>()));
	lua_setfield(l, -2, "Lootable");
	lua_pushcfunction(l, (LuaHelper::THUNK<Collectable, &Collectable::BeenApplied>()));
	lua_setfield(l, -2, "BeenApplied");
}

Collectable::CollectableStructType* Collectable::GetCollectableStruct()
{
	char* ptr = ((char*)GetEntityPointer()) + OFFSET_STRUCT;
	return reinterpret_cast<CollectableStructType*>(ptr);
}
