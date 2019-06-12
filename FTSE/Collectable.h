#pragma once
#include "Entity.h"
class Collectable :
	public Entity
{
public:
	Collectable(void* ptr);
	Collectable(uint16_t id);
	~Collectable();

	float GetWeightPerUnit();
	float GetWeightTotal();
	int GetCount();
	int GetCountNonLootable();
	int GetValuePerUnit();
	int GetValueTotal();
	bool IsUnique();
	bool CanBeRemoved();
	bool BeenApplied();
	bool Lootable();

	int GetRequiredAttribute(std::string const& name);
	int GetEffectAttribute(std::string const& name);

private:
#pragma pack(push,1)
	typedef struct {
		char unique;
		char canberemoved;
		char beenapplied;
		float weight;
		int count;
		int countnonlootable;
		bool lootable;
		int valuebase;
		int rankreq;
		char required_attributes[0x339];
		char effect_attributes[0x339];
	} CollectableStructType;

	CollectableStructType* GetCollectableStruct();

	static const int OFFSET_STRUCT = 0x1a6;

#pragma pack(pop)
};

