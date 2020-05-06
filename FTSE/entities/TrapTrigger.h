#pragma once
#include "Collectable.h"

class TrapTrigger :
	public Collectable
{
public:
	TrapTrigger(EntityID id);
	TrapTrigger(void* ptr);
	virtual ~TrapTrigger();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x821958;
};
