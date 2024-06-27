#pragma once

#include <stdint.h>
#include "EntityID.h"

#pragma pack(push, 1)

typedef struct
{
	int32_t status;
	EntityID unk_entity_1;
	int32_t unk2;
	int32_t unk3;
	wchar_t* errstring;
	EntityID unk_entity_4;
	int32_t unk_5;
} InventoryActionResult;

class EntityIDVector
{
public:
	int32_t unk;
	EntityID* start;
	EntityID* end;
	EntityID* cap;

	EntityIDVector() : unk(0), start(nullptr), end(nullptr), cap(nullptr) {}
	~EntityIDVector()
	{
		if (start != nullptr)
		{
			auto fxn = (void(__cdecl *)(void*))0x6c4e50;
			fxn(reinterpret_cast<void*>(start));

		}
	}
};

#pragma pack(pop)