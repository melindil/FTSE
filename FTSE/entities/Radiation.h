#pragma once
#include "Entity.h"
class Radiation :
	public Entity
{
public:
	Radiation(EntityID id);
	Radiation(void* ptr);
	virtual ~Radiation();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x8115e8;

	float GetRadius();
	float GetRadiationLevel();

private:
#pragma pack (push,1)
	typedef struct
	{
		float radius;
		float radiationlevel;
	} RadiationStructType;
#pragma pack(pop)
	RadiationStructType* GetStruct();
	static const uint32_t OFFSET_RADIATION_STRUCT = 0x1a5;
};

