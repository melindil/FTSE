#pragma once
#include "Usable.h"

class Container :
	public Usable
{
public:
	Container(EntityID id);
	Container(void* ptr);
	virtual ~Container();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x817a2c;

	int32_t GetMaxWeight();
	std::shared_ptr<Entity> GetInventory();
	std::string GetAction();
	bool IsOpen();
	bool AlwaysOpen();
	bool IsLocked();
	bool IsLockable();
	int32_t GetUnlockChance();
	std::string GetSound();

private:
#pragma pack (push,1)
	typedef struct
	{
		int32_t maxweight;
		EntityID inventory;
		int32_t unknown1;
		wchar_t* action;
		char unknown2;
		char isopen;
		char alwaysopen;
		char islocked;
		char islockable;
		int32_t unlockchance;
		wchar_t* sound;
	} ContainerStructType;
#pragma pack(pop)
	ContainerStructType* GetStruct();
	static const uint32_t OFFSET_CONTAINER_STRUCT = 0x1a6;
};
