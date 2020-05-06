#pragma once
#include "Collectable.h"
class Book :
	public Collectable
{
public:
	Book(EntityID id);
	Book(void* ptr);
	virtual ~Book();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81c5a0;
	std::string GetSkillAffected();

private:
#pragma pack (push,1)
	typedef struct
	{
		wchar_t* skill;
	} BookStructType;
#pragma pack(pop)
	BookStructType* GetStruct();
	static const uint32_t OFFSET_BOOK_STRUCT = 0x830;

};

