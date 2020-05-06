#pragma once
#include "Door.h"

class RotatingDoor :
	public Door
{
public:
	RotatingDoor(EntityID id);
	RotatingDoor(void* ptr);
	virtual ~RotatingDoor();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x819bb4;

};


