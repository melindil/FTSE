#pragma once

#include <stdint.h>

#pragma pack(push,1)
typedef struct
{
	uint32_t vtable;
	int32_t hp;
	int32_t bandaged;
	int32_t ap;
	int32_t radpoints;
	int32_t radlevel;
	int32_t poisonpoints;
	int32_t poisonlevel;
	int32_t overdosepoints;
	bool goneuplevel;

} ActorStatus;
#pragma pack(pop)