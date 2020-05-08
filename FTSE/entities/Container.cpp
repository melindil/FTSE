#include "Container.h"
#include "Inventory.h"

#include "LuaHelper.h"
#include "Helpers.h"

Container::Container(EntityID id)
	: Usable(id)
{
}

Container::Container(void * ptr)
	: Usable(ptr)
{
}

Container::~Container()
{
}

void Container::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "ContainerMetaTable");
	lua_setmetatable(l, -2);
}

Container::ContainerStructType * Container::GetStruct()
{
	return (ContainerStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_CONTAINER_STRUCT);
}

int l_container_inventorylist(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Container>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		auto inv_entity = std::dynamic_pointer_cast<Inventory>(entity->GetInventory());
		if (inv_entity)
		{
			lua_newtable(l);
			auto items = inv_entity->GetItemList();
			int i = 1;
			for (auto item : items)
			{
				item->MakeLuaObject(l);
				lua_rawseti(l, -2, i);
				i++;
			}
		}
		else
		{
			lua_pushnil(l);
		}
	}
	else
	{
		lua_pushnil(l);
	}
	return 1;
}

void Container::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "ContainerMetaTable");
	Usable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Container, &Container::GetMaxWeight>()));
	lua_setfield(l, -2, "GetMaxWeight");
	lua_pushcfunction(l, (LuaHelper::THUNK<Container, &Container::GetUnlockChance>()));
	lua_setfield(l, -2, "GetUnlockChance");
	lua_pushcfunction(l, l_container_inventorylist);
	lua_setfield(l, -2, "GetInventory");
	lua_pushcfunction(l, (LuaHelper::THUNK<Container, &Container::IsOpen>()));
	lua_setfield(l, -2, "IsOpen");
	lua_pushcfunction(l, (LuaHelper::THUNK<Container, &Container::AlwaysOpen>()));
	lua_setfield(l, -2, "AlwaysOpen");
	lua_pushcfunction(l, (LuaHelper::THUNK<Container, &Container::IsLocked>()));
	lua_setfield(l, -2, "IsLocked");
	lua_pushcfunction(l, (LuaHelper::THUNK<Container, &Container::IsLockable>()));
	lua_setfield(l, -2, "IsLockable");
	lua_pushcfunction(l, (LuaHelper::THUNK<Container, &Container::GetAction>()));
	lua_setfield(l, -2, "GetAction");
	lua_pushcfunction(l, (LuaHelper::THUNK<Container, &Container::GetSound>()));
	lua_setfield(l, -2, "GetSound");

	lua_pushstring(l, "Container");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "ContainerMetaTable");
}


int32_t Container::GetMaxWeight()
{
	return GetStruct()->maxweight;
}

std::shared_ptr<Entity> Container::GetInventory()
{
	return Entity::GetEntityByID(GetStruct()->inventory);
}

std::string Container::GetAction()
{
	return Helpers::WcharToUTF8(GetStruct()->action);
}

bool Container::IsOpen()
{
	return GetStruct()->isopen;
}

bool Container::AlwaysOpen()
{
	return GetStruct()->alwaysopen;
}

bool Container::IsLocked()
{
	return GetStruct()->islocked;
}

bool Container::IsLockable()
{
	return GetStruct()->islockable;
}

int32_t Container::GetUnlockChance()
{
	return GetStruct()->unlockchance;
}

std::string Container::GetSound()
{
	return Helpers::WcharToUTF8(GetStruct()->sound);
}
