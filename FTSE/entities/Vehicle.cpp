#include "Vehicle.h"

#include "LuaHelper.h"
#include "AttributesTable.h"

Vehicle::Vehicle(EntityID id)
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
	auto entity = std::dynamic_pointer_cast<Vehicle>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		std::string attrib = lua_tostring(l, 2);
		lua_pushinteger(l, entity->GetAttribute(attrib));
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

int l_vehicle_getpassenger(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Vehicle>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		int passnum = (int)lua_tointeger(l, 2);
		auto passenger = entity->GetPassenger(passnum);
		if (passenger)
		{
			passenger->MakeLuaObject(l);
		}
		else
		{
			lua_pushnil(l);
		}
	}
	else
	{
		lua_pushnil(l);
	}
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
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetHP>()));
	lua_setfield(l, -2, "GetHP");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetDriver>()));
	lua_setfield(l, -2, "GetDriver");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetGunner>()));
	lua_setfield(l, -2, "GetGunner");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetController>()));
	lua_setfield(l, -2, "GetController");
	lua_pushcfunction(l, l_vehicle_getpassenger);
	lua_setfield(l, -2, "GetPassenger");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetRepairObject>()));
	lua_setfield(l, -2, "GetRepairObject");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetVehicleWeapon>()));
	lua_setfield(l, -2, "GetVehicleWeapon");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetDriverLeftHandItem>()));
	lua_setfield(l, -2, "GetDriverLeftHandItem");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetGunnerLeftHandItem>()));
	lua_setfield(l, -2, "GetGunnerLeftHandItem");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetInventory>()));
	lua_setfield(l, -2, "GetInventory");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetNumPassengers>()));
	lua_setfield(l, -2, "GetNumPassengers");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetTotalOccupants>()));
	lua_setfield(l, -2, "GetTotalOccupants");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetMaxPassengers>()));
	lua_setfield(l, -2, "GetMaxPassengers");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetMaxTotalOccupants>()));
	lua_setfield(l, -2, "GetMaxTotalOccupants");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetTurnRate>()));
	lua_setfield(l, -2, "GetTurnRate");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetCriticalChance>()));
	lua_setfield(l, -2, "GetCriticalChance");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetDamageMultiplier>()));
	lua_setfield(l, -2, "GetDamageMultiplier");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetAcceleration>()));
	lua_setfield(l, -2, "GetAcceleration");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetMaxSlowSpeed>()));
	lua_setfield(l, -2, "GetMaxSlowSpeed");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetMaxHighSpeed>()));
	lua_setfield(l, -2, "GetMaxHighSpeed");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetEndTurnTime>()));
	lua_setfield(l, -2, "GetEndTurnTime");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::NeedsRepairObject>()));
	lua_setfield(l, -2, "NeedsRepair");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::HasWeapon>()));
	lua_setfield(l, -2, "HasWeapon");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::IsRunSpeed>()));
	lua_setfield(l, -2, "IsRunSpeed");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::IsReverse>()));
	lua_setfield(l, -2, "IsReverse");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::IsDestinationReached>()));
	lua_setfield(l, -2, "IsDestinationReached");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::IsTurnReached>()));
	lua_setfield(l, -2, "IsTurnReached");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::IsHavingTurn>()));
	lua_setfield(l, -2, "IsHavingTurn");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetCurrentAction>()));
	lua_setfield(l, -2, "GetCurrentAction");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetButtonSprite>()));
	lua_setfield(l, -2, "GetButtonSprite");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetExitPoint>()));
	lua_setfield(l, -2, "GetExitPoint");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetDestination>()));
	lua_setfield(l, -2, "GetDestination");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetVelocity>()));
	lua_setfield(l, -2, "GetVelocity");
	lua_pushcfunction(l, (LuaHelper::THUNK<Vehicle, &Vehicle::GetDimensions>()));
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
	if (GetStruct()->driver.id == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(GetStruct()->driver);
}

shared_ptr<Entity> Vehicle::GetGunner()
{
	if (GetStruct()->gunner.id == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(GetStruct()->gunner);
}

shared_ptr<Entity> Vehicle::GetController()
{
	if (GetStruct()->controller.id == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(GetStruct()->controller);
}

shared_ptr<Entity> Vehicle::GetPassenger(int num)
{
	if (GetStruct()->passenger[num].id == 0)
		return shared_ptr<Entity>();

	return Entity::GetEntityByID(GetStruct()->passenger[num]);
}

int Vehicle::GetNumPassengers()
{
	int ret = 0;

	for (int i = 0; i < 7; i++)
		if ((GetStruct()->passenger[i].id) != 0)
			ret++;
	return ret;
}

int Vehicle::GetTotalOccupants()
{
	int ret = 0;
	if (GetStruct()->driver.id != 0)
		ret++;
	if (GetStruct()->gunner.id != 0)
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
		return Entity::GetEntityByID(GetStruct()->repairobject);
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
	if (GetStruct()->driver_lefthand.id == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(GetStruct()->driver_lefthand);
}

shared_ptr<Entity> Vehicle::GetGunnerLeftHandItem()
{
	if (GetStruct()->gunner_lefthand.id == 0)
		return shared_ptr<Entity>();
	return Entity::GetEntityByID(GetStruct()->gunner_lefthand);
}

shared_ptr<Entity> Vehicle::GetInventory()
{
	if (GetStruct()->inventory.id != 0)
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