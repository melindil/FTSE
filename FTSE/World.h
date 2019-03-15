#pragma once

#include <stdint.h>
#include "lua.hpp"
#include <string>
#include <vector>
#include "Helpers.h"

class Logger;
#pragma pack(push,1)
class World
{
public:
	World();
	~World();
	struct EntityTable
	{
		void* entityptr;
		int16_t unknown_1;
		int16_t unknown_2;
	};

	struct PlayerFOTLinkedList
	{
		PlayerFOTLinkedList* next;
		PlayerFOTLinkedList* prev;
		uint16_t flags;
		uint16_t entity_id;

	};

	struct PlayerFOTObject
	{
		uint32_t vtable;
		char unknown_1[0x47];
		PlayerFOTLinkedList* players;
		char unknown_2[0x10e];
	};

	struct WorldFOTObject
	{
		uint32_t vtable;
		void* level_object;
		char unknown_1[0x2ef];
		float realTime;
		long long gameTime;
		char unknown_2a[0x83];
		wchar_t** missionvar_val_start;
		wchar_t** missionvar_val_end;
		wchar_t** missionvar_val_end_2;
		char skip1[4];
		wchar_t** missionvar_key_start;
		wchar_t** missionvar_key_end;
		wchar_t** missionvar_key_end_2;
		char skip2[4];

		char unknown_2[0x38d];
		PlayerFOTObject* ptrPlayerList;
		PlayerFOTObject* ptrPlayerListEnd;
		char unknown_3[8];
		EntityTable* entityStart;
		EntityTable* entityEnd;

	};
	static WorldFOTObject* GetGlobal();
	static void* GetEntity(uint16_t id);
	static std::vector<uint16_t> GetSquad();
	static void CombatLog(int level, std::string const& txt);
	static void RegisterLua(lua_State* l,Logger* logger);
	static void SetVariable(std::string const& key, std::string const& value, bool campaign);
	static bool CheckBlocked(Vector3 src, Vector3 tgt);

	static const uint32_t CVAR_GLOBAL_PTR = 0x8bdd2c;


private:
	static const uint32_t WORLD_GLOBAL_PTR = 0x8bc564;
	static const uint32_t FXN_WORLD_SETVARIABLE = 0x67ee30;
	static const uint32_t FXN_WORLD_COMBATLOG = 0x5e6d60;
	static const uint32_t ACTOR_VTABLE = 0x80c1d0;

};

struct BlockSearch {
	uint32_t vtable;
	uint16_t unk1;
	uint8_t blockedflags;
	int32_t pt1[3];
	int32_t pt2[3];
	int32_t pt3[3];
	int32_t pt4[3];
	uint32_t ctr;
	uint8_t unk2;
	uint32_t unk3;
};

static const uint32_t BLOCKSEARCH_VTABLE = 0x8095cc;
static const uint32_t* BLOCKSEARCH_CTR_PTR = (const uint32_t*)0x8b0a60;
#pragma pack(pop)