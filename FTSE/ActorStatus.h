#pragma once

#include <stdint.h>
#include <map>
#include <string>

#pragma pack(push,1)
typedef struct
{
	uint32_t vtable;
	int32_t hp;
	int32_t bandaged;
	float ap;
	int32_t radpoints;
	int32_t radlevel;
	int32_t poisonpoints;
	int32_t poisonlevel;
	int32_t overdosepoints;
	bool goneuplevel;

	static const uint32_t VTABLE = 0x824c24;

} ActorStatus;


#pragma pack(pop)

extern std::map<std::string, uint32_t> ActorStatusOffsets;
void ActorStatus_Initialize();