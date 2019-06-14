#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include "Logger.h"
#include "Helpers.h"


struct lua_State;

class Entity
{
public:
	Entity(void* ptr);
	Entity(uint16_t id);
	virtual ~Entity();

	virtual int16_t GetID() { return ((EntityHeaderType*)entity_ptr_)->id; }
	virtual void MakeLuaObject(lua_State* l);

	static std::shared_ptr<Entity> GetEntityByID(uint16_t id);

	virtual std::string GetEntityName();
	virtual bool isAlive();
	virtual void ShotAtMissed(void* cmsg);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static void SetLuaSubclass(lua_State* l);
	virtual void* GetEntityPointer()
	{
		return entity_ptr_;
	}
	virtual Vector3 GetLocation();
	virtual uint16_t GetFlags();
	virtual uint32_t GetVtable();
	virtual uint32_t GetVtableFxn(uint32_t offset);

protected:
	typedef struct
	{
		uint32_t vtable;
		uint16_t id;
		uint16_t flags;
	} EntityHeaderType;
	static const uint32_t OFFSET_ENTITY_ID = 0x004;
	static const uint32_t OBJECT_ACTOR_GETNAME = 0x8bd8b8;
	static const uint32_t FXN_ACTOR_GETNAME = 0x64f5d0;

	static const uint32_t VTABLE_ACTOR = 0x80c1d0;
	static const uint32_t VTABLE_WEAPON = 0x822a04;

	void* entity_ptr_;
};

