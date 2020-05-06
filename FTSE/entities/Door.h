#pragma once
#include "Usable.h"

class Door :
	public Usable
{
public:
	Door(EntityID id);
	Door(void* ptr);
	virtual ~Door();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x818290;

	bool IsOpen();
	bool IsOpenOutwards();
	bool IsLocked();
	bool IsLockable();
	int32_t GetChanceUnlock();
	bool IsAutoclose();
	int32_t GetTimeOpen();
	bool CanBreak();
	std::string GetSounds();
	bool IsSensorOpen();
	float GetSensorRange();
	bool IsTrapDoor();
	bool IsLaserDoor();

	static void SetLuaSubclass(lua_State* l);

private:
#pragma pack (push,1)
	typedef struct
	{
		bool isopen;
		bool openoutwards;
		bool islocked;
		bool islockable;
		int32_t chanceunlock;
		bool autoclose;
		int32_t timeopen;
		bool canbreak;
		wchar_t* sounds;
		bool sensoropen;
		float sensorrange;
		bool trapdoor;
		bool laserdoor;
	} DoorStructType;
#pragma pack(pop)
	DoorStructType* GetStruct();
	static const uint32_t OFFSET_DOOR_STRUCT = 0x1a6;
};
