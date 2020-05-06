#pragma once
#include "Entity.h"

struct lua_State;

class Collectable :
	public Entity
{
public:
	Collectable(void* ptr);
	Collectable(uint16_t id) = delete;
	Collectable(EntityID id);
	~Collectable();

	float GetWeightPerUnit();
	float GetWeightTotal();
	int GetCount();
	int GetCountNonLootable();
	int GetCountTotal();
	int GetValuePerUnit();
	int GetValueTotal();
	bool IsUnique();
	bool CanBeRemoved();
	bool BeenApplied();
	bool Lootable();

	void OverrideCount(int newcount);

	int GetRequiredAttribute(std::string const& name);
	int GetEffectAttribute(std::string const& name);

	static void SetLuaSubclass(lua_State* l);

protected:
#pragma pack(push,1)
	typedef struct {
		char removeonexit;
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
#pragma pack(pop)

	CollectableStructType* GetCollectableStruct();

	static const int OFFSET_STRUCT = 0x1a5;

};

