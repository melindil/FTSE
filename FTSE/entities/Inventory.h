#pragma once
#include "Entity.h"
#include <vector>

class Inventory :
	public Entity
{
public:
	Inventory(EntityID id);
	Inventory(void* ptr);
	virtual ~Inventory();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x818af8;

	struct InventoryFOTLinkedList
	{
		InventoryFOTLinkedList* next;
		InventoryFOTLinkedList* prev;
		EntityID entity_id;

	};

	std::vector<std::shared_ptr<Entity> > GetItemList();
	int32_t GetNumItems();
#pragma pack (push,1)
	typedef struct
	{
		char unk1;
		char unused[3];
		InventoryFOTLinkedList* itemlist;
		int32_t numitems;
	} InventoryStructType;
#pragma pack(pop)

private:
	/*
	1a5 unknown flag
	1a6 unused[3]
	1a9 linked list
	1ad size (int)
	*/

	InventoryStructType* GetStruct();
	static const uint32_t OFFSET_INVENTORY_STRUCT = 0x1a5;


};

