#pragma once
#include <stdint.h>
#include <memory>

#include "FOTString.h"

#pragma pack(push,1)
struct CombatMessage
{
	uint32_t vtable;
	unsigned short attacker_flags;
	unsigned short attacker;
	unsigned short target_flags;
	unsigned short target;
	unsigned short weapon_flags;
	unsigned short weapon;
	unsigned short unk1;
	unsigned short unk2;
	float attacker_x;
	float attacker_y;
	float attacker_z;
	float target_x;
	float target_y;
	float target_z;
	uint32_t damage;
	uint32_t critflags;
	short unk3;
	char unk4;
	uint32_t aimedlocation;
	uint32_t numshots;
	char unk5;
	char unused[3];
	uint32_t arraystart;
	uint32_t arrayend;
	uint32_t arrayptr;
	char unk6[3];
	short target_2;
	char unk7[7];

	std::unique_ptr<FOTString> GetAimedLocation()
	{
		auto fxn = (void* (*)(void**, int32_t))0x62f830;
		void* ptr;
		fxn(&ptr, aimedlocation);
		return std::make_unique<FOTString>(ptr);
	}

};

struct ChanceToHit
{
	uint8_t hit_chance;
	int16_t raw_chance;
	uint8_t ineligible_flags;
	uint8_t unk2;
};

#pragma pack(pop)