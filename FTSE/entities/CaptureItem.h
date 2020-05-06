#pragma once
#include "Entity.h"


class CaptureItem :
	public Entity
{
public:
	CaptureItem(EntityID id);
	CaptureItem(void* ptr);
	virtual ~CaptureItem();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x823ae0;

	int32_t GetCaptureTeam();
	std::shared_ptr<Entity> GetInventoryItem();

private:
	/*
	1a5 repeat bool 1a6 repeatinterval float
	1aa damagetype string 1ae damage int 1b2 animated bool
	1b3 scriptstate bool 1b4 nextactivatetime float
	*/
#pragma pack (push,1)
	typedef struct
	{
		int32_t captureteam;
		int32_t unknown1[3];
		EntityID captureitem;
	} CaptureItemStructType;
#pragma pack(pop)
	CaptureItemStructType* GetStruct();
	static const uint32_t OFFSET_CAPTUREITEM_STRUCT = 0x1a5;

};

