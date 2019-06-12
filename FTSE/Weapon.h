#pragma once
#include "Collectable.h"
#include <stdint.h>

struct lua_State;
class Actor;
class Weapon :
	public Collectable
{
public:
	Weapon(uint16_t id);
	Weapon(void* ptr);
	~Weapon();

	float GetRange(Actor& attacker);
	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
};

