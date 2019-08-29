#pragma once
#include "Breakable.h"
class StateBreakable :
	public Breakable
{
public:
	StateBreakable(uint16_t id);
	StateBreakable(void* ptr);
	virtual ~StateBreakable();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);

	bool IsOn();

	static const uint32_t VTABLE = 0x814814;

private:
	static const uint32_t OFFSET_STATEBREAKABLE_STRUCT = 0x51d;
};

