#pragma once
#include "Entity.h"

class StateScenary :
	public Entity
{
public:
	StateScenary(EntityID id);
	StateScenary(void* ptr);
	virtual ~StateScenary();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x815074;

	bool IsOn();

private:
#pragma pack (push,1)
	typedef struct
	{
		bool on;
	} StateScenaryStructType;
#pragma pack(pop)
	StateScenaryStructType* GetStruct();
	static const uint32_t OFFSET_STATESCENARY_STRUCT = 0x1a5;

};


