#include "Consumable.h"
#include "AttributesTable.h"
#include "ActorStatus.h"

#include "LuaHelper.h"
#include "Helpers.h"

Consumable::Consumable(EntityID id)
	: Collectable(id)
{
}

Consumable::Consumable(void * ptr)
	: Collectable(ptr)
{
}

Consumable::~Consumable()
{
}

void Consumable::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ConsumableMetaTable");
	lua_setmetatable(l, -2);
}

Consumable::ConsumableStructType * Consumable::GetStruct()
{
	return (ConsumableStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_CONSUMABLE_STRUCT);
}

int l_consumable_effectattribute(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Consumable>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		int effectnum = (int)lua_tointeger(l, 2);
		std::string effectname = lua_tostring(l, 3);
		if (effectname == "ap")
		{
			lua_pushnumber(l, entity->GetEffectAttributeFloat(effectnum, effectname));
		}
		else
		{
			lua_pushinteger(l, entity->GetEffectAttribute(effectnum, effectname));
		}
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_consumable_effectenabled(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Consumable>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		int effectnum = (int)lua_tointeger(l, 2);
		lua_pushboolean(l, entity->IsEffectEnabled(effectnum));
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_consumable_effectduration(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Consumable>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		int effectnum = (int)lua_tointeger(l, 2);
		lua_pushinteger(l, entity->GetEffectDuration(effectnum));
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_consumable_effectmultiplier(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Consumable>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		int effectnum = (int)lua_tointeger(l, 2);
		lua_pushnumber(l, entity->GetEffectMultiplier(effectnum));
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

void Consumable::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "ConsumableMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Consumable, &Consumable::GetEffectedAttribute>()));
	lua_setfield(l, -2, "GetEffectedMainAttribute");
	lua_pushcfunction(l, (LuaHelper::THUNK<Consumable, &Consumable::GetCategory>()));
	lua_setfield(l, -2, "GetCategory");
	lua_pushcfunction(l, (LuaHelper::THUNK<Consumable, &Consumable::IsAddicted>()));
	lua_setfield(l, -2, "IsAddicted");
	lua_pushcfunction(l, (LuaHelper::THUNK<Consumable, &Consumable::GetEffectMinimum>()));
	lua_setfield(l, -2, "GetEffectMinimum");
	lua_pushcfunction(l, (LuaHelper::THUNK<Consumable, &Consumable::GetEFfectMaximum>()));
	lua_setfield(l, -2, "GetEFfectMaximum");
	lua_pushcfunction(l, (LuaHelper::THUNK<Consumable, &Consumable::GetOverdosePoints>()));
	lua_setfield(l, -2, "GetOverdosePoints");
	lua_pushcfunction(l, (LuaHelper::THUNK<Consumable, &Consumable::GetChanceAddiction>()));
	lua_setfield(l, -2, "GetChanceAddiction");
	lua_pushcfunction(l, (LuaHelper::THUNK<Consumable, &Consumable::GetState>()));
	lua_setfield(l, -2, "GetState");


	lua_pushcfunction(l, l_consumable_effectduration);
	lua_setfield(l, -2, "GetEffectDuration");
	lua_pushcfunction(l, l_consumable_effectmultiplier);
	lua_setfield(l, -2, "GetEffectMultiplier");
	lua_pushcfunction(l, l_consumable_effectattribute);
	lua_setfield(l, -2, "GetEffectAttribute");
	lua_pushcfunction(l, l_consumable_effectenabled);
	lua_setfield(l, -2, "IsEffectEnabled");

	lua_pushstring(l, "Consumable");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ConsumableMetaTable");
}

int32_t Consumable::GetEffectAttribute(int effectnum, std::string const & effectname)
{
	uint32_t offset = AttributesTable::GetOffsetByName(effectname);
	if (offset != 0)
	{
		if (std::string(effectname, 0, 4) == "tag_" || AttributesTable::GetGroupByName(effectname) == "otraits")
		{
			char result = GetStruct()->effects[effectnum].attributes_table[offset];
			return (int32_t)result;
		}
		int32_t result;
		memcpy(&result, GetStruct()->effects[effectnum].attributes_table + offset, sizeof(int32_t));
		return result;
	}

	offset = ActorStatusOffsets[effectname];
	if (offset != 0)
	{
		if (effectname == "goneuplevel")
		{
			char result = GetStruct()->effects[effectnum].actorstatus_table[offset];
			return (int32_t)result;
		}
		int32_t result;
		memcpy(&result, GetStruct()->effects[effectnum].actorstatus_table + offset, sizeof(int32_t));
		return result;
	}

	return 0;
}

float Consumable::GetEffectAttributeFloat(int effectnum, std::string const & effectname)
{
	if (effectname != "ap")
	{
		return 0.0;
	}

	uint32_t offset = ActorStatusOffsets[effectname];
	if (offset != 0)
	{
		float result;
		memcpy(&result, GetStruct()->effects[effectnum].actorstatus_table + offset, sizeof(float));
		return result;
	}

	return 0.0;
}

bool Consumable::IsEffectEnabled(int effectnum)
{
	return GetStruct()->effects[effectnum].enabled;
}

int32_t Consumable::GetEffectDuration(int effectnum)
{
	if (Helpers::WcharToUTF8(GetStruct()->effects[effectnum].durationtype) == "Hours")
	{
		// Hours are 60 minutes, but tick by 3 times faster
		return GetStruct()->effects[effectnum].duration * 20;
	}
	return GetStruct()->effects[effectnum].duration;
}

float Consumable::GetEffectMultiplier(int effectnum)
{
	return GetStruct()->effects[effectnum].multiplier;
}

std::string Consumable::GetEffectedAttribute()
{
	return Helpers::WcharToUTF8(GetStruct()->effectedattribute);
}

int32_t Consumable::GetEffectMinimum()
{
	return GetStruct()->minimum;
}

int32_t Consumable::GetEFfectMaximum()
{
	return GetStruct()->maximum;
}

std::string Consumable::GetCategory()
{
	return Helpers::WcharToUTF8(GetStruct()->category);
}

int32_t Consumable::GetOverdosePoints()
{
	return GetStruct()->overdosepoints;
}

int32_t Consumable::GetChanceAddiction()
{
	return GetStruct()->chanceaddiction;
}

bool Consumable::IsAddicted()
{
	return GetStruct()->addicted;
}

int32_t Consumable::GetState()
{
	return GetStruct()->state;
}
