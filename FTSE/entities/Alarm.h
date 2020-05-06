#pragma once
#include "Usable.h"
class Alarm :
	public Usable
{
public:
	Alarm(EntityID id);
	Alarm(void* ptr);
	virtual ~Alarm();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x816988;
	bool IsActivated();

	private:
#pragma pack (push,1)
		typedef struct
		{
			bool isactivated;
		} AlarmStructType;
#pragma pack(pop)
		AlarmStructType* GetStruct();
		static const uint32_t OFFSET_ALARM_STRUCT = 0x1a6;


};

