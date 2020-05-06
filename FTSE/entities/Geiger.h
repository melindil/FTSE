#pragma once
#include "Collectable.h"

class Geiger :
	public Collectable
{
public:
	Geiger(EntityID id);
	Geiger(void* ptr);
	virtual ~Geiger();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81d674;
};

