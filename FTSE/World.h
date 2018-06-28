#pragma once

#include <stdint.h>
#include "lua.hpp"
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

	struct WorldFOTObject
	{
		uint32_t vtable;
		char unknown_1[0x2f3];
		float realTime;
		long long gameTime;
		char unknown_2[0x430];
		void* ptrPlayerList;
		void* ptrPlayerListEnd;
		char unknown_3[8];
		EntityTable* entityStart;
		EntityTable* entityEnd;

	};
	static WorldFOTObject* GetGlobal();
	static void* GetEntity(uint16_t id);
	static void RegisterLua(lua_State* l,Logger* logger);


private:
	static const uint32_t WORLD_GLOBAL_PTR = 0x8bc564;


};
#pragma pack(pop)