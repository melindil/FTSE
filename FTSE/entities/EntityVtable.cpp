#include <memory>

#include "EntityVtable.h"

#include "Actor.h"
#include "Weapon.h"
#include "VehicleWeapon.h"
#include "Breakable.h"
#include "StateBreakable.h"
#include "Ammo.h"
#include "Trap.h"
#include "GenericItem.h"
#include "SpawnPoint.h"
#include "Keys.h"
#include "Radiation.h"
#include "Armour.h"
#include "Book.h"
#include "Holodisk.h"
#include "Lockpick.h"
#include "SkillConsumable.h"
#include "VehicleController.h"
#include "Alarm.h"
#include "Switch.h"
#include "ScienceSwitch.h"
#include "Geiger.h"
#include "Sneak.h"
#include "TrapTrigger.h"
#include "Container.h"
#include "Light.h"
#include "Scenary.h"
#include "ScenaryMove.h"
#include "ScenarySpawn.h"
#include "StateScenary.h"
#include "Deathtrap.h"
#include "Powernode.h"
#include "RepairObject.h"
#include "CaptureInvItem.h"
#include "CaptureItem.h"
#include "Door.h"
#include "RotatingDoor.h"
#include "EffectSpawn.h"
#include "Waypoint.h"
#include "SentryAI.h"
#include "CombatFX.h"
#include "BaseAI.h"
#include "Controller.h"
#include "Consumable.h"
#include "Inventory.h"
#include "Vehicle.h"

#include "FOTString.h"
#include "ZoneDefinition.h"
#include "ControllerCommandStruct.h"

#include "Windows.h"

EntityVtable::EntityVtable(lua_State* l)
	: orig_vtables_(),class_name_to_vtable_(), lua_(l)
{
	// Back up the original vtables
	CopyVtable("Actor", Actor::VTABLE, 536);
	CopyVtable("Alarm", Alarm::VTABLE, 525);
	CopyVtable("Ammo", Ammo::VTABLE, 525);
	CopyVtable("Armour", Armour::VTABLE, 525);
	CopyVtable("BaseAI", BaseAI::VTABLE, 525);
	CopyVtable("Book", Book::VTABLE, 525);
	CopyVtable("Breakable", Breakable::VTABLE, 526);
	CopyVtable("CaptureInvItem", CaptureInvItem::VTABLE, 525);
	CopyVtable("CaptureItem", CaptureItem::VTABLE, 525);
	CopyVtable("CombatFX", CombatFX::VTABLE, 525);
	CopyVtable("Consumable", Consumable::VTABLE, 526);
	CopyVtable("Container", Container::VTABLE, 527);
	CopyVtable("Controller", Controller::VTABLE, 527);
	CopyVtable("Deathtrap", Deathtrap::VTABLE, 525);
	CopyVtable("Door", Door::VTABLE, 528);
	CopyVtable("EffectSpawn", EffectSpawn::VTABLE, 525);
	CopyVtable("Entity", Entity::VTABLE, 525);
	CopyVtable("Geiger", Geiger::VTABLE, 525);
	CopyVtable("GenericItem", GenericItem::VTABLE, 525);
	CopyVtable("Holodisk", Holodisk::VTABLE, 525);
	CopyVtable("Inventory", Inventory::VTABLE, 525);
	CopyVtable("Keys", Keys::VTABLE, 525);
	CopyVtable("Light", Light::VTABLE, 526);
	CopyVtable("Lockpick", Lockpick::VTABLE, 525);
	CopyVtable("Powernode", Powernode::VTABLE, 526);
	CopyVtable("Radiation", Radiation::VTABLE, 525);
	CopyVtable("RepairObject", RepairObject::VTABLE, 525);
	CopyVtable("RotatingDoor", RotatingDoor::VTABLE, 528);
	CopyVtable("Scenary", Scenary::VTABLE, 525);
	CopyVtable("ScenaryMove", ScenaryMove::VTABLE, 525);
	CopyVtable("ScenarySpawn", ScenarySpawn::VTABLE, 525);
	CopyVtable("ScienceSwitch", ScienceSwitch::VTABLE, 525);
	CopyVtable("SentryAI", SentryAI::VTABLE, 526);
	CopyVtable("SkillConsumable", SkillConsumable::VTABLE, 525);
	CopyVtable("Sneak", Sneak::VTABLE, 525);
	CopyVtable("SpawnPoint", SpawnPoint::VTABLE, 525);
	CopyVtable("StateBreakable", StateBreakable::VTABLE, 526);
	CopyVtable("StateScenary", StateScenary::VTABLE, 525);
	CopyVtable("Switch", Switch::VTABLE, 525);
	CopyVtable("Trap", Trap::VTABLE, 525);
	CopyVtable("TrapTrigger", TrapTrigger::VTABLE, 525);
	CopyVtable("Vehicle", Vehicle::VTABLE, 531);
	CopyVtable("VehicleController", VehicleController::VTABLE, 525);
	CopyVtable("VehicleWeapon", VehicleWeapon::VTABLE, 527);
	CopyVtable("Waypoint", Waypoint::VTABLE, 525);
	CopyVtable("Weapon", Weapon::VTABLE, 525);

	InitVtableCallTemplates();

}

EntityVtable::~EntityVtable()
{
}

void EntityVtable::CopyVtable(std::string const& classname, size_t vtable_addr, int entries)
{
	EntityVtableArray new_array = new size_t[entries];
	memcpy(new_array, (void*)vtable_addr, entries * sizeof(size_t));
	orig_vtables_[vtable_addr] = new_array;
	class_name_to_vtable_[classname] = vtable_addr;
}

size_t EntityVtable::GetVtableAddrForClass(lua_State* l)
{
	std::shared_ptr<Entity> ent = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1));
	return ent->GetVtable();
}
size_t EntityVtable::GetOrigVtableAddrForClass(lua_State* l)
{
	std::shared_ptr<Entity> ent = Entity::GetEntityByID(LuaHelper::GetEntityId(l, 1));
	return reinterpret_cast<size_t>(orig_vtables_[ent->GetVtable()]);
}

VtableCallTemplateFxn EntityVtable::GetVtableCallTemplateByIndex(int idx)
{
	return vtable_call_templates_[idx];
}

