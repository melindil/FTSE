#pragma once
#include "Collectable.h"
class Holodisk :
	public Collectable
{
public:
	Holodisk(EntityID id);
	Holodisk(void* ptr);
	virtual ~Holodisk();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81e72c;
	std::string GetText();

private:
#pragma pack (push,1)
	typedef struct
	{
		wchar_t* text;
	} HolodiskStructType;
#pragma pack(pop)
	HolodiskStructType* GetStruct();
	static const uint32_t OFFSET_HOLODISK_STRUCT = 0x830;

};
