#pragma once
#include "Collectable.h"

class CaptureInvItem :
	public Collectable
{
public:
	CaptureInvItem(EntityID id);
	CaptureInvItem(void* ptr);
	virtual ~CaptureInvItem();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x823288;

};

