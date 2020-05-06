#pragma once
#include "Collectable.h"
#include <stdint.h>
#include <memory>
using std::shared_ptr;

struct lua_State;
class Actor;
class Weapon :
	public Collectable
{
public:
	Weapon(EntityID id);
	Weapon(void* ptr);
	virtual ~Weapon();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x822a04;

	shared_ptr<Entity> GetEquippedAmmo();
	int32_t GetMagCapacity();
	std::string GetProjectileSprite();
	std::string GetAmmoType();
	std::string GetDefaultAmmoVariant();
	std::string GetWeaponPerk();
	std::string GetPrimaryDamageType();
	std::string GetSecondaryDamageType();
	int32_t GetMinBaseDamage();
	int32_t GetMaxBaseDamage();
	float GetDamageRadius();
	
	int32_t GetSelectedModeID();
	bool GetCalledShotSelected();
	bool GetDestroyUser();
	std::string GetSoundType();

	static void SetLuaSubclass(lua_State* l);

#pragma pack(push,1)
	typedef struct
	{
		uint32_t vtable;
		bool enabled;
		wchar_t* modename;
		wchar_t* weaponskill;
		wchar_t* animation;
		wchar_t* rangetype;
		float maxrange;
		wchar_t* spreadtype;
		int32_t  accuracybonus;
		float damagemult;
		int32_t ammousage;
		wchar_t* actioncosttype;
		bool cancallshot;
		int32_t critbonus;
		float knockovermult;
		wchar_t* impactfx;
		wchar_t* soundfx;
		bool hasprojectile;
		bool destroyonuse;
		wchar_t* lightfx;
	} WeaponModeStructType;

	typedef struct
	{
		EntityID ammoentity;
		int32_t magcapacity;
		wchar_t* projectile;
		wchar_t* ammotype;
		wchar_t* weaponperk;
		wchar_t* primarydamagetype;
		wchar_t* secondarydamagetype;
		wchar_t* defaultammovariant;
		char effectstable[0x339];
		int32_t mindamage;
		int32_t maxdamage;
		float damageradius;
		WeaponModeStructType mode[6];
		int32_t selectedmode;
		bool iscalledshot;
		wchar_t* soundtype;
		bool destroyuser;
	} WeaponStructType;
#pragma pack(pop)

	WeaponModeStructType& GetWeaponModeStruct(int mode);
	void MakeModeLuaObject(lua_State* l, int mode);
private:

	WeaponStructType* GetWeaponStruct();

	static const uint32_t OFFSET_WEAPON_STRUCT = 0x830;


};

