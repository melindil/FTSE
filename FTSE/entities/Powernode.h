#pragma once
#include "Usable.h"

class Powernode :
	public Usable
{
public:
	Powernode(EntityID id);
	Powernode(void* ptr);
	virtual ~Powernode();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x819354;

	uint64_t GetLastHealTime();
	int32_t GetCurrentHP();
	int32_t GetMaxHP();
	std::string GetCurrentAction();
	int32_t GetArmorClass();
	float GetHealRate();
	int32_t GetDifficulty();

private:

#pragma pack (push,1)
	typedef struct
	{
		uint64_t lasthealtime;
		float currenthp;		// Yes, this is a float, but we'll return int
		wchar_t* currentaction;
		float maxhp;			// Yes, this one too
		int32_t armorclass;
		float healrate;
		int32_t difficulty;
	} PowernodeStructType;
#pragma pack(pop)
	PowernodeStructType* GetStruct();
	static const uint32_t OFFSET_POWERNODE_STRUCT = 0x1a6;
};


