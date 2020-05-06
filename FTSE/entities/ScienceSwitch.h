#pragma once
#include "Switch.h"

class ScienceSwitch :
	public Switch
{
public:
	ScienceSwitch(EntityID id);
	ScienceSwitch(void* ptr);
	virtual ~ScienceSwitch();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81a41c;
	int32_t GetDifficulty();

private:
#pragma pack (push,1)
	typedef struct
	{
		int32_t difficulty;
	} ScienceSwitchStructType;
#pragma pack(pop)
	ScienceSwitchStructType* GetStruct();
	static const uint32_t OFFSET_SCIENCESWITCH_STRUCT = 0x1ac;
};

