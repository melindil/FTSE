#pragma once
#include <stdint.h>

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
};

struct ChanceToHit
{
	uint8_t hit_chance;
	uint16_t unk1;
	uint8_t ineligible_flags;
	uint8_t unk2;
};

#pragma pack(pop)