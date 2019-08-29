#include "Entity.h"
#include "World.h"
#include "Helpers.h"
#include "LuaHelper.h"

#include "Actor.h"
#include "Weapon.h"
#include "VehicleWeapon.h"
#include "Breakable.h"
#include "StateBreakable.h"
#include "Ammo.h"
#include "Trap.h"

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
	wchar_t* wcharname;

	auto fxn = (void(__thiscall*)(void*, wchar_t**, void* ))FXN_ENTITY_GETNAME;
	fxn((void*)OBJECT_ENTITY_GETNAME, &wcharname, entity_ptr_);


	// We need to decrement the usage counter for the name string, or it might leak
	uint32_t* obj = (uint32_t*)wcharname;
	obj[-3]--;

	return Helpers::WcharToUTF8(wcharname);

}

void Entity::DisplayMessage(std::string const& msg)
{
	void* entity = GetEntityPointer();
	auto fxn = (void(__thiscall*)(void*, uint32_t, uint32_t))FXN_ENTITY_SHOWMESSAGE;


	// Things work much better if we let FoT allocate the memory for the
	// message.  Note that there are three DWORDs before the message
	// content: A usage counter (which should start at 0 in this code
	// location), an entity size?, and a string length in chars
	wchar_t* convmsg = Helpers::UTF8ToWcharFOTHeap(msg, 0);
	fxn(entity, ((uint32_t)&convmsg), 0x8be1c8);

}


bool Entity::isAlive()
{

	auto fxn = (bool(__thiscall *)(void*))(GetVtableFxn(VTABLE_OFFSET_ISALIVE));
	return (*fxn)(GetEntityPointer());
}

void Entity::ShotAtMissed(void* cmsg)
{

	auto fxn = (bool(__thiscall *)(void*, void*))(GetVtableFxn(VTABLE_OFFSET_SHOTATMISSED));
	(*fxn)(GetEntityPointer(),cmsg);
}

uint16_t Entity::GetSeqnum()
{
	return ((EntityHeaderType*)entity_ptr_)->entityseqnum;
}

Vector3 Entity::GetLocation()
{
	return Vector3(((EntityHeaderType*)entity_ptr_)->pos);
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
	return GetEntityByPointer(World::GetEntity(id));
}
std::shared_ptr<Entity> Entity::GetEntityByPointer(void* ptr)

{
	uint32_t vtable = ((EntityHeaderType*)ptr)->vtable;
	switch (vtable)
	{
	case Actor::VTABLE:
		return std::make_shared<Actor>(ptr);

	case Weapon::VTABLE:
		return std::make_shared<Weapon>(ptr);

	case VehicleWeapon::VTABLE:
		return std::make_shared<VehicleWeapon>(ptr);

	case Ammo::VTABLE:
		return std::make_shared<Ammo>(ptr);

	case Trap::VTABLE:
		return std::make_shared<Trap>(ptr);

	case Breakable::VTABLE:
		return std::make_shared<Breakable>(ptr);

	case StateBreakable::VTABLE:
		return std::make_shared<StateBreakable>(ptr);

	default:
		return std::make_shared<Entity>(ptr);
	}
}
int l_entity_getname(lua_State* l);

std::string Entity::GetTag()
{
    wchar_t* tagptr =((EntityHeaderType*)entity_ptr_)->tag;
    return Helpers::WcharToUTF8(tagptr);
}

std::string Entity::GetEntitySubType()
{
	wchar_t* type = ((EntityHeaderType*)entity_ptr_)->subtype;
	return Helpers::WcharToUTF8(type);

}

int32_t Entity::GetMinEffectiveDamage(Entity& holder)
{
	auto fxn = (int32_t(__thiscall*)(void*, uint32_t))this->GetVtableFxn(VTABLE_OFFSET_MINDAMAGE);
	return fxn(GetEntityPointer(), (uint32_t)holder.GetID() << 16 | holder.GetSeqnum());
}
int32_t Entity::GetMaxEffectiveDamage(Entity& holder)
{
	auto fxn = (int32_t(__thiscall*)(void*, uint32_t))this->GetVtableFxn(VTABLE_OFFSET_MAXDAMAGE);
	return fxn(GetEntityPointer(), (uint32_t)holder.GetID() << 16 | holder.GetSeqnum());
}

