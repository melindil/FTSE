#pragma once
#include "Entity.h"
#include <stdint.h>

struct lua_State;
class Actor;
class Weapon :
	public Entity
{
public:
	Weapon(uint16_t id);
	Weapon(void* ptr);
	~Weapon();

	float GetRange(Actor& attacker);
	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
};

