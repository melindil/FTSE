#include "Entity.h"
#include "World.h"
#include "Helpers.h"
#include "LuaHelper.h"
#include <Windows.h>

#include "Actor.h"
#include "Weapon.h"

static Logger* logger_;

Entity::Entity(void* ptr)
	: entity_ptr_(ptr)
{
}

Entity::Entity(uint16_t id)
	: entity_ptr_(World::GetEntity(id))
{

}

Entity::~Entity()
{
}

uint32_t Entity::GetVtable()
{
	return ((EntityHeaderType*)entity_ptr_)->vtable;
}

uint32_t Entity::GetVtableFxn(uint32_t offset)
{
	uint32_t* ptr = (uint32_t*)(GetVtable() + offset);
	return *ptr;
}
std::string Entity::GetEntityName()
{
	WCHAR* wcharname;

	auto fxn = (void(__thiscall*)(void*, wchar_t**, void* ))FXN_ACTOR_GETNAME;
	fxn((void*)OBJECT_ACTOR_GETNAME, &wcharname, entity_ptr_);


	// We need to decrement the usage counter for the name string, or it might leak
	DWORD* obj = (DWORD*)wcharname;
	obj[-3]--;

	return Helpers::WcharToUTF8(wcharname);

}

bool Entity::isAlive()
{

	auto fxn = (bool(__thiscall *)(void*))(GetVtableFxn(0x480));
	return (*fxn)(GetEntityPointer());
}

void Entity::ShotAtMissed(void* cmsg)
{

	auto fxn = (bool(__thiscall *)(void*, void*))(GetVtableFxn(0x4ac));
	(*fxn)(GetEntityPointer(),cmsg);
}

uint16_t Entity::GetFlags()
{
	return *(uint16_t*)((char*)entity_ptr_ + 0x146);
}

Vector3 Entity::GetLocation()
{
	return Vector3((float*)((char*)entity_ptr_ + 0x9a));
}

void Entity::MakeLuaObject(lua_State* l)
{
	// Represent an entity as a table containing name and ID
	// Can add other elements here for quick access (if needed)
	// Otherwise, use the entity ID as the identifier to re-obtain
	// the pointer from the entity table if we have to make any
	// changes to it.

	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "EntityMetaTable");
	lua_setmetatable(l, -2);

}

std::shared_ptr<Entity> Entity::GetEntityByID(uint16_t id)
{
	void* ptr = World::GetEntity(id);
	uint32_t vtable = ((EntityHeaderType*)ptr)->vtable;
	switch (vtable)
	{
	case VTABLE_ACTOR:
		return std::make_shared<Actor>(ptr);

	case VTABLE_WEAPON:
		return std::make_shared<Weapon>(ptr);

	default:
		return std::make_shared<Entity>(ptr);
	}
}
int l_entity_getname(lua_State* l);

std::string Entity::GetTag()
{
    wchar_t* tagptr = GetEntityHeader()->tag;
    return Helpers.WcharToUTF8(tagptr);
}


void Entity::RegisterLua(lua_State* l, Logger* tmp)
{
	logger_ = tmp;
	luaL_newmetatable(l, "EntityMetaTable");

	lua_pushcfunction(l, l_entity_getname);
	lua_setfield(l, -2, "GetName");

	lua_pushstring(l, "Entity");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "EntityMetaTable");
}

int l_entity_getposition(lua_State* l)
{
    uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
    Entity e(id);
    Vector3 loc = e.GetPosition();
    lua_newtable(l);
    lua_pushnumber(l, loc.x);
    lua_setfield(l, -2, "x");
    lua_pushnumber(l, loc.y);
    lua_setfield(l, -2, "y");
    lua_pushnumber(l, loc.z);
    lua_setfield(l, -2, "z");
    return 1;
}

int l_entity_gettag(lua_State* l)
{
    uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
    Entity e(id);
    std::string tag = e.GetTag();
    lua_pushstring(l, tag.c_str());
    return 1;
}
}

int l_entity_getname(lua_State* l)
{
	uint16_t id = LuaHelper::GetTableInteger(l, 1, "id");
	Entity e(id);
	std::string name = e.GetEntityName();
	lua_pushstring(l, name.c_str());
	return 1;
}

void Entity::SetLuaSubclass(lua_State * l)
{
	lua_pushboolean(l, true);
	lua_setfield(l, -2, "isEntity");

	lua_pushcfunction(l, l_entity_getname);
	lua_setfield(l, -2, "GetName");

}
