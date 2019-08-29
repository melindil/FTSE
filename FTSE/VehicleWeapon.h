#pragma once
#include "Weapon.h"
#include "CombatMessage.h"
class VehicleWeapon :
	public Weapon
{
public:
	VehicleWeapon(uint16_t id);
	VehicleWeapon(void* ptr);
	virtual ~VehicleWeapon();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x8221b4;

	typedef struct
	{
		wchar_t* motionstate;
		float destx;
		float desty;
		float destz;
		bool isdestreached;
		CombatMessage internal_cmsg;
		float targetloc;
		bool vff;
	} VehicleWeaponStructType;

private:
	static const uint32_t OFFSET_VEHICLEWEAPON_STRUCT = 0xd37;
};

