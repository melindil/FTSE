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
#include "GenericItem.h"
#include "SpawnPoint.h"
#include "Keys.h"
#include "Radiation.h"
#include "Armour.h"
#include "Book.h"
#include "Holodisk.h"
#include "Lockpick.h"
#include "SkillConsumable.h"
#include "VehicleController.h"
#include "Alarm.h"
#include "Switch.h"
#include "ScienceSwitch.h"
#include "Geiger.h"
#include "Sneak.h"
#include "TrapTrigger.h"
#include "Container.h"
#include "Light.h"
#include "Scenary.h"
#include "ScenaryMove.h"
#include "ScenarySpawn.h"
#include "StateScenary.h"
#include "Deathtrap.h"
#include "Powernode.h"
#include "RepairObject.h"
#include "CaptureInvItem.h"
#include "CaptureItem.h"
#include "Door.h"
#include "RotatingDoor.h"
#include "EffectSpawn.h"
#include "Waypoint.h"
#include "SentryAI.h"
#include "CombatFX.h"
#include "BaseAI.h"
#include "Controller.h"
#include "Consumable.h"
#include "Inventory.h"
#include "Vehicle.h"

#include "EntityVtable.h"

static Logger* logger_;

// static
std::shared_ptr<EntityVtable> Entity::entity_vtable_;


Entity::Entity(void* ptr)
	: entity_ptr_(ptr)
{
}

Entity::Entity(EntityID id)
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
std::string Entity::GetEntityInternalName()
{
	return Helpers::WcharToUTF8(((EntityHeaderType*)entity_ptr_)->internalname);

}

void Entity::SetColor(int coloridx, RGBSAColor & c)
{
	char* ptr = ((EntityHeaderType*)entity_ptr_)->basecolor;
	memcpy(ptr + (coloridx * sizeof(RGBSAColor)), &c, sizeof(RGBSAColor));
}

