#include "Weapon.h"
#include "Actor.h"

#include "LuaHelper.h"
#include "Helpers.h"

Weapon::Weapon(uint16_t id)
	: Collectable(id)
{
}

Weapon::Weapon(void* ptr)
	: Collectable(ptr)
{

}


Weapon::~Weapon()
{
}

shared_ptr<Entity> Weapon::GetEquippedAmmo()
{
	return Entity::GetEntityByID(GetWeaponStruct()->ammoentity);
}

int32_t Weapon::GetMagCapacity()
{
	return GetWeaponStruct()->magcapacity;
}

std::string Weapon::GetProjectileSprite()
{
	return Helpers::WcharToUTF8(GetWeaponStruct()->projectile);
}

std::string Weapon::GetAmmoType()
{
	return Helpers::WcharToUTF8(GetWeaponStruct()->ammotype);
}

std::string Weapon::GetDefaultAmmoVariant()
{
	return Helpers::WcharToUTF8(GetWeaponStruct()->defaultammovariant);
}

std::string Weapon::GetWeaponPerk()
{
	return Helpers::WcharToUTF8(GetWeaponStruct()->weaponperk);
}

std::string Weapon::GetPrimaryDamageType()
{
	return Helpers::WcharToUTF8(GetWeaponStruct()->primarydamagetype);
}

std::string Weapon::GetSecondaryDamageType()
{
	return Helpers::WcharToUTF8(GetWeaponStruct()->secondarydamagetype);
}

int32_t Weapon::GetMinBaseDamage()
{
	return GetWeaponStruct()->mindamage;
}

int32_t Weapon::GetMaxBaseDamage()
{
	return GetWeaponStruct()->maxdamage;
}

float Weapon::GetDamageRadius()
{
	return GetWeaponStruct()->damageradius;
}

Weapon::WeaponModeStructType& Weapon::GetWeaponModeStruct(int mode)
{
	return GetWeaponStruct()->mode[mode];
}
int32_t Weapon::GetSelectedModeID()
{
	return GetWeaponStruct()->selectedmode;
}

bool Weapon::GetCalledShotSelected()
{
	return GetWeaponStruct()->iscalledshot;
}

bool Weapon::GetDestroyUser()
{
	return GetWeaponStruct()->destroyuser;
}

std::string Weapon::GetSoundType()
{
	return Helpers::WcharToUTF8(GetWeaponStruct()->soundtype);
}

Weapon::WeaponStructType * Weapon::GetWeaponStruct()
{
	return (WeaponStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_WEAPON_STRUCT);
}

int l_weapon_getammo(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	e.GetEquippedAmmo()->MakeLuaObject(l);
	return 1;
}

int l_weapon_getmagcapacity(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMagCapacity());
	return 1;
}

int l_weapon_getmindamage(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMinBaseDamage());
	return 1;
}
int l_weapon_getmaxdamage(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetMaxBaseDamage());
	return 1;
}
int l_weapon_getselectedmode(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushinteger(l, e.GetSelectedModeID());
	return 1;

}

int l_weapon_getdamageradius(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushnumber(l, e.GetDamageRadius());
	return 1;
}

int l_weapon_iscalledshot(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.GetCalledShotSelected());
	return 1;
}
int l_weapon_destroyuser(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushboolean(l, e.GetDestroyUser());
	return 1;
}

int l_weapon_getprojectilesprite(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetProjectileSprite().c_str());
	return 1;
}
int l_weapon_getammotype(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetAmmoType().c_str());
	return 1;
}
int l_weapon_getdefaultammovariant(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetDefaultAmmoVariant().c_str());
	return 1;
}
int l_weapon_getweaponperk(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetWeaponPerk().c_str());
	return 1;
}
int l_weapon_getprimarydamagetype(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetPrimaryDamageType().c_str());
	return 1;
}
int l_weapon_getsecondarydamagetype(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetSecondaryDamageType().c_str());
	return 1;
}
int l_weapon_getsoundtype(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	lua_pushstring(l, e.GetSoundType().c_str());
	return 1;
}

int l_weapon_getmode(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	e.MakeModeLuaObject(l, (int)lua_tointeger(l, 2));
	return 1;
}

