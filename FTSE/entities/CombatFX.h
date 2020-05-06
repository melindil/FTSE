#pragma once
#include "Entity.h"
#include "CombatMessage.h"

class CombatFX :
	public Entity
{
public:
	CombatFX(EntityID id);
	CombatFX(void* ptr);
	virtual ~CombatFX();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x80ec10;

	int32_t GetType();
	Vector3 GetAimpoint();
	int32_t GetNumFlames();
	int32_t GetLastFrame();

private:
	/*
1a5 type int 1a9 combatmessage
   1f8 x float 1fc y float 200 z float
   204 numflames int 208 lastframe int 	*/
#pragma pack (push,1)
	typedef struct
	{
		int32_t type;
		CombatMessage cmsg;
		float aimloc[3];
		int32_t numflames;
		int32_t lastframe;
	} CombatFXStructType;
#pragma pack(pop)
	CombatFXStructType* GetStruct();
	static const uint32_t OFFSET_COMBATFX_STRUCT = 0x1a5;

};
