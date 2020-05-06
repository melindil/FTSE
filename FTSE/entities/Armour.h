#pragma once
#include "Collectable.h"
class Armour :
	public Collectable
{
public:
	Armour(EntityID id);
	Armour(void* ptr);
	virtual ~Armour();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81bd44;

};

