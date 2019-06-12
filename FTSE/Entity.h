#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include "Logger.h"
#include "Helpers.h"

#include "World.h"

struct lua_State;

class Entity
{
public:
	Entity(void* ptr);
	Entity(uint16_t id);
	virtual ~Entity();

	int16_t GetID() { return entity_id_; }
	virtual void MakeLuaObject(lua_State* l);

	static std::shared_ptr<Entity> GetEntityByID(uint16_t id);

	std::string GetEntityName();
	bool isAlive();
	void ShotAtMissed(void* cmsg);
	static void RegisterLua(lua_State* l, Logger* tmp);
	void* GetEntityPointer()
	{
		return World::GetEntity(entity_id_);
	}
	Vector3 GetLocation();
	uint16_t GetFlags();
	uint32_t GetVtable();
	uint32_t GetVtableFxn(uint32_t offset);

protected:
	static const uint32_t OFFSET_ENTITY_ID = 0x004;
	static const uint32_t OBJECT_ACTOR_GETNAME = 0x8bd8b8;
	static const uint32_t FXN_ACTOR_GETNAME = 0x64f5d0;

	static const uint32_t VTABLE_ACTOR = 0x80c1d0;
	static const uint32_t VTABLE_WEAPON = 0x822a04;

	uint16_t entity_id_;
};

