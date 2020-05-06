#pragma once
#include "Entity.h"

class Deathtrap :
	public Entity
{
public:
	Deathtrap(EntityID id);
	Deathtrap(void* ptr);
	virtual ~Deathtrap();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x80f46c;

	bool IsRepeat();
	float GetRepeatInterval();
	std::string GetDamageType();
	int32_t GetDamage();
	bool IsAnimated();
	bool IsScriptState();
	float GetNextActivateTime();

private:
	/*
	1a5 repeat bool 1a6 repeatinterval float
    1aa damagetype string 1ae damage int 1b2 animated bool 
    1b3 scriptstate bool 1b4 nextactivatetime float
	*/
#pragma pack (push,1)
	typedef struct
	{
		bool repeat;
		float repeatinterval;
		wchar_t* damagetype;
		int32_t damage;
		bool animated;
		bool scriptstate;
		float nextactivatetime;
	} DeathtrapStructType;
#pragma pack(pop)
	DeathtrapStructType* GetStruct();
	static const uint32_t OFFSET_DEATHTRAP_STRUCT = 0x1a5;

};


