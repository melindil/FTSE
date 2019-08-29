#pragma once
#include "Entity.h"
#include "ActorStatus.h"
class Breakable :
	public Entity
{
public:
	Breakable(uint16_t id);
	Breakable(void* ptr);
	virtual ~Breakable();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static void SetLuaSubclass(lua_State* l);
	static const uint32_t VTABLE = 0x80e3ac;

	int32_t GetAttribute(std::string const& name);
	int32_t GetHP();
	bool isEtherealWhenDead();
	int32_t GetMinDamage();
	int32_t GetMaxDamage();
	std::string GetPrimaryDamageType();
	std::string GetSecondaryDamageType();
	std::string GetAmmoVariant();
	float GetDamageRadius();
	bool IsMetal();

private:
#pragma pack(push, 1)
	typedef struct
	{
		char attributes[0x339];
		ActorStatus actorstatus;
		bool etherealwhendead;
		int32_t mindamage;
		int32_t maxdamage;
		wchar_t* primarydamagetype;
		wchar_t* secondarydamagetype;
		wchar_t* ammovariant;
		float damageradius;
		bool ismetal;
	} BreakableStructType;
#pragma pack(pop)

	BreakableStructType* GetStruct();
	static const uint32_t OFFSET_BREAKABLE_STRUCT = 0x1a5;


};