int l_weapon_getcurrentmode(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	e.MakeModeLuaObject(l, e.GetSelectedModeID());
	return 1;
}

int l_weaponmode_getname(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.modename).c_str());
	return 1;
}

int l_weaponmode_isvalid(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushboolean(l, wms.enabled);
	return 1;
}

int l_weaponmode_cancallshot(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushboolean(l, wms.cancallshot);
	return 1;
}

int l_weaponmode_hasprojectile(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushboolean(l, wms.hasprojectile);
	return 1;
}

int l_weaponmode_destroyonuse(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushboolean(l, wms.destroyonuse);
	return 1;
}

int l_weaponmode_getweaponskill(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.weaponskill).c_str());
	return 1;
}

int l_weaponmode_getanimation(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.animation).c_str());
	return 1;
}

int l_weaponmode_getrangetype(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.rangetype).c_str());
	return 1;
}

int l_weaponmode_getbasemaxrange(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushnumber(l, wms.maxrange);
	return 1;
}

int l_weaponmode_getspreadtype(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.spreadtype).c_str());
	return 1;
}

int l_weaponmode_getactioncosttype(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.actioncosttype).c_str());
	return 1;
}

int l_weaponmode_getimpactfx(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.impactfx).c_str());
	return 1;
}

int l_weaponmode_getsoundfx(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.soundfx).c_str());
	return 1;
}

int l_weaponmode_getlightfx(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushstring(l, Helpers::WcharToUTF8(wms.lightfx).c_str());
	return 1;
}

int l_weaponmode_getaccuracybonus(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushinteger(l, wms.accuracybonus);
	return 1;
}

int l_weaponmode_getammousage(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushinteger(l, wms.ammousage);
	return 1;
}

int l_weaponmode_getcriticalbonus(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushinteger(l, wms.critbonus);
	return 1;
}

int l_weaponmode_getdamagemultiplier(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushnumber(l, wms.damagemult);
	return 1;
}

int l_weaponmode_getknockovermultiplier(lua_State* l)
{
	Weapon e(LuaHelper::GetEntityId(l));
	auto wms = e.GetWeaponModeStruct(LuaHelper::GetTableInteger(l, 1, "mode"));
	lua_pushnumber(l, wms.knockovermult);
	return 1;
}

