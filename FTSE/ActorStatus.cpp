#include "ActorStatus.h"

std::map<std::string, uint32_t> ActorStatusOffsets;

void ActorStatus_Initialize()
{
	ActorStatusOffsets["hp"] = 4;
	ActorStatusOffsets["bandaged"] = 8;
	ActorStatusOffsets["ap"] = 12;
	ActorStatusOffsets["radpoints"] = 16;
	ActorStatusOffsets["radlevel"] = 20;
	ActorStatusOffsets["poisonpoints"] = 24;
	ActorStatusOffsets["poisonlevel"] = 28;
	ActorStatusOffsets["overdosepoints"] = 32;
	ActorStatusOffsets["goneuplevel"] = 36;
}

