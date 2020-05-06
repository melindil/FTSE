#pragma once
#include "Entity.h"
class SpawnPoint :
	public Entity
{
public:
	SpawnPoint(EntityID id);
	SpawnPoint(void* ptr);
	virtual ~SpawnPoint();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x813fb8;
	
	int32_t GetDesiredTeam();
	int32_t GetSpawnCounter();
	std::string GetSpawnType();
	bool GetMainDudeOnly();

private:
#pragma pack (push,1)
	typedef struct
	{
		int32_t desiredteam;
		int32_t spawncounter;
		wchar_t* spawntype;
		bool maindudeonly;
	} SpawnPointStructType;
#pragma pack(pop)
	SpawnPointStructType* GetStruct();
	static const uint32_t OFFSET_SPAWNPOINT_STRUCT = 0x1a5;

};

