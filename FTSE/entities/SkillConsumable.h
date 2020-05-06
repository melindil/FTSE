#pragma once
#include "Collectable.h"
class SkillConsumable :
	public Collectable
{
public:
	SkillConsumable(EntityID id);
	SkillConsumable(void* ptr);
	virtual ~SkillConsumable();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x82089c;
	int32_t GetTotalUses();
	int32_t GetRemainingUses();

private:
#pragma pack (push,1)
	typedef struct
	{
		int32_t totaluses;
		int32_t remaining;
	} SkillConsumableStructType;
#pragma pack(pop)
	SkillConsumableStructType* GetStruct();
	static const uint32_t OFFSET_SKILLCONSUMABLE_STRUCT = 0x830;

};
