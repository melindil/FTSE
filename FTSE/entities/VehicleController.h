#pragma once
#include "Collectable.h"

class VehicleController :
	public Collectable
{
public:
	VehicleController(EntityID id);
	VehicleController(void* ptr);
	virtual ~VehicleController();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x80ca88;
};


