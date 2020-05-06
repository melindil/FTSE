#pragma once
#include "Entity.h"

class ScenarySpawn :
	public Entity
{
public:
	ScenarySpawn(EntityID id);
	ScenarySpawn(void* ptr);
	virtual ~ScenarySpawn();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x8126a0;

	float GetSpeed();
	float GetFrequency();
	std::string GetSpawnSprite();
	int32_t GetMaxSpawn();

private:
#pragma pack (push,1)
	typedef struct
	{
		float speed;
		float frequency;
		wchar_t* sprite;
		int32_t maxspawn;
	} ScenarySpawnStructType;
#pragma pack(pop)
	ScenarySpawnStructType* GetStruct();
	static const uint32_t OFFSET_SCENARYSPAWN_STRUCT = 0x1a5;

};

