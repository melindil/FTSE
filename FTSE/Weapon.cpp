#include "Weapon.h"
#include "Actor.h"

#include "LuaHelper.h"

Weapon::Weapon(uint16_t id)
	: Collectable(id)
{
}

Weapon::Weapon(void* ptr)
	: Collectable(ptr)
{

}


Weapon::~Weapon()
{
}

int l_weapon_getrange(lua_State* lua);
void Weapon::RegisterLua(lua_State* l, Logger* )
{
	luaL_newmetatable(l, "WeaponMetaTable");
	lua_pushcfunction(l, l_weapon_getrange);
	lua_setfield(l, -2, "GetRange");


	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "WeaponMetaTable");
}

void Weapon::MakeLuaObject(lua_State* l)
{
	// Represent an entity as a table containing name and ID
	// Can add other elements here for quick access (if needed)
	// Otherwise, use the entity ID as the identifier to re-obtain
	// the pointer from the entity table if we have to make any
	// changes to it.

	std::string WeaponName = GetEntityName();
	lua_newtable(l);
	lua_pushstring(l, WeaponName.c_str());
	lua_setfield(l, -2, "name");
	lua_pushinteger(l, entity_id_);
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "WeaponMetaTable");
	lua_setmetatable(l, -2);

}

float Weapon::GetRange(Actor& attacker)
{
	auto fxn = (float(__thiscall*)(void*,uint32_t))0x5dbb00;
	return fxn(GetEntityPointer(), (uint32_t)attacker.GetID() << 16 | attacker.GetFlags());
}

int l_weapon_getrange(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	uint16_t attackerid = LuaHelper::GetTableInteger(l, 2, "id");
	
	Weapon e(id);
	Actor a(attackerid);
	float result = e.GetRange(a);
	lua_pushnumber(l, result);
	return 1;
}