#include "Entity.h"
#include "World.h"
#include "Helpers.h"
#include <Windows.h>

#include "Actor.h"
#include "Weapon.h"

static Logger* logger_;

Entity::Entity(void* ptr)
	: entity_id_(*((uint16_t*)(((char*)ptr) + OFFSET_ENTITY_ID)))
{
}

Entity::Entity(uint16_t id)
	: entity_id_(id)
{

}

Entity::~Entity()
{
}

uint32_t Entity::GetVtable()
{
	return *(uint32_t*)(GetEntityPointer());
}

uint32_t Entity::GetVtableFxn(uint32_t offset)
{
	uint32_t* ptr = (uint32_t*)(GetVtable() + offset);
	return *ptr;
}
std::string Entity::GetEntityName()
{
	void* entity = GetEntityPointer();
	WCHAR* wcharname;

	auto fxn = (void(__thiscall*)(void*, wchar_t**, void* ))FXN_ACTOR_GETNAME;
	fxn((void*)OBJECT_ACTOR_GETNAME, &wcharname, entity);


	// We need to decrement the usage counter for the name string, or it might leak
	DWORD* obj = (DWORD*)wcharname;
	obj[-3]--;

	return Helpers::WcharToUTF8(wcharname);

}

bool Entity::isAlive()
{
	uint32_t* vtable = *(uint32_t**)GetEntityPointer();

	uint32_t fxnaddr = vtable[0x120];
	auto fxn = (bool(__thiscall *)(void*))(fxnaddr);
	return (*fxn)(GetEntityPointer());
}

void Entity::ShotAtMissed(void* cmsg)
{
	uint32_t* vtable = *(uint32_t**)GetEntityPointer();

	uint32_t fxnaddr = vtable[0x12b];
	auto fxn = (bool(__thiscall *)(void*, void*))(fxnaddr);
	(*fxn)(GetEntityPointer(),cmsg);
}

uint16_t Entity::GetFlags()
{
	return *(uint16_t*)((char*)GetEntityPointer() + 0x146);
}

Vector3 Entity::GetLocation()
{
	return Vector3((float*)((char*)GetEntityPointer() + 0x9a));
}

void Entity::MakeLuaObject(lua_State* l)
{
	// Represent an entity as a table containing name and ID
	// Can add other elements here for quick access (if needed)
	// Otherwise, use the entity ID as the identifier to re-obtain
	// the pointer from the entity table if we have to make any
	// changes to it.

	std::string name = GetEntityName();
	lua_newtable(l);
	lua_pushstring(l, name.c_str());
	lua_setfield(l, -2, "name");
	lua_pushinteger(l, entity_id_);
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "EntityMetaTable");
	lua_setmetatable(l, -2);

}

std::shared_ptr<Entity> Entity::GetEntityByID(uint16_t id)
{
	void* ptr = World::GetEntity(id);
	int vtable;
	memcpy(&vtable, ptr, sizeof(int));
	switch (vtable)
	{
	case VTABLE_ACTOR:
		return std::make_shared<Actor>(id);

	case VTABLE_WEAPON:
		return std::make_shared<Weapon>(id);

	default:
		return std::make_shared<Entity>(id);
	}
}

void Entity::RegisterLua(lua_State* l, Logger* tmp)
{
	logger_ = tmp;
	luaL_newmetatable(l, "EntityMetaTable");


	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "EntityMetaTable");
}