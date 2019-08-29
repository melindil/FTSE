#pragma once
#include "Collectable.h"
class Trap :
	public Collectable
{
public:
	Trap(uint16_t id);
	Trap(void* ptr);
	virtual ~Trap();

	int32_t GetSkillRoll();
	int16_t GetOwnerID();
	int16_t GetHolderID();
	bool IsActivated();
	bool IsHidden();
	uint32_t GetDetonationTime1();
	uint32_t GetDetonationTime2();
	float GetFrequency();
	bool IsReusable();
	int32_t GetDifficulty();
	int32_t GetCriticalChance();
	int32_t GetMinBaseDamage();
	int32_t GetMaxBaseDamage();
	std::string GetPrimaryDamageType();
	std::string GetSecondaryDamageType();
	std::string GetAmmoVariant();
	float GetDamageRadius();
	std::string GetImpactFX();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);

	static const uint32_t VTABLE = 0x8210f8;

private:
#pragma pack(push,1)
	typedef struct
	{
		int32_t skillroll;
		uint16_t ownerseqnum;
		uint16_t ownerid;
		uint16_t holderseqnum;
		uint16_t holderid;
		bool activated;
		bool hidden;
		uint32_t detonationtime;
		uint32_t detonationtime2;
		float frequency;
		char unk1[5];
		bool reusable;
		int32_t difficulty;
		int32_t critchance;
		int32_t mindmg;
		int32_t maxdmg;
		wchar_t* primarydamagetype;
		wchar_t* secondarydamagetype;
		wchar_t* ammovariant;
		float damageradius;
		wchar_t* impactfx;
	} TrapStructType;

	TrapStructType* GetStruct();
	static const uint32_t OFFSET_TRAP_STRUCT = 0x830;

#pragma pack(pop)
};

/*
830: Skill roll
834: Owner (flags,entityid)
838: Holder (same)
83c: Activated (bool)
83d: Hidden (bool)
83e: Detonation time check
842: Detonation time check
846: Frequency (float?)
84f: Reusable bool
850: Difficulty
854: Critical chance
858: min dmg
85c: max dmg
860: damage base type
864: damage special type
868: Ammo variant
86c: Damage radius (float)
870: impact fx
*/

