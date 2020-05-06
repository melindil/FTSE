#pragma once

#include <stdint.h>

#pragma pack(push, 1)

typedef struct
{
	int32_t status;
	int32_t unk_entity_1;
	int32_t unk2;
	int32_t unk3;
	wchar_t* errstring;
	int32_t unk_entity_4;
	int32_t unk_5;
} InventoryActionResult;
#pragma pack(pop)