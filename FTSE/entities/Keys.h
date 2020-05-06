#pragma once
#include "GenericItem.h"
class Keys :
	public GenericItem
{
public:
	Keys(EntityID id);
	Keys(void* ptr);
	virtual ~Keys();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81ef88;
};

