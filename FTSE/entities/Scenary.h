#pragma once
#include "Entity.h"

class Scenary :
	public Entity
{
public:
	Scenary(EntityID id);
	Scenary(void* ptr);
	virtual ~Scenary();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x811e44;
};
