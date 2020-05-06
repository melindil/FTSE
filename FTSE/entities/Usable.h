#pragma once
#include "Entity.h"
struct lua_State;

class Usable :
	public Entity
{
public:
	Usable(void* ptr);
	Usable(EntityID id);
	~Usable();

	bool IsReusable();
	static void SetLuaSubclass(lua_State* l);

protected:
#pragma pack(push,1)
	typedef struct {
		char reusable;
	} UsableStructType;
#pragma pack(pop)

	UsableStructType* GetUsableStruct();

	static const int OFFSET_STRUCT = 0x1a5;

};
