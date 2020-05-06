#pragma once
#include "Entity.h"

class SentryAI :
	public Entity
{
public:
	SentryAI(EntityID id);
	SentryAI(void* ptr);
	virtual ~SentryAI();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x80b870;

	float GetRepathTime();
	int32_t GetMode();
	int32_t GetAccuracy();
	int32_t GetAimLocation();
	std::shared_ptr<Entity> GetCurrentTarget();
	std::shared_ptr<Entity> GetUserTarget();

private:
	/*
1a5 repathtime float 1a9 unk float 1ad mode int
   1b1 accuracy int 1b5 location int 1b9 currenttarget ent
   1bd usertarget ent 	*/
#pragma pack (push,1)
	typedef struct
	{
		float repathtime;
		float unknown;
		int32_t mode;
		int32_t accuracy;
		int32_t location;
		EntityID currenttarget;
		EntityID usertarget;
	} SentryAIStructType;
#pragma pack(pop)
	SentryAIStructType* GetStruct();
	static const uint32_t OFFSET_SENTRYAI_STRUCT = 0x1a5;

};
