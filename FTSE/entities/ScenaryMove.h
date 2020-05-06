#pragma once
#include "Entity.h"

class ScenaryMove :
	public Entity
{
public:
	ScenaryMove(EntityID id);
	ScenaryMove(void* ptr);
	virtual ~ScenaryMove();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x812ef8;
};
