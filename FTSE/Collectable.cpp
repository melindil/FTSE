#include "Collectable.h"

#include "AttributesTable.h"

Collectable::Collectable(void * ptr)
	: Entity(ptr)
{
}

Collectable::Collectable(uint16_t id)
	: Entity(id)
{
}

Collectable::~Collectable()
{
}

float Collectable::GetWeightPerUnit()
{
	return GetCollectableStruct()->weight;
}

float Collectable::GetWeightTotal()
{
	return GetCollectableStruct()->weight *
		(GetCollectableStruct()->count + GetCollectableStruct()->countnonlootable);
}

int Collectable::GetCount()
{
	return GetCollectableStruct()->count;
}

int Collectable::GetCountNonLootable()
{
	return GetCollectableStruct()->countnonlootable;
}

int Collectable::GetValuePerUnit()
{
	return GetCollectableStruct()->valuebase;
}

int Collectable::GetValueTotal()
{
	return GetCollectableStruct()->valuebase *
		(GetCollectableStruct()->count + GetCollectableStruct()->countnonlootable);
}

bool Collectable::IsUnique()
{
	return GetCollectableStruct()->unique;
}

bool Collectable::CanBeRemoved()
{
	return GetCollectableStruct()->canberemoved;
}

bool Collectable::BeenApplied()
{
	return GetCollectableStruct()->beenapplied;
}

bool Collectable::Lootable()
{
	return GetCollectableStruct()->lootable;
}

int Collectable::GetRequiredAttribute(std::string const & name)
{
	int offset = AttributesTable::GetOffsetByName(name);
	if (std::string(name, 0, 4) == "tag_" || AttributesTable::GetGroupByName(name) == "otraits")
	{
		char result = GetCollectableStruct()->required_attributes[offset];
		return (int)result;
	}
	int result;
	memcpy(&result, GetCollectableStruct()->required_attributes + offset, sizeof(int32_t));
	return result;
}

int Collectable::GetEffectAttribute(std::string const & name)
{
	int offset = AttributesTable::GetOffsetByName(name);
	if (std::string(name, 0, 4) == "tag_" || AttributesTable::GetGroupByName(name) == "otraits")
	{
		char result = GetCollectableStruct()->effect_attributes[offset];
		return (int)result;
	}
	int result;
	memcpy(&result, GetCollectableStruct()->effect_attributes + offset, sizeof(int32_t));
	return result;
}

Collectable::CollectableStructType* Collectable::GetCollectableStruct()
{
	char* ptr = ((char*)GetEntityPointer()) + OFFSET_STRUCT;
	return reinterpret_cast<CollectableStructType*>(ptr);
}