void Entity::RefreshSprite()
{
	auto fxn = (void(__thiscall *)(void*,wchar_t**))(GetVtableFxn(VTABLE_OFFSET_REFRESHSPRITE));
	wchar_t** spritenameptr = &((EntityHeaderType*)entity_ptr_)->spritename;
	return (*fxn)(GetEntityPointer(),spritenameptr);

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

int Entity::CallVtable(lua_State* l)
{
	
	size_t vtable_address = GetVtable();

	if (lua_isinteger(l, 2))
	{
		int idx = (int)lua_tointeger(l, 2);
		auto fxn = entity_vtable_->GetVtableCallTemplateByIndex(idx);
		return (*fxn)(GetEntityPointer(), vtable_address, l);
	}

	return 0;
}

int Entity::CallOrigVtable(lua_State* l)
{

	size_t vtable_address = entity_vtable_->GetOrigVtableAddrForClass(l);;

	if (lua_isinteger(l, 2))
	{
		int idx = (int)lua_tointeger(l, 2);
		auto fxn = entity_vtable_->GetVtableCallTemplateByIndex(idx);
		return (*fxn)(GetEntityPointer(), vtable_address, l);
	}

	return 0;
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

std::shared_ptr<Entity> Entity::GetEntityByID(EntityID id)
{
	return GetEntityByPointer(World::GetEntity(id));
}
std::shared_ptr<Entity> Entity::GetEntityByIDBase(uint16_t id)
{
	return GetEntityByPointer(World::GetEntityBaseID(id));
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
	case GenericItem::VTABLE:
		return std::make_shared<GenericItem>(ptr);
	case SpawnPoint::VTABLE:
		return std::make_shared<SpawnPoint>(ptr);
	case Keys::VTABLE:
		return std::make_shared<Keys>(ptr);
	case Radiation::VTABLE:
		return std::make_shared<Radiation>(ptr);
	case Armour::VTABLE:
		return std::make_shared<Armour>(ptr);
	case Book::VTABLE:
		return std::make_shared<Book>(ptr);
	case Holodisk::VTABLE:
		return std::make_shared<Holodisk>(ptr);
	case Lockpick::VTABLE:
		return std::make_shared<Lockpick>(ptr);
	case SkillConsumable::VTABLE:
		return std::make_shared<SkillConsumable>(ptr);
	case VehicleController::VTABLE:
		return std::make_shared<VehicleController>(ptr);
	case Alarm::VTABLE:
		return std::make_shared<Alarm>(ptr);
	case Switch::VTABLE:
		return std::make_shared<Switch>(ptr);
	case ScienceSwitch::VTABLE:
		return std::make_shared<ScienceSwitch>(ptr);
	case Geiger::VTABLE:
		return std::make_shared<Geiger>(ptr);
	case Sneak::VTABLE:
		return std::make_shared<Sneak>(ptr);
	case TrapTrigger::VTABLE:
		return std::make_shared<TrapTrigger>(ptr);
	case Container::VTABLE:
		return std::make_shared<Container>(ptr);
	case Light::VTABLE:
		return std::make_shared<Light>(ptr);
	case Scenary::VTABLE:
		return std::make_shared<Scenary>(ptr);
	case ScenaryMove::VTABLE:
		return std::make_shared<ScenaryMove>(ptr);
	case ScenarySpawn::VTABLE:
		return std::make_shared<ScenarySpawn>(ptr);
	case StateScenary::VTABLE:
		return std::make_shared<StateScenary>(ptr);
	case Deathtrap::VTABLE:
		return std::make_shared<Deathtrap>(ptr);
	case Powernode::VTABLE:
		return std::make_shared<Powernode>(ptr);
	case RepairObject::VTABLE:
		return std::make_shared<RepairObject>(ptr);
	case CaptureInvItem::VTABLE:
		return std::make_shared<CaptureInvItem>(ptr);
	case CaptureItem::VTABLE:
		return std::make_shared<CaptureItem>(ptr);
	case Door::VTABLE:
		return std::make_shared<Door>(ptr);
	case RotatingDoor::VTABLE:
		return std::make_shared<RotatingDoor>(ptr);
	case EffectSpawn::VTABLE:
		return std::make_shared<EffectSpawn>(ptr);
	case Waypoint::VTABLE:
		return std::make_shared<Waypoint>(ptr);
	case SentryAI::VTABLE:
		return std::make_shared<SentryAI>(ptr);
	case CombatFX::VTABLE:
		return std::make_shared<CombatFX>(ptr);
	case BaseAI::VTABLE:
		return std::make_shared<BaseAI>(ptr);
	case Controller::VTABLE:
		return std::make_shared<Controller>(ptr);
	case Consumable::VTABLE:
		return std::make_shared<Consumable>(ptr);
	case Inventory::VTABLE:
		return std::make_shared<Inventory>(ptr);
	case Vehicle::VTABLE:
		return std::make_shared<Vehicle>(ptr);

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
	return fxn(GetEntityPointer(), holder.GetID());
}
int32_t Entity::GetMaxEffectiveDamage(Entity& holder)
{
	auto fxn = (int32_t(__thiscall*)(void*, uint32_t))this->GetVtableFxn(VTABLE_OFFSET_MAXDAMAGE);
	return fxn(GetEntityPointer(), holder.GetID());
}

std::pair<InventoryActionResult, std::shared_ptr<Entity>> Entity::AddToInventory(std::shared_ptr<Entity> item, int32_t count)
{
	InventoryActionResult res;
	EntityID id = item->GetID();

	auto fxn = (void* (__thiscall*)(void*, InventoryActionResult*, EntityID, EntityID*, int32_t, int32_t))(Entity::GetVtableFxn(VTABLE_OFFSET_ADDINVENTORY));
	fxn(GetEntityPointer(), &res, id, &id, count, 0);

	return std::pair<InventoryActionResult, shared_ptr<Entity>>(res, Entity::GetEntityByID(id));
}

std::pair<InventoryActionResult, std::shared_ptr<Entity>> Entity::RemoveInventory(std::shared_ptr<Entity> item, int32_t count)
{
	InventoryActionResult res;
	EntityID id = item->GetID();

	auto fxn = (void* (__thiscall*)(void*, InventoryActionResult*, EntityID, EntityID*, int32_t, int32_t))(Entity::GetVtableFxn(VTABLE_OFFSET_REMOVEINVENTORY));
	fxn(GetEntityPointer(), &res, id, &id, count, 0);

	return std::pair<InventoryActionResult, shared_ptr<Entity>>(res, Entity::GetEntityByID(id));
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

int l_entity_addtoinventory(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1));
	auto i = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 2));
	auto coll = std::dynamic_pointer_cast<Collectable>(i);
	int32_t count = 1;
	if (coll)
	{
		count = coll->GetCount();
		if (lua_isinteger(l, 3))
		{
			int32_t desiredcount = (int32_t)lua_tointeger(l, 3);
			count = (count > desiredcount) ? desiredcount : count;
		}
	}
	auto result = e->AddToInventory(i, count);
	if (result.first.status != 0)
	{
		lua_pushstring(l, Helpers::WcharToUTF8(result.first.errstring).c_str());
		result.second->MakeLuaObject(l);
		return 2;
	}
	return 0;

}

