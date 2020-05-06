#pragma once
#include "Collectable.h"

class RepairObject :
	public Collectable
{
public:
	RepairObject(EntityID id);
	RepairObject(void* ptr);
	virtual ~RepairObject();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x820040;
	int32_t GetDifficulty();

private:
#pragma pack (push,1)
	typedef struct
	{
		int32_t difficulty;
	} RepairObjectStructType;
#pragma pack(pop)
	RepairObjectStructType* GetStruct();
	static const uint32_t OFFSET_REPAIROBJECT_STRUCT = 0x830;

};

