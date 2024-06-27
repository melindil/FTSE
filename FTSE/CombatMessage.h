#pragma once
#include <stdint.h>
#include <memory>

#include "FOTString.h"
#include "Entity.h"
#include "Helpers.h"

#pragma pack(push,1)
struct CombatMessage
{
	uint32_t vtable;
	EntityID attacker;
	EntityID target;
	EntityID weapon;
	uint32_t unk1;
	Vector3 attacker_pos;
	Vector3 target_pos;
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


	std::unique_ptr<FOTString> GetAimedLocation()
	{
		auto fxn = (void* (*)(void**, int32_t))0x62f830;
		void* ptr;
		fxn(&ptr, aimedlocation);
		return std::make_unique<FOTString>(ptr);
	}
	static const uint32_t VTABLE = 0x80c14c;

};

struct ChanceToHit
{
	uint8_t hit_chance;
	int16_t raw_chance;
	uint8_t ineligible_flags;
	uint8_t unk2;
};

#pragma pack(pop)