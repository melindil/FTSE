#pragma once
#include "Collectable.h"
class Lockpick :
	public Collectable
{
public:
	Lockpick(EntityID id);
	Lockpick(void* ptr);
	virtual ~Lockpick();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81f7e4;
	bool IsElectronic();

private:
#pragma pack (push,1)
	typedef struct
	{
		char electronic;
	} LockpickStructType;
#pragma pack(pop)
	LockpickStructType* GetStruct();
	static const uint32_t OFFSET_LOCKPICK_STRUCT = 0x830;

};