void EntityVtable::InstallVtableHook(std::string const& classname, size_t idx, size_t new_fxn)
{
	if (classname != "ALL")
	{
		DWORD oldprotect;
		size_t vtable = class_name_to_vtable_[classname] + idx * sizeof(size_t);
		VirtualProtect((LPVOID)vtable, 4, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy((void*)vtable, (void*)&new_fxn, 4);
		VirtualProtect((LPVOID)vtable, 4, oldprotect, &oldprotect);
		return;
	}

	for (auto item : class_name_to_vtable_)
	{
		DWORD oldprotect;
		size_t vtable = item.second + idx * sizeof(size_t);
		VirtualProtect((LPVOID)vtable, 4, PAGE_EXECUTE_READWRITE, &oldprotect);
		memcpy((void*)vtable, (void*)&new_fxn, 4);
		VirtualProtect((LPVOID)vtable, 4, oldprotect, &oldprotect);
	}

}

void EntityVtable::InitVtableCallTemplates()
{
	// See the template definitions in EntityVtable.h for naming convention
	vtable_call_templates_[0] = vtable_call_template_3r1 < 0, RGBColor, Vector3*, float> ;								// RGBColor* (*GetRenderLightLevel)(struct Entity *, struct RGBColor *, struct LocationXYZFloat *, float);
	vtable_call_templates_[1] = vtable_call_template_noop<1>; // void (*RenderObjectMaybe)(struct Entity *, void *, int *);
	vtable_call_templates_[2] = vtable_call_template_0<2>;																// void (*InitDefaultEntityFields)(struct Entity *);
	vtable_call_templates_[3] = vtable_call_template_1r<3,wchar_t**,bool>;												// bool (*SetAnimationType)(struct Entity *, struct FOTString **);
	vtable_call_templates_[4] = vtable_call_template_1r<4,wchar_t**,bool>;												// bool (*UpdateSprite)(struct Entity *, struct FOTString **);
	vtable_call_templates_[5] = vtable_call_template_0<5>;																// void (*Vtable5NoOp)(struct Entity *);
	vtable_call_templates_[6] = vtable_call_template_0<6>;																// void (*InitEntityAfterSpawn)(struct Entity *);
	vtable_call_templates_[7] = vtable_call_template_0<7>;																// void (*PrepareEntity)(struct Entity *);
	vtable_call_templates_[8] = vtable_call_template_0<8>;																// void (*EraseInventory)(struct Entity *);
	vtable_call_templates_[9] = vtable_call_template_0<9>;																// void (*MakeFreshCharacter)(struct Entity *);
	vtable_call_templates_[10] = vtable_call_template_0<10>;															// void (*DestructEntity)(struct Entity *);
	vtable_call_templates_[11] = vtable_call_template_0<11>;															// void (*SetDoFade)(struct Entity *);
	vtable_call_templates_[12] = vtable_call_template_0<12>;															// void (*AliasToDestructEntity)(struct Entity *);
	vtable_call_templates_[13] = vtable_call_template_0<13>;															// void (*SomethingWithBoundingBoxAndCollision)(struct Entity *);
	vtable_call_templates_[14] = vtable_call_template_0<14>;															// void (*FlagCaptureFunction1)(struct Entity *);
	vtable_call_templates_[15] = vtable_call_template_noop<15>; // void (*AttachNewFileToActor)(struct Entity *, void *);
	vtable_call_templates_[16] = vtable_call_template_0<16>;															// void (*AttachNewFileToActor2)(struct Entity *);
	vtable_call_templates_[17] = vtable_call_template_0<17>;															// void (*SomethingNewSpawnRelated)(struct Entity *);
	vtable_call_templates_[18] = vtable_call_template_1<18,EntityID>;													// void (*Vtable18EquipRelated)(struct Entity *, struct EntityID);
	vtable_call_templates_[19] = vtable_call_template_0<19>;															// void (*AdjustSpriteOffsetForRotatingDoor)(struct Entity *);
	vtable_call_templates_[20] = vtable_call_template_0<20>;															// void (*FinishedAnimationSequenceMaybe)(struct Entity *);
	vtable_call_templates_[21] = vtable_call_template_0<21>;															// void (*Vtable21NoOp)(struct Entity *);
	vtable_call_templates_[22] = vtable_call_template_0<22>;															// void (*Vtable22NoOp)(struct Entity *);
	vtable_call_templates_[23] = vtable_call_template_0<23>;															// void (*Vtable23NoOp)(struct Entity *);
	vtable_call_templates_[24] = vtable_call_template_noop<24>; // float (*RenderRelated)(struct Entity *, void *, int *);
	vtable_call_templates_[25] = vtable_call_template_0<25>;															// void (*CheckThinkTimers)(struct Entity *);
	vtable_call_templates_[26] = vtable_call_template_0<26>;															// void (*DoEngineThink)(struct Entity *);
	vtable_call_templates_[27] = vtable_call_template_0<27>;															// void (*DoGameThink)(struct Entity *);
	vtable_call_templates_[28] = vtable_call_template_1<28,int>;														// void (*SoundRelated1)(struct Entity *, int);
	vtable_call_templates_[29] = vtable_call_template_1r1<29,wchar_t*>;													// FOTString ** (*GetSoundType)(struct Entity *, struct FOTString **);
	vtable_call_templates_[30] = vtable_call_template_1<30,int>;														// void (*SetRicochetSoundCounters)(struct Entity *, int);
	vtable_call_templates_[31] = vtable_call_template_1r1<31,Vector3>;													// int* (*SetParameterToZero)(struct Entity *, int *);
	vtable_call_templates_[32] = vtable_call_template_X32;																// void (*InitLightParameters)(struct Entity *, float *);
	vtable_call_templates_[33] = vtable_call_template_1<33,EntityID>;													// void (*SetAttached)(struct Entity *, struct EntityID);
	vtable_call_templates_[34] = vtable_call_template_1<34,wchar_t**>;													// void (*SetCustomName)(struct Entity *, struct FOTString **);
	vtable_call_templates_[35] = vtable_call_template_1r1<35, wchar_t*>;												// FOTString ** (*GetCustomName)(struct Entity *, struct FOTString **);
	vtable_call_templates_[36] = vtable_call_template_1r1<36, wchar_t*>;												// FOTString ** (*GetDisplayName)(struct Entity *, struct FOTString **);
	vtable_call_templates_[37] = vtable_call_template_1r1<37, wchar_t*>;												// FOTString ** (*GetHealthStatusString)(struct Entity *, struct FOTString **);
	vtable_call_templates_[38] = vtable_call_template_1r1<38, wchar_t*>;												// FOTString ** (*Vtable38NoOpEmptyStringRet)(struct Entity *, struct FOTString **);
	vtable_call_templates_[39] = vtable_call_template_1r1<39,wchar_t*>;													// FOTString ** (*GetUseActionName)(struct Entity *, struct FOTString **);
	vtable_call_templates_[40] = vtable_call_template_0r<40, bool>;														// bool (*CanBeHeldInHand)(struct Entity *);
	vtable_call_templates_[41] = vtable_call_template_0r<41, bool>; 													// bool (*IsTimerMine)(struct Entity *);
	vtable_call_templates_[42] = vtable_call_template_0r<42, bool>; 													// bool (*IsTriggerTrap)(struct Entity *);
	vtable_call_templates_[43] = vtable_call_template_0r<43, bool>; 													// bool (*IsCollectable)(struct Entity *);
	vtable_call_templates_[44] = vtable_call_template_0r<44, bool>; 													// bool (*IsSingleUseItem)(struct Entity *);
	vtable_call_templates_[45] = vtable_call_template_0r<45, bool>; 													// bool (*IsSkillConsumable)(struct Entity *);
	vtable_call_templates_[46] = vtable_call_template_0r<46, bool>; 													// bool (*IsArmour)(struct Entity *);
	vtable_call_templates_[47] = vtable_call_template_0r<47, bool>; 													// bool (*IsWeapon)(struct Entity *);
	vtable_call_templates_[48] = vtable_call_template_0r<48, bool>; 													// bool (*IsAmmo)(struct Entity *);
	vtable_call_templates_[49] = vtable_call_template_0r<49, bool>; 													// bool (*IsKey)(struct Entity *);
	vtable_call_templates_[50] = vtable_call_template_0r<50, bool>; 													// bool (*IsContainer)(struct Entity *);
	vtable_call_templates_[51] = vtable_call_template_0r<51, bool>; 													// bool (*IsUnarmedAttack)(struct Entity *);
	vtable_call_templates_[52] = vtable_call_template_0r<52, bool>; 													// bool (*IsMeleeAttack)(struct Entity *);
	vtable_call_templates_[53] = vtable_call_template_0r<53, bool>; 													// bool (*IsBaseAI)(struct Entity *);
	vtable_call_templates_[54] = vtable_call_template_0r<54, bool>; 													// bool (*IsController)(struct Entity *);
	vtable_call_templates_[55] = vtable_call_template_0r<55, bool>; 													// bool (*IsActor)(struct Entity *);
	vtable_call_templates_[56] = vtable_call_template_0r<56, bool>; 													// bool (*IsActorUnk20Set)(struct Entity *);
	vtable_call_templates_[57] = vtable_call_template_0r<57, bool>; 													// bool (*IsLight)(struct Entity *);
	vtable_call_templates_[58] = vtable_call_template_0r<58, bool>; 													// bool (*IsVehicle)(struct Entity *);
	vtable_call_templates_[59] = vtable_call_template_0r<59, bool>; 													// bool (*IsVehicleController)(struct Entity *);
	vtable_call_templates_[60] = vtable_call_template_0r<60, bool>; 													// bool (*IsVehicleWeapon)(struct Entity *);
	vtable_call_templates_[61] = vtable_call_template_0r<61, bool>; 													// bool (*CanBeActivated)(struct Entity *);
	vtable_call_templates_[62] = vtable_call_template_0r<62, bool>; 													// bool (*IsDeadOrDestroyedMaybe)(struct Entity *);
	vtable_call_templates_[63] = vtable_call_template_0r<63, bool>; 													// bool (*IsAttachedBaseAIMaybe)(struct Entity *);
	vtable_call_templates_[64] = vtable_call_template_0r<64, bool>; 													// bool (*IsLiving)(struct Entity *);
	vtable_call_templates_[65] = vtable_call_template_0r<65, bool>; 													// bool (*IsTrap)(struct Entity *);
	vtable_call_templates_[66] = vtable_call_template_0r<66, bool>;														// bool (*IsSentryAI)(struct Entity *);
	vtable_call_templates_[67] = vtable_call_template_0r<67, bool>; 													// bool (*IsGeiger)(struct Entity *);
	vtable_call_templates_[68] = vtable_call_template_0r<68, bool>; 													// bool (*IsWaypoint)(struct Entity *);
	vtable_call_templates_[69] = vtable_call_template_0r<69, bool>; 													// bool (*IsSpawnpoint)(struct Entity *);
	vtable_call_templates_[70] = vtable_call_template_0r<70, bool>; 													// bool (*IsAlarm)(struct Entity *);
	vtable_call_templates_[71] = vtable_call_template_0r<71, bool>; 													// bool (*IsCombatFX)(struct Entity *);
	vtable_call_templates_[72] = vtable_call_template_0r<72, bool>; 													// bool (*IsSwitch)(struct Entity *);
	vtable_call_templates_[73] = vtable_call_template_0r<73, bool>; 													// bool (*IsDoor)(struct Entity *);
	vtable_call_templates_[74] = vtable_call_template_0r<74, bool>; 													// bool (*IsGunTurret)(struct Entity *);
	vtable_call_templates_[75] = vtable_call_template_0r<75, bool>; 													// bool (*IsBrainJar)(struct Entity *);
	vtable_call_templates_[76] = vtable_call_template_0r<76, bool>; 													// bool (*IsBreakableType)(struct Entity *);
	vtable_call_templates_[77] = vtable_call_template_0r<77, bool>; 													// bool (*IsCaptureFlag)(struct Entity *);
	vtable_call_templates_[78] = vtable_call_template_0r<78, bool>; 													// bool (*IsTrapDoor)(struct Entity *);
	vtable_call_templates_[79] = vtable_call_template_0r<79, bool>; 													// bool (*IsCyborgGeneral)(struct Entity *);
	vtable_call_templates_[80] = vtable_call_template_0r<80, bool>; 													// bool (*IsPopupTurret)(struct Entity *);
	vtable_call_templates_[81] = vtable_call_template_0r<81, bool>; 													// bool (*IsNotCaptureInvItem)(struct Entity *);
	vtable_call_templates_[82] = vtable_call_template_1r1<82,wchar_t*>;													// FOTString * (*RetTypeStringForCarryableEntity)(struct Entity *, struct FOTString *);
	vtable_call_templates_[83] = vtable_call_template_3r1<83,InventoryActionResult,EntityID,EntityID>;					// UseItemResponse * (*MaybeUseEntity)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID);
	vtable_call_templates_[84] = vtable_call_template_2r1<84,EntityID,int>;												// EntityID * (*SplitEntity)(struct Entity *, struct EntityID *, dword);
	vtable_call_templates_[85] = vtable_call_template_2r<85,EntityID,int,bool>;											// bool (*MergeEntities)(struct Entity *, struct EntityID, dword);
	vtable_call_templates_[86] = vtable_call_template_1r<86,EntityID,bool>;												// bool (*MaybeCheckingIfMergeableAndMatching)(struct Entity *, struct EntityID);
	vtable_call_templates_[87] = vtable_call_template_0r<87, bool>;														// bool (*CanBeRemoved)(struct Entity *);
	vtable_call_templates_[88] = vtable_call_template_1<88, bool>;														// void (*SetCanBeRemovedFlag)(struct Entity *, bool);
	vtable_call_templates_[89] = vtable_call_template_1<89, bool>;														// void (*SetBeenAppliedFlag)(struct Entity *, bool);
	vtable_call_templates_[90] = vtable_call_template_0r<90, bool>;														// bool (*IsUnique)(struct Entity *);
	vtable_call_templates_[91] = vtable_call_template_1<91, bool>;														// void (*SetIsUniqueFlag)(struct Entity *, bool);
	vtable_call_templates_[92] = vtable_call_template_1r<92,AttributesTable*,bool>;										// bool (*MeetRequirementsCheck)(struct Entity *, struct AttributesArray *);
	vtable_call_templates_[93] = vtable_call_template_noop<93>;	// Stats * (*GetItemSPECIALRequired)(struct Entity *);   // (Maybe editor only?)
	vtable_call_templates_[94] = vtable_call_template_2<94,EntityID,int>;												// void (*ApplyEquip)(struct Entity *, struct EntityID, int);
	vtable_call_templates_[95] = vtable_call_template_2<95,EntityID,int>;												// void (*UnApply)(struct Entity *, struct EntityID, dword);
	vtable_call_templates_[96] = vtable_call_template_1<96,EntityID>;													// void (*SetTrapHolder)(struct Entity *, struct EntityID);
	vtable_call_templates_[97] = vtable_call_template_1<97,EntityID>;													// void (*ClearHolder)(struct Entity *, struct EntityID);
	vtable_call_templates_[98] = vtable_call_template_0r<98,int>;														// int (*GetCount)(struct Entity *);
	vtable_call_templates_[99] = vtable_call_template_0r<99,int>;														// int (*GetCountNonLootable)(struct Entity *);
	vtable_call_templates_[100] = vtable_call_template_0r<100,bool>;													// bool (*IsLootable)(struct Entity *);
	vtable_call_templates_[101] = vtable_call_template_1<101,EntityID>;													// void (*SetUnk25EntityID)(struct Entity *, struct EntityID);
	vtable_call_templates_[102] = vtable_call_template_0r<102,int>;														// int (*GetValueBaseOfItems)(struct Entity *);
	vtable_call_templates_[103] = vtable_call_template_0r<103,int>;														// int (*GetCollectableRankReq)(struct Entity *);
	vtable_call_templates_[104] = vtable_call_template_0r<104,float>;													// float (*GetWeight)(struct Entity *);
	vtable_call_templates_[105] = vtable_call_template_1<105,int>;														// void (*SetCount)(struct Entity *, int);
	vtable_call_templates_[106] = vtable_call_template_1r<106,int,int>;													// int (*SetNonLootableCount)(struct Entity *, int);
	vtable_call_templates_[107] = vtable_call_template_0r<107,bool>;													// bool (*BreakEntity)(struct Entity *);
	vtable_call_templates_[108] = vtable_call_template_0r<108, bool>;													// bool (*IsBroken)(struct Entity *);
	vtable_call_templates_[109] = vtable_call_template_0r<109, bool>;													// bool (*RepairEntity)(struct Entity *);
	vtable_call_templates_[110] = vtable_call_template_0r<110,int>;														// int (*GetTrapTotalDifficulty)(struct Entity *);
	vtable_call_templates_[111] = vtable_call_template_0<111>;															// void (*Vtable111Controller)(struct Entity *);
	vtable_call_templates_[112] = vtable_call_template_1<112,EntityID>;													// bool (*Vtable112Controller)(struct Entity *, struct EntityID);
	vtable_call_templates_[113] = vtable_call_template_1<113,EntityID>;													// bool (*Vtable113Controller)(struct Entity *, struct EntityID);
	vtable_call_templates_[114] = vtable_call_template_1<114,int>;														// void (*SetControllerMoveTypePriority)(struct Entity *, dword);
	vtable_call_templates_[115] = vtable_call_template_1<115,ZoneDefinition*>;											// void (*SetControllerWorkingZone)(struct Entity *, struct ZoneDefinition *);
	vtable_call_templates_[116] = vtable_call_template_1<116,ZoneDefinition*>;											// void (*SetControllerRetreatZone)(struct Entity *, struct ZoneDefinition *);
	vtable_call_templates_[117] = vtable_call_template_1<117,wchar_t*>;													// void (*SetControllerMoveTargetTag)(struct Entity *, struct FOTString *);
	vtable_call_templates_[118] = vtable_call_template_1<118,int>;														// void (*SetControllerAggression)(struct Entity *, int);
	vtable_call_templates_[119] = vtable_call_template_1<119,int>;														// void (*SetControllerCommunicate)(struct Entity *, int);
	vtable_call_templates_[120] = vtable_call_template_1<120,int>;														// void (*SetControllerMobility)(struct Entity *, int);
	vtable_call_templates_[121] = vtable_call_template_2<121,bool,int>;													// void (*SetControllerRandomMove)(struct Entity *, bool, byte);
	vtable_call_templates_[122] = vtable_call_template_0r<122,int>;														// int (*GetControllerMoveTypePriority)(struct Entity *);
	vtable_call_templates_[123] = vtable_call_template_1r1<123,ZoneDefinition>;											// void (*GetControllerWorkingZone)(struct Entity *, struct ZoneDefinition *);
	vtable_call_templates_[124] = vtable_call_template_1r1<124,ZoneDefinition>;											// void (*GetControllerRetreatZone)(struct Entity *, struct ZoneDefinition *);
	vtable_call_templates_[125] = vtable_call_template_1r1<125,wchar_t*>;												// FOTString * (*GetControllerMoveTargetTag)(struct Entity *, struct FOTString *);
	vtable_call_templates_[126] = vtable_call_template_0r<126,int>;														// int (*GetControllerAggression)(struct Entity *);
	vtable_call_templates_[127] = vtable_call_template_0r<127,int>;														// int (*GetControllerCommunicate)(struct Entity *);
	vtable_call_templates_[128] = vtable_call_template_0r<128,int>;														// int (*GetControllerMobility)(struct Entity *);
	vtable_call_templates_[129] = vtable_call_template_0r<129,bool>;													// bool (*GetControllerRandomMove)(struct Entity *);
	vtable_call_templates_[130] = vtable_call_template_1r1<130,wchar_t*>;												// FOTString * (*GetControllerType)(struct Entity *, struct FOTString *);
	vtable_call_templates_[131] = vtable_call_template_1<131,wchar_t**>;												// void (*SetControllerType)(struct Entity *, struct FOTString **);
	vtable_call_templates_[132] = vtable_call_template_0<132>;															// void (*DeactivateController)(struct Entity *);
	vtable_call_templates_[133] = vtable_call_template_1<133,wchar_t*>;													// void (*SetControllerWaypointModeAndMoveTargetTag)(struct Entity *, struct FOTString *);
	vtable_call_templates_[134] = vtable_call_template_0<134>;															// void (*SetControllerStationary)(struct Entity *);
	vtable_call_templates_[135] = vtable_call_template_0<135>;															// void (*SetControllerToPerimeterZone)(struct Entity *);
	vtable_call_templates_[136] = vtable_call_template_1<136,int>;														// void (*SetControllerToFollow)(struct Entity *, int);
	vtable_call_templates_[137] = vtable_call_template_0<137>;															// void (*SetControllerToFlee)(struct Entity *);
	vtable_call_templates_[138] = vtable_call_template_0<138>;															// void (*ControllerDoSomethingWithUnitList)(struct Entity *);
	vtable_call_templates_[139] = vtable_call_template_1<139,wchar_t*>;													// void (*SetControllerUnknown4String)(struct Entity *, struct FOTString *);
	vtable_call_templates_[140] = vtable_call_template_1r1<140,wchar_t*>;												// FOTString * (*GetControllerUnknown4String)(struct Entity *, struct FOTString *);
	vtable_call_templates_[141] = vtable_call_template_1<141,int>;														// void (*ControllerWakeUpUnitList)(struct Entity *, int);
	vtable_call_templates_[142] = vtable_call_template_2<142,EntityID,int>;												// void (*ControllerIssueTauntForEntity)(struct Entity *, struct EntityID, int);
	vtable_call_templates_[143] = vtable_call_template_1r<143,EntityID,float>;											// float (*ControllerWaypointMaybeTimingRelated)(struct Entity *, struct EntityID);
	vtable_call_templates_[144] = vtable_call_template_2r1<144,wchar_t*,EntityID>;										// FOTString * (*ControllerGetUnitWaypointActionString)(struct Entity *, struct FOTString *, struct EntityID);
	vtable_call_templates_[145] = vtable_call_template_1r<145,EntityID,bool>;											// bool (*ControllerWaypointCheckingRepeatFlag)(struct Entity *, struct EntityID);
	vtable_call_templates_[146] = vtable_call_template_2r<146,EntityID,Vector3*,float>;									// float (*ControllerMoveRelated)(struct Entity *, struct EntityID, struct LocationXYZFloat *);
	vtable_call_templates_[147] = vtable_call_template_3r1<147,EntityID,EntityID,bool>;									// EntityID * (*Vtable147Controller)(struct Entity *, struct EntityID *, struct EntityID, bool);
	vtable_call_templates_[148] = vtable_call_template_2r1<148,EntityID,wchar_t*>;										// EntityID * (*Vtable148Controller)(struct Entity *, struct EntityID *, struct EntityID, struct FOTString *);
	vtable_call_templates_[149] = vtable_call_template_2r<149,EntityID,int,float>;										// float (*ControllerGetFromUnknownList2)(struct Entity *, struct EntityID, int);
	vtable_call_templates_[150] = vtable_call_template_0r<150,bool>;													// bool (*Vtable150Controller)(struct Entity *);
	vtable_call_templates_[151] = vtable_call_template_2r<151,int,int,float>;											// float (*Vtable151NoOpReturnFloat0)(struct Entity *, int, int);
	vtable_call_templates_[152] = vtable_call_template_2r<152, int, int, float>;										// float (*Vtable152NoOpReturnFloat0)(struct Entity *, int, int);
	vtable_call_templates_[153] = vtable_call_template_2r<153,EntityID,Vector3*,float>;									// float (*ControllerMoreMoveRelated)(struct Entity *, struct EntityID, struct LocationXYZFloat *);
	vtable_call_templates_[154] = vtable_call_template_2r1<154,Vector3,EntityID>;										// LocationXYZFloat * (*ControllerWaypointStuff)(struct Entity *, struct LocationXYZFloat *, struct EntityID);
	vtable_call_templates_[155] = vtable_call_template_1r<155,EntityID,int64_t>;										// longlong (*ControllerSomethingAboutTookDamage)(struct Entity *, struct EntityID);
	vtable_call_templates_[156] = vtable_call_template_2<156,EntityID,Vector3*>;										// void (*Vtable156Controller)(struct Entity *, struct EntityID, struct LocationXYZFloat *);
	vtable_call_templates_[157] = vtable_call_template_3r<157,EntityID,Vector3*,bool,bool>;								// bool (*ControllerUsesWorkingZoneAndMoveType)(struct Entity *, struct EntityID, struct LocationXYZFloat *, bool);
	vtable_call_templates_[158] = vtable_call_template_0r<158,bool>;													// bool (*BaseAINeedsToSwitchTargetMaybe)(struct Entity *);
	vtable_call_templates_[159] = vtable_call_template_noop<159>; // void (*BaseAIEngineThinkRelated)(struct Entity *, int *);
	vtable_call_templates_[160] = vtable_call_template_1<160,EntityID>;													// void (*SetBaseAIController)(struct Entity *, struct EntityID);
	vtable_call_templates_[161] = vtable_call_template_1r1<161,EntityID>;												// EntityID * (*GetBaseAIController)(struct Entity *, struct EntityID *);
	vtable_call_templates_[162] = vtable_call_template_noop<162>; // void (*Vtable162BaseAI)(struct Entity *, int *);
	vtable_call_templates_[163] = vtable_call_template_1r1<163,wchar_t*>;												// FOTString * (*GetBaseAINature)(struct Entity *, struct FOTString *);
	vtable_call_templates_[164] = vtable_call_template_1<164,bool>;														// void (*StopMovement)(struct Entity *, bool);
	vtable_call_templates_[165] = vtable_call_template_1r1<165,EntityID>;												// EntityID * (*BaseAIGetCurrentTarget)(struct Entity *, struct EntityID *);
	vtable_call_templates_[166] = vtable_call_template_0r<166,bool>;													// bool (*BaseAIHasATargetMaybe)(struct Entity *);
	vtable_call_templates_[167] = vtable_call_template_0r<167,bool>;													// bool (*IsSleeping)(struct Entity *);
	vtable_call_templates_[168] = vtable_call_template_1<168,bool>;														// void (*SetSleepingFlag)(struct Entity *, bool);
	vtable_call_templates_[169] = vtable_call_template_1<169,float>;													// void (*SetBaseAIUnknown7)(struct Entity *, float);
	vtable_call_templates_[170] = vtable_call_template_1<170,int>;														// void (*SetBaseAIUnknown7ToRealTimeTimerValue)(struct Entity *, int);
	vtable_call_templates_[171] = vtable_call_template_0<171>;															// void (*BaseAIStopMovementIfHighestDesireSet)(struct Entity *);
	vtable_call_templates_[172] = vtable_call_template_noop<172>; // bool (*BaseAIMaybeOpenDoorRelated)(struct Entity *, struct BaseAIEntityPlus4Struct *);
	vtable_call_templates_[173] = vtable_call_template_noop<173>; // void (*Vtable173BaseAI)(struct Entity *, struct BaseAIEntityPlus4Struct *);
	vtable_call_templates_[174] = vtable_call_template_1<174,EntityID>;													// void (*Vtable174BaseAI)(struct Entity *, struct EntityID);
	vtable_call_templates_[175] = vtable_call_template_1r1<175,wchar_t*>;												// FOTString * (*GetBaseAITauntTypeString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[176] = vtable_call_template_0r<176,int>;														// int (*GetBaseAIHighestDesireRoundedUp)(struct Entity *);
	vtable_call_templates_[177] = vtable_call_template_0<177>;															// void (*ClearSentryAITargets)(struct Entity *);
	vtable_call_templates_[178] = vtable_call_template_1<178,int>;														// void (*SetSentryMode)(struct Entity *, int);
	vtable_call_templates_[179] = vtable_call_template_0r<179,int>;														// int (*GetSentryMode)(struct Entity *);
	vtable_call_templates_[180] = vtable_call_template_1<180,int>;														// void (*SetSentryAccuracyTrigger)(struct Entity *, int);
	vtable_call_templates_[181] = vtable_call_template_0r<181,int>;														// int (*GetSentryAccuracyTrigger)(struct Entity *);
	vtable_call_templates_[182] = vtable_call_template_1<182,int>;														// void (*SetSentryLocation)(struct Entity *, int);
	vtable_call_templates_[183] = vtable_call_template_0r<183,int>;														// int (*GetSentryLocation)(struct Entity *);
	vtable_call_templates_[184] = vtable_call_template_2<184,EntityID,int>;												// void (*SetSentryUserTarget)(struct Entity *, struct EntityID, int);
	vtable_call_templates_[185] = vtable_call_template_1<185,EntityID>;													// void (*SentryAIMaybeCheckingTargets)(struct Entity *, struct EntityID);
	vtable_call_templates_[186] = vtable_call_template_noop<186>; // ActorSPECIALCopyAndFields * (*GetActorCopyofSPECIALStats)(struct Entity *);
	vtable_call_templates_[187] = vtable_call_template_noop<187>; // void (*CopySomeActorFields)(struct Entity *, struct ActorSPECIALCopyAndFields *);
	vtable_call_templates_[188] = vtable_call_template_noop<188>; // bool (*CopySomeActorFields2)(struct Entity *, struct ActorSPECIALCopyAndFields *, void *);
	vtable_call_templates_[189] = vtable_call_template_noop<189>; // bool (*Vtable189Actor)(struct Entity *, void *, void *, void *);
	vtable_call_templates_[190] = vtable_call_template_0r<190,int>;														// int (*GetActorStartIndex)(struct Entity *);
	vtable_call_templates_[191] = vtable_call_template_1<191,int>;														// void (*SetActorStartIndex)(struct Entity *, int);
	vtable_call_templates_[192] = vtable_call_template_0r<192, float>;													// float (*GetActorFractionOfHitPointsLost)(struct Entity *);
	vtable_call_templates_[193] = vtable_call_template_0r<193, float>;													// float (*CheckBestHPRatioIfUseHealingItemMaybe)(struct Entity *);
	vtable_call_templates_[194] = vtable_call_template_0r<194, float>;													// float (*ActorHasItemToPermBoostStats)(struct Entity *);
	vtable_call_templates_[195] = vtable_call_template_0r<195, float>;													// float (*ActorHasItemToPermBoostStats2)(struct Entity *);
	vtable_call_templates_[196] = vtable_call_template_0r<196,float>;													// float (*GetActorPoisonPointsTimes15Cap90)(struct Entity *);
	vtable_call_templates_[197] = vtable_call_template_0r<197, float>;													// float (*ActorLooksLikeNoOp)(struct Entity *);
	vtable_call_templates_[198] = vtable_call_template_0r<198, float>;													// float (*GetActorRadPointsDivided10Cap90)(struct Entity *);
	vtable_call_templates_[199] = vtable_call_template_0r<199, float>;													// float (*ActorCheckItemsForRadHealing)(struct Entity *);
	vtable_call_templates_[200] = vtable_call_template_2<200,int,int>;													// void (*MakeConsumableGiveRadiation)(struct Entity *, int, int);
	vtable_call_templates_[201] = vtable_call_template_2<201, int, int>;												// void (*MakeConsumableGivePoison)(struct Entity *, int, int);
	vtable_call_templates_[202] = vtable_call_template_0<202>;															// void (*ApplyBonusesToAttachedActor)(struct Entity *);
	vtable_call_templates_[203] = vtable_call_template_1r<203,EntityID,float>;											// float (*GetExpectedHPRatioIfConsumableUsed)(struct Entity *, struct EntityID);
	vtable_call_templates_[204] = vtable_call_template_1r<204,EntityID,float>;											// float (*DoesConsumablePermBoostSPEAL)(struct Entity *, struct EntityID);
	vtable_call_templates_[205] = vtable_call_template_0r<205,float>;													// float (*Vtable205NoOpRetZeroFloat)(struct Entity *);
	vtable_call_templates_[206] = vtable_call_template_1r<206,EntityID,float>;											// float (*GetRatioOfRadPointHealingForConsumable)(struct Entity *, struct EntityID);
	vtable_call_templates_[207] = vtable_call_template_0r<207,bool>;													// bool (*Vtable207ActorMaybeCheckForPlayerCharacter)(struct Entity *);
	vtable_call_templates_[208] = vtable_call_template_1r1<208,wchar_t*>;												// FOTString * (*GetRankString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[209] = vtable_call_template_0r<209,int>;														// int (*GetActorRankID)(struct Entity *);
	vtable_call_templates_[210] = vtable_call_template_1r<210,EntityID,int>;											// int (*GetRanksGivenEntityHasAboveThis)(struct Entity *, struct EntityID);
	vtable_call_templates_[211] = vtable_call_template_1r1<211,wchar_t*>;												// FOTString * (*GetReputationString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[212] = vtable_call_template_0r<212,int>;														// int (*GetCappedReputation)(struct Entity *);
	vtable_call_templates_[213] = vtable_call_template_0r<213,int>;														// int (*GetMissionsCom)(struct Entity *);
	vtable_call_templates_[214] = vtable_call_template_1<214,int>;														// void (*IncrementMissionsCom)(struct Entity *, int);
	vtable_call_templates_[215] = vtable_call_template_1<215, int>;														// void (*AddToReputation)(struct Entity *, int);
	vtable_call_templates_[216] = vtable_call_template_0r<216,bool>;													// bool (*IsGlowing)(struct Entity *);
	vtable_call_templates_[217] = vtable_call_template_noop<217>; // int (*GetActorAge)(struct Entity *, struct Traits *);
	vtable_call_templates_[218] = vtable_call_template_2<218,wchar_t**, wchar_t**>;										// void (*SetCurrentActionStringMaybe)(struct Entity *, struct FOTString **, struct FOTString **);
	vtable_call_templates_[219] = vtable_call_template_1r1<219,wchar_t*>;												// FOTString * (*GetCurrentActionString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[220] = vtable_call_template_0r<220,bool>;													// bool (*IsIdle)(struct Entity *);
	vtable_call_templates_[221] = vtable_call_template_0r<221, bool>;													// bool (*IsMoving_WalkRunClimbLadderOrDriving)(struct Entity *);
	vtable_call_templates_[222] = vtable_call_template_0r<222, bool>;													// bool (*IsClimbing)(struct Entity *);
	vtable_call_templates_[223] = vtable_call_template_0r<223, bool>;													// bool (*IsIdleOrWalkRun)(struct Entity *);
	vtable_call_templates_[224] = vtable_call_template_0r<224, bool>;													// bool (*HasFallen)(struct Entity *);
	vtable_call_templates_[225] = vtable_call_template_0<225>;															// void (*Vtable225ThinkRelated)(struct Entity *);
	vtable_call_templates_[226] = vtable_call_template_0r<226,bool>;													// bool (*CanMove)(struct Entity *);
	vtable_call_templates_[227] = vtable_call_template_0r<227, bool>;													// bool (*CanRun)(struct Entity *);
	vtable_call_templates_[228] = vtable_call_template_1<228, bool>;													// void (*AdjustStanceDownward)(struct Entity *, bool);
	vtable_call_templates_[229] = vtable_call_template_1<229, bool>;													// void (*AdjustStanceUpward)(struct Entity *, bool);
	vtable_call_templates_[230] = vtable_call_template_0r<230, bool>;													// bool (*IsHumanoidOrDog)(struct Entity *);
	vtable_call_templates_[231] = vtable_call_template_0r<231,float>;													// float (*MaxMovementSpeedMaybe)(struct Entity *);
	vtable_call_templates_[232] = vtable_call_template_0r<232, bool>;													// bool (*IsEncumbered)(struct Entity *);
	vtable_call_templates_[233] = vtable_call_template_0r<233, bool>;													// bool (*IsStanding)(struct Entity *);
	vtable_call_templates_[234] = vtable_call_template_0r<234, bool>;													// bool (*IsCrouching)(struct Entity *);
	vtable_call_templates_[235] = vtable_call_template_0r<235, bool>;													// bool (*IsProne)(struct Entity *);
	vtable_call_templates_[236] = vtable_call_template_0r<236,int>;														// int (*GetSelectedStance)(struct Entity *);
	vtable_call_templates_[237] = vtable_call_template_1<237,wchar_t**>;												// void (*ActoSetSelectedStance)(struct Entity *, struct FOTString *);
	vtable_call_templates_[238] = vtable_call_template_0r<238, bool>;													// bool (*IsHidden)(struct Entity *);
	vtable_call_templates_[239] = vtable_call_template_1r1<239,wchar_t*>;												// FOTString * (*GetPostureString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[240] = vtable_call_template_1<240, bool>;													// void (*SetSneakState)(struct Entity *, bool);
	vtable_call_templates_[241] = vtable_call_template_0r<241, bool>;													// bool (*IsBurrowed)(struct Entity *);
	vtable_call_templates_[242] = vtable_call_template_1<242, bool>;													// void (*SetBurrowingState)(struct Entity *, bool);
	vtable_call_templates_[243] = vtable_call_template_2r<243,wchar_t**,int,bool>;										// bool (*RollAgainstStat)(struct Entity *, struct FOTString *, int);
	vtable_call_templates_[244] = vtable_call_template_4r1<244,InventoryActionResult,wchar_t**,EntityID,EntityID>;		// UseItemResponse * (*ApplySkillToTargetEntity)(struct Entity *, struct UseItemResponse *, struct FOTString *, struct EntityID, struct EntityID);
	vtable_call_templates_[245] = vtable_call_template_1r1<245,wchar_t*>;												// FOTString * (*GetSkillStringBasedOnUnk25c)(struct Entity *, struct FOTString *);
	vtable_call_templates_[246] = vtable_call_template_1r1<246,EntityID>;												// EntityID * (*GetActorUnk2EntityID)(struct Entity *, struct EntityID *);
	vtable_call_templates_[247] = vtable_call_template_0r<247,int>;														// int (*GetAdjustedPilotSkill)(struct Entity *);
	vtable_call_templates_[248] = vtable_call_template_1r<248,wchar_t**,bool>;											// bool (*DoesSkillStringMatchHelperItem)(struct Entity *, struct FOTString *);
	vtable_call_templates_[249] = vtable_call_template_1r<249,wchar_t**,bool>;											// bool (*IsValidTargetForSkill)(struct Entity *, struct FOTString *);
	vtable_call_templates_[250] = vtable_call_template_2r<250,EntityID,EntityID,bool>;									// bool (*IsValidTargetForDefaultAction)(struct Entity *, struct EntityID, struct EntityID);
	vtable_call_templates_[251] = vtable_call_template_0r<251, bool>;													// bool (*IsGeneral)(struct Entity *);
	vtable_call_templates_[252] = vtable_call_template_0r<252, bool>;													// bool (*IsRecruitMaster)(struct Entity *);
	vtable_call_templates_[253] = vtable_call_template_0r<253, bool>;													// bool (*IsQuartermaster)(struct Entity *);
	vtable_call_templates_[254] = vtable_call_template_0r<254, bool>;													// bool (*CanBarterWith)(struct Entity *);
	vtable_call_templates_[255] = vtable_call_template_0r<255, bool>;													// bool (*BartersWithBOSScript)(struct Entity *);
	vtable_call_templates_[256] = vtable_call_template_0r<256, bool>;													// bool (*CanGambleWith)(struct Entity *);
	vtable_call_templates_[257] = vtable_call_template_0r<257,int>;														// int (*GetRepairObjectDifficulty)(struct Entity *);
	vtable_call_templates_[258] = vtable_call_template_1r<258,float,float>;												// float (*GetScaledKnockdownChance)(struct Entity *, float);
	vtable_call_templates_[259] = vtable_call_template_0<259>;															// void (*ActorKnockoutRelated)(struct Entity *);
	vtable_call_templates_[260] = vtable_call_template_3r1<260,InventoryActionResult,int,int>;							// UseItemResponse * (*ActorInjureLocation)(struct Entity *, struct UseItemResponse *, int, int);
	vtable_call_templates_[261] = vtable_call_template_4r1<261, InventoryActionResult, int, int,int>;					// UseItemResponse * (*ActorKnockout)(struct Entity *, struct UseItemResponse *, int, int, int);
	vtable_call_templates_[262] = vtable_call_template_3r1<262, InventoryActionResult, int, int>;						// UseItemResponse * (*ActorDisarm)(struct Entity *, struct UseItemResponse *, int, int);
	vtable_call_templates_[263] = vtable_call_template_3r1<263, InventoryActionResult, int, int>;						// UseItemResponse * (*ActorBreakWeaponMaybe)(struct Entity *, struct UseItemResponse *, int, int);
	vtable_call_templates_[264] = vtable_call_template_2r1<264, InventoryActionResult, int>;							// UseItemResponse * (*ActorCriticalMissHitSelfMaybe)(struct Entity *, struct UseItemResponse *, int);
	vtable_call_templates_[265] = vtable_call_template_2r1<265, InventoryActionResult, int>;							// UseItemResponse * (*ActorCriticalMissHitSelfMaybe2)(struct Entity *, struct UseItemResponse *, int);
	vtable_call_templates_[266] = vtable_call_template_1<266,int>;														// void (*StunActor)(struct Entity *, int);
	vtable_call_templates_[267] = vtable_call_template_1r<267, bool, bool>;												// bool (*IsRightLegInjured)(struct Entity *, bool);
	vtable_call_templates_[268] = vtable_call_template_1r<268, bool, bool>;												// bool (*IsLeftLegInjured)(struct Entity *, bool);
	vtable_call_templates_[269] = vtable_call_template_1r<269, bool, bool>; 											// bool (*IsRightArmInjured)(struct Entity *, bool);
	vtable_call_templates_[270] = vtable_call_template_1r<270, bool, bool>; 											// bool (*IsLeftArmInjured)(struct Entity *, bool);
	vtable_call_templates_[271] = vtable_call_template_1r<271, bool, bool>; 											// bool (*IsBlinded)(struct Entity *, bool);
	vtable_call_templates_[272] = vtable_call_template_1r<272, bool, bool>; 											// bool (*IsWinded)(struct Entity *, bool);
	vtable_call_templates_[273] = vtable_call_template_1r<273, bool, bool>; 											// bool (*IsImmobilized)(struct Entity *, bool);
	vtable_call_templates_[274] = vtable_call_template_1r<274, bool, bool>; 											// bool (*IsConcussed)(struct Entity *, bool);
	vtable_call_templates_[275] = vtable_call_template_1r<275, bool, bool>; 											// bool (*IsStunned)(struct Entity *, bool);
	vtable_call_templates_[276] = vtable_call_template_0r<276, bool>; 													// bool (*IsUnconscious)(struct Entity *);
	vtable_call_templates_[277] = vtable_call_template_0r<277, bool>;													// bool (*IsBandagedStatus)(struct Entity *);
	vtable_call_templates_[278] = vtable_call_template_1r<278,Vector3*,int>; 											// int (*GetActorMovementCostToLocation)(struct Entity *, struct LocationXYZFloat *);
	vtable_call_templates_[279] = vtable_call_template_0r<279, bool>; 													// bool (*IsActorUnk23SetPlusSomeOtherFieldFalse)(struct Entity *);
	vtable_call_templates_[280] = vtable_call_template_0<280>;															// void (*Move)(struct Entity *);
	vtable_call_templates_[281] = vtable_call_template_5<281,Vector3*,int,float,float,bool>;							// void (*SetDestination)(struct Entity *, struct LocationXYZFloat *, int, float, float, bool);
	vtable_call_templates_[282] = vtable_call_template_1r1<282,Vector3>;												// LocationXYZFloat * (*GetLocationBasedOnUnk23)(struct Entity *, struct LocationXYZFloat *);
	vtable_call_templates_[283] = vtable_call_template_1r1<283,EntityID>;												// EntityID * (*ActorGetUnk24bEntityID)(struct Entity *, struct EntityID *);
	vtable_call_templates_[284] = vtable_call_template_2r<284,Vector3*,bool,bool>;										// bool (*ActorSomethingTouchSearch)(struct Entity *, struct LocationXYZFloat *, bool);
	vtable_call_templates_[285] = vtable_call_template_2r<285, Vector3*, bool, bool>;									// bool (*ActorSomethingTouchSearch2)(struct Entity *, struct LocationXYZFloat *, bool);
	vtable_call_templates_[286] = vtable_call_template_2r<286, Vector3*, bool, bool>;									// bool (*ActorSomethingTouchSearch3)(struct Entity *, struct LocationXYZFloat *, bool);
	vtable_call_templates_[287] = vtable_call_template_0r<287,bool>;													// bool (*Vtable287NoOpRet0)(struct Entity *);
	vtable_call_templates_[288] = vtable_call_template_0r<288, bool>;													// bool (*IsLegalTarget)(struct Entity *);
	vtable_call_templates_[289] = vtable_call_template_0r<289, bool>;													// bool (*ActorIsNotLegalTargetAndUnk1Set)(struct Entity *);
	vtable_call_templates_[290] = vtable_call_template_0r<290, bool>;													// bool (*IsVehicleDisabled)(struct Entity *);
	vtable_call_templates_[291] = vtable_call_template_3r1<291,InventoryActionResult,EntityID,int>;						// UseItemResponse * (*ActorPickupItem)(struct Entity *, struct UseItemResponse *, struct EntityID, int);
	vtable_call_templates_[292] = vtable_call_template_0<292>;															// void (*Vtable292Actor)(struct Entity *);
	vtable_call_templates_[293] = vtable_call_template_1r1<293,InventoryActionResult>;									// UseItemResponse * (*ReturnUseItemResponseType0x40)(struct Entity *, struct UseItemResponse *);
	vtable_call_templates_[294] = vtable_call_template_3r1<294,ChanceToHit,EntityID,bool>;								// HitChanceStruct * (*GetSomeKindOfFlagForMouseOverTarget)(struct Entity *, struct HitChanceStruct *, struct EntityID, bool);
	vtable_call_templates_[295] = vtable_call_template_2r1<295,ChanceToHit,Vector3*>;									// HitChanceStruct * (*GetSomeKindOfFlagForMouseOverTarget2)(struct Entity *, struct HitChanceStruct *, struct LocationXYZFloat *);
	vtable_call_templates_[296] = vtable_call_template_4r1<296,ChanceToHit,EntityID,int,int>;							// HitChanceStruct * (*BeginAttackAnEntity)(struct Entity *, struct HitChanceStruct *, struct EntityID, int, int);
	vtable_call_templates_[297] = vtable_call_template_2r1<297,ChanceToHit,Vector3*>;									// HitChanceStruct * (*BeginAttackALocation)(struct Entity *, struct HitChanceStruct *, struct LocationXYZFloat *);
	vtable_call_templates_[298] = vtable_call_template_1<298,CombatMessage*>;											// void (*ApplyAttackResultToEntity)(struct Entity *, struct CombatMessage *);
	vtable_call_templates_[299] = vtable_call_template_1<299,CombatMessage*>;											// void (*ShotAtMissed)(struct Entity *, struct CombatMessage *);
	vtable_call_templates_[300] = vtable_call_template_6<300,EntityID,int,int,int,wchar_t*,int>;						// void (*ApplyDamage)(struct Entity *, struct EntityID, int, int, int, struct FOTString *, int);
	vtable_call_templates_[301] = vtable_call_template_4<301,EntityID,int,int,wchar_t*>;								// void (*EntityWasKilled)(struct Entity *, struct EntityID, int, int, struct FOTString *);
	vtable_call_templates_[302] = vtable_call_template_1<302,EntityID>;													// void (*IncrementKillCount)(struct Entity *, struct EntityID);
	vtable_call_templates_[303] = vtable_call_template_noop<303>; // void (*CopyActorKillList)(struct Entity *, void *);
	vtable_call_templates_[304] = vtable_call_template_0r<304, bool>;													// bool (*Vtable304NoOpRet0)(struct Entity *);
	vtable_call_templates_[305] = vtable_call_template_1<305,AnimOffset*>;												// void (*ResolveCombatMessageForAttacker)(struct Entity *, AnimOffset *);
	vtable_call_templates_[306] = vtable_call_template_1r1<306,EntityID>;												// EntityID * (*GetIDOfEquippedItemInActiveSlot)(struct Entity *, struct EntityID *);
	vtable_call_templates_[307] = vtable_call_template_1r1<307,EntityID>;												// EntityID * (*GetEntityIDOfNonEquippedWeapon)(struct Entity *, struct EntityID *);
	vtable_call_templates_[308] = vtable_call_template_1r1<308,InventoryActionResult>;									// UseItemResponse * (*ReloadActiveWeaponWithAnyAmmo)(struct Entity *, struct UseItemResponse *);
	vtable_call_templates_[309] = vtable_call_template_3r1<309,InventoryActionResult,int,EntityID>;						// UseItemResponse * (*ReloadWeaponWithSelectedAmmo)(struct Entity *, struct UseItemResponse *, int, struct EntityID);
	vtable_call_templates_[310] = vtable_call_template_0r<310,bool>;													// bool (*ActorCanAttackWithActiveWeapon)(struct Entity *);
	vtable_call_templates_[311] = vtable_call_template_0r<311, bool>;													// bool (*Vtable311ActorCheckingLastTimeRun)(struct Entity *);
	vtable_call_templates_[312] = vtable_call_template_0<312>;															// void (*InitActorCombatMessages)(struct Entity *);
	vtable_call_templates_[313] = vtable_call_template_1<313,int>;														// void (*LevelUp)(struct Entity *, int);
	vtable_call_templates_[314] = vtable_call_template_1r1<314,EntityID>;												// EntityID * (*GetCombatMessage2TargetID)(struct Entity *, struct EntityID *);
	vtable_call_templates_[315] = vtable_call_template_2r<315,EntityID,EntityID,int>;									// int (*GetPlayerBuyPriceForItem)(struct Entity *, struct EntityID, struct EntityID);
	vtable_call_templates_[316] = vtable_call_template_2r<316, EntityID, EntityID, int>;								// int (*GetPlayerSellBackPriceForItem)(struct Entity *, struct EntityID, struct EntityID);
	vtable_call_templates_[317] = vtable_call_template_1r<317,EntityID,int>;											// int (*GetCountOfMatchingItems)(struct Entity *, struct EntityID);
	vtable_call_templates_[318] = vtable_call_template_5r2<318,InventoryActionResult,EntityID,EntityID,int,int>;		// UseItemResponse * (*EquipItem)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	vtable_call_templates_[319] = vtable_call_template_3r2<319, InventoryActionResult, int, EntityID>;					// UseItemResponse * (*RemoveItem)(struct Entity *, struct UseItemResponse *, int, struct EntityID *);
	vtable_call_templates_[320] = vtable_call_template_3r2<320, InventoryActionResult, EntityID, EntityID>;				// UseItemResponse * (*RemoveItemIfEquipped)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *);
	vtable_call_templates_[321] = vtable_call_template_5r2<321, InventoryActionResult, EntityID, EntityID, int, int>;	// UseItemResponse * (*AddToInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	vtable_call_templates_[322] = vtable_call_template_5r2<322, InventoryActionResult, EntityID, EntityID, int, int>;	// UseItemResponse * (*RemoveFromInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	vtable_call_templates_[323] = vtable_call_template_3r2<323, InventoryActionResult, int, EntityID>;					// UseItemResponse * (*UnequipItemFromSlotAndPutInInventory)(struct Entity *, struct UseItemResponse *, int, struct EntityID *);
	vtable_call_templates_[324] = vtable_call_template_3r2<324, InventoryActionResult, EntityID, EntityID>;				// UseItemResponse * (*UnequipItemByIDAndReturnToInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *);
	vtable_call_templates_[325] = vtable_call_template_4r2<325, InventoryActionResult, EntityID, EntityID,int>;			// UseItemResponse * (*EquipItemFromInventoryToSlot)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int);
	vtable_call_templates_[326] = vtable_call_template_4r2<326, InventoryActionResult, EntityID, EntityID, int>;		// UseItemResponse * (*RemoveFromInventory2)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int);
	vtable_call_templates_[327] = vtable_call_template_5r2<327, InventoryActionResult, EntityID, EntityID, int, int>;	// UseItemResponse * (*ActorSomethingRelatedToPickup)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	vtable_call_templates_[328] = vtable_call_template_3r2<328, InventoryActionResult, int, EntityID>;					// UseItemResponse * (*RemoveItemFromSlot2)(struct Entity *, struct UseItemResponse *, int, struct EntityID *);
	vtable_call_templates_[329] = vtable_call_template_3r2<329, InventoryActionResult, EntityID, EntityID>;				// UseItemResponse * (*RemoveItemIfEquipped2)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *);
	vtable_call_templates_[330] = vtable_call_template_5r2<330, InventoryActionResult, EntityID, EntityID, int, int>;	// UseItemResponse * (*PickupItemAndEquipInSlot)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	vtable_call_templates_[331] = vtable_call_template_4r2<331, InventoryActionResult, EntityID, EntityID, int>;		// UseItemResponse * (*RemoveItemFromEquippedThenInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int);
	vtable_call_templates_[332] = vtable_call_template_4r2<332, InventoryActionResult, EntityID, EntityID, int>;		// UseItemResponse * (*RemoveItemFromInventoryThenEquipped)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int);
	vtable_call_templates_[333] = vtable_call_template_3r2<333, InventoryActionResult, int, EntityID>;					// UseItemResponse * (*Vtable333SomethingForApplyingItem)(struct Entity *, struct UseItemResponse *, int, struct EntityID *);
	vtable_call_templates_[334] = vtable_call_template_1<334,int>;														// void (*SetActiveWeaponHand)(struct Entity *, char);
	vtable_call_templates_[335] = vtable_call_template_0<335>;															// void (*ToggleActiveHand)(struct Entity *);
	vtable_call_templates_[336] = vtable_call_template_1r<336,EntityID,bool>;											// bool (*CheckIfTargetInReachableRange)(struct Entity *, struct EntityID);
	vtable_call_templates_[337] = vtable_call_template_0r<337,uint8_t>;													// byte (*GetSelectedHand)(struct Entity *);
	vtable_call_templates_[338] = vtable_call_template_0r<338,uint8_t>;													// byte (*GetUnselectedHand)(struct Entity *);
	vtable_call_templates_[339] = vtable_call_template_2r1<339,EntityID,int>;											// EntityID * (*GetIDofEquippedItemInSlot)(struct Entity *, struct EntityID *, int);
	vtable_call_templates_[340] = vtable_call_template_2r1<340,InventoryActionResult,EntityID>;							// UseItemResponse * (*Vtable340InventoryCombineRelatedMaybe)(struct Entity *, struct UseItemResponse *, struct EntityID);
	vtable_call_templates_[341] = vtable_call_template_1r<341,wchar_t*,bool>;											// bool (*PlayerEquippedItemWithTagName)(struct Entity *, struct FOTString *);
	vtable_call_templates_[342] = vtable_call_template_1r<342,EntityID,bool>;											// bool (*IsItemIDEquipped)(struct Entity *, struct EntityID);
	vtable_call_templates_[343] = vtable_call_template_1r<343,wchar_t*,bool>;											// bool (*ContainsItemWithTagName)(struct Entity *, struct FOTString *);
	vtable_call_templates_[344] = vtable_call_template_1r<344,EntityID,bool>;											// bool (*ContainsItemByID)(struct Entity *, struct EntityID);
	vtable_call_templates_[345] = vtable_call_template_1r<345,wchar_t*,bool>;											// bool (*EquippedOrContainsItemWithTagName)(struct Entity *, struct FOTString *);
	vtable_call_templates_[346] = vtable_call_template_1r<346,EntityID,bool>;											// bool (*FindIfEquippedOrCarriedItemByID)(struct Entity *, struct EntityID);
	vtable_call_templates_[347] = vtable_call_template_4r1<347,InventoryActionResult,int,int,uint8_t>;					// UseItemResponse * (*Vtable347ActorSomethingAboutUsingItem)(struct Entity *, struct UseItemResponse *, word, word, byte);
	vtable_call_templates_[348] = vtable_call_template_2r1<348,InventoryActionResult,EntityID>;							// UseItemResponse * (*CanUseItem)(struct Entity *, struct UseItemResponse *, struct EntityID);
	vtable_call_templates_[349] = vtable_call_template_2r1<349, InventoryActionResult, EntityID>;						// UseItemResponse * (*CanReachInteractable)(struct Entity *, struct UseItemResponse *, struct EntityID);
	vtable_call_templates_[350] = vtable_call_template_1r1<350,wchar_t*>;												// FOTString * (*GetSizeStringMaybe)(struct Entity *, struct FOTString *);
	vtable_call_templates_[351] = vtable_call_template_0r<351,AttributesTable*>;										// AttributesArray * (*GetCurrentStats)(struct Entity *);
	vtable_call_templates_[352] = vtable_call_template_0r<352, AttributesTable*>;										// AttributesArray * (*GetCopyStats)(struct Entity *);
	vtable_call_templates_[353] = vtable_call_template_0r<353, AttributesTable*>;										// AttributesArray * (*GetTempStats)(struct Entity *);
	vtable_call_templates_[354] = vtable_call_template_0r<354, AttributesTable*>;										// AttributesArray * (*GetBaseStats)(struct Entity *);
	vtable_call_templates_[355] = vtable_call_template_1<355,AttributesTable*>;											// void (*CopyAttributesToBaseStats)(struct Entity *, struct AttributesArray *);
	vtable_call_templates_[356] = vtable_call_template_0r<356,ActorStatus*>;											// ActorStatus * (*GetActorStatus)(struct Entity *);
	vtable_call_templates_[357] = vtable_call_template_1<357,ActorStatus*>;												// void (*CopyActorStatus)(struct Entity *, struct ActorStatus *);
	vtable_call_templates_[358] = vtable_call_template_1<358,float>;													// void (*AdjustAP)(struct Entity *, float);
	vtable_call_templates_[359] = vtable_call_template_1<359,int>;														// void (*AdjustAP2)(struct Entity *, int);
	vtable_call_templates_[360] = vtable_call_template_0r<360,float>;													// float (*GetCurrentAP)(struct Entity *);
	vtable_call_templates_[361] = vtable_call_template_0r<361,float>;													// float (*GetAPAvailablePercentage)(struct Entity *);
	vtable_call_templates_[362] = vtable_call_template_1<362,bool>;														// void (*ActorUnconsciousAPHandling)(struct Entity *, bool);
	vtable_call_templates_[363] = vtable_call_template_0r<363,bool>;													// bool (*IsMale)(struct Entity *);
	vtable_call_templates_[364] = vtable_call_template_0r<364,bool>;													// bool (*IsFemale)(struct Entity *);
	vtable_call_templates_[365] = vtable_call_template_1r1<365,wchar_t*>;												// FOTString * (*GetSexString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[366] = vtable_call_template_1r1<366,wchar_t*>;												// FOTString * (*GetEntityCharacterTypeString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[367] = vtable_call_template_3<367,AttributesTable*,int,float>;								// void (*AddTempBonuses)(struct Entity *, struct AttributesArray *, dword, float);
	vtable_call_templates_[368] = vtable_call_template_3<368, AttributesTable*, int, float>;							// void (*RemoveTempBonuses)(struct Entity *, struct AttributesArray *, dword, float);
	vtable_call_templates_[369] = vtable_call_template_3r<369,int,int,wchar_t**,int>;									// int (*IncreaseAttributeByRange)(struct Entity *, int, int, struct FOTString *);
	vtable_call_templates_[370] = vtable_call_template_1r<370,int,bool>;												// bool (*GrantXP)(struct Entity *, int);
	vtable_call_templates_[371] = vtable_call_template_0r<371,int>;														// int (*GetNPCLevel)(struct Entity *);
	vtable_call_templates_[372] = vtable_call_template_0<372>;															// void (*HandleXPForNewPlayerCharacterMaybe)(struct Entity *);
	vtable_call_templates_[373] = vtable_call_template_0<373>;															// void (*GiveXPForLevelUp)(struct Entity *);
	vtable_call_templates_[374] = vtable_call_template_0r<374,int>;														// int (*GetExpectedLevelForMyXP)(struct Entity *);
	vtable_call_templates_[375] = vtable_call_template_0r<375,int>;														// int (*XPAndValueRelatedCalc)(struct Entity *);
	vtable_call_templates_[376] = vtable_call_template_1<376,float>;														// void (*RadiateEntity)(struct Entity *, float);
	vtable_call_templates_[377] = vtable_call_template_1<377,float>;														// void (*PoisonEntity)(struct Entity *, float);
	vtable_call_templates_[378] = vtable_call_template_1<378,float>;													// void (*AddOverdosePoints)(struct Entity *, float);
	vtable_call_templates_[379] = vtable_call_template_1<379,float>;													// void (*MaybeHealOverTime)(struct Entity *, float);
	vtable_call_templates_[380] = vtable_call_template_0r<380,bool>;													// bool (*IsAlwaysFriend)(struct Entity *);
	vtable_call_templates_[381] = vtable_call_template_noop<381>; // void (*AdjustActorStatusBasedOnEffectChangeEtc)(struct Entity *, dword *, float);
	vtable_call_templates_[382] = vtable_call_template_0r<382,int>;														// int (*GetHP)(struct Entity *);
	vtable_call_templates_[383] = vtable_call_template_0r<383, bool>;													// bool (*HasAddiction)(struct Entity *);
	vtable_call_templates_[384] = vtable_call_template_0r<384, bool>;													// bool (*CheckOverdosed)(struct Entity *);
	vtable_call_templates_[385] = vtable_call_template_0r<385, bool>;													// bool (*IsInWithdrawal)(struct Entity *);
	vtable_call_templates_[386] = vtable_call_template_0r<386, bool>;													// bool (*IsRadiated)(struct Entity *);
	vtable_call_templates_[387] = vtable_call_template_0r<387, bool>;													// bool (*IsPoisoned)(struct Entity *);
	vtable_call_templates_[388] = vtable_call_template_0r<388,bool>;													// bool (*IsDrunk)(struct Entity *);
	vtable_call_templates_[389] = vtable_call_template_0r<389,float>;													// float (*GetWaypointPauseTime)(struct Entity *);
	vtable_call_templates_[390] = vtable_call_template_0r<390,int>;														// int (*GetWaypointIndex)(struct Entity *);
	vtable_call_templates_[391] = vtable_call_template_0r<391,int>;														// int (*AlwaysReturnMaxInt)(struct Entity *);
	vtable_call_templates_[392] = vtable_call_template_1r1<392,wchar_t*>;												// FOTString * (*GetNextWaypointList)(struct Entity *, struct FOTString *);
	vtable_call_templates_[393] = vtable_call_template_1r1<393,wchar_t*>;												// FOTString * (*GetWaypointActionString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[394] = vtable_call_template_0r<394, bool>;													// bool (*GetRepeat)(struct Entity *);
	vtable_call_templates_[395] = vtable_call_template_0r<395, bool>;													// bool (*GetWaypointUseDirection)(struct Entity *);
	vtable_call_templates_[396] = vtable_call_template_1<396,int>;														// void (*SetWaypointIndex)(struct Entity *, int);
	vtable_call_templates_[397] = vtable_call_template_1r1<397,Vector3>;												// LocationXYZFloat * (*GetActorTargetLocationMaybe)(struct Entity *, struct LocationXYZFloat *);
	vtable_call_templates_[398] = vtable_call_template_1<398,Vector3*>;													// void (*SetActorTargetLocationMaybe)(struct Entity *, struct LocationXYZFloat *);
	vtable_call_templates_[399] = vtable_call_template_1r1<399, Vector3>;												// LocationXYZFloat * (*GetAttackDir)(struct Entity *, struct LocationXYZFloat *);
	vtable_call_templates_[400] = vtable_call_template_1<400,int>;														// void (*SetGUISlot)(struct Entity *, int);
	vtable_call_templates_[401] = vtable_call_template_0r<401,int>;														// int (*GetGUISlot)(struct Entity *);
	vtable_call_templates_[402] = vtable_call_template_1r1<402,Inventory::InventoryStructType>;							// Inventory::ClassType * (*GetInventoryList)(struct Entity *, struct Inventory::ClassType *);
	vtable_call_templates_[403] = vtable_call_template_1r1<403,EntityID>;												// EntityID * (*GetInventoryID)(struct Entity *, struct EntityID *);
	vtable_call_templates_[404] = vtable_call_template_1<404,EntityID>;													// void (*SetInventoryID)(struct Entity *, struct EntityID);
	vtable_call_templates_[405] = vtable_call_template_0<405>;															// void (*ClearInventoryObject)(struct Entity *);
	vtable_call_templates_[406] = vtable_call_template_1r1<406,EntityID>;												// EntityID * (*GetUnk22EntityID)(struct Entity *, struct EntityID *);
	vtable_call_templates_[407] = vtable_call_template_1<407,EntityID>;													// void (*ResetUnk22Entity)(struct Entity *, struct EntityID);
	vtable_call_templates_[408] = vtable_call_template_1<408,bool>;														// void (*InitUnkFields)(struct Entity *, bool);
	vtable_call_templates_[409] = vtable_call_template_1r<409,EntityID,int>;											// int (*GetMinDmg)(struct Entity *, struct EntityID);
	vtable_call_templates_[410] = vtable_call_template_1r<410, EntityID, int>;											// int (*GetMaxDmg)(struct Entity *, struct EntityID);
	vtable_call_templates_[411] = vtable_call_template_1<411,int>;														// void (*SetWeaponModeByIndex)(struct Entity *, int);
	vtable_call_templates_[412] = vtable_call_template_0r<412,int>;														// int (*GetNumWeaponModes)(struct Entity *);
	vtable_call_templates_[413] = vtable_call_template_0r<413,Weapon::WeaponModeStructType*>;							// WeaponMode * (*GetActiveWeaponMode)(struct Entity *);
	vtable_call_templates_[414] = vtable_call_template_1r<414,bool,bool>;												// void (*SetNextWeaponMode)(struct Entity *, bool);
	vtable_call_templates_[415] = vtable_call_template_1r<415,bool,bool>;												// bool (*SetWeaponCalledShot)(struct Entity *, bool);
	vtable_call_templates_[416] = vtable_call_template_0r<416,bool>;													// bool (*WeaponCanUseCalledShot)(struct Entity *);
	vtable_call_templates_[417] = vtable_call_template_1r<417,int,bool>;												// bool (*CanWeaponAndAttackerUseCalledShot)(struct Entity *, dword);
	vtable_call_templates_[418] = vtable_call_template_0r<418,bool>;													// bool (*UseAmmoForCurrentMode)(struct Entity *);
	vtable_call_templates_[419] = vtable_call_template_0r<419,bool>;													// bool (*HaveEnoughAmmoAndCanShoot)(struct Entity *);
	vtable_call_templates_[420] = vtable_call_template_0r<420,bool>;													// bool (*WeaponHasNonNormalPrimaryDamageTypeMaybe)(struct Entity *);
	vtable_call_templates_[421] = vtable_call_template_0r<421,int>;														// int (*ExplodeHeldWeaponMaybe)(struct Entity *);
	vtable_call_templates_[422] = vtable_call_template_1r<422,EntityID,int>;											// int (*GetAPCost)(struct Entity *, struct EntityID);
	vtable_call_templates_[423] = vtable_call_template_3r1<423,InventoryActionResult,EntityID,EntityID>;				// UseItemResponse * (*WeaponLoadingAmmoRelated)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID);
	vtable_call_templates_[424] = vtable_call_template_3r1<424,EntityID,EntityID,EntityID>;								// EntityID * (*MaybeUnloadAmmoFromWeapon)(struct Entity *, struct EntityID *, struct EntityID, struct EntityID);
	vtable_call_templates_[425] = vtable_call_template_1r1<425,wchar_t*>;												// FOTString * (*GetAmmoTypeString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[426] = vtable_call_template_1r1<426, wchar_t*>;												// FOTString * (*GetWeaponPerkString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[427] = vtable_call_template_0r<427,float>;													// float (*GetWeaponDamageRadius)(struct Entity *);
	vtable_call_templates_[428] = vtable_call_template_1r1<428, wchar_t*>;												// FOTString * (*GetImpactFX)(struct Entity *, struct FOTString *);
	vtable_call_templates_[429] = vtable_call_template_1r1<429, wchar_t*>;												// FOTString * (*GetSoundFX)(struct Entity *, struct FOTString *);
	vtable_call_templates_[430] = vtable_call_template_1r1<430, wchar_t*>;												// FOTString * (*GetProjectileName)(struct Entity *, struct FOTString *);
	vtable_call_templates_[431] = vtable_call_template_1r1<431, wchar_t*>;												// FOTString * (*GetPrimaryDamageType)(struct Entity *, struct FOTString *);
	vtable_call_templates_[432] = vtable_call_template_1r1<432, wchar_t*>;												// FOTString * (*GetSecondaryDamageType)(struct Entity *, struct FOTString *);
	vtable_call_templates_[433] = vtable_call_template_1r1<433, wchar_t*>;												// FOTString * (*GetAmmoVariantString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[434] = vtable_call_template_0r<434,int>;														// int (*GetAmmoOrUsesLeft)(struct Entity *);
	vtable_call_templates_[435] = vtable_call_template_0r<435, int>;													// int (*GetMaxUsesOrAmmoCapacity)(struct Entity *);
	vtable_call_templates_[436] = vtable_call_template_0r<436,bool>;													// bool (*WeaponIsDestroyUser)(struct Entity *);
	vtable_call_templates_[437] = vtable_call_template_2r1<437,EntityID,Inventory::InventoryStructType*>;				// EntityID * (*FindSuitableAmmoInInventory)(struct Entity *, struct EntityID *, struct Inventory::ClassType *);
	vtable_call_templates_[438] = vtable_call_template_1r<438,EntityID*,bool>;											// bool (*ContainsEntity)(struct Entity *, struct EntityID *);
	vtable_call_templates_[439] = vtable_call_template_0r<439,float>;													// float (*GetEffectiveWeaponRangeByRangeType)(struct Entity *);
	vtable_call_templates_[440] = vtable_call_template_1r<440,EntityID,float>;											// float (*GetRange)(struct Entity *, struct EntityID);
	vtable_call_templates_[441] = vtable_call_template_0r<441,float>;													// float (*GetMinRange)(struct Entity *);
	vtable_call_templates_[442] = vtable_call_template_0r<442,int>;														// int (*GetAverageDamage)(struct Entity *);
	vtable_call_templates_[443] = vtable_call_template_0r<443,bool>;													// bool (*CanWeaponAOE)(struct Entity *);
	vtable_call_templates_[444] = vtable_call_template_0r<444, bool>;													// bool (*CanWeaponNotAOE)(struct Entity *);
	vtable_call_templates_[445] = vtable_call_template_2r<445,int,EntityID,bool>;										// bool (*CanFireWithRemainingAP)(struct Entity *, int, struct EntityID);
	vtable_call_templates_[446] = vtable_call_template_0r<446,bool>;													// bool (*IsActivated)(struct Entity *);
	vtable_call_templates_[447] = vtable_call_template_0r<447,int>;														// int (*GetFrequency)(struct Entity *);
	vtable_call_templates_[448] = vtable_call_template_2<448,EntityID,EntityID>;										// void (*ActivateTrapRelated)(struct Entity *, struct EntityID, struct EntityID);
	vtable_call_templates_[449] = vtable_call_template_0r<449,float>;													// float (*GetTrapTimeToDetonation)(struct Entity *);
	vtable_call_templates_[450] = vtable_call_template_2<450,int,int>;													// void (*SetTrapFrequencyCode)(struct Entity *, int, int);
	vtable_call_templates_[451] = vtable_call_template_0<451>;															// void (*DetonateTrap)(struct Entity *);
	vtable_call_templates_[452] = vtable_call_template_0r<452,bool>;													// bool (*HasActiveTrap)(struct Entity *);
	vtable_call_templates_[453] = vtable_call_template_1r<453,EntityID,bool>;											// bool (*TrapRelatedEitherDisarmOrTrigger)(struct Entity *, struct EntityID);
	vtable_call_templates_[454] = vtable_call_template_1r<454,EntityID,bool>;											// bool (*SetContainedTrap)(struct Entity *, struct EntityID);
	vtable_call_templates_[455] = vtable_call_template_3<455,EntityID,EntityID,bool>;									// void (*PlaceTrapMaybe)(struct Entity *, struct EntityID, struct EntityID, bool);
	vtable_call_templates_[456] = vtable_call_template_0<456>;															// void (*DisarmTrap)(struct Entity *);
	vtable_call_templates_[457] = vtable_call_template_1<457,EntityID>;													// bool (*ActivateOrBreakAlarm)(struct Entity *, struct EntityID);
	vtable_call_templates_[458] = vtable_call_template_0r<458,bool>;													// bool (*IsDoorOpen)(struct Entity *);
	vtable_call_templates_[459] = vtable_call_template_0r<459,bool>;													// bool (*IsDoorClosed)(struct Entity *);
	vtable_call_templates_[460] = vtable_call_template_0r<460, bool>;													// bool (*IsLocked)(struct Entity *);
	vtable_call_templates_[461] = vtable_call_template_1r<461,EntityID, bool>;											// bool (*CanDoorOpen)(struct Entity *, struct EntityID);
	vtable_call_templates_[462] = vtable_call_template_1r<462,EntityID, bool>;											// bool (*Vtable462DoorRelated)(struct Entity *, struct EntityID);
	vtable_call_templates_[463] = vtable_call_template_2<463,CombatMessage*, int>;										// void (*SetupCombatFXCombatMessageStruct)(struct Entity *, struct CombatMessage *, int);
	vtable_call_templates_[464] = vtable_call_template_6<464,int,int,int,int,int,int>;									// void (*Vtable464NoOp)(struct Entity *, int, int, int, int, int, int);
	vtable_call_templates_[465] = vtable_call_template_1<465,CombatMessage*>;											// void (*Vtable465CombatFX)(struct Entity *, struct CombatMessage *);
	vtable_call_templates_[466] = vtable_call_template_4<466, int, int, int, int>;										// void (*Vtable466NoOp)(struct Entity *, int, int, int, int);
	vtable_call_templates_[467] = vtable_call_template_1<467,Vector3*>;													// void (*Vtable467CombatFX)(struct Entity *, struct LocationXYZFloat *);
	vtable_call_templates_[468] = vtable_call_template_0<468>;															// void (*Vtable468CombatFX)(struct Entity *);
	vtable_call_templates_[469] = vtable_call_template_1<469, Vector3*>;												// void (*Vtable469CombatFX)(struct Entity *, struct LocationXYZFloat *);
	vtable_call_templates_[470] = vtable_call_template_1<470,CombatMessage*>;											// void (*CombatFXBloodSprayRelated)(struct Entity *, struct CombatMessage *);
	vtable_call_templates_[471] = vtable_call_template_2<471, CombatMessage*,int>;										// void (*CombatFXBloodTrailRelated)(struct Entity *, struct CombatMessage *, int);
	vtable_call_templates_[472] = vtable_call_template_1<472,EntityID>;													// void (*CombatFXBloodPoolRelated)(struct Entity *, struct EntityID);
	vtable_call_templates_[473] = vtable_call_template_2<473, Vector3*,int>;											// void (*SetupScenaryMove)(struct Entity *, struct LocationXYZFloat *, int);
	vtable_call_templates_[474] = vtable_call_template_6<474, int, int, int, int, int, int>;							// void (*Vtable474NoOp)(struct Entity *, int, int, int, int, int, int);
	vtable_call_templates_[475] = vtable_call_template_6<475, int, int, int, int, int, int>;							// void (*Vtable475NoOp)(struct Entity *, int, int, int, int, int, int);
	vtable_call_templates_[476] = vtable_call_template_2r<476,int,int,float>;											// float (*Vtable476ReturnFloatZero)(struct Entity *, int, int);
	vtable_call_templates_[477] = vtable_call_template_2r<477,int, Vector3*,float>;										// float (*Vtable477ActorMaybeWeaponImpactKnockdownRelated)(struct Entity *, int, struct LocationXYZFloat *);
	vtable_call_templates_[478] = vtable_call_template_0<478>;															// void (*Vtable478NoOp)(struct Entity *);
	vtable_call_templates_[479] = vtable_call_template_0r<479,bool>;													// bool (*Vtable479ReturnFalse)(struct Entity *);
	vtable_call_templates_[480] = vtable_call_template_3<480,int,int,int>;												// void (*Vtable480DestructsStuff)(struct Entity *, int, int, int);
	vtable_call_templates_[481] = vtable_call_template_0r<481,bool>;													// bool (*IsVehicleEmpty)(struct Entity *);
	vtable_call_templates_[482] = vtable_call_template_1r<482,EntityID,bool>;											// bool (*IsThisEntityTheDriver)(struct Entity *, struct EntityID);
	vtable_call_templates_[483] = vtable_call_template_1r<483, EntityID, bool>;											// bool (*IsThisEntityTheGunner)(struct Entity *, struct EntityID);
	vtable_call_templates_[484] = vtable_call_template_1r<484, EntityID, bool>;											// bool (*IsThisEntityInVehicle)(struct Entity *, struct EntityID);
	vtable_call_templates_[485] = vtable_call_template_1r1<485,EntityID>;												// EntityID * (*GetVehicleDriver)(struct Entity *, struct EntityID *);
	vtable_call_templates_[486] = vtable_call_template_1r1<486,EntityIDVector>;											// void (*GetVehiclePassengerVector)(struct Entity *, struct SpecialVectorToPointers *);
	vtable_call_templates_[487] = vtable_call_template_noop<487>; // bool (*VehicleBoundingBoxStuff1)(struct Entity *, struct BoundingBoxIntVehicleSpecial *);
	vtable_call_templates_[488] = vtable_call_template_0r<488,bool>;													// bool (*IsInReverse)(struct Entity *);
	vtable_call_templates_[489] = vtable_call_template_2<489,EntityID, Vector3*>;										// bool (*VehiclePassengerDisembarkAllowed)(struct Entity *, struct EntityID, struct LocationXYZFloat *);
	vtable_call_templates_[490] = vtable_call_template_1r1<490, Vector3>;												// LocationXYZFloat * (*SetExitPoint)(struct Entity *, struct LocationXYZFloat *);
	vtable_call_templates_[491] = vtable_call_template_0r<491,bool>;													// bool (*IsNotDeactivated)(struct Entity *);
	vtable_call_templates_[492] = vtable_call_template_0<492>;															// void (*ActivateActor)(struct Entity *);
	vtable_call_templates_[493] = vtable_call_template_0<493>;															// void (*DeactivateActor)(struct Entity *);
	vtable_call_templates_[494] = vtable_call_template_1<494,EntityID>;													// void (*Vtable494ActorEffectsRelated)(struct Entity *, struct EntityID);
	vtable_call_templates_[495] = vtable_call_template_1<495,EntityID>;													// void (*RemoveEffectID)(struct Entity *, struct EntityID);
	vtable_call_templates_[496] = vtable_call_template_1<496,wchar_t*>;													// void (*ApplyEffectsWithGivenTagName)(struct Entity *, struct FOTString *);
	vtable_call_templates_[497] = vtable_call_template_0r<497,bool>;													// bool (*BoundingBox2dSet)(struct Entity *);
	vtable_call_templates_[498] = vtable_call_template_1r1<498,EntityIDVector>;											// void (*GetListOfContainedEntities)(struct Entity *, struct SpecialVectorToPointers *);
	vtable_call_templates_[499] = vtable_call_template_2r<499,wchar_t**, ZoneDefinition*,bool>;							// bool (*ScriptConditionCheckMaybe)(struct Entity *, struct FOTString **, ZoneDefinition *);
	vtable_call_templates_[500] = vtable_call_template_1<500,wchar_t**>;												// void (*KillEntity)(struct Entity *, struct FOTString **);
	vtable_call_templates_[501] = vtable_call_template_1<501,int>;														// void (*SetActivationState)(struct Entity *, int);
	vtable_call_templates_[502] = vtable_call_template_0r<502,bool>;													// bool (*IsInActiveState)(struct Entity *);
	vtable_call_templates_[503] = vtable_call_template_0r<503, bool>;													// bool (*IsEnabledForScriptEvents)(struct Entity *);
	vtable_call_templates_[504] = vtable_call_template_1<504, bool>;													// void (*SetLockedState)(struct Entity *, bool);
	vtable_call_templates_[505] = vtable_call_template_0<505>;															// void (*Vtable505ActorCallsWorldForSomething)(struct Entity *);
	vtable_call_templates_[506] = vtable_call_template_1r1<506,wchar_t*>;												// FOTString * (*GetRandomSpeechIDString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[507] = vtable_call_template_1r1<507, wchar_t*>;												// FOTString * (*GetClickSpeechIDString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[508] = vtable_call_template_1<508, wchar_t**>;												// void (*SetRandomSpeechIDString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[509] = vtable_call_template_1<509, wchar_t**>;												// void (*SetClickSpeechIDString)(struct Entity *, struct FOTString *);
	vtable_call_templates_[510] = vtable_call_template_X510; // void (*ActOnControllerCommand)(struct Entity *, struct ControllerCommandStruct *);
	vtable_call_templates_[511] = vtable_call_template_0r<511,bool>;													// bool (*Vtable511ActorVehicle)(struct Entity *);
	vtable_call_templates_[512] = vtable_call_template_1<512,bool>;														// void (*SetHavingTurn)(struct Entity *, bool);
	vtable_call_templates_[513] = vtable_call_template_0r<513,bool>;													// bool (*IsHavingTurn)(struct Entity *);
	vtable_call_templates_[514] = vtable_call_template_1<514,bool>;														// void (*SetOverwatch)(struct Entity *, bool);
	vtable_call_templates_[515] = vtable_call_template_0r<515,bool>;													// bool (*IsOverwatch)(struct Entity *);
	vtable_call_templates_[516] = vtable_call_template_0<516>;															// void (*ResetBonusAC)(struct Entity *);
	vtable_call_templates_[517] = vtable_call_template_0<517>;															// void (*SetBonusACForHTHEvade)(struct Entity *);
	vtable_call_templates_[518] = vtable_call_template_0r<518,uint8_t>;													// byte (*GetUnk23)(struct Entity *);
	vtable_call_templates_[519] = vtable_call_template_1r<519,int,bool>;												// bool (*Vtable519EntityESSRelated)(struct Entity *, int);
	vtable_call_templates_[520] = vtable_call_template_1r<520,int,bool>;												// bool (*DoSomethingWithHP)(struct Entity *, dword);
	vtable_call_templates_[521] = vtable_call_template_noop<521>; // void (*Vtable521ALLSomethingWithAllFields)(struct Entity *, int *);
	vtable_call_templates_[522] = vtable_call_template_1<522,wchar_t*>;													// void (*Vtable522EntityDunno)(struct Entity *, struct FOTString *);
	vtable_call_templates_[523] = vtable_call_template_noop<523>; // classtype_vtable * (*GetClassType)(struct Entity *);
	vtable_call_templates_[524] = vtable_call_template_1<524,uint8_t>;													// Entity * (*ResetAndOrDestruct)(struct Entity *, byte);
	vtable_call_templates_[525] = vtable_call_template_0r<525,bool>;													// bool(*IsSomethingMDCRelated)(struct Actor *);
	vtable_call_templates_[526] = vtable_call_template_noop<526>; // void(*ActorPostureRelated)(struct Actor *, int *);
	vtable_call_templates_[527] = vtable_call_template_noop<527>; // float * (*RenderUnconscious)(struct Actor *, float *, int, float, bool, bool);
	vtable_call_templates_[528] = vtable_call_template_2r<528,int,int,float>;											// float(*Vtable528NoOp)(struct Actor *, int, int);
	vtable_call_templates_[529] = vtable_call_template_0<529>;															// void(*AdjustRadLevelFromRadPoints)(struct Actor *);
	vtable_call_templates_[530] = vtable_call_template_1<530,int>;														// void(*ApplyRadLevelPenalties)(struct Actor *, int);
	vtable_call_templates_[531] = vtable_call_template_0<531>;															// void(*UndoExistingRadLevelPenalties)(struct Entity *);
	vtable_call_templates_[532] = vtable_call_template_2<532,float,bool>;												// void(*PoisonEntity2)(struct Entity *, float, bool);
	vtable_call_templates_[533] = vtable_call_template_0<533>;															// void(*ApplyPoisonDamage)(struct Actor *);
	vtable_call_templates_[534] = vtable_call_template_0<534>;															// void(*ApplyOverdoseDamage)(struct Actor *);
	vtable_call_templates_[535] = vtable_call_template_2r<535,int,int,int>;												// int(*ContestedSkillRollMaybe)(struct Actor *, int, int);


}

int vtable_call_template_X32(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void(__thiscall *FXNType)(void*, float*);
	FXNType* fxn = (FXNType*)(vtable_addr + 32 * sizeof(size_t));

	float param[12];
	if (lua_istable(l, 3))
	{
		for (int i = 0; i < 12; i++)
		{
			lua_rawgeti(l, 3, i+1);
			if (lua_isnumber(l, -1))
			{
				param[i] = (float)lua_tonumber(l, -1);
			}
			else
			{
				param[i] = 0.0f;
			}
			lua_pop(l, 1);
		}
	}
	(*fxn)(ent, param);

	return 0;
}

int vtable_call_template_X510(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void(__thiscall *FXNType)(void*, ControllerCommandStruct*);
	FXNType* fxn = (FXNType*)(vtable_addr + 510 * sizeof(size_t));

	ControllerCommandStruct param;
	if (lua_istable(l, 3))
	{
		param.type = (uint8_t)LuaHelper::GetTableInteger(l, 3, "type");
		param.entity_id_1 = LuaHelper::RetrieveFromTable<EntityID>(l, 3, "entity1");
		param.entity_id_2 = LuaHelper::RetrieveFromTable<EntityID>(l, 3, "entity2");
		param.loc = LuaHelper::RetrieveFromTable<Vector3>(l, 3, "loc");
		param.param = (int32_t)LuaHelper::GetTableInteger(l, 3, "param");
		param.action_entity_halfid = Entity::GetEntityByPointer(ent)->GetID().id;
	}
	(*fxn)(ent, &param);

	return 0;
}
// Original vtable definitnion follows:

/*
	void (*GetRenderLightLevel)(struct Entity *, struct RGBColor *, struct LocationXYZFloat *, float);
	void (*RenderObjectMaybe)(struct Entity *, void *, int *);
	void (*InitDefaultEntityFields)(struct Entity *);
	bool (*SetAnimationType)(struct Entity *, struct FOTString *);
	bool (*UpdateSprite)(struct Entity *, struct FOTString **);
	void (*Vtable5NoOp)(struct Entity *);
	void (*InitEntityAfterSpawn)(struct Entity *);
	void (*PrepareEntity)(struct Entity *);
	void (*EraseInventory)(struct Entity *);
	void (*MakeFreshCharacter)(struct Entity *);
	void (*DestructEntity)(struct Entity *);
	void (*SetDoFade)(struct Entity *);
	void (*AliasToDestructEntity)(struct Entity *);
	void (*SomethingWithBoundingBoxAndCollision)(struct Entity *);
	void (*FlagCaptureFunction1)(struct Entity *);
	void (*AttachNewFileToActor)(struct Entity *, void *);
	void (*AttachNewFileToActor2)(struct Entity *);
	void (*SomethingNewSpawnRelated)(struct Entity *);
	void (*Vtable18EquipRelated)(struct Entity *, struct EntityID);
	void (*AdjustSpriteOffsetForRotatingDoor)(struct Entity *);
	void (*FinishedAnimationSequenceMaybe)(struct Entity *);
	void (*Vtable21NoOp)(struct Entity *);
	void (*Vtable22NoOp)(struct Entity *);
	void (*Vtable23NoOp)(struct Entity *);
	float (*RenderRelated)(struct Entity *, void *, int *);
	void (*CheckThinkTimers)(struct Entity *);
	void (*DoEngineThink)(struct Entity *);
	void (*DoGameThink)(struct Entity *);
	void (*SoundRelated1)(struct Entity *, int);
	FOTString * (*GetSoundType)(struct Entity *, struct FOTString *);
	void (*SetRicochetSoundCounters)(struct Entity *, int);
	void (*SetParameterToZero)(struct Entity *, int *);
	void (*InitLightParameters)(struct Entity *, float *);
	void (*SetAttached)(struct Entity *, struct EntityID);
	void (*SetCustomName)(struct Entity *, struct FOTString *);
	FOTString * (*GetCustomName)(struct Entity *, struct FOTString *);
	FOTString * (*GetDisplayName)(struct Entity *, struct FOTString *);
	FOTString * (*GetHealthStatusString)(struct Entity *, struct FOTString *);
	FOTString * (*Vtable38NoOpEmptyStringRet)(struct Entity *, struct FOTString *);
	FOTString * (*GetUseActionName)(struct Entity *, struct FOTString *);
	bool (*CanBeHeldInHand)(struct Entity *);
	bool (*IsTimerMine)(struct Entity *);
	bool (*IsTriggerTrap)(struct Entity *);
	bool (*IsCollectable)(struct Entity *);
	bool (*IsSingleUseItem)(struct Entity *);
	bool (*IsSkillConsumable)(struct Entity *);
	bool (*IsArmour)(struct Entity *);
	bool (*IsWeapon)(struct Entity *);
	bool (*IsAmmo)(struct Entity *);
	bool (*IsKey)(struct Entity *);
	bool (*IsContainer)(struct Entity *);
	bool (*IsUnarmedAttack)(struct Entity *);
	bool (*IsMeleeAttack)(struct Entity *);
	bool (*IsBaseAI)(struct Entity *);
	bool (*IsController)(struct Entity *);
	bool (*IsActor)(struct Entity *);
	bool (*IsActorUnk20Set)(struct Entity *);
	bool (*IsLight)(struct Entity *);
	bool (*IsVehicle)(struct Entity *);
	bool (*IsVehicleController)(struct Entity *);
	bool (*IsVehicleWeapon)(struct Entity *);
	bool (*CanBeActivated)(struct Entity *);
	bool (*IsDeadOrDestroyedMaybe)(struct Entity *);
	bool (*IsAttachedBaseAIMaybe)(struct Entity *);
	bool (*IsLiving)(struct Entity *);
	bool (*IsTrap)(struct Entity *);
	bool (*IsSentryAI)(struct Entity *);
	bool (*IsGeiger)(struct Entity *);
	bool (*IsWaypoint)(struct Entity *);
	bool (*IsSpawnpoint)(struct Entity *);
	bool (*IsAlarm)(struct Entity *);
	bool (*IsCombatFX)(struct Entity *);
	bool (*IsSwitch)(struct Entity *);
	bool (*IsDoor)(struct Entity *);
	bool (*IsGunTurret)(struct Entity *);
	bool (*IsBrainJar)(struct Entity *);
	bool (*IsBreakableType)(struct Entity *);
	bool (*IsCaptureFlag)(struct Entity *);
	bool (*IsTrapDoor)(struct Entity *);
	bool (*IsCyborgGeneral)(struct Entity *);
	bool (*IsPopupTurret)(struct Entity *);
	bool (*IsNotCaptureInvItem)(struct Entity *);
	FOTString * (*RetTypeStringForCarryableEntity)(struct Entity *, struct FOTString *);
	UseItemResponse * (*MaybeUseEntity)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID);
	EntityID * (*SplitEntity)(struct Entity *, struct EntityID *, dword);
	bool (*MergeEntities)(struct Entity *, struct EntityID, dword);
	bool (*MaybeCheckingIfMergeableAndMatching)(struct Entity *, struct EntityID);
	bool (*CanBeRemoved)(struct Entity *);
	void (*SetCanBeRemovedFlag)(struct Entity *, bool);
	void (*SetBeenAppliedFlag)(struct Entity *, bool);
	bool (*IsUnique)(struct Entity *);
	void (*SetIsUniqueFlag)(struct Entity *, bool);
	bool (*MeetRequirementsCheck)(struct Entity *, struct AttributesArray *);
	Stats * (*GetItemSPECIALRequired)(struct Entity *);
	void (*ApplyEquip)(struct Entity *, struct EntityID, int);
	void (*UnApply)(struct Entity *, struct EntityID, dword);
	void (*SetTrapHolder)(struct Entity *, struct EntityID);
	void (*ClearHolder)(struct Entity *, struct EntityID);
	int (*GetCount)(struct Entity *);
	int (*GetCountNonLootable)(struct Entity *);
	bool (*IsLootable)(struct Entity *);
	void (*SetUnk25EntityID)(struct Entity *, struct EntityID);
	int (*GetValueBaseOfItems)(struct Entity *);
	int (*GetCollectableRankReq)(struct Entity *);
	float (*GetWeight)(struct Entity *);
	void (*SetCount)(struct Entity *, int);
	int (*SetNonLootableCount)(struct Entity *, int);
	bool (*BreakEntity)(struct Entity *);
	bool (*IsBroken)(struct Entity *);
	bool (*RepairEntity)(struct Entity *);
	int (*GetTrapTotalDifficulty)(struct Entity *);
	void (*Vtable111Controller)(struct Entity *);
	bool (*Vtable112Controller)(struct Entity *, struct EntityID);
	bool (*Vtable113Controller)(struct Entity *, struct EntityID);
	void (*SetControllerMoveTypePriority)(struct Entity *, dword);
	void (*SetControllerWorkingZone)(struct Entity *, struct ZoneDefinition *);
	void (*SetControllerRetreatZone)(struct Entity *, struct ZoneDefinition *);
	void (*SetControllerMoveTargetTag)(struct Entity *, struct FOTString *);
	void (*SetControllerAggression)(struct Entity *, int);
	void (*SetControllerCommunicate)(struct Entity *, int);
	void (*SetControllerMobility)(struct Entity *, int);
	void (*SetControllerRandomMove)(struct Entity *, bool, byte);
	int (*GetControllerMoveTypePriority)(struct Entity *);
	void (*GetControllerWorkingZone)(struct Entity *, struct ZoneDefinition *);
	void (*GetControllerRetreatZone)(struct Entity *, struct ZoneDefinition *);
	FOTString * (*GetControllerMoveTargetTag)(struct Entity *, struct FOTString *);
	int (*GetControllerAggression)(struct Entity *);
	int (*GetControllerCommunicate)(struct Entity *);
	int (*GetControllerMobility)(struct Entity *);
	bool (*GetControllerRandomMove)(struct Entity *);
	FOTString * (*GetControllerType)(struct Entity *, struct FOTString *);
	void (*SetControllerType)(struct Entity *, struct FOTString *);
	void (*DeactivateController)(struct Entity *);
	void (*SetControllerWaypointModeAndMoveTargetTag)(struct Entity *, struct FOTString *);
	void (*SetControllerStationary)(struct Entity *);
	void (*SetControllerToPerimeterZone)(struct Entity *);
	void (*SetControllerToFollow)(struct Entity *, int);
	void (*SetControllerToFlee)(struct Entity *);
	void (*ControllerDoSomethingWithUnitList)(struct Entity *);
	void (*SetControllerUnknown4String)(struct Entity *, struct FOTString *);
	FOTString * (*GetControllerUnknown4String)(struct Entity *, struct FOTString *);
	void (*ControllerWakeUpUnitList)(struct Entity *, int);
	void (*ControllerIssueTauntForEntity)(struct Entity *, struct EntityID, int);
	float (*ControllerWaypointMaybeTimingRelated)(struct Entity *, struct EntityID);
	FOTString * (*ControllerGetUnitWaypointActionString)(struct Entity *, struct FOTString *, struct EntityID);
	bool (*ControllerWaypointCheckingRepeatFlag)(struct Entity *, struct EntityID);
	float (*ControllerMoveRelated)(struct Entity *, struct EntityID, struct LocationXYZFloat *);
	EntityID * (*Vtable147Controller)(struct Entity *, struct EntityID *, struct EntityID, bool);
	EntityID * (*Vtable148Controller)(struct Entity *, struct EntityID *, struct EntityID, struct FOTString *);
	float (*ControllerGetFromUnknownList2)(struct Entity *, struct EntityID, int);
	bool (*Vtable150Controller)(struct Entity *);
	float (*Vtable151NoOpReturnFloat0)(struct Entity *, int, int);
	float (*Vtable152NoOpReturnFloat0)(struct Entity *, int, int);
	float (*ControllerMoreMoveRelated)(struct Entity *, struct EntityID, struct LocationXYZFloat *);
	LocationXYZFloat * (*ControllerWaypointStuff)(struct Entity *, struct LocationXYZFloat *, struct EntityID);
	longlong (*ControllerSomethingAboutTookDamage)(struct Entity *, struct EntityID);
	void (*Vtable156Controller)(struct Entity *, struct EntityID, struct LocationXYZFloat *);
	bool (*ControllerUsesWorkingZoneAndMoveType)(struct Entity *, struct EntityID, struct LocationXYZFloat *, struct FOTString *);
	bool (*BaseAINeedsToSwitchTargetMaybe)(struct Entity *);
	void (*BaseAIEngineThinkRelated)(struct Entity *, int *);
	void (*SetBaseAIController)(struct Entity *, struct EntityID);
	EntityID * (*GetBaseAIController)(struct Entity *, struct EntityID *);
	void (*Vtable162BaseAI)(struct Entity *, int *);
	FOTString * (*GetBaseAINature)(struct Entity *, struct FOTString *);
	void (*StopMovement)(struct Entity *, bool);
	EntityID * (*BaseAIGetCurrentTarget)(struct Entity *, struct EntityID *);
	bool (*BaseAIHasATargetMaybe)(struct Entity *);
	bool (*IsSleeping)(struct Entity *);
	void (*SetSleepingFlag)(struct Entity *, bool);
	void (*SetBaseAIUnknown7)(struct Entity *, float);
	void (*SetBaseAIUnknown7ToRealTimeTimerValue)(struct Entity *, int);
	void (*BaseAIStopMovementIfHighestDesireSet)(struct Entity *);
	bool (*BaseAIMaybeOpenDoorRelated)(struct Entity *, struct BaseAIEntityPlus4Struct *);
	void (*Vtable173BaseAI)(struct Entity *, struct BaseAIEntityPlus4Struct *);
	void (*Vtable174BaseAI)(struct Entity *, struct EntityID);
	FOTString * (*GetBaseAITauntTypeString)(struct Entity *, struct FOTString *);
	int (*GetBaseAIHighestDesireRoundedUp)(struct Entity *);
	void (*ClearSentryAITargets)(struct Entity *);
	void (*SetSentryMode)(struct Entity *, int);
	int (*GetSentryMode)(struct Entity *);
	void (*SetSentryAccuracyTrigger)(struct Entity *, int);
	int (*GetSentryAccuracyTrigger)(struct Entity *);
	void (*SetSentryLocation)(struct Entity *, int);
	int (*GetSentryLocation)(struct Entity *);
	void (*SetSentryUserTarget)(struct Entity *, struct EntityID, int);
	void (*SentryAIMaybeCheckingTargets)(struct Entity *, struct EntityID);
	ActorSPECIALCopyAndFields * (*GetActorCopyofSPECIALStats)(struct Entity *);
	void (*CopySomeActorFields)(struct Entity *, struct ActorSPECIALCopyAndFields *);
	bool (*CopySomeActorFields2)(struct Entity *, struct ActorSPECIALCopyAndFields *, void *);
	bool (*Vtable189Actor)(struct Entity *, void *, void *, void *);
	int (*GetActorStartIndex)(struct Entity *);
	void (*SetActorStartIndex)(struct Entity *, int);
	float (*GetActorFractionOfHitPointsLost)(struct Entity *);
	float (*CheckBestHPRatioIfUseHealingItemMaybe)(struct Entity *);
	float (*ActorHasItemToPermBoostStats)(struct Entity *);
	float (*ActorHasItemToPermBoostStats2)(struct Entity *);
	float (*GetActorPoisonPointsTimes15Cap90)(struct Entity *);
	float (*ActorLooksLikeNoOp)(struct Entity *);
	float (*GetActorRadPointsDivided10Cap90)(struct Entity *);
	float (*ActorCheckItemsForRadHealing)(struct Entity *);
	void (*MakeConsumableGiveRadiation)(struct Entity *, int, int);
	void (*MakeConsumableGivePoison)(struct Entity *, int, int);
	void (*ApplyBonusesToAttachedActor)(struct Entity *);
	float (*GetExpectedHPRatioIfConsumableUsed)(struct Entity *, struct EntityID);
	float (*DoesConsumablePermBoostSPEAL)(struct Entity *, struct EntityID);
	float (*Vtable205NoOpRetZeroFloat)(struct Entity *);
	float (*GetRatioOfRadPointHealingForConsumable)(struct Entity *, struct EntityID);
	bool (*Vtable207ActorMaybeCheckForPlayerCharacter)(struct Entity *);
	FOTString * (*GetRankString)(struct Entity *, struct FOTString *);
	int (*GetActorRankID)(struct Entity *);
	int (*GetRanksGivenEntityHasAboveThis)(struct Entity *, struct EntityID);
	FOTString * (*GetReputationString)(struct Entity *, struct FOTString *);
	int (*GetCappedReputation)(struct Entity *);
	int (*GetMissionsCom)(struct Entity *);
	void (*IncrementMissionsCom)(struct Entity *, int);
	void (*AddToReputation)(struct Entity *, int);
	bool (*IsGlowing)(struct Entity *);
	int (*GetActorAge)(struct Entity *, struct Traits *);
	void (*SetCurrentActionStringMaybe)(struct Entity *, struct FOTString *, struct FOTString *);
	FOTString * (*GetCurrentActionString)(struct Entity *, struct FOTString *);
	bool (*IsIdle)(struct Entity *);
	bool (*IsMoving_WalkRunClimbLadderOrDriving)(struct Entity *);
	bool (*IsClimbing)(struct Entity *);
	bool (*IsIdleOrWalkRun)(struct Entity *);
	bool (*HasFallen)(struct Entity *);
	void (*Vtable225ThinkRelated)(struct Entity *);
	bool (*CanMove)(struct Entity *);
	bool (*CanRun)(struct Entity *);
	void (*AdjustStanceDownward)(struct Entity *, bool);
	void (*AdjustStanceUpward)(struct Entity *, bool);
	bool (*IsHumanoidOrDog)(struct Entity *);
	float (*MaxMovementSpeedMaybe)(struct Entity *);
	bool (*IsEncumbered)(struct Entity *);
	bool (*IsStanding)(struct Entity *);
	bool (*IsCrouching)(struct Entity *);
	bool (*IsProne)(struct Entity *);
	int (*GetSelectedStance)(struct Entity *);
	void (*ActoSetSelectedStance)(struct Entity *, struct FOTString *);
	bool (*IsHidden)(struct Entity *);
	FOTString * (*GetPostureString)(struct Entity *, struct FOTString *);
	void (*SetSneakState)(struct Entity *, bool);
	bool (*IsBurrowed)(struct Entity *);
	void (*SetBurrowingState)(struct Entity *, bool);
	bool (*RollAgainstStat)(struct Entity *, struct FOTString *, int);
	UseItemResponse * (*ApplySkillToTargetEntity)(struct Entity *, struct UseItemResponse *, struct FOTString *, struct EntityID, struct EntityID);
	FOTString * (*GetSkillStringBasedOnUnk25c)(struct Entity *, struct FOTString *);
	EntityID * (*GetActorUnk2EntityID)(struct Entity *, struct EntityID *);
	int (*GetAdjustedPilotSkill)(struct Entity *);
	bool (*DoesSkillStringMatchHelperItem)(struct Entity *, struct FOTString *);
	bool (*IsValidTargetForSkill)(struct Entity *, struct FOTString *);
	bool (*IsValidTargetForDefaultAction)(struct Entity *, struct EntityID, struct EntityID);
	bool (*IsGeneral)(struct Entity *);
	bool (*IsRecruitMaster)(struct Entity *);
	bool (*IsQuartermaster)(struct Entity *);
	bool (*CanBarterWith)(struct Entity *);
	bool (*BartersWithBOSScript)(struct Entity *);
	bool (*CanGambleWith)(struct Entity *);
	int (*GetRepairObjectDifficulty)(struct Entity *);
	float (*GetScaledKnockdownChance)(struct Entity *, float);
	void (*ActorKnockoutRelated)(struct Entity *);
	UseItemResponse * (*ActorInjureLocation)(struct Entity *, struct UseItemResponse *, int, int);
	UseItemResponse * (*ActorKnockout)(struct Entity *, struct UseItemResponse *, int, int, int);
	UseItemResponse * (*ActorDisarm)(struct Entity *, struct UseItemResponse *, int, int);
	UseItemResponse * (*ActorBreakWeaponMaybe)(struct Entity *, struct UseItemResponse *, int, int);
	UseItemResponse * (*ActorCriticalMissHitSelfMaybe)(struct Entity *, struct UseItemResponse *, int);
	UseItemResponse * (*ActorCriticalMissHitSelfMaybe2)(struct Entity *, struct UseItemResponse *, int);
	void (*StunActor)(struct Entity *, int);
	bool (*IsRightLegInjured)(struct Entity *, bool);
	bool (*IsLeftLegInjured)(struct Entity *, bool);
	bool (*IsRightArmInjured)(struct Entity *, bool);
	bool (*IsLeftArmInjured)(struct Entity *, bool);
	bool (*IsBlinded)(struct Entity *, bool);
	bool (*IsWinded)(struct Entity *, bool);
	bool (*IsImmobilized)(struct Entity *, bool);
	bool (*IsConcussed)(struct Entity *, bool);
	bool (*IsStunned)(struct Entity *, bool);
	bool (*IsUnconscious)(struct Entity *);
	bool (*IsBandagedStatus)(struct Entity *);
	int (*GetActorMovementCostToLocation)(struct Entity *, struct LocationXYZFloat *);
	bool (*IsActorUnk23SetPlusSomeOtherFieldFalse)(struct Entity *);
	void (*Move)(struct Entity *);
	void (*SetDestination)(struct Entity *, struct LocationXYZFloat *, int, float, float, bool);
	LocationXYZFloat * (*GetLocationBasedOnUnk23)(struct Entity *, struct LocationXYZFloat *);
	EntityID * (*ActorGetUnk24bEntityID)(struct Entity *, struct EntityID *);
	bool (*ActorSomethingTouchSearch)(struct Entity *, struct LocationXYZFloat *, bool);
	bool (*ActorSomethingTouchSearch2)(struct Entity *, struct LocationXYZFloat *, bool);
	bool (*ActorSomethingTouchSearch3)(struct Entity *, struct LocationXYZFloat *, bool);
	bool (*Vtable287NoOpRet0)(struct Entity *);
	bool (*IsLegalTarget)(struct Entity *);
	bool (*ActorIsNotLegalTargetAndUnk1Set)(struct Entity *);
	bool (*IsVehicleDisabled)(struct Entity *);
	UseItemResponse * (*ActorPickupItem)(struct Entity *, struct UseItemResponse *, struct EntityID, int);
	void (*Vtable292Actor)(struct Entity *);
	UseItemResponse * (*ReturnUseItemResponseType0x40)(struct Entity *, struct UseItemResponse *);
	HitChanceStruct * (*GetSomeKindOfFlagForMouseOverTarget)(struct Entity *, struct HitChanceStruct *, struct EntityID, bool);
	HitChanceStruct * (*GetSomeKindOfFlagForMouseOverTarget2)(struct Entity *, struct HitChanceStruct *, struct LocationXYZFloat *);
	HitChanceStruct * (*BeginAttackAnEntity)(struct Entity *, struct HitChanceStruct *, struct EntityID, int, int);
	HitChanceStruct * (*BeginAttackALocation)(struct Entity *, struct HitChanceStruct *, struct LocationXYZFloat *);
	void (*ApplyAttackResultToEntity)(struct Entity *, struct CombatMessage *);
	void (*ShotAtMissed)(struct Entity *, struct CombatMessage *);
	void (*ApplyDamage)(struct Entity *, struct EntityID, int, int, int, struct FOTString *, int);
	void (*EntityWasKilled)(struct Entity *, struct EntityID, int, int, struct FOTString *);
	void (*IncrementKillCount)(struct Entity *, struct EntityID);
	void (*CopyActorKillList)(struct Entity *, void *);
	bool (*Vtable304NoOpRet0)(struct Entity *);
	void (*ResolveCombatMessageForAttacker)(struct Entity *, int *);
	EntityID * (*GetIDOfEquippedItemInActiveSlot)(struct Entity *, struct EntityID *);
	EntityID * (*GetEntityIDOfNonEquippedWeapon)(struct Entity *, struct EntityID *);
	UseItemResponse * (*ReloadActiveWeaponWithAnyAmmo)(struct Entity *, struct UseItemResponse *);
	UseItemResponse * (*ReloadWeaponWithSelectedAmmo)(struct Entity *, struct UseItemResponse *, int, struct EntityID);
	bool (*ActorCanAttackWithActiveWeapon)(struct Entity *);
	bool (*Vtable311ActorCheckingLastTimeRun)(struct Entity *);
	void (*InitActorCombatMessages)(struct Entity *);
	void (*LevelUp)(struct Entity *, int);
	EntityID * (*GetCombatMessage2TargetID)(struct Entity *, struct EntityID *);
	int (*GetPlayerBuyPriceForItem)(struct Entity *, struct EntityID, struct EntityID);
	int (*GetPlayerSellBackPriceForItem)(struct Entity *, struct EntityID, struct EntityID);
	int (*GetCountOfMatchingItems)(struct Entity *, struct EntityID);
	UseItemResponse * (*EquipItem)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	UseItemResponse * (*RemoveItem)(struct Entity *, struct UseItemResponse *, int, struct EntityID *);
	UseItemResponse * (*RemoveItemIfEquipped)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *);
	UseItemResponse * (*AddToInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	UseItemResponse * (*RemoveFromInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	UseItemResponse * (*UnequipItemFromSlotAndPutInInventory)(struct Entity *, struct UseItemResponse *, int, struct EntityID *);
	UseItemResponse * (*UnequipItemByIDAndReturnToInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *);
	UseItemResponse * (*EquipItemFromInventoryToSlot)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int);
	UseItemResponse * (*RemoveFromInventory2)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int);
	UseItemResponse * (*ActorSomethingRelatedToPickup)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	UseItemResponse * (*RemoveItemFromSlot2)(struct Entity *, struct UseItemResponse *, int, struct EntityID *);
	UseItemResponse * (*RemoveItemIfEquipped2)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *);
	UseItemResponse * (*PickupItemAndEquipInSlot)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int, int);
	UseItemResponse * (*RemoveItemFromEquippedThenInventory)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int);
	UseItemResponse * (*RemoveItemFromInventoryThenEquipped)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID *, int);
	UseItemResponse * (*Vtable333SomethingForApplyingItem)(struct Entity *, struct UseItemResponse *, int, struct EntityID *);
	void (*SetActiveWeaponHand)(struct Entity *, char);
	void (*ToggleActiveHand)(struct Entity *);
	bool (*CheckIfTargetInReachableRange)(struct Entity *, struct EntityID);
	byte (*GetSelectedHand)(struct Entity *);
	byte (*GetUnselectedHand)(struct Entity *);
	EntityID * (*GetIDofEquippedItemInSlot)(struct Entity *, struct EntityID *, int);
	UseItemResponse * (*Vtable340InventoryCombineRelatedMaybe)(struct Entity *, struct UseItemResponse *, struct EntityID);
	bool (*PlayerEquippedItemWithTagName)(struct Entity *, struct FOTString *);
	bool (*IsItemIDEquipped)(struct Entity *, struct EntityID);
	bool (*ContainsItemWithTagName)(struct Entity *, struct FOTString *);
	bool (*ContainsItemByID)(struct Entity *, struct EntityID);
	bool (*EquippedOrContainsItemWithTagName)(struct Entity *, struct FOTString *);
	bool (*FindIfEquippedOrCarriedItemByID)(struct Entity *, struct EntityID);
	UseItemResponse * (*Vtable347ActorSomethingAboutUsingItem)(struct Entity *, struct UseItemResponse *, word, word, byte);
	UseItemResponse * (*CanUseItem)(struct Entity *, struct UseItemResponse *, struct EntityID);
	UseItemResponse * (*CanReachInteractable)(struct Entity *, struct UseItemResponse *, struct EntityID);
	FOTString * (*GetSizeStringMaybe)(struct Entity *, struct FOTString *);
	AttributesArray * (*GetCurrentStats)(struct Entity *);
	AttributesArray * (*GetCopyStats)(struct Entity *);
	AttributesArray * (*GetTempStats)(struct Entity *);
	AttributesArray * (*GetBaseStats)(struct Entity *);
	void (*CopyAttributesToBaseStats)(struct Entity *, struct AttributesArray *);
	ActorStatus * (*GetActorStatus)(struct Entity *);
	void (*CopyActorStatus)(struct Entity *, struct ActorStatus *);
	void (*AdjustAP)(struct Entity *, float);
	void (*AdjustAP2)(struct Entity *, int);
	float (*GetCurrentAP)(struct Entity *);
	float (*GetAPAvailablePercentage)(struct Entity *);
	void (*ActorUnconsciousAPHandling)(struct Entity *, bool);
	bool (*IsMale)(struct Entity *);
	bool (*IsFemale)(struct Entity *);
	FOTString * (*GetSexString)(struct Entity *, struct FOTString *);
	FOTString * (*GetEntityCharacterTypeString)(struct Entity *, struct FOTString *);
	void (*AddTempBonuses)(struct Entity *, struct AttributesArray *, dword, float);
	void (*RemoveTempBonuses)(struct Entity *, struct AttributesArray *, dword, float);
	int (*IncreaseAttributeByRange)(struct Entity *, int, int, struct FOTString *);
	bool (*GrantXP)(struct Entity *, int);
	int (*GetNPCLevel)(struct Entity *);
	void (*HandleXPForNewPlayerCharacterMaybe)(struct Entity *);
	void (*GiveXPForLevelUp)(struct Entity *);
	int (*GetExpectedLevelForMyXP)(struct Entity *);
	int (*XPAndValueRelatedCalc)(struct Entity *);
	void (*RadiateEntity)(struct Entity *, dword);
	void (*PoisonEntity)(struct Entity *, dword);
	void (*AddOverdosePoints)(struct Entity *, float);
	void (*MaybeHealOverTime)(struct Entity *, float);
	bool (*IsAlwaysFriend)(struct Entity *);
	void (*AdjustActorStatusBasedOnEffectChangeEtc)(struct Entity *, dword *, float);
	int (*GetHP)(struct Entity *);
	bool (*HasAddiction)(struct Entity *);
	bool (*CheckOverdosed)(struct Entity *);
	bool (*IsInWithdrawal)(struct Entity *);
	bool (*IsRadiated)(struct Entity *);
	bool (*IsPoisoned)(struct Entity *);
	bool (*IsDrunk)(struct Entity *);
	float (*GetWaypointPauseTime)(struct Entity *);
	int (*GetWaypointIndex)(struct Entity *);
	int (*AlwaysReturnMaxInt)(struct Entity *);
	FOTString * (*GetNextWaypointList)(struct Entity *, struct FOTString *);
	FOTString * (*GetWaypointActionString)(struct Entity *, struct FOTString *);
	bool (*GetRepeat)(struct Entity *);
	bool (*GetWaypointUseDirection)(struct Entity *);
	void (*SetWaypointIndex)(struct Entity *, int);
	LocationXYZFloat * (*GetActorTargetLocationMaybe)(struct Entity *, struct LocationXYZFloat *);
	void (*SetActorTargetLocationMaybe)(struct Entity *, struct LocationXYZFloat *);
	LocationXYZFloat * (*GetAttackDir)(struct Entity *, struct LocationXYZFloat *);
	void (*SetGUISlot)(struct Entity *, int);
	int (*GetGUISlot)(struct Entity *);
	Inventory * (*CopyInventory)(struct Entity *, struct Inventory::ClassType *);
	EntityID * (*GetInventoryID)(struct Entity *, struct EntityID *);
	void (*SetInventoryID)(struct Entity *, struct EntityID);
	void (*ClearInventoryObject)(struct Entity *);
	EntityID * (*GetUnk22EntityID)(struct Entity *, struct EntityID *);
	void (*ResetUnk22Entity)(struct Entity *, struct EntityID);
	void (*InitUnkFields)(struct Entity *, bool);
	int (*GetMinDmg)(struct Entity *, struct EntityID);
	int (*GetMaxDmg)(struct Entity *, struct EntityID);
	void (*SetWeaponModeByIndex)(struct Entity *, int);
	int (*GetNumWeaponModes)(struct Entity *);
	WeaponMode * (*GetActiveWeaponMode)(struct Entity *);
	void (*SetNextWeaponMode)(struct Entity *, bool);
	bool (*SetWeaponCalledShot)(struct Entity *, bool);
	bool (*WeaponCanUseCalledShot)(struct Entity *);
	bool (*CanWeaponAndAttackerUseCalledShot)(struct Entity *, dword);
	bool (*UseAmmoForCurrentMode)(struct Entity *);
	bool (*HaveEnoughAmmoAndCanShoot)(struct Entity *);
	bool (*WeaponHasNonNormalPrimaryDamageTypeMaybe)(struct Entity *);
	int (*ExplodeHeldWeaponMaybe)(struct Entity *);
	int (*GetAPCost)(struct Entity *, struct EntityID);
	UseItemResponse * (*WeaponLoadingAmmoRelated)(struct Entity *, struct UseItemResponse *, struct EntityID, struct EntityID);
	EntityID * (*MaybeUnloadAmmoFromWeapon)(struct Entity *, struct EntityID *, struct EntityID, struct EntityID);
	FOTString * (*GetAmmoTypeString)(struct Entity *, struct FOTString *);
	FOTString * (*GetWeaponPerkString)(struct Entity *, struct FOTString *);
	float (*GetWeaponDamageRadius)(struct Entity *);
	FOTString * (*GetImpactFX)(struct Entity *, struct FOTString *);
	FOTString * (*GetSoundFX)(struct Entity *, struct FOTString *);
	FOTString * (*GetProjectileName)(struct Entity *, struct FOTString *);
	FOTString * (*GetPrimaryDamageType)(struct Entity *, struct FOTString *);
	FOTString * (*GetSecondaryDamageType)(struct Entity *, struct FOTString *);
	FOTString * (*GetAmmoVariantString)(struct Entity *, struct FOTString *);
	int (*GetAmmoOrUsesLeft)(struct Entity *);
	int (*GetMaxUsesOrAmmoCapacity)(struct Entity *);
	bool (*WeaponIsDestroyUser)(struct Entity *);
	EntityID * (*FindSuitableAmmoInInventory)(struct Entity *, struct EntityID *, struct AmmoSearchStructTest *);
	bool (*ContainsEntity)(struct Entity *, struct EntityID *);
	float (*GetEffectiveWeaponRangeByRangeType)(struct Entity *);
	float (*GetRange)(struct Entity *, struct EntityID);
	float (*GetMinRange)(struct Entity *);
	int (*GetAverageDamage)(struct Entity *);
	bool (*CanWeaponAOE)(struct Entity *);
	bool (*CanWeaponNotAOE)(struct Entity *);
	bool (*CanFireWithRemainingAP)(struct Entity *, int, struct EntityID);
	bool (*IsActivated)(struct Entity *);
	int (*GetFrequency)(struct Entity *);
	void (*ActivateTrapRelated)(struct Entity *, struct EntityID, struct EntityID);
	float (*GetTrapTimeToDetonation)(struct Entity *);
	void (*SetTrapFrequencyCode)(struct Entity *, int, int);
	void (*DetonateTrap)(struct Entity *);
	bool (*HasActiveTrap)(struct Entity *);
	bool (*TrapRelatedEitherDisarmOrTrigger)(struct Entity *, struct EntityID);
	bool (*SetContainedTrap)(struct Entity *, struct EntityID);
	void (*PlaceTrapMaybe)(struct Entity *, struct EntityID, struct EntityID, bool);
	void (*DisarmTrap)(struct Entity *);
	bool (*ActivateOrBreakAlarm)(struct Entity *, struct EntityID);
	bool (*IsDoorOpen)(struct Entity *);
	bool (*IsDoorClosed)(struct Entity *);
	bool (*IsLocked)(struct Entity *);
	bool (*CanDoorOpen)(struct Entity *, struct EntityID);
	bool (*Vtable462DoorRelated)(struct Entity *, struct EntityID);
	void (*SetupCombatFXCombatMessageStruct)(struct Entity *, struct CombatMessage *, int);
	void (*Vtable464NoOp)(struct Entity *, int, int, int, int, int, int);
	void (*Vtable465CombatFX)(struct Entity *, struct CombatMessage *);
	void (*Vtable466NoOp)(struct Entity *, int, int, int, int);
	void (*Vtable467CombatFX)(struct Entity *, struct LocationXYZFloat *);
	void (*Vtable468CombatFX)(struct Entity *);
	void (*Vtable469CombatFX)(struct Entity *, struct LocationXYZFloat *);
	void (*CombatFXBloodSprayRelated)(struct Entity *, struct CombatMessage *);
	void (*CombatFXBloodTrailRelated)(struct Entity *, struct CombatMessage *, int);
	void (*CombatFXBloodPoolRelated)(struct Entity *, struct EntityID);
	void (*SetupScenaryMove)(struct Entity *, struct LocationXYZFloat *, int);
	void (*Vtable474NoOp)(struct Entity *, int, int, int, int, int, int);
	void (*Vtable475NoOp)(struct Entity *, int, int, int, int, int, int);
	float (*Vtable476ReturnFloatZero)(struct Entity *, int, int);
	float (*Vtable477ActorMaybeWeaponImpactKnockdownRelated)(struct Entity *, int, struct LocationXYZFloat *);
	void (*Vtable478NoOp)(struct Entity *);
	bool (*Vtable479ReturnFalse)(struct Entity *);
	void (*Vtable480DestructsStuff)(struct Entity *, int, int, int);
	bool (*IsVehicleEmpty)(struct Entity *);
	bool (*IsThisEntityTheDriver)(struct Entity *, struct EntityID);
	bool (*IsThisEntityTheGunner)(struct Entity *, struct EntityID);
	bool (*IsThisEntityInVehicle)(struct Entity *, struct EntityID);
	EntityID * (*GetVehicleDriver)(struct Entity *, struct EntityID *);
	void (*GetVehiclePassengerVector)(struct Entity *, struct SpecialVectorToPointers *);
	bool (*VehicleBoundingBoxStuff1)(struct Entity *, struct BoundingBoxIntVehicleSpecial *);
	bool (*IsInReverse)(struct Entity *);
	bool (*VehiclePassengerDisembarkAllowed)(struct Entity *, struct EntityID, struct LocationXYZFloat *);
	LocationXYZFloat * (*SetExitPoint)(struct Entity *, struct LocationXYZFloat *);
	bool (*IsNotDeactivated)(struct Entity *);
	void (*ActivateActor)(struct Entity *);
	void (*DeactivateActor)(struct Entity *);
	void (*Vtable494ActorEffectsRelated)(struct Entity *, struct EntityID);
	void (*RemoveEffectID)(struct Entity *, struct EntityID);
	void (*ApplyEffectsWithGivenTagName)(struct Entity *, struct FOTString *);
	bool (*BoundingBox2dSet)(struct Entity *);
	void (*GetListOfContainedEntities)(struct Entity *, struct SpecialVectorToPointers *);
	bool (*ScriptConditionCheckMaybe)(struct Entity *, struct FOTString **, void *);
	void (*KillEntity)(struct Entity *, struct FOTString *);
	void (*SetActivationState)(struct Entity *, int);
	bool (*IsInActiveState)(struct Entity *);
	bool (*IsEnabledForScriptEvents)(struct Entity *);
	void (*SetLockedState)(struct Entity *, bool);
	void (*Vtable505ActorCallsWorldForSomething)(struct Entity *);
	FOTString * (*GetRandomSpeechIDString)(struct Entity *, struct FOTString *);
	FOTString * (*GetClickSpeechIDString)(struct Entity *, struct FOTString *);
	void (*SetRandomSpeechIDString)(struct Entity *, struct FOTString *);
	void (*SetClickSpeechIDString)(struct Entity *, struct FOTString *);
	void (*ActOnControllerCommand)(struct Entity *, struct ControllerCommandStruct *);
	bool (*Vtable511ActorVehicle)(struct Entity *);
	void (*SetHavingTurn)(struct Entity *, bool);
	bool (*IsHavingTurn)(struct Entity *);
	void (*SetOverwatch)(struct Entity *, bool);
	bool (*IsOverwatch)(struct Entity *);
	void (*ResetBonusAC)(struct Entity *);
	void (*SetBonusACForHTHEvade)(struct Entity *);
	byte (*GetUnk23)(struct Entity *);
	bool (*Vtable519EntityESSRelated)(struct Entity *, int);
	bool (*DoSomethingWithHP)(struct Entity *, dword);
	void (*Vtable521ALLSomethingWithAllFields)(struct Entity *, int *);
	void (*Vtable522EntityDunno)(struct Entity *, struct FOTString *);
	classtype_vtable * (*GetClassType)(struct Entity *);
	Entity * (*ResetAndOrDestruct)(struct Entity *, byte);
*/
