#pragma once
#include "Collectable.h"
class Ammo :
	public Collectable
{
public:
	Ammo(EntityID id);
	Ammo(void* ptr);
	virtual ~Ammo();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81b4e8;
	std::string GetType();
	std::string GetVariant();
	std::string GetPrimaryDamageType();
	std::string GetSecondaryDamageType();
	std::string GetImpactFX();
	std::string GetImpactSoundFX();

private:
#pragma pack (push,1)
	typedef struct
	{
		wchar_t* type;
		wchar_t* variant;
		wchar_t* primarydamagetype;
		wchar_t* secondarydamagetype;
		wchar_t* impactfx;
		wchar_t* impactsoundfx;
	} AmmoStructType;
#pragma pack(pop)
	AmmoStructType* GetStruct();
	static const uint32_t OFFSET_AMMO_STRUCT = 0x830;
};

