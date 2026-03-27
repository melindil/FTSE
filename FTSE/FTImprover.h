#pragma once
#include <vector>
#include "lua.hpp"
#include "Logger.h"
#include <set>

class Entity;

#pragma pack (push, 1)

struct FTI_AmmoVariant
{
	char* name;
	float dmg_mult;
	float resist_mult;

	FTI_AmmoVariant(char* n, float d, float r) : name(n), dmg_mult(d), resist_mult(r) {}
};

struct FTI_APCT
{
	int purpose;
	int actiontype;
	char* name;
	int cost;

	FTI_APCT(int p, int a, char* n, int c) : purpose(p), actiontype(a), name(n), cost(c) {}
};

struct FTI_ArmourRule
{
	std::string armour;
	std::string sex;
	std::string race;
	std::string output;

	FTI_ArmourRule(std::string const& a, std::string const& s, std::string const& r, std::string const& o)
		: armour(a), sex(s), race(r), output(o) {}
};

struct FTI_GotoMissionStaticStruct;

struct FTI_GotoMissionAllocatedStruct
{
	uint8_t zerobyte;
	uint32_t one_int;
	uint64_t timestamp;
	FTI_GotoMissionStaticStruct* static_struct;
	size_t fxnptr;
	uint32_t zero_int;
	FTI_GotoMissionAllocatedStruct* self;
	void* orig_action_msg;
};

struct FTI_GotoMissionEventStruct
{
	FTI_GotoMissionStaticStruct* static_struct;
	size_t fxnptr;
	int32_t zerovalue;
	FTI_GotoMissionAllocatedStruct* alloc_struct;
	void* orig_action_msg;
};

struct FTI_GotoMissionLinkedList {
	FTI_GotoMissionLinkedList* next;
	FTI_GotoMissionLinkedList* prev;
	FTI_GotoMissionEventStruct* event;
};

struct FTI_GotoMissionStaticStruct {
	uint64_t unk_zero;
	FTI_GotoMissionLinkedList* mylist;
	uint32_t ctr;
	uint32_t flags_maybe;
	uint32_t safety_padding[64];

	void FTI_GotoMission_Called2(int32_t unk);
	void FTI_GotoMission_Called1();
};

#pragma pack(pop)



class FTImprover
{
public:
	FTImprover();
	~FTImprover();

	void RegisterLua(lua_State* l, Logger* logger);

	void Parse(std::vector<char> const& buf);
	void Emit(std::string & token);

	void ClearAmmoTypes();
	void AddAmmoType(std::string const& ammotype);
	void ClearImpactSounds();
	void AddImpactSound(std::string const& impactsound);
	void ClearAnimationNames();
	void AddAnimationName(std::string const& animationname);
	void ClearWeaponItemTypes();
	void AddWeaponItemType(std::string const& weapontype);
	void ClearWeaponSoundTypes();
	void AddWeaponSoundType(std::string const& weaponsound);
	void ClearAmmoVariants();
	void AddAmmoVariant(std::string const& ammovariant, float dmg, float resist);
	void ClearAPCT();
	void AddAPCT(std::string const& apctname, int purpose, int actiontype, int cost);
	void ClearArmourTypes();

	void EnableScriptTriggers();
	void EnableGotoMissionFix();
	void EnableMultiProcessFix();

	void UpdateMemoryLists();

	bool IsArmourReplaceEnabled();
	void GetArmourSpriteName(wchar_t** dest, wchar_t** sex, wchar_t** race, wchar_t** armour);
	void GetSoundPrefixName(wchar_t** dest, Entity* ent);

	void AddArmourType(std::string const& name);
	void AddArmourRule(std::string const& armour, std::string const& sex, std::string const& race, std::string const& output);
	void AddSoundRule(std::string const& armour, std::string const& sex, std::string const& race, std::string const& output);

	void DisableDefaultArmourTypes();
	void DisableDefaultArmourRules();

	bool IsFeatureEnabled(std::string const& feature);

	static FTI_GotoMissionStaticStruct static_struct;

private:
	
	// Functions for handling lists
	void UpdateAmmoTypes();
	void UpdateImpactSounds();
	void UpdateAnimationNames();
	void UpdateWeaponItemTypes();
	void UpdateWeaponSoundTypes();
	void UpdateAmmoVariants();
	void UpdateAPCT();
	void UpdateArmourTypes();
	void BuildDefaultArmourSoundRules();
	void BuildDefaultArmourTypes();
	
	void AddArmourTypeFTI(std::string const& name, std::string const& soundprefix, int32_t sex, std::string const& overrd);

	void Patch(size_t offset, size_t value, size_t len = 4);

	std::string BuildStringFromElements(std::string const& output, std::string const& armour, std::string const& sex, std::string const& race);

	void HandleFeatureFlag(std::string const& name, std::string const& value);

	// Parsing enums
	enum {
		OUTSIDE,
		INSIDE_CONFIG,
		INSIDE_1,
		INSIDE_2
	} state;
	
	enum {
		AMMOTYPES,
		AMMOVARIANTS,
		ANIMATIONNAMES,
		APCTSTRUCTURES,
		ARMOURTYPES,
		ATTACKMODENAMES,
		IMPACTSOUNDS,
		WEAPONITEMTYPES,
		WEAPONSOUNDTYPES,
		CONFIG
	} configtype;

	// Parsing temp storage
	bool waiting_for_param_name;
	std::string param_name;
	std::string param_value;
	int32_t param_on;
	int32_t int_param_1;
	int32_t int_param_2;
	std::string param_value_2;

	// Vector arrays for replacement of various lists
	std::vector<char*> ammo_types;
	bool ammo_types_active;

	std::vector<char*> impact_sounds;
	bool impact_sounds_active;

	std::vector<char*> animation_names;
	bool animation_names_active;

	std::vector<char*> weapon_item_types;
	bool weapon_item_types_active;

	std::vector<char*> weapon_sound_types;
	bool weapon_sound_types_active;

	std::vector<char*> attack_mode_names;
	bool attack_mode_names_active;

	std::vector<FTI_AmmoVariant> ammo_variants;
	bool ammo_variants_active;

	std::vector<FTI_APCT> apct;
	bool apct_active;

	std::vector<char*> armour_types;
	bool armour_types_active;
	bool include_default_armour_types;
	bool include_default_armour_rules;

	std::vector<FTI_ArmourRule> armour_rules;
	std::vector<FTI_ArmourRule> sound_rules;

	std::set<std::string> enabled_features;

	Logger* logger_;

	bool disabled;

};

