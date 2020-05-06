#pragma once
#include "Entity.h"
#pragma once
#include "Collectable.h"

class Sneak :
	public Entity
{
public:
	Sneak(EntityID id);
	Sneak(void* ptr);
	virtual ~Sneak();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x813758;
};

