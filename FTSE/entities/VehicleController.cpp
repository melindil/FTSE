#include "VehicleController.h"

#include "LuaHelper.h"

VehicleController::VehicleController(EntityID id) : Collectable(id)
{
}

VehicleController::VehicleController(void * ptr) : Collectable(ptr)
{
}

VehicleController::~VehicleController()
{
}

void VehicleController::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "VehicleControllerMetaTable");
	lua_setmetatable(l, -2);
}

void VehicleController::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "VehicleControllerMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushstring(l, "VehicleController");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "VehicleControllerMetaTable");

}