void Entity::RegisterLua(lua_State* l, Logger* tmp)
{
	logger_ = tmp;
	luaL_newmetatable(l, "EntityMetaTable");

	SetLuaSubclass(l);

	lua_pushstring(l, "Entity");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "EntityMetaTable");
}

int l_entity_getposition(lua_State* l)
{
	Entity e(LuaHelper::GetEntityId(l));
	Vector3 loc = e.GetLocation();
    lua_newtable(l);
    lua_pushnumber(l, loc.v[0]);
    lua_setfield(l, -2, "x");
    lua_pushnumber(l, loc.v[1]);
    lua_setfield(l, -2, "y");
    lua_pushnumber(l, loc.v[2]);
    lua_setfield(l, -2, "z");
    return 1;
}

int l_entity_gettag(lua_State* l)
{
	Entity e(LuaHelper::GetEntityId(l));
	std::string tag = e.GetTag();
    lua_pushstring(l, tag.c_str());
    return 1;
}
int l_entity_getentitysubtype(lua_State* l)
{
	Entity e(LuaHelper::GetEntityId(l));
	std::string type = e.GetEntitySubType();
	lua_pushstring(l, type.c_str());
	return 1;
}

int l_entity_getname(lua_State* l)
{
	Entity e(LuaHelper::GetEntityId(l));
	std::string name = e.GetEntityName();
	lua_pushstring(l, name.c_str());
	return 1;
}

int l_entity_getmaxrange(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1));
	auto a = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 2));
	if (a)
	{
		float result = e->GetMaxRange(*a);
		lua_pushnumber(l, result);
	}
	else
	{
		lua_pushnumber(l, 0.0f);
	}
	return 1;
}

int l_entity_getmindamage(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1));
	auto a = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 2));
	if (a)
	{
		int32_t result = e->GetMinEffectiveDamage(*a);
		lua_pushinteger(l, result);
	}
	else
	{
		lua_pushinteger(l, 0);
	}
	return 1;
}
int l_entity_getmaxdamage(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1));
	auto a = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 2));
	if (a)
	{
		int32_t result = e->GetMaxEffectiveDamage(*a);
		lua_pushinteger(l, result);
	}
	else
	{
		lua_pushinteger(l, 0);
	}
	return 1;
}

void Entity::SetLuaSubclass(lua_State * l)
{
	lua_pushboolean(l, true);
	lua_setfield(l, -2, "isEntity");

	lua_pushcfunction(l, l_entity_gettag);
	lua_setfield(l, -2, "GetTag");
	lua_pushcfunction(l, l_entity_getname);
	lua_setfield(l, -2, "GetName");
	lua_pushcfunction(l, l_entity_getposition);
	lua_setfield(l, -2, "GetPosition");
	lua_pushcfunction(l, l_entity_getmaxrange);
	lua_setfield(l, -2, "GetMaxRange");
	lua_pushcfunction(l, l_entity_getmindamage);
	lua_setfield(l, -2, "GetMinEffectiveDamage");
	lua_pushcfunction(l, l_entity_getmaxdamage);
	lua_setfield(l, -2, "GetMaxEffectiveDamage");
	lua_pushcfunction(l, l_entity_getentitysubtype);
	lua_setfield(l, -2, "GetEntitySubType");
	
}

float Entity::GetBoundingBoxSum()
{
	uint32_t* base = ((EntityHeaderType*)entity_ptr_)->boundbox;
	int size = base[3] - base[0] + base[5] - base[2];
	return ((float)size) * 0.25f;
}

float Entity::GetHeight()
{
	uint32_t* base = ((EntityHeaderType*)entity_ptr_)->boundbox;
	int size = base[4] - base[1];
	return ((float)size) * 0.25f;
}

float Entity::GetMaxRange(Entity& holder)
{
	auto fxn = (float(__thiscall*)(void*, uint32_t))this->GetVtableFxn(VTABLE_OFFSET_GETRANGE);
	return fxn(GetEntityPointer(), (uint32_t)holder.GetID() << 16 | holder.GetSeqnum());
}