int l_entity_removeinventory(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1));
	auto i = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 2));
	auto coll = std::dynamic_pointer_cast<Collectable>(i);
	int32_t count = 1;
	if (coll)
	{
		count = coll->GetCount();
		if (lua_isinteger(l, 3))
		{
			int32_t desiredcount = (int32_t)lua_tointeger(l, 3);
			count = (count > desiredcount) ? desiredcount : count;
		}
	}
	auto result = e->RemoveInventory(i, count);
	if (result.first.status != 0)
	{
		lua_pushstring(l, Helpers::WcharToUTF8(result.first.errstring).c_str());
	}
	else
	{
		result.second->MakeLuaObject(l);
	}
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

int l_entity_destruct(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l));
	if (e && e->GetBaseID() != 0)
	{
		e->Destruct();
	}
	return 0;
}

int l_callvtable(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l));
	if (e)
	{
		return e->CallVtable(l);
	}
	return 0;
}

int l_callorigvtable(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l));
	if (e)
	{
		return e->CallOrigVtable(l);
	}
	return 0;
}

int l_setcolor(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l));
	if (e)
	{
		int coloridx = (int)lua_tointeger(l, 2);
		RGBSAColor clr;
		if (lua_istable(l, 3))
		{
			clr.v[0] = LuaHelper::GetTableFloat(l, 3, "r");
			clr.v[1] = LuaHelper::GetTableFloat(l, 3, "g");
			clr.v[2] = LuaHelper::GetTableFloat(l, 3, "b");
			clr.v[3] = LuaHelper::GetTableFloat(l, 3, "s");
			clr.v[4] = LuaHelper::GetTableFloat(l, 3, "a");
		}
		e->SetColor(coloridx, clr);
	}
	return 0;
}
int l_refreshsprite(lua_State* l)
{
	auto e = Entity::GetEntityByID(LuaHelper::GetEntityId(l));
	if (e)
	{
		e->RefreshSprite();
	}
	return 0;
}

void Entity::SetLuaSubclass(lua_State * l)
{
	lua_pushboolean(l, true);
	lua_setfield(l, -2, "isEntity");

	lua_pushcfunction(l, (LuaHelper::THUNK<Entity, &Entity::GetTag>()));
	lua_setfield(l, -2, "GetTag");
	lua_pushcfunction(l, (LuaHelper::THUNK<Entity, &Entity::GetEntityName>()));
	lua_setfield(l, -2, "GetName");
	lua_pushcfunction(l, (LuaHelper::THUNK<Entity, &Entity::GetEntityInternalName>()));
	lua_setfield(l, -2, "GetInternalName");
	lua_pushcfunction(l, (LuaHelper::THUNK<Entity, &Entity::GetLocation>()));
	lua_setfield(l, -2, "GetPosition");
	lua_pushcfunction(l, (LuaHelper::THUNK<Entity, &Entity::GetBaseID>()));
	lua_setfield(l, -2, "GetBaseID");
	lua_pushcfunction(l, l_entity_getmaxrange);
	lua_setfield(l, -2, "GetMaxRange");
	lua_pushcfunction(l, l_entity_getmindamage);
	lua_setfield(l, -2, "GetMinEffectiveDamage");
	lua_pushcfunction(l, l_entity_getmaxdamage);
	lua_setfield(l, -2, "GetMaxEffectiveDamage");
	lua_pushcfunction(l, (LuaHelper::THUNK<Entity, &Entity::GetEntitySubType>()));
	lua_setfield(l, -2, "GetEntitySubType");
	lua_pushcfunction(l, l_entity_addtoinventory);
	lua_setfield(l, -2, "AddToInventory");
	lua_pushcfunction(l, l_entity_removeinventory);
	lua_setfield(l, -2, "RemoveInventory");
	lua_pushcfunction(l, l_entity_destruct);
	lua_setfield(l, -2, "Destruct");
	lua_pushcfunction(l, l_setcolor);
	lua_setfield(l, -2, "SetColor");
	lua_pushcfunction(l, l_refreshsprite);
	lua_setfield(l, -2, "RefreshSprite");
	lua_pushcfunction(l, l_callvtable);
	lua_setfield(l, -2, "CallVtable");
	lua_pushcfunction(l, l_callorigvtable);
	lua_setfield(l, -2, "CallOrigVtable");

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
	return fxn(GetEntityPointer(), holder.GetID());
}

void Entity::Destruct()
{
	auto fxn = (void(__thiscall*)(void*))this->GetVtableFxn(VTABLE_OFFSET_DESTRUCT);
	fxn(GetEntityPointer());
}

// static
void Entity::RegisterEntityVtable(std::shared_ptr<EntityVtable> vt)
{
	entity_vtable_ = vt;
}