#include "Inventory.h"

#include "LuaHelper.h"

Inventory::Inventory(EntityID id)
	: Entity(id)
{
}

Inventory::Inventory(void * ptr)
	: Entity(ptr)
{
}

Inventory::~Inventory()
{
}

void Inventory::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "InventoryMetaTable");
	lua_setmetatable(l, -2);
}

int l_inventory_itemlist(lua_State* l)
{
	auto entity = std::dynamic_pointer_cast<Inventory>(Entity::GetEntityByID(LuaHelper::GetEntityId(l)));
	if (entity)
	{
		lua_newtable(l);
		auto items = entity->GetItemList();
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
	return 1;
}

void Inventory::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "InventoryMetaTable");
	Entity::SetLuaSubclass(l);

	lua_pushcfunction(l, l_inventory_itemlist);
	lua_setfield(l, -2, "GetItemList");
	lua_pushcfunction(l, (LuaHelper::THUNK<Inventory, &Inventory::GetNumItems>()));
	lua_setfield(l, -2, "GetNumItems");

	lua_pushstring(l, "Inventory");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "InventoryMetaTable");

}

std::vector<std::shared_ptr<Entity> > Inventory::GetItemList()
{
	std::vector<std::shared_ptr<Entity>> ret;
	auto iter = GetStruct()->itemlist->next;
	while (iter != GetStruct()->itemlist)
	{
		if (iter->entity_id.id != 0)
			ret.push_back(Entity::GetEntityByID(iter->entity_id));
		iter = iter->next;
	}
	return ret;
}
int32_t Inventory::GetNumItems()
{
	return GetStruct()->numitems;
}

Inventory::InventoryStructType * Inventory::GetStruct()
{
	return (InventoryStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_INVENTORY_STRUCT);
}

void Inventory::Validate()
{
	void* entity = GetEntityPointer();
	auto fxn2 = (void(__thiscall*)(void*))FXN_VALIDATE_INVENTORY;
	(*fxn2)(entity);
	
}