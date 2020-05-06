#pragma once
#include "Usable.h"

class Switch :
	public Usable
{
public:
	Switch(EntityID id);
	Switch(void* ptr);
	virtual ~Switch();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81ac64;
	bool IsOn();
	std::string GetKeyTagName();
	bool IsBrainExtractor();

	static void SetLuaSubclass(lua_State* l);

private:
#pragma pack (push,1)
	typedef struct
	{
		bool on;
		wchar_t* keytagname;
		bool brainextractor;
	} SwitchStructType;
#pragma pack(pop)
	SwitchStructType* GetStruct();
	static const uint32_t OFFSET_SWITCH_STRUCT = 0x1a6;
};