void Weapon::SetLuaSubclass(lua_State* l)
{
	Collectable::SetLuaSubclass(l);
	lua_pushcfunction(l, l_weapon_getammo);
	lua_setfield(l, -2, "GetEquippedAmmo");
	lua_pushcfunction(l, l_weapon_getmagcapacity);
	lua_setfield(l, -2, "GetMagCapacity");
	lua_pushcfunction(l, l_weapon_getmindamage);
	lua_setfield(l, -2, "GetMinBaseDamage");
	lua_pushcfunction(l, l_weapon_getmaxdamage);
	lua_setfield(l, -2, "GetMaxBaseDamage");
	lua_pushcfunction(l, l_weapon_getselectedmode);
	lua_setfield(l, -2, "GetSelectedModeID");
	lua_pushcfunction(l, l_weapon_getdamageradius);
	lua_setfield(l, -2, "GetDamageRadius");
	lua_pushcfunction(l, l_weapon_iscalledshot);
	lua_setfield(l, -2, "IsCalledShot");
	lua_pushcfunction(l, l_weapon_destroyuser);
	lua_setfield(l, -2, "DestroyUser");
	lua_pushcfunction(l, l_weapon_getprojectilesprite);
	lua_setfield(l, -2, "GetProjectileSprite");
	lua_pushcfunction(l, l_weapon_getammotype);
	lua_setfield(l, -2, "GetAmmoType");
	lua_pushcfunction(l, l_weapon_getdefaultammovariant);
	lua_setfield(l, -2, "GetDefaultAmmoVariant");
	lua_pushcfunction(l, l_weapon_getweaponperk);
	lua_setfield(l, -2, "GetWeaponPerk");
	lua_pushcfunction(l, l_weapon_getprimarydamagetype);
	lua_setfield(l, -2, "GetPrimaryDamageType");
	lua_pushcfunction(l, l_weapon_getsecondarydamagetype);
	lua_setfield(l, -2, "GetSecondaryDamageType");
	lua_pushcfunction(l, l_weapon_getsoundtype);
	lua_setfield(l, -2, "GetSoundType");
	lua_pushcfunction(l, l_weapon_getmode);
	lua_setfield(l, -2, "GetMode");
	lua_pushcfunction(l, l_weapon_getcurrentmode);
	lua_setfield(l, -2, "GetCurrentMode");
}
void Weapon::RegisterLua(lua_State* l, Logger*)
{
	luaL_newmetatable(l, "WeaponMetaTable");

	SetLuaSubclass(l);

	lua_pushstring(l, "Weapon");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "WeaponMetaTable");

	// Also make the metatable for weapon mode
	luaL_newmetatable(l, "WeaponModeMetaTable");
	lua_pushstring(l, "WeaponMode");
	lua_setfield(l, -2, "ClassType");
	
	lua_pushcfunction(l, l_weaponmode_getname);
	lua_setfield(l, -2, "GetName");
	lua_pushcfunction(l, l_weaponmode_isvalid);
	lua_setfield(l, -2, "IsValid");
	lua_pushcfunction(l, l_weaponmode_getweaponskill);
	lua_setfield(l, -2, "GetWeaponSkill");
	lua_pushcfunction(l, l_weaponmode_getanimation);
	lua_setfield(l, -2, "GetAnimation");
	lua_pushcfunction(l, l_weaponmode_getrangetype);
	lua_setfield(l, -2, "GetRangeType");
	lua_pushcfunction(l, l_weaponmode_getbasemaxrange);
	lua_setfield(l, -2, "GetBaseMaxRange");
	lua_pushcfunction(l, l_weaponmode_getspreadtype);
	lua_setfield(l, -2, "GetSpreadType");
	lua_pushcfunction(l, l_weaponmode_getaccuracybonus);
	lua_setfield(l, -2, "GetAccuracyBonus");
	lua_pushcfunction(l, l_weaponmode_getdamagemultiplier);
	lua_setfield(l, -2, "GetDamageMultiplier");
	lua_pushcfunction(l, l_weaponmode_getammousage);
	lua_setfield(l, -2, "GetAmmoUsage");
	lua_pushcfunction(l, l_weaponmode_getactioncosttype);
	lua_setfield(l, -2, "GetActionCostType");
	lua_pushcfunction(l, l_weaponmode_getimpactfx);
	lua_setfield(l, -2, "GetImpactFX");
	lua_pushcfunction(l, l_weaponmode_getsoundfx);
	lua_setfield(l, -2, "GetSoundFX");
	lua_pushcfunction(l, l_weaponmode_getlightfx);
	lua_setfield(l, -2, "GetLightFX");
	lua_pushcfunction(l, l_weaponmode_cancallshot);
	lua_setfield(l, -2, "CanCallShot");
	lua_pushcfunction(l, l_weaponmode_hasprojectile);
	lua_setfield(l, -2, "HasProjectile");
	lua_pushcfunction(l, l_weaponmode_destroyonuse);
	lua_setfield(l, -2, "DestroyOnUse");
	lua_pushcfunction(l, l_weaponmode_getknockovermultiplier);
	lua_setfield(l, -2, "GetKnockoverMultiplier");
	lua_pushcfunction(l, l_weaponmode_getcriticalbonus);
	lua_setfield(l, -2, "GetCriticalBonus");


	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "WeaponModeMetaTable");

}

void Weapon::MakeLuaObject(lua_State* l)
{
	// Represent an entity as a table containing name and ID
	// Can add other elements here for quick access (if needed)
	// Otherwise, use the entity ID as the identifier to re-obtain
	// the pointer from the entity table if we have to make any
	// changes to it.

	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "WeaponMetaTable");
	lua_setmetatable(l, -2);

}

void Weapon::MakeModeLuaObject(lua_State* l,int mode)
{
	// Represent an entity as a table containing name and ID
	// Can add other elements here for quick access (if needed)
	// Otherwise, use the entity ID as the identifier to re-obtain
	// the pointer from the entity table if we have to make any
	// changes to it.

	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_pushinteger(l, mode);
	lua_setfield(l, -2, "mode");
	lua_getglobal(l, "WeaponModeMetaTable");
	lua_setmetatable(l, -2);

}

