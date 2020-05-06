#pragma once
#include "Collectable.h"

class GenericItem :
	public Collectable
{
public:
	GenericItem(EntityID id);
	GenericItem(void* ptr);
	virtual ~GenericItem();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81ded0;
};

