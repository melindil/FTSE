#pragma once
#include "Collectable.h"

class Consumable :
	public Collectable
{
public:
	Consumable(EntityID id);
	Consumable(void* ptr);
	virtual ~Consumable();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81ce10;

	int32_t GetEffectAttribute(int effectnum, std::string const& effectname);
	float GetEffectAttributeFloat(int effectnum, std::string const& effectname);
	bool IsEffectEnabled(int effectnum);
	int32_t GetEffectDuration(int effectnum);
	float GetEffectMultiplier(int effectnum);

	std::string GetEffectedAttribute();
	int32_t GetEffectMinimum();
	int32_t GetEffectMaximum();
	std::string GetCategory();
	int32_t GetOverdosePoints();
	int32_t GetChanceAddiction();
	bool IsAddicted();
	int32_t GetState();

private:
#pragma pack (push,1)
	typedef struct
	{
		char enabled;
		char attributes_table[0x339];
		char actorstatus_table[0x25];
		int32_t duration;
		wchar_t* durationtype;
		float multiplier;
	} ConsumableEffectStructType;

	typedef struct
	{
		wchar_t* effectedattribute;
		int32_t minimum;
		int32_t maximum;
		wchar_t* category;
		int32_t overdosepoints;
		ConsumableEffectStructType effects[4];
		int32_t chanceaddiction;
		bool addicted;
		int32_t state;
	} ConsumableStructType;
#pragma pack(pop)
	ConsumableStructType* GetStruct();
	static const uint32_t OFFSET_CONSUMABLE_STRUCT = 0x830;

};


