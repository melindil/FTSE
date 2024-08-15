#pragma once
#include <stdint.h>

#include "EntityID.h"
#include "Helpers.h"

#pragma pack (push,1)
struct ControllerCommandStruct
{
	uint8_t type;
	uint16_t action_entity_halfid;
	EntityID entity_id_1;
	Vector3 loc;
	EntityID entity_id_2;
	int32_t param;
};
#pragma pack (pop)
