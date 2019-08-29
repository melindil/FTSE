#include "VehicleWeapon.h"
#include "LuaHelper.h"

VehicleWeapon::VehicleWeapon(uint16_t id)
	: Weapon(id)
{
}

VehicleWeapon::VehicleWeapon(void* ptr)
	: Weapon(ptr)
{

}

VehicleWeapon::~VehicleWeapon()
{
}

void VehicleWeapon::RegisterLua(lua_State* l, Logger*)
{
	luaL_newmetatable(l, "VehicleWeaponMetaTable");
	Weapon::SetLuaSubclass(l);

	lua_pushstring(l, "VehicleWeapon");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "VehicleWeaponMetaTable");
}

void VehicleWeapon::MakeLuaObject(lua_State* l)
{
	// Represent an entity as a table containing name and ID
	// Can add other elements here for quick access (if needed)
	// Otherwise, use the entity ID as the identifier to re-obtain
	// the pointer from the entity table if we have to make any
	// changes to it.

	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "VehicleWeaponMetaTable");
	lua_setmetatable(l, -2);

}