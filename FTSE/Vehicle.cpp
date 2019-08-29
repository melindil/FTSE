#include "Vehicle.h"

#include "LuaHelper.h"
#include "AttributesTable.h"

Vehicle::Vehicle(uint16_t id)
	: Entity(id)
{
}

Vehicle::Vehicle(void * ptr)
	: Entity(ptr)
{
}

Vehicle::~Vehicle()
{
}

void Vehicle::MakeLuaObject(lua_State * l)
{
	// Represent an entity as a table containing name and ID
// Can add other elements here for quick access (if needed)
// Otherwise, use the entity ID as the identifier to re-obtain
// the pointer from the entity table if we have to make any
// changes to it.

	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "VehicleMetaTable");
	lua_setmetatable(l, -2);

}

int l_vehicle_getattribute(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	std::string attrib = lua_tostring(l, 2);
	lua_pushinteger(l, e.GetAttribute(attrib));
	return 1;
}

int l_vehicle_gethp(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetHP());
	return 1;
}

int l_vehicle_getdriver(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	auto driver = e.GetDriver();
	if (driver)
	{
		driver->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

int l_vehicle_getgunner(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	auto gunner = e.GetGunner();
	if (gunner)
	{
		gunner->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

int l_vehicle_getcontroller(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	auto controller = e.GetController();
	if (controller)
	{
		controller->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

int l_vehicle_getpassenger(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	int passnum = (int)lua_tointeger(l, 2);
	auto passenger = e.GetPassenger(passnum);
	if (passenger)
	{
		passenger->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_vehicle_getrepairobject(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	auto item = e.GetRepairObject();
	if (item)
	{
		item->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_vehicle_getweapon(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	auto weapon = e.GetVehicleWeapon();
	if (weapon)
	{
		weapon->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_vehicle_getdriveritem(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	auto weapon = e.GetDriverLeftHandItem();
	if (weapon)
	{
		weapon->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_vehicle_getgunneritem(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	auto weapon = e.GetGunnerLeftHandItem();
	if (weapon)
	{
		weapon->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}
int l_vehicle_getinventory(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	auto inv = e.GetInventory();
	if (inv)
	{
		inv->MakeLuaObject(l);
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

int l_vehicle_getnumpassengers(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetNumPassengers());
	return 1;
}
int l_vehicle_gettotaloccupants(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetTotalOccupants());
	return 1;
}
int l_vehicle_getmaxpassengers(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMaxPassengers());
	return 1;
}
int l_vehicle_getmaxtotaloccupants(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMaxTotalOccupants());
	return 1;
}
int l_vehicle_getturnrate(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetTurnRate());
	return 1;
}
int l_vehicle_getcriticalchance(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetCriticalChance());
	return 1;
}
int l_vehicle_getdamagemult(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetCriticalChance());
	return 1;
}
int l_vehicle_getacceleration(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetAcceleration());
	return 1;
}
int l_vehicle_getmaxspeed(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetMaxSlowSpeed());
	return 1;
}
int l_vehicle_getmaxrunspeed(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetMaxHighSpeed());
	return 1;
}
int l_vehicle_getendturntime(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetEndTurnTime());
	return 1;
}

int l_vehicle_needsrepair(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.NeedsRepairObject());
	return 1;
}
int l_vehicle_hasweapon(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.HasWeapon());
	return 1;
}
int l_vehicle_isrunspeed(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsRunSpeed());
	return 1;
}
int l_vehicle_isreverse(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsReverse());
	return 1;
}
int l_vehicle_destreached(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsDestinationReached());
	return 1;
}
int l_vehicle_turnreached(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsTurnReached());
	return 1;
}
int l_vehicle_havingturn(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.IsHavingTurn());
	return 1;
}

int l_vehicle_currentaction(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetCurrentAction().c_str());
	return 1;
}
int l_vehicle_buttonsprite(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetButtonSprite().c_str());
	return 1;
}

/* START HERE */
int l_vehicle_getexitpoint(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	Vector3 vect = e.GetExitPoint();
	lua_newtable(l);
	lua_pushnumber(l, vect.v[0]);
	lua_setfield(l, -2, "x");
	lua_pushnumber(l, vect.v[1]);
	lua_setfield(l, -2, "y");
	lua_pushnumber(l, vect.v[2]);
	lua_setfield(l, -2, "z");
	return 1;
}
int l_vehicle_getdestination(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	Vector3 vect = e.GetDestination();
	lua_newtable(l);
	lua_pushnumber(l, vect.v[0]);
	lua_setfield(l, -2, "x");
	lua_pushnumber(l, vect.v[1]);
	lua_setfield(l, -2, "y");
	lua_pushnumber(l, vect.v[2]);
	lua_setfield(l, -2, "z");
	return 1;
}
int l_vehicle_getvelocity(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	Vector3 vect = e.GetVelocity();
	lua_newtable(l);
	lua_pushnumber(l, vect.v[0]);
	lua_setfield(l, -2, "x");
	lua_pushnumber(l, vect.v[1]);
	lua_setfield(l, -2, "y");
	lua_pushnumber(l, vect.v[2]);
	lua_setfield(l, -2, "z");
	return 1;
}
int l_vehicle_getdimensions(lua_State* l)
{
	Vehicle e(LuaHelper::GetEntityId(l));
	Vector3 vect = e.GetDimensions();
	lua_newtable(l);
	lua_pushnumber(l, vect.v[0]);
	lua_setfield(l, -2, "x");
	lua_pushnumber(l, vect.v[1]);
	lua_setfield(l, -2, "y");
	lua_pushnumber(l, vect.v[2]);
	lua_setfield(l, -2, "z");
	return 1;
}

void Vehicle::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "VehicleMetaTable");
	Entity::SetLuaSubclass(l);
	lua_pushstring(l, "Vehicle");
	lua_setfield(l, -2, "ClassType");

	lua_pushcfunction(l, l_vehicle_getattribute);
	lua_setfield(l, -2, "GetAttribute");
	lua_pushcfunction(l, l_vehicle_gethp);
	lua_setfield(l, -2, "GetHP");
	lua_pushcfunction(l, l_vehicle_getdriver);
	lua_setfield(l, -2, "GetDriver");
	lua_pushcfunction(l, l_vehicle_getgunner);
	lua_setfield(l, -2, "GetGunner");
	lua_pushcfunction(l, l_vehicle_getcontroller);
	lua_setfield(l, -2, "GetController");
	lua_pushcfunction(l, l_vehicle_getpassenger);
	lua_setfield(l, -2, "GetPassenger");
	lua_pushcfunction(l, l_vehicle_getrepairobject);
	lua_setfield(l, -2, "GetRepairObject");
	lua_pushcfunction(l, l_vehicle_getweapon);
	lua_setfield(l, -2, "GetVehicleWeapon");
	lua_pushcfunction(l, l_vehicle_getdriveritem);
	lua_setfield(l, -2, "GetDriverLeftHandItem");
	lua_pushcfunction(l, l_vehicle_getgunneritem);
	lua_setfield(l, -2, "GetGunnerLeftHandItem");
	lua_pushcfunction(l, l_vehicle_getinventory);
	lua_setfield(l, -2, "GetInventory");
	lua_pushcfunction(l, l_vehicle_getnumpassengers);
	lua_setfield(l, -2, "GetNumPassengers");
	lua_pushcfunction(l, l_vehicle_gettotaloccupants);
	lua_setfield(l, -2, "GetTotalOccupants");
	lua_pushcfunction(l, l_vehicle_getmaxpassengers);
	lua_setfield(l, -2, "GetMaxPassengers");
	lua_pushcfunction(l, l_vehicle_getmaxtotaloccupants);
	lua_setfield(l, -2, "GetMaxTotalOccupants");
	lua_pushcfunction(l, l_vehicle_getturnrate);
	lua_setfield(l, -2, "GetTurnRate");
	lua_pushcfunction(l, l_vehicle_getcriticalchance);
	lua_setfield(l, -2, "GetCriticalChance");
	lua_pushcfunction(l, l_vehicle_getdamagemult);
	lua_setfield(l, -2, "GetDamageMultiplier");
	lua_pushcfunction(l, l_vehicle_getacceleration);
	lua_setfield(l, -2, "GetAcceleration");
	lua_pushcfunction(l, l_vehicle_getmaxspeed);
	lua_setfield(l, -2, "GetMaxSlowSpeed");
	lua_pushcfunction(l, l_vehicle_getmaxrunspeed);
	lua_setfield(l, -2, "GetMaxHighSpeed");
	lua_pushcfunction(l, l_vehicle_getendturntime);
	lua_setfield(l, -2, "GetEndTurnTime");
	lua_pushcfunction(l, l_vehicle_needsrepair);
	lua_setfield(l, -2, "NeedsRepair");
	lua_pushcfunction(l, l_vehicle_hasweapon);
	lua_setfield(l, -2, "HasWeapon");
	lua_pushcfunction(l, l_vehicle_isrunspeed);
	lua_setfield(l, -2, "IsRunSpeed");
	lua_pushcfunction(l, l_vehicle_isreverse);
	lua_setfield(l, -2, "IsReverse");
	lua_pushcfunction(l, l_vehicle_destreached);
	lua_setfield(l, -2, "IsDestinationReached");
	lua_pushcfunction(l, l_vehicle_turnreached);
	lua_setfield(l, -2, "IsTurnReached");
	lua_pushcfunction(l, l_vehicle_havingturn);
	lua_setfield(l, -2, "IsHavingTurn");
	lua_pushcfunction(l, l_vehicle_currentaction);
	lua_setfield(l, -2, "GetCurrentAction");
	lua_pushcfunction(l, l_vehicle_buttonsprite);
	lua_setfield(l, -2, "GetButtonSprite");
	lua_pushcfunction(l, l_vehicle_getexitpoint);
	lua_setfield(l, -2, "GetExitPoint");
	lua_pushcfunction(l, l_vehicle_getdestination);
	lua_setfield(l, -2, "GetDestination");
	lua_pushcfunction(l, l_vehicle_getvelocity);
	lua_setfield(l, -2, "GetVelocity");
	lua_pushcfunction(l, l_vehicle_getdimensions);
	lua_setfield(l, -2, "GetDimensions");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "VehicleMetaTable");
}

int32_t Vehicle::GetAttribute(std::string const & name)
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

int32_t Vehicle::GetHP()
{
	return GetStruct()->actorstatus.hp;
}

shared_ptr<Entity> Vehicle::GetDriver()
{
	if (GetStruct()->driver == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(GetStruct()->driver);
}

shared_ptr<Entity> Vehicle::GetGunner()
{
	if (GetStruct()->gunner == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(GetStruct()->gunner);
}

shared_ptr<Entity> Vehicle::GetController()
{
	if (GetStruct()->controller == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(GetStruct()->controller);
}

shared_ptr<Entity> Vehicle::GetPassenger(int num)
{
	uint16_t id = GetStruct()->passenger[num] >> 16;
	if (id == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(id);
}

int Vehicle::GetNumPassengers()
{
	int ret = 0;

	for (int i = 0; i < 7; i++)
		if ((GetStruct()->passenger[i] >> 16) != 0)
			ret++;
	return ret;
}

int Vehicle::GetTotalOccupants()
{
	int ret = 0;
	if (GetStruct()->driver != 0)
		ret++;
	if (GetStruct()->gunner != 0)
		ret++;
	return ret + GetNumPassengers();
}

int Vehicle::GetMaxPassengers()
{
	return GetStruct()->maxpassengers;
}

int Vehicle::GetMaxTotalOccupants()
{
	int ret = GetMaxPassengers() + 1;
	if (GetStruct()->hasweapon)
		ret++;
	return ret;
}

Vector3 Vehicle::GetExitPoint()
{
	return Vector3(GetStruct()->exitx,
		GetStruct()->exity,
		GetStruct()->exitz);
}

shared_ptr<Entity> Vehicle::GetRepairObject()
{
	if (NeedsRepairObject())
		return Entity::GetEntityByID(GetStruct()->repairobject_id);
	return shared_ptr<Entity>();
}

bool Vehicle::NeedsRepairObject()
{
	return GetStruct()->repairneeded;
}

bool Vehicle::HasWeapon()
{
	return GetStruct()->hasweapon;
}

shared_ptr<Entity> Vehicle::GetVehicleWeapon()
{
	if (HasWeapon())
		return Entity::GetEntityByID(GetStruct()->weapon);

	return shared_ptr<Entity>();
}

shared_ptr<Entity> Vehicle::GetDriverLeftHandItem()
{
	uint16_t id = GetStruct()->driver_lefthand_id;
	if (id != 0)
		return Entity::GetEntityByID(id);
	return shared_ptr<Entity>();
}

shared_ptr<Entity> Vehicle::GetGunnerLeftHandItem()
{
	uint16_t id = GetStruct()->gunner_lefthand_id;
	if (id != 0)
		return Entity::GetEntityByID(id);
	return shared_ptr<Entity>();
}

shared_ptr<Entity> Vehicle::GetInventory()
{
	if (GetStruct()->inventory != 0)
		return Entity::GetEntityByID(GetStruct()->inventory);

	return shared_ptr<Entity>();
}

bool Vehicle::IsRunSpeed()
{
	return GetStruct()->runspeed;
}

bool Vehicle::IsReverse()
{
	return GetStruct()->inreverse;
}

std::string Vehicle::GetCurrentAction()
{
	return Helpers::WcharToUTF8(GetStruct()->action);
}

Vector3 Vehicle::GetDestination()
{
	return Vector3(GetStruct()->destx,
		GetStruct()->desty,
		GetStruct()->destz);
}

bool Vehicle::IsDestinationReached()
{
	return GetStruct()->destreached;
}

bool Vehicle::IsTurnReached()
{
	return GetStruct()->turnreached;
}

int32_t Vehicle::GetTurnRate()
{
	return GetStruct()->turnrate;
}

float Vehicle::GetDamageMultiplier()
{
	return GetStruct()->damagemult;
}

int32_t Vehicle::GetCriticalChance()
{
	return GetStruct()->critchance;
}

float Vehicle::GetAcceleration()
{
	return GetStruct()->acceleration;
}

float Vehicle::GetMaxSlowSpeed()
{
	return GetStruct()->maxspeed;
}

float Vehicle::GetMaxHighSpeed()
{
	return GetStruct()->maxrunspeed;
}

Vector3 Vehicle::GetVelocity()
{
	return Vector3(GetStruct()->velx,
		GetStruct()->vely,
		GetStruct()->velz);
}

Vector3 Vehicle::GetDimensions()
{
	return Vector3(GetStruct()->width,
		GetStruct()->height,
		GetStruct()->length);
}

float Vehicle::GetEndTurnTime()
{
	return GetStruct()->endturntime;
}

std::string Vehicle::GetButtonSprite()
{
	return Helpers::WcharToUTF8(GetStruct()->buttonsprite);
}

bool Vehicle::IsHavingTurn()
{
	return GetStruct()->havingturn;
}

Vehicle::VehicleStructType * Vehicle::GetStruct()
{
	return (VehicleStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_VEHICLE_STRUCT);
}