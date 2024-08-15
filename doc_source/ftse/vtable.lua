--- Describes the known functions in the Entity Vtable.
-- Vtable function IDs are included in the beginning of the description (in parentheses); this function ID is what must be passed to CallVtable and/or InstallVtableHook.
--
-- Functions with a typical name are those which have been investigated and/or are simple enough that the functionality could be determined without extensive reverse engineering. Note that there are no guarantees for correctness, or that there hasn't been a typo in the parameters
-- that could cause crashes. Any failures in these functions should be reported, so that they can be fixed to allow investigation of the game behavior of that function to continue.
--
-- Functions named with "maybe" or similar are those which appear to have a certain behavior, but are not confirmed.
--
-- Functions named with just a vtable number and a rough area of effect are those which are complex and will require further investigation to determine what they are actually for.
--
-- Functions with "NoOp" in the name appear to do nothing at all.
--
-- Functions with "BROKEN" in the description or name are currently not supported by FTSE, as the parameter types could not be determined. Calling or hooking these functions is almost certain to cause a crash.
--
-- Note: If no parameters or return values are mentioned, then the function does not take any parameters or return anything.
--
-- @vtable Entity_Vtable

--- (0) GetRenderLightLevel returns light and color information for rendering. Specifics not yet known.
-- @param location A @{Vector3} type for some related location.
-- @param scale A floating-point value used as a scalar.
-- @return An @{RGBColor} object giving the color-based light level.

function GetRenderLightLevel(location, scale)
end

--- (1) RenderObjectMaybe is BROKEN. Do not use.

function RenderObjectMaybe()
end

--- (2) InitDefaultEntityFields initalizes this entity's members to a default state.

function InitDefaultEntityFields()
end

--- (3) SetAnimationType sets the current animation sequence for the entity.
-- @param param1 A string indicating the sequence to set.
-- @return A boolean value indicating if the sequence was successfully set.

function SetAnimationType(param1)
end

--- (4) UpdateSprite sets the current sprite for the entity. This will also refresh the sprite if other parameters are changed; e.g. colors.
-- @param param1 A string indicating which sprite file to use for the entity.
-- @return A boolean value indicating if the sprite was changed successfully.

function UpdateSprite(param1)
end

--- (5) Vtable5NoOp

function Vtable5NoOp()
end

--- (6) InitEntityAfterSpawn

function InitEntityAfterSpawn()
end

--- (7) PrepareEntity does additional setup for the entity.

function PrepareEntity()
end

--- (8) EraseInventory clears any inventory for the given entity. (Seems to not work for vehicles?)

function EraseInventory()
end

--- (9) MakeFreshCharacter resets an Actor entity to the defaults for its race.

function MakeFreshCharacter()
end

--- (10) DestructEntity removes the given entity from the game state. This has the same effect as @{Entity:Destruct}.

function DestructEntity()
end

--- (11) SetDoFade

function SetDoFade()
end

--- (12) AliasToDestructEntity

function AliasToDestructEntity()
end

--- (13) SomethingWithBoundingBoxAndCollision

function SomethingWithBoundingBoxAndCollision()
end

--- (14) FlagCaptureFunction1

function FlagCaptureFunction1()
end

--- (15) AttachNewFileToActor is BROKEN. Do not use.

function AttachNewFileToActor()
end

--- (16) AttachNewFileToActor2

function AttachNewFileToActor2()
end

--- (17) SomethingNewSpawnRelated

function SomethingNewSpawnRelated()
end

--- (18) Vtable18EquipRelated
-- @param param1 An entity 

function Vtable18EquipRelated(param1)
end

--- (19) AdjustSpriteOffsetForRotatingDoor

function AdjustSpriteOffsetForRotatingDoor()
end

--- (20) FinishedAnimationSequenceMaybe is called on an entity when it has finished its current animation sequence. (?)

function FinishedAnimationSequenceMaybe()
end

--- (21) Vtable21NoOp

function Vtable21NoOp()
end

--- (22) Vtable22NoOp

function Vtable22NoOp()
end

--- (23) Vtable23NoOp

function Vtable23NoOp()
end

--- (24) RenderRelated is BROKEN. Do not use.

function RenderRelated()
end

--- (25) CheckThinkTimers is called whenever the game should check the status of an @{Entity}'s timers. This is called frequently (seems to be less than once per frame though). The time between calls does not appear to be fixed.

function CheckThinkTimers()
end

--- (26) DoEngineThink is called when the engine think timer for an @{Entity} expires. This varies depending on entity type, and generally involves processing the typical logic for that entity.

function DoEngineThink()
end

--- (27) DoGameThink is called when the game think timer for an @{Entity} expires. Only a few entities use this timer, typically for events which have a set time until action (timer traps, chem effects, etc.).

function DoGameThink()
end

--- (28) SoundRelated1
-- @param param1 An integer 

function SoundRelated1(param1)
end

--- (29) GetSoundType
-- @return A string 

function GetSoundType()
end

--- (30) SetRicochetSoundCounters
-- @param param1 An integer 

function SetRicochetSoundCounters(param1)
end

--- (31) SetParameterToZero
-- @return A @{Vector3} table 

function SetParameterToZero()
end

--- (32) InitLightParameters
-- @param param1 An array of 12 floating-point values 

function InitLightParameters(param1)
end

--- (33) SetAttached
-- @param param1 An entity 

function SetAttached(param1)
end

--- (34) SetCustomName
-- @param param1 A string 

function SetCustomName(param1)
end

--- (35) GetCustomName
-- @return A string 

function GetCustomName()
end

--- (36) GetDisplayName
-- @return A string 

function GetDisplayName()
end

--- (37) GetHealthStatusString
-- @return A string 

function GetHealthStatusString()
end

--- (38) Vtable38NoOpEmptyStringRet
-- @return A string 

function Vtable38NoOpEmptyStringRet()
end

--- (39) GetUseActionName
-- @return A string 

function GetUseActionName()
end

--- (40) CanBeHeldInHand
-- @return A boolean value 

function CanBeHeldInHand()
end

--- (41) IsTimerMine
-- @return A boolean value 

function IsTimerMine()
end

--- (42) IsTriggerTrap
-- @return A boolean value 

function IsTriggerTrap()
end

--- (43) IsCollectable
-- @return A boolean value 

function IsCollectable()
end

--- (44) IsSingleUseItem
-- @return A boolean value 

function IsSingleUseItem()
end

--- (45) IsSkillConsumable
-- @return A boolean value 

function IsSkillConsumable()
end

--- (46) IsArmour
-- @return A boolean value 

function IsArmour()
end

--- (47) IsWeapon
-- @return A boolean value 

function IsWeapon()
end

--- (48) IsAmmo
-- @return A boolean value 

function IsAmmo()
end

--- (49) IsKey
-- @return A boolean value 

function IsKey()
end

--- (50) IsContainer
-- @return A boolean value 

function IsContainer()
end

--- (51) IsUnarmedAttack
-- @return A boolean value 

function IsUnarmedAttack()
end

--- (52) IsMeleeAttack
-- @return A boolean value 

function IsMeleeAttack()
end

--- (53) IsBaseAI
-- @return A boolean value 

function IsBaseAI()
end

--- (54) IsController
-- @return A boolean value 

function IsController()
end

--- (55) IsActor
-- @return A boolean value 

function IsActor()
end

--- (56) IsActorUnk20Set
-- @return A boolean value 

function IsActorUnk20Set()
end

--- (57) IsLight
-- @return A boolean value 

function IsLight()
end

--- (58) IsVehicle
-- @return A boolean value 

function IsVehicle()
end

--- (59) IsVehicleController
-- @return A boolean value 

function IsVehicleController()
end

--- (60) IsVehicleWeapon
-- @return A boolean value 

function IsVehicleWeapon()
end

--- (61) CanBeActivated
-- @return A boolean value 

function CanBeActivated()
end

--- (62) IsDeadOrDestroyedMaybe
-- @return A boolean value 

function IsDeadOrDestroyedMaybe()
end

--- (63) IsAttachedBaseAIMaybe
-- @return A boolean value 

function IsAttachedBaseAIMaybe()
end

--- (64) IsLiving
-- @return A boolean value 

function IsLiving()
end

--- (65) IsTrap
-- @return A boolean value 

function IsTrap()
end

--- (66) IsSentryAI
-- @return A boolean value 

function IsSentryAI()
end

--- (67) IsGeiger
-- @return A boolean value 

function IsGeiger()
end

--- (68) IsWaypoint
-- @return A boolean value 

function IsWaypoint()
end

--- (69) IsSpawnpoint
-- @return A boolean value 

function IsSpawnpoint()
end

--- (70) IsAlarm
-- @return A boolean value 

function IsAlarm()
end

--- (71) IsCombatFX
-- @return A boolean value 

function IsCombatFX()
end

--- (72) IsSwitch
-- @return A boolean value 

function IsSwitch()
end

--- (73) IsDoor
-- @return A boolean value 

function IsDoor()
end

--- (74) IsGunTurret
-- @return A boolean value 

function IsGunTurret()
end

--- (75) IsBrainJar
-- @return A boolean value 

function IsBrainJar()
end

--- (76) IsBreakableType
-- @return A boolean value 

function IsBreakableType()
end

--- (77) IsCaptureFlag
-- @return A boolean value 

function IsCaptureFlag()
end

--- (78) IsTrapDoor
-- @return A boolean value 

function IsTrapDoor()
end

--- (79) IsCyborgGeneral
-- @return A boolean value 

function IsCyborgGeneral()
end

--- (80) IsPopupTurret
-- @return A boolean value 

function IsPopupTurret()
end

--- (81) IsNotCaptureInvItem
-- @return A boolean value 

function IsNotCaptureInvItem()
end

--- (82) RetTypeStringForCarryableEntity
-- @return A string 

function RetTypeStringForCarryableEntity()
end

--- (83) MaybeUseEntity is called when the default action for an @{Entity} is triggered. 
-- @param param1 The @{Entity} (usually an @{Actor}) which used the triggered entity.
-- @param param2 An @{Entity} which is the target for the triggered entity. Only a few triggerable entities use this (notably @{Consumable}).
-- @return An @{InventoryActionResult} table indicating the result (success or failure) of the entity being triggered.

function MaybeUseEntity(param1, param2)
end

--- (84) SplitEntity splits a @{Collectable} into two entities. This is used to split a stack of items.
-- @param param1 An integer indicating how many to remove from the stack. If equal or greater to the size of the stack, the original entity will be destructed.
-- @return An entity containing the split items.

function SplitEntity(param1)
end

--- (85) MergeEntities
-- @param param1 An entity 
-- @param param2 An integer 
-- @return A boolean value 

function MergeEntities(param1, param2)
end

--- (86) MaybeCheckingIfMergeableAndMatching
-- @param param1 An entity 
-- @return A boolean value 

function MaybeCheckingIfMergeableAndMatching(param1)
end

--- (87) CanBeRemoved
-- @return A boolean value 

function CanBeRemoved()
end

--- (88) SetCanBeRemovedFlag
-- @param param1 A boolean value 

function SetCanBeRemovedFlag(param1)
end

--- (89) SetBeenAppliedFlag
-- @param param1 A boolean value 

function SetBeenAppliedFlag(param1)
end

--- (90) IsUnique
-- @return A boolean value 

function IsUnique()
end

--- (91) SetIsUniqueFlag
-- @param param1 A boolean value 

function SetIsUniqueFlag(param1)
end

--- (92) MeetRequirementsCheck
-- @param param1 An @{AttributesTable} table 
-- @return A boolean value 

function MeetRequirementsCheck(param1)
end

--- (93) GetItemSPECIALRequired is BROKEN. Do not use.

function GetItemSPECIALRequired()
end

--- (94) ApplyEquip
-- @param param1 An entity 
-- @param param2 An integer 

function ApplyEquip(param1, param2)
end

--- (95) UnApply
-- @param param1 An entity 
-- @param param2 An integer 

function UnApply(param1, param2)
end

--- (96) SetTrapHolder
-- @param param1 An entity 

function SetTrapHolder(param1)
end

--- (97) ClearHolder
-- @param param1 An entity 

function ClearHolder(param1)
end

--- (98) GetCount
-- @return An integer 

function GetCount()
end

--- (99) GetCountNonLootable
-- @return An integer 

function GetCountNonLootable()
end

--- (100) IsLootable
-- @return A boolean value 

function IsLootable()
end

--- (101) SetUnk25EntityID
-- @param param1 An entity 

function SetUnk25EntityID(param1)
end

--- (102) GetValueBaseOfItems
-- @return An integer 

function GetValueBaseOfItems()
end

--- (103) GetCollectableRankReq
-- @return An integer 

function GetCollectableRankReq()
end

--- (104) GetWeight
-- @return A floating-point value 

function GetWeight()
end

--- (105) SetCount
-- @param param1 An integer 

function SetCount(param1)
end

--- (106) SetNonLootableCount
-- @param param1 An integer 
-- @return An integer 

function SetNonLootableCount(param1)
end

--- (107) BreakEntity
-- @return A boolean value 

function BreakEntity()
end

--- (108) IsBroken
-- @return A boolean value 

function IsBroken()
end

--- (109) RepairEntity
-- @return A boolean value 

function RepairEntity()
end

--- (110) GetTrapTotalDifficulty
-- @return An integer 

function GetTrapTotalDifficulty()
end

--- (111) Vtable111Controller

function Vtable111Controller()
end

--- (112) Vtable112Controller
-- @param param1 An entity 

function Vtable112Controller(param1)
end

--- (113) Vtable113Controller
-- @param param1 An entity 

function Vtable113Controller(param1)
end

--- (114) SetControllerMoveTypePriority
-- @param param1 An integer 

function SetControllerMoveTypePriority(param1)
end

--- (115) SetControllerWorkingZone
-- @param param1 A @{ZoneDefinition} table 

function SetControllerWorkingZone(param1)
end

--- (116) SetControllerRetreatZone
-- @param param1 A @{ZoneDefinition} table 

function SetControllerRetreatZone(param1)
end

--- (117) SetControllerMoveTargetTag
-- @param param1 A string 

function SetControllerMoveTargetTag(param1)
end

--- (118) SetControllerAggression
-- @param param1 An integer 

function SetControllerAggression(param1)
end

--- (119) SetControllerCommunicate
-- @param param1 An integer 

function SetControllerCommunicate(param1)
end

--- (120) SetControllerMobility
-- @param param1 An integer 

function SetControllerMobility(param1)
end

--- (121) SetControllerRandomMove
-- @param param1 A boolean value 
-- @param param2 An integer 

function SetControllerRandomMove(param1, param2)
end

--- (122) GetControllerMoveTypePriority
-- @return An integer 

function GetControllerMoveTypePriority()
end

--- (123) GetControllerWorkingZone
-- @return A @{ZoneDefinition} table 

function GetControllerWorkingZone()
end

--- (124) GetControllerRetreatZone
-- @return A @{ZoneDefinition} table 

function GetControllerRetreatZone()
end

--- (125) GetControllerMoveTargetTag
-- @return A string 

function GetControllerMoveTargetTag()
end

--- (126) GetControllerAggression
-- @return An integer 

function GetControllerAggression()
end

--- (127) GetControllerCommunicate
-- @return An integer 

function GetControllerCommunicate()
end

--- (128) GetControllerMobility
-- @return An integer 

function GetControllerMobility()
end

--- (129) GetControllerRandomMove
-- @return A boolean value 

function GetControllerRandomMove()
end

--- (130) GetControllerType
-- @return A string 

function GetControllerType()
end

--- (131) SetControllerType
-- @param param1 A string 

function SetControllerType(param1)
end

--- (132) DeactivateController

function DeactivateController()
end

--- (133) SetControllerWaypointModeAndMoveTargetTag
-- @param param1 A string 

function SetControllerWaypointModeAndMoveTargetTag(param1)
end

--- (134) SetControllerStationary

function SetControllerStationary()
end

--- (135) SetControllerToPerimeterZone

function SetControllerToPerimeterZone()
end

--- (136) SetControllerToFollow
-- @param param1 An integer 

function SetControllerToFollow(param1)
end

--- (137) SetControllerToFlee

function SetControllerToFlee()
end

--- (138) ControllerDoSomethingWithUnitList

function ControllerDoSomethingWithUnitList()
end

--- (139) SetControllerUnknown4String
-- @param param1 A string 

function SetControllerUnknown4String(param1)
end

--- (140) GetControllerUnknown4String
-- @return A string 

function GetControllerUnknown4String()
end

--- (141) ControllerWakeUpUnitList
-- @param param1 An integer 

function ControllerWakeUpUnitList(param1)
end

--- (142) ControllerIssueTauntForEntity
-- @param param1 An entity 
-- @param param2 An integer 

function ControllerIssueTauntForEntity(param1, param2)
end

--- (143) ControllerWaypointMaybeTimingRelated
-- @param param1 An entity 
-- @return A floating-point value 

function ControllerWaypointMaybeTimingRelated(param1)
end

--- (144) ControllerGetUnitWaypointActionString
-- @param param1 An entity 
-- @return A string 

function ControllerGetUnitWaypointActionString(param1)
end

--- (145) ControllerWaypointCheckingRepeatFlag
-- @param param1 An entity 
-- @return A boolean value 

function ControllerWaypointCheckingRepeatFlag(param1)
end

--- (146) ControllerMoveRelated
-- @param param1 An entity 
-- @param param2 A @{Vector3} table 
-- @return A floating-point value 

function ControllerMoveRelated(param1, param2)
end

--- (147) Vtable147Controller
-- @param param1 An entity 
-- @param param2 A boolean value 
-- @return An entity 

function Vtable147Controller(param1, param2)
end

--- (148) Vtable148Controller
-- @param param1 A string 
-- @return An entity 

function Vtable148Controller(param1)
end

--- (149) ControllerGetFromUnknownList2
-- @param param1 An entity 
-- @param param2 An integer 
-- @return A floating-point value 

function ControllerGetFromUnknownList2(param1, param2)
end

--- (150) Vtable150Controller
-- @return A boolean value 

function Vtable150Controller()
end

--- (151) Vtable151NoOpReturnFloat0
-- @param param1 An integer 
-- @param param2 An integer 
-- @return A floating-point value 

function Vtable151NoOpReturnFloat0(param1, param2)
end

--- (152) Vtable152NoOpReturnFloat0
-- @param param1 An integer 
-- @param param2 An integer 
-- @return A floating-point value 

function Vtable152NoOpReturnFloat0(param1, param2)
end

--- (153) ControllerMoreMoveRelated
-- @param param1 An entity 
-- @param param2 A @{Vector3} table 
-- @return A floating-point value 

function ControllerMoreMoveRelated(param1, param2)
end

--- (154) ControllerWaypointStuff
-- @param param1 An entity 
-- @return A @{Vector3} table 

function ControllerWaypointStuff(param1)
end

--- (155) ControllerSomethingAboutTookDamage
-- @param param1 An entity 
-- @return An integer 

function ControllerSomethingAboutTookDamage(param1)
end

--- (156) Vtable156Controller
-- @param param1 An entity 
-- @param param2 A @{Vector3} table 

function Vtable156Controller(param1, param2)
end

--- (157) ControllerUsesWorkingZoneAndMoveType
-- @param param1 An entity 
-- @param param2 A @{Vector3} table 
-- @param param3 A boolean value 
-- @return A boolean value 

function ControllerUsesWorkingZoneAndMoveType(param1, param2, param3)
end

--- (158) BaseAINeedsToSwitchTargetMaybe
-- @return A boolean value 

function BaseAINeedsToSwitchTargetMaybe()
end

--- (159) BaseAIEngineThinkRelated is BROKEN. Do not use.

function BaseAIEngineThinkRelated()
end

--- (160) SetBaseAIController
-- @param param1 An entity 

function SetBaseAIController(param1)
end

--- (161) GetBaseAIController
-- @return An entity 

function GetBaseAIController()
end

--- (162) Vtable162BaseAI is BROKEN. Do not use.

function Vtable162BaseAI()
end

--- (163) GetBaseAINature
-- @return A string 

function GetBaseAINature()
end

--- (164) StopMovement
-- @param param1 A boolean value 

function StopMovement(param1)
end

--- (165) BaseAIGetCurrentTarget
-- @return An entity 

function BaseAIGetCurrentTarget()
end

--- (166) BaseAIHasATargetMaybe
-- @return A boolean value 

function BaseAIHasATargetMaybe()
end

--- (167) IsSleeping
-- @return A boolean value 

function IsSleeping()
end

--- (168) SetSleepingFlag
-- @param param1 A boolean value 

function SetSleepingFlag(param1)
end

--- (169) SetBaseAIUnknown7
-- @param param1 A floating-point value 

function SetBaseAIUnknown7(param1)
end

--- (170) SetBaseAIUnknown7ToRealTimeTimerValue
-- @param param1 An integer 

function SetBaseAIUnknown7ToRealTimeTimerValue(param1)
end

--- (171) BaseAIStopMovementIfHighestDesireSet

function BaseAIStopMovementIfHighestDesireSet()
end

--- (172) BaseAIMaybeOpenDoorRelated is BROKEN. Do not use.

function BaseAIMaybeOpenDoorRelated()
end

--- (173) Vtable173BaseAI is BROKEN. Do not use.

function Vtable173BaseAI()
end

--- (174) Vtable174BaseAI
-- @param param1 An entity 

function Vtable174BaseAI(param1)
end

--- (175) GetBaseAITauntTypeString
-- @return A string 

function GetBaseAITauntTypeString()
end

--- (176) GetBaseAIHighestDesireRoundedUp
-- @return An integer 

function GetBaseAIHighestDesireRoundedUp()
end

--- (177) ClearSentryAITargets

function ClearSentryAITargets()
end

--- (178) SetSentryMode
-- @param param1 An integer 

function SetSentryMode(param1)
end

--- (179) GetSentryMode
-- @return An integer 

function GetSentryMode()
end

--- (180) SetSentryAccuracyTrigger
-- @param param1 An integer 

function SetSentryAccuracyTrigger(param1)
end

--- (181) GetSentryAccuracyTrigger
-- @return An integer 

function GetSentryAccuracyTrigger()
end

--- (182) SetSentryLocation
-- @param param1 An integer 

function SetSentryLocation(param1)
end

--- (183) GetSentryLocation
-- @return An integer 

function GetSentryLocation()
end

--- (184) SetSentryUserTarget
-- @param param1 An entity 
-- @param param2 An integer 

function SetSentryUserTarget(param1, param2)
end

--- (185) SentryAIMaybeCheckingTargets
-- @param param1 An entity 

function SentryAIMaybeCheckingTargets(param1)
end

--- (186) GetActorCopyofSPECIALStats is BROKEN. Do not use.

function GetActorCopyofSPECIALStats()
end

--- (187) CopySomeActorFields is BROKEN. Do not use.

function CopySomeActorFields()
end

--- (188) CopySomeActorFields2 is BROKEN. Do not use.

function CopySomeActorFields2()
end

--- (189) Vtable189Actor is BROKEN. Do not use.

function Vtable189Actor()
end

--- (190) GetActorStartIndex
-- @return An integer 

function GetActorStartIndex()
end

--- (191) SetActorStartIndex
-- @param param1 An integer 

function SetActorStartIndex(param1)
end

--- (192) GetActorFractionOfHitPointsLost
-- @return A floating-point value 

function GetActorFractionOfHitPointsLost()
end

--- (193) CheckBestHPRatioIfUseHealingItemMaybe
-- @return A floating-point value 

function CheckBestHPRatioIfUseHealingItemMaybe()
end

--- (194) ActorHasItemToPermBoostStats
-- @return A floating-point value 

function ActorHasItemToPermBoostStats()
end

--- (195) ActorHasItemToPermBoostStats2
-- @return A floating-point value 

function ActorHasItemToPermBoostStats2()
end

--- (196) GetActorPoisonPointsTimes15Cap90
-- @return A floating-point value 

function GetActorPoisonPointsTimes15Cap90()
end

--- (197) ActorLooksLikeNoOp
-- @return A floating-point value 

function ActorLooksLikeNoOp()
end

--- (198) GetActorRadPointsDivided10Cap90
-- @return A floating-point value 

function GetActorRadPointsDivided10Cap90()
end

--- (199) ActorCheckItemsForRadHealing
-- @return A floating-point value 

function ActorCheckItemsForRadHealing()
end

--- (200) MakeConsumableGiveRadiation
-- @param param1 An integer 
-- @param param2 An integer 

function MakeConsumableGiveRadiation(param1, param2)
end

--- (201) MakeConsumableGivePoison
-- @param param1 An integer 
-- @param param2 An integer 

function MakeConsumableGivePoison(param1, param2)
end

--- (202) ApplyBonusesToAttachedActor

function ApplyBonusesToAttachedActor()
end

--- (203) GetExpectedHPRatioIfConsumableUsed
-- @param param1 An entity 
-- @return A floating-point value 

function GetExpectedHPRatioIfConsumableUsed(param1)
end

--- (204) DoesConsumablePermBoostSPEAL
-- @param param1 An entity 
-- @return A floating-point value 

function DoesConsumablePermBoostSPEAL(param1)
end

--- (205) Vtable205NoOpRetZeroFloat
-- @return A floating-point value 

function Vtable205NoOpRetZeroFloat()
end

--- (206) GetRatioOfRadPointHealingForConsumable
-- @param param1 An entity 
-- @return A floating-point value 

function GetRatioOfRadPointHealingForConsumable(param1)
end

--- (207) Vtable207ActorMaybeCheckForPlayerCharacter
-- @return A boolean value 

function Vtable207ActorMaybeCheckForPlayerCharacter()
end

--- (208) GetRankString
-- @return A string 

function GetRankString()
end

--- (209) GetActorRankID
-- @return An integer 

function GetActorRankID()
end

--- (210) GetRanksGivenEntityHasAboveThis
-- @param param1 An entity 
-- @return An integer 

function GetRanksGivenEntityHasAboveThis(param1)
end

--- (211) GetReputationString
-- @return A string 

function GetReputationString()
end

--- (212) GetCappedReputation
-- @return An integer 

function GetCappedReputation()
end

--- (213) GetMissionsCom
-- @return An integer 

function GetMissionsCom()
end

--- (214) IncrementMissionsCom
-- @param param1 An integer 

function IncrementMissionsCom(param1)
end

--- (215) AddToReputation
-- @param param1 An integer 

function AddToReputation(param1)
end

--- (216) IsGlowing
-- @return A boolean value 

function IsGlowing()
end

--- (217) GetActorAge is BROKEN. Do not use.

function GetActorAge()
end

--- (218) SetCurrentActionStringMaybe
-- @param param1 A string 
-- @param param2 A string 

function SetCurrentActionStringMaybe(param1, param2)
end

--- (219) GetCurrentActionString
-- @return A string 

function GetCurrentActionString()
end

--- (220) IsIdle
-- @return A boolean value 

function IsIdle()
end

--- (221) IsMoving_WalkRunClimbLadderOrDriving
-- @return A boolean value 

function IsMoving_WalkRunClimbLadderOrDriving()
end

--- (222) IsClimbing
-- @return A boolean value 

function IsClimbing()
end

--- (223) IsIdleOrWalkRun
-- @return A boolean value 

function IsIdleOrWalkRun()
end

--- (224) HasFallen
-- @return A boolean value 

function HasFallen()
end

--- (225) Vtable225ThinkRelated

function Vtable225ThinkRelated()
end

--- (226) CanMove
-- @return A boolean value 

function CanMove()
end

--- (227) CanRun
-- @return A boolean value 

function CanRun()
end

--- (228) AdjustStanceDownward
-- @param param1 A boolean value 

function AdjustStanceDownward(param1)
end

--- (229) AdjustStanceUpward
-- @param param1 A boolean value 

function AdjustStanceUpward(param1)
end

--- (230) IsHumanoidOrDog
-- @return A boolean value 

function IsHumanoidOrDog()
end

--- (231) MaxMovementSpeedMaybe
-- @return A floating-point value 

function MaxMovementSpeedMaybe()
end

--- (232) IsEncumbered
-- @return A boolean value 

function IsEncumbered()
end

--- (233) IsStanding
-- @return A boolean value 

function IsStanding()
end

--- (234) IsCrouching
-- @return A boolean value 

function IsCrouching()
end

--- (235) IsProne
-- @return A boolean value 

function IsProne()
end

--- (236) GetSelectedStance
-- @return An integer 

function GetSelectedStance()
end

--- (237) ActoSetSelectedStance
-- @param param1 A string 

function ActoSetSelectedStance(param1)
end

--- (238) IsHidden
-- @return A boolean value 

function IsHidden()
end

--- (239) GetPostureString
-- @return A string 

function GetPostureString()
end

--- (240) SetSneakState
-- @param param1 A boolean value 

function SetSneakState(param1)
end

--- (241) IsBurrowed
-- @return A boolean value 

function IsBurrowed()
end

--- (242) SetBurrowingState
-- @param param1 A boolean value 

function SetBurrowingState(param1)
end

--- (243) RollAgainstStat
-- @param param1 A string 
-- @param param2 An integer 
-- @return A boolean value 

function RollAgainstStat(param1, param2)
end

--- (244) ApplySkillToTargetEntity triggers behavior of a given skill against this @{Entity}. 
-- @param param1 A string indicating the internal name of the skill being used (same string as those used in @{AttributesTable}).
-- @param param2 An entity indicating the @{Actor} using this skill.
-- @param param3 An entity related to the skill being used. Appears to be used to indicate a @{SkillConsumable} used for First Aid / Doctor / Repair, etc.
-- @return An @{InventoryActionResult} table indicating the result of the skill use (success or failure).

function ApplySkillToTargetEntity(param1, param2, param3)
end

--- (245) GetSkillStringBasedOnUnk25c
-- @return A string 

function GetSkillStringBasedOnUnk25c()
end

--- (246) GetActorUnk2EntityID
-- @return An entity 

function GetActorUnk2EntityID()
end

--- (247) GetAdjustedPilotSkill
-- @return An integer 

function GetAdjustedPilotSkill()
end

--- (248) DoesSkillStringMatchHelperItem
-- @param param1 A string 
-- @return A boolean value 

function DoesSkillStringMatchHelperItem(param1)
end

--- (249) IsValidTargetForSkill checks if the listed skill can be used against this @{Entity}.
-- @param param1 A string indicating the internal name of the skill being checked.
-- @return A boolean value indicating if the skill is allowed to be used.

function IsValidTargetForSkill(param1)
end

--- (250) IsValidTargetForDefaultAction checks if an @{Entity} can be the target of a "default" action.
-- @param param1 An unknown entity. Seems to only be used for @{Trap} type.
-- @param param2 An unknown entity. Seems to only be used for @{Trap} type. 
-- @return A boolean value indicating if the entity supports default action.

function IsValidTargetForDefaultAction(param1, param2)
end

--- (251) IsGeneral
-- @return A boolean value 

function IsGeneral()
end

--- (252) IsRecruitMaster
-- @return A boolean value 

function IsRecruitMaster()
end

--- (253) IsQuartermaster
-- @return A boolean value 

function IsQuartermaster()
end

--- (254) CanBarterWith
-- @return A boolean value 

function CanBarterWith()
end

--- (255) BartersWithBOSScript
-- @return A boolean value 

function BartersWithBOSScript()
end

--- (256) CanGambleWith
-- @return A boolean value 

function CanGambleWith()
end

--- (257) GetRepairObjectDifficulty
-- @return An integer 

function GetRepairObjectDifficulty()
end

--- (258) GetScaledKnockdownChance
-- @param param1 A floating-point value 
-- @return A floating-point value 

function GetScaledKnockdownChance(param1)
end

--- (259) ActorKnockoutRelated

function ActorKnockoutRelated()
end

--- (260) ActorInjureLocation
-- @param param1 An integer 
-- @param param2 An integer 
-- @return An @{InventoryActionResult} table 

function ActorInjureLocation(param1, param2)
end

--- (261) ActorKnockout
-- @param param1 An integer 
-- @param param2 An integer 
-- @param param3 An integer 
-- @return An @{InventoryActionResult} table 

function ActorKnockout(param1, param2, param3)
end

--- (262) ActorDisarm
-- @param param1 An integer 
-- @param param2 An integer 
-- @return An @{InventoryActionResult} table 

function ActorDisarm(param1, param2)
end

--- (263) ActorBreakWeaponMaybe
-- @param param1 An integer 
-- @param param2 An integer 
-- @return An @{InventoryActionResult} table 

function ActorBreakWeaponMaybe(param1, param2)
end

--- (264) ActorCriticalMissHitSelfMaybe
-- @param param1 An integer 
-- @return An @{InventoryActionResult} table 

function ActorCriticalMissHitSelfMaybe(param1)
end

--- (265) ActorCriticalMissHitSelfMaybe2
-- @param param1 An integer 
-- @return An @{InventoryActionResult} table 

function ActorCriticalMissHitSelfMaybe2(param1)
end

--- (266) StunActor
-- @param param1 An integer 

function StunActor(param1)
end

--- (267) IsRightLegInjured
-- @param param1 A boolean value 
-- @return A boolean value 

function IsRightLegInjured(param1)
end

--- (268) IsLeftLegInjured
-- @param param1 A boolean value 
-- @return A boolean value 

function IsLeftLegInjured(param1)
end

--- (269) IsRightArmInjured
-- @param param1 A boolean value 
-- @return A boolean value 

function IsRightArmInjured(param1)
end

--- (270) IsLeftArmInjured
-- @param param1 A boolean value 
-- @return A boolean value 

function IsLeftArmInjured(param1)
end

--- (271) IsBlinded
-- @param param1 A boolean value 
-- @return A boolean value 

function IsBlinded(param1)
end

--- (272) IsWinded
-- @param param1 A boolean value 
-- @return A boolean value 

function IsWinded(param1)
end

--- (273) IsImmobilized
-- @param param1 A boolean value 
-- @return A boolean value 

function IsImmobilized(param1)
end

--- (274) IsConcussed
-- @param param1 A boolean value 
-- @return A boolean value 

function IsConcussed(param1)
end

--- (275) IsStunned
-- @param param1 A boolean value 
-- @return A boolean value 

function IsStunned(param1)
end

--- (276) IsUnconscious
-- @return A boolean value 

function IsUnconscious()
end

--- (277) IsBandagedStatus
-- @return A boolean value 

function IsBandagedStatus()
end

--- (278) GetActorMovementCostToLocation
-- @param param1 A @{Vector3} table 
-- @return An integer 

function GetActorMovementCostToLocation(param1)
end

--- (279) IsActorUnk23SetPlusSomeOtherFieldFalse
-- @return A boolean value 

function IsActorUnk23SetPlusSomeOtherFieldFalse()
end

--- (280) Move

function Move()
end

--- (281) SetDestination
-- @param param1 A @{Vector3} table 
-- @param param2 An integer 
-- @param param3 A floating-point value 
-- @param param4 A floating-point value 
-- @param param5 A boolean value 

function SetDestination(param1, param2, param3, param4, param5)
end

--- (282) GetLocationBasedOnUnk23
-- @return A @{Vector3} table 

function GetLocationBasedOnUnk23()
end

--- (283) ActorGetUnk24bEntityID
-- @return An entity 

function ActorGetUnk24bEntityID()
end

--- (284) ActorSomethingTouchSearch
-- @param param1 A @{Vector3} table 
-- @param param2 A boolean value 
-- @return A boolean value 

function ActorSomethingTouchSearch(param1, param2)
end

--- (285) ActorSomethingTouchSearch2
-- @param param1 A @{Vector3} table 
-- @param param2 A boolean value 
-- @return A boolean value 

function ActorSomethingTouchSearch2(param1, param2)
end

--- (286) ActorSomethingTouchSearch3
-- @param param1 A @{Vector3} table 
-- @param param2 A boolean value 
-- @return A boolean value 

function ActorSomethingTouchSearch3(param1, param2)
end

--- (287) Vtable287NoOpRet0
-- @return A boolean value 

function Vtable287NoOpRet0()
end

--- (288) IsLegalTarget
-- @return A boolean value 

function IsLegalTarget()
end

--- (289) ActorIsNotLegalTargetAndUnk1Set
-- @return A boolean value 

function ActorIsNotLegalTargetAndUnk1Set()
end

--- (290) IsVehicleDisabled
-- @return A boolean value 

function IsVehicleDisabled()
end

--- (291) ActorPickupItem
-- @param param1 An entity 
-- @param param2 An integer 
-- @return An @{InventoryActionResult} table 

function ActorPickupItem(param1, param2)
end

--- (292) Vtable292Actor

function Vtable292Actor()
end

--- (293) ReturnUseItemResponseType0x40
-- @return An @{InventoryActionResult} table 

function ReturnUseItemResponseType0x40()
end

--- (294) GetSomeKindOfFlagForMouseOverTarget
-- @param param1 An entity 
-- @param param2 A boolean value 
-- @return A @{ChanceToHit} table 

function GetSomeKindOfFlagForMouseOverTarget(param1, param2)
end

--- (295) GetSomeKindOfFlagForMouseOverTarget2
-- @param param1 A @{Vector3} table 
-- @return A @{ChanceToHit} table 

function GetSomeKindOfFlagForMouseOverTarget2(param1)
end

--- (296) BeginAttackAnEntity
-- @param param1 An entity 
-- @param param2 An integer 
-- @param param3 An integer 
-- @return A @{ChanceToHit} table 

function BeginAttackAnEntity(param1, param2, param3)
end

--- (297) BeginAttackALocation
-- @param param1 A @{Vector3} table 
-- @return A @{ChanceToHit} table 

function BeginAttackALocation(param1)
end

--- (298) ApplyAttackResultToEntity
-- @param param1 A @{CombatMessage} table 

function ApplyAttackResultToEntity(param1)
end

--- (299) ShotAtMissed
-- @param param1 A @{CombatMessage} table 

function ShotAtMissed(param1)
end

--- (300) ApplyDamage applies damage to an @{Actor}. This damage is directly reduced from HP, so the caller should take appropriate resistances into account, if applicable.
-- @param param1 An entity indicating the source of the damage (often an @{Actor}).
-- @param param2 An integer indicating the number of hits (maybe).
-- @param param3 An integer indicating the damage that was done.
-- @param param4 An unknown, possibly unused, integer value.
-- @param param5 A string indicating the type of damage done to the entity. Used for damage or death animation effects.
-- @param param6 An unknown, possibly unused, integer value.

function ApplyDamage(param1, param2, param3, param4, param5, param6)
end

--- (301) EntityWasKilled is called when an @{Actor} is killed by damage. 
-- @param param1 An entity indicating the attacker that caused the damage.
-- @param param2 An integer indicating the number of hits taken by the target. Generally used to select the death animation.
-- @param param3 An integer indicating the total damage taken by the target. Generally used to select the death animation.
-- @param param4 A string indicating the type of damage done. Generally used to select the death animation.

function EntityWasKilled(param1, param2, param3, param4)
end

--- (302) IncrementKillCount
-- @param param1 An entity 

function IncrementKillCount(param1)
end

--- (303) CopyActorKillList is BROKEN. Do not use.

function CopyActorKillList()
end

--- (304) Vtable304NoOpRet0
-- @return A boolean value 

function Vtable304NoOpRet0()
end

--- (305) ResolveCombatMessageForAttacker
-- @param param1 An @{AnimOffset} table 

function ResolveCombatMessageForAttacker(param1)
end

--- (306) GetIDOfEquippedItemInActiveSlot
-- @return An entity 

function GetIDOfEquippedItemInActiveSlot()
end

--- (307) GetEntityIDOfNonEquippedWeapon
-- @return An entity 

function GetEntityIDOfNonEquippedWeapon()
end

--- (308) ReloadActiveWeaponWithAnyAmmo
-- @return An @{InventoryActionResult} table 

function ReloadActiveWeaponWithAnyAmmo()
end

--- (309) ReloadWeaponWithSelectedAmmo
-- @param param1 An integer 
-- @param param2 An entity 
-- @return An @{InventoryActionResult} table 

function ReloadWeaponWithSelectedAmmo(param1, param2)
end

--- (310) ActorCanAttackWithActiveWeapon
-- @return A boolean value 

function ActorCanAttackWithActiveWeapon()
end

--- (311) Vtable311ActorCheckingLastTimeRun
-- @return A boolean value 

function Vtable311ActorCheckingLastTimeRun()
end

--- (312) InitActorCombatMessages

function InitActorCombatMessages()
end

--- (313) LevelUp
-- @param param1 An integer 

function LevelUp(param1)
end

--- (314) GetCombatMessage2TargetID
-- @return An entity 

function GetCombatMessage2TargetID()
end

--- (315) GetPlayerBuyPriceForItem
-- @param param1 An entity 
-- @param param2 An entity 
-- @return An integer 

function GetPlayerBuyPriceForItem(param1, param2)
end

--- (316) GetPlayerSellBackPriceForItem
-- @param param1 An entity 
-- @param param2 An entity 
-- @return An integer 

function GetPlayerSellBackPriceForItem(param1, param2)
end

--- (317) GetCountOfMatchingItems
-- @param param1 An entity 
-- @return An integer 

function GetCountOfMatchingItems(param1)
end

--- (318) EquipItem
-- @param param1 An entity 
-- @param param2 An integer 
-- @param param3 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function EquipItem(param1, param2, param3)
end

--- (319) RemoveItem
-- @param param1 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function RemoveItem(param1)
end

--- (320) RemoveItemIfEquipped
-- @param param1 An entity 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function RemoveItemIfEquipped(param1)
end

--- (321) AddToInventory
-- @param param1 An entity 
-- @param param2 An integer 
-- @param param3 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function AddToInventory(param1, param2, param3)
end

--- (322) RemoveFromInventory
-- @param param1 An entity 
-- @param param2 An integer 
-- @param param3 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function RemoveFromInventory(param1, param2, param3)
end

--- (323) UnequipItemFromSlotAndPutInInventory
-- @param param1 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function UnequipItemFromSlotAndPutInInventory(param1)
end

--- (324) UnequipItemByIDAndReturnToInventory
-- @param param1 An entity 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function UnequipItemByIDAndReturnToInventory(param1)
end

--- (325) EquipItemFromInventoryToSlot
-- @param param1 An entity 
-- @param param2 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function EquipItemFromInventoryToSlot(param1, param2)
end

--- (326) RemoveFromInventory2
-- @param param1 An entity 
-- @param param2 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function RemoveFromInventory2(param1, param2)
end

--- (327) ActorSomethingRelatedToPickup
-- @param param1 An entity 
-- @param param2 An integer 
-- @param param3 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function ActorSomethingRelatedToPickup(param1, param2, param3)
end

--- (328) RemoveItemFromSlot2
-- @param param1 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function RemoveItemFromSlot2(param1)
end

--- (329) RemoveItemIfEquipped2
-- @param param1 An entity 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function RemoveItemIfEquipped2(param1)
end

--- (330) PickupItemAndEquipInSlot
-- @param param1 An entity 
-- @param param2 An integer 
-- @param param3 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function PickupItemAndEquipInSlot(param1, param2, param3)
end

--- (331) RemoveItemFromEquippedThenInventory
-- @param param1 An entity 
-- @param param2 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function RemoveItemFromEquippedThenInventory(param1, param2)
end

--- (332) RemoveItemFromInventoryThenEquipped
-- @param param1 An entity 
-- @param param2 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function RemoveItemFromInventoryThenEquipped(param1, param2)
end

--- (333) Vtable333SomethingForApplyingItem
-- @param param1 An integer 
-- @return An @{InventoryActionResult} table 
-- @return An entity 

function Vtable333SomethingForApplyingItem(param1)
end

--- (334) SetActiveWeaponHand
-- @param param1 An integer 

function SetActiveWeaponHand(param1)
end

--- (335) ToggleActiveHand

function ToggleActiveHand()
end

--- (336) CheckIfTargetInReachableRange
-- @param param1 An entity 
-- @return A boolean value 

function CheckIfTargetInReachableRange(param1)
end

--- (337) GetSelectedHand
-- @return An integer 

function GetSelectedHand()
end

--- (338) GetUnselectedHand
-- @return An integer 

function GetUnselectedHand()
end

--- (339) GetIDofEquippedItemInSlot
-- @param param1 An integer 
-- @return An entity 

function GetIDofEquippedItemInSlot(param1)
end

--- (340) Vtable340InventoryCombineRelatedMaybe
-- @param param1 An entity 
-- @return An @{InventoryActionResult} table 

function Vtable340InventoryCombineRelatedMaybe(param1)
end

--- (341) PlayerEquippedItemWithTagName
-- @param param1 A string 
-- @return A boolean value 

function PlayerEquippedItemWithTagName(param1)
end

--- (342) IsItemIDEquipped
-- @param param1 An entity 
-- @return A boolean value 

function IsItemIDEquipped(param1)
end

--- (343) ContainsItemWithTagName
-- @param param1 A string 
-- @return A boolean value 

function ContainsItemWithTagName(param1)
end

--- (344) ContainsItemByID
-- @param param1 An entity 
-- @return A boolean value 

function ContainsItemByID(param1)
end

--- (345) EquippedOrContainsItemWithTagName
-- @param param1 A string 
-- @return A boolean value 

function EquippedOrContainsItemWithTagName(param1)
end

--- (346) FindIfEquippedOrCarriedItemByID
-- @param param1 An entity 
-- @return A boolean value 

function FindIfEquippedOrCarriedItemByID(param1)
end

--- (347) Vtable347ActorSomethingAboutUsingItem
-- @param param1 An integer 
-- @param param2 An integer 
-- @param param3 An integer 
-- @return An @{InventoryActionResult} table 

function Vtable347ActorSomethingAboutUsingItem(param1, param2, param3)
end

--- (348) CanUseItem
-- @param param1 An entity 
-- @return An @{InventoryActionResult} table 

function CanUseItem(param1)
end

--- (349) CanReachInteractable
-- @param param1 An entity 
-- @return An @{InventoryActionResult} table 

function CanReachInteractable(param1)
end

--- (350) GetSizeStringMaybe
-- @return A string 

function GetSizeStringMaybe()
end

--- (351) GetCurrentStats
-- @return An @{AttributesTable} table 

function GetCurrentStats()
end

--- (352) GetCopyStats
-- @return An @{AttributesTable} table 

function GetCopyStats()
end

--- (353) GetTempStats
-- @return An @{AttributesTable} table 

function GetTempStats()
end

--- (354) GetBaseStats
-- @return An @{AttributesTable} table 

function GetBaseStats()
end

--- (355) CopyAttributesToBaseStats
-- @param param1 An @{AttributesTable} table 

function CopyAttributesToBaseStats(param1)
end

--- (356) GetActorStatus
-- @return An @{ActorStatus} table 

function GetActorStatus()
end

--- (357) CopyActorStatus
-- @param param1 An @{ActorStatus} table 

function CopyActorStatus(param1)
end

--- (358) AdjustAP
-- @param param1 A floating-point value 

function AdjustAP(param1)
end

--- (359) AdjustAP2
-- @param param1 An integer 

function AdjustAP2(param1)
end

--- (360) GetCurrentAP
-- @return A floating-point value 

function GetCurrentAP()
end

--- (361) GetAPAvailablePercentage
-- @return A floating-point value 

function GetAPAvailablePercentage()
end

--- (362) ActorUnconsciousAPHandling
-- @param param1 A boolean value 

function ActorUnconsciousAPHandling(param1)
end

--- (363) IsMale
-- @return A boolean value 

function IsMale()
end

--- (364) IsFemale
-- @return A boolean value 

function IsFemale()
end

--- (365) GetSexString
-- @return A string 

function GetSexString()
end

--- (366) GetEntityCharacterTypeString
-- @return A string 

function GetEntityCharacterTypeString()
end

--- (367) AddTempBonuses
-- @param param1 An @{AttributesTable} table 
-- @param param2 An integer 
-- @param param3 A floating-point value 

function AddTempBonuses(param1, param2, param3)
end

--- (368) RemoveTempBonuses
-- @param param1 An @{AttributesTable} table 
-- @param param2 An integer 
-- @param param3 A floating-point value 

function RemoveTempBonuses(param1, param2, param3)
end

--- (369) IncreaseAttributeByRange will increase an attribute by a random amount between a lower and upper limit.
-- @param param1 An integer indicating the minimum to adjust by.
-- @param param2 An integer indicating the maximum to adjust by.
-- @param param3 An internal string indicating the attribute to increase. Can be "hitPoints", or most other attribute names.
-- @return An integer indicating the amount the attribute was increased by.

function IncreaseAttributeByRange(param1, param2, param3)
end

--- (370) GrantXP
-- @param param1 An integer 
-- @return A boolean value 

function GrantXP(param1)
end

--- (371) GetNPCLevel
-- @return An integer 

function GetNPCLevel()
end

--- (372) HandleXPForNewPlayerCharacterMaybe

function HandleXPForNewPlayerCharacterMaybe()
end

--- (373) GiveXPForLevelUp

function GiveXPForLevelUp()
end

--- (374) GetExpectedLevelForMyXP
-- @return An integer 

function GetExpectedLevelForMyXP()
end

--- (375) XPAndValueRelatedCalc
-- @return An integer 

function XPAndValueRelatedCalc()
end

--- (376) RadiateEntity
-- @param param1 A floating-point value 

function RadiateEntity(param1)
end

--- (377) PoisonEntity
-- @param param1 A floating-point value 

function PoisonEntity(param1)
end

--- (378) AddOverdosePoints
-- @param param1 A floating-point value 

function AddOverdosePoints(param1)
end

--- (379) MaybeHealOverTime
-- @param param1 A floating-point value 

function MaybeHealOverTime(param1)
end

--- (380) IsAlwaysFriend
-- @return A boolean value 

function IsAlwaysFriend()
end

--- (381) AdjustActorStatusBasedOnEffectChangeEtc is BROKEN. Do not use.

function AdjustActorStatusBasedOnEffectChangeEtc()
end

--- (382) GetHP
-- @return An integer 

function GetHP()
end

--- (383) HasAddiction
-- @return A boolean value 

function HasAddiction()
end

--- (384) CheckOverdosed
-- @return A boolean value 

function CheckOverdosed()
end

--- (385) IsInWithdrawal
-- @return A boolean value 

function IsInWithdrawal()
end

--- (386) IsRadiated
-- @return A boolean value 

function IsRadiated()
end

--- (387) IsPoisoned
-- @return A boolean value 

function IsPoisoned()
end

--- (388) IsDrunk
-- @return A boolean value 

function IsDrunk()
end

--- (389) GetWaypointPauseTime
-- @return A floating-point value 

function GetWaypointPauseTime()
end

--- (390) GetWaypointIndex
-- @return An integer 

function GetWaypointIndex()
end

--- (391) AlwaysReturnMaxInt
-- @return An integer 

function AlwaysReturnMaxInt()
end

--- (392) GetNextWaypointList
-- @return A string 

function GetNextWaypointList()
end

--- (393) GetWaypointActionString
-- @return A string 

function GetWaypointActionString()
end

--- (394) GetRepeat
-- @return A boolean value 

function GetRepeat()
end

--- (395) GetWaypointUseDirection
-- @return A boolean value 

function GetWaypointUseDirection()
end

--- (396) SetWaypointIndex
-- @param param1 An integer 

function SetWaypointIndex(param1)
end

--- (397) GetActorTargetLocationMaybe
-- @return A @{Vector3} table 

function GetActorTargetLocationMaybe()
end

--- (398) SetActorTargetLocationMaybe
-- @param param1 A @{Vector3} table 

function SetActorTargetLocationMaybe(param1)
end

--- (399) GetAttackDir
-- @return A @{Vector3} table 

function GetAttackDir()
end

--- (400) SetGUISlot
-- @param param1 An integer 

function SetGUISlot(param1)
end

--- (401) GetGUISlot
-- @return An integer 

function GetGUISlot()
end

--- (402) GetInventoryList
-- @return An array of entities 

function GetInventoryList()
end

--- (403) GetInventoryID
-- @return An entity 

function GetInventoryID()
end

--- (404) SetInventoryID
-- @param param1 An entity 

function SetInventoryID(param1)
end

--- (405) ClearInventoryObject

function ClearInventoryObject()
end

--- (406) GetUnk22EntityID
-- @return An entity 

function GetUnk22EntityID()
end

--- (407) ResetUnk22Entity
-- @param param1 An entity 

function ResetUnk22Entity(param1)
end

--- (408) InitUnkFields
-- @param param1 A boolean value 

function InitUnkFields(param1)
end

--- (409) GetMinDmg
-- @param param1 An entity 
-- @return An integer 

function GetMinDmg(param1)
end

--- (410) GetMaxDmg
-- @param param1 An entity 
-- @return An integer 

function GetMaxDmg(param1)
end

--- (411) SetWeaponModeByIndex
-- @param param1 An integer 

function SetWeaponModeByIndex(param1)
end

--- (412) GetNumWeaponModes
-- @return An integer 

function GetNumWeaponModes()
end

--- (413) GetActiveWeaponMode
-- @return A @{WeaponMode} table 

function GetActiveWeaponMode()
end

--- (414) SetNextWeaponMode
-- @param param1 A boolean value 
-- @return A boolean value 

function SetNextWeaponMode(param1)
end

--- (415) SetWeaponCalledShot
-- @param param1 A boolean value 
-- @return A boolean value 

function SetWeaponCalledShot(param1)
end

--- (416) WeaponCanUseCalledShot
-- @return A boolean value 

function WeaponCanUseCalledShot()
end

--- (417) CanWeaponAndAttackerUseCalledShot
-- @param param1 An integer 
-- @return A boolean value 

function CanWeaponAndAttackerUseCalledShot(param1)
end

--- (418) UseAmmoForCurrentMode
-- @return A boolean value 

function UseAmmoForCurrentMode()
end

--- (419) HaveEnoughAmmoAndCanShoot
-- @return A boolean value 

function HaveEnoughAmmoAndCanShoot()
end

--- (420) WeaponHasNonNormalPrimaryDamageTypeMaybe
-- @return A boolean value 

function WeaponHasNonNormalPrimaryDamageTypeMaybe()
end

--- (421) ExplodeHeldWeaponMaybe
-- @return An integer 

function ExplodeHeldWeaponMaybe()
end

--- (422) GetAPCost
-- @param param1 An entity 
-- @return An integer 

function GetAPCost(param1)
end

--- (423) WeaponLoadingAmmoRelated
-- @param param1 An entity 
-- @param param2 An entity 
-- @return An @{InventoryActionResult} table 

function WeaponLoadingAmmoRelated(param1, param2)
end

--- (424) MaybeUnloadAmmoFromWeapon
-- @param param1 An entity 
-- @param param2 An entity 
-- @return An entity 

function MaybeUnloadAmmoFromWeapon(param1, param2)
end

--- (425) GetAmmoTypeString
-- @return A string 

function GetAmmoTypeString()
end

--- (426) GetWeaponPerkString
-- @return A string 

function GetWeaponPerkString()
end

--- (427) GetWeaponDamageRadius
-- @return A floating-point value 

function GetWeaponDamageRadius()
end

--- (428) GetImpactFX
-- @return A string 

function GetImpactFX()
end

--- (429) GetSoundFX
-- @return A string 

function GetSoundFX()
end

--- (430) GetProjectileName
-- @return A string 

function GetProjectileName()
end

--- (431) GetPrimaryDamageType
-- @return A string 

function GetPrimaryDamageType()
end

--- (432) GetSecondaryDamageType
-- @return A string 

function GetSecondaryDamageType()
end

--- (433) GetAmmoVariantString
-- @return A string 

function GetAmmoVariantString()
end

--- (434) GetAmmoOrUsesLeft
-- @return An integer 

function GetAmmoOrUsesLeft()
end

--- (435) GetMaxUsesOrAmmoCapacity
-- @return An integer 

function GetMaxUsesOrAmmoCapacity()
end

--- (436) WeaponIsDestroyUser
-- @return A boolean value 

function WeaponIsDestroyUser()
end

--- (437) FindSuitableAmmoInInventory
-- @param param1 An array of entities 
-- @return An entity 

function FindSuitableAmmoInInventory(param1)
end

--- (438) ContainsEntity
-- @param param1 An entity 
-- @return A boolean value 

function ContainsEntity(param1)
end

--- (439) GetEffectiveWeaponRangeByRangeType
-- @return A floating-point value 

function GetEffectiveWeaponRangeByRangeType()
end

--- (440) GetRange
-- @param param1 An entity 
-- @return A floating-point value 

function GetRange(param1)
end

--- (441) GetMinRange
-- @return A floating-point value 

function GetMinRange()
end

--- (442) GetAverageDamage
-- @return An integer 

function GetAverageDamage()
end

--- (443) CanWeaponAOE
-- @return A boolean value 

function CanWeaponAOE()
end

--- (444) CanWeaponNotAOE
-- @return A boolean value 

function CanWeaponNotAOE()
end

--- (445) CanFireWithRemainingAP
-- @param param1 An integer 
-- @param param2 An entity 
-- @return A boolean value 

function CanFireWithRemainingAP(param1, param2)
end

--- (446) IsActivated
-- @return A boolean value 

function IsActivated()
end

--- (447) GetFrequency
-- @return An integer 

function GetFrequency()
end

--- (448) ActivateTrapRelated
-- @param param1 An entity 
-- @param param2 An entity 

function ActivateTrapRelated(param1, param2)
end

--- (449) GetTrapTimeToDetonation
-- @return A floating-point value 

function GetTrapTimeToDetonation()
end

--- (450) SetTrapFrequencyCode
-- @param param1 An integer 
-- @param param2 An integer 

function SetTrapFrequencyCode(param1, param2)
end

--- (451) DetonateTrap sets off a trap immediately.

function DetonateTrap()
end

--- (452) HasActiveTrap
-- @return A boolean value 

function HasActiveTrap()
end

--- (453) TrapRelatedEitherDisarmOrTrigger
-- @param param1 An entity 
-- @return A boolean value 

function TrapRelatedEitherDisarmOrTrigger(param1)
end

--- (454) SetContainedTrap
-- @param param1 An entity 
-- @return A boolean value 

function SetContainedTrap(param1)
end

--- (455) PlaceTrapMaybe
-- @param param1 An entity 
-- @param param2 An entity 
-- @param param3 A boolean value 

function PlaceTrapMaybe(param1, param2, param3)
end

--- (456) DisarmTrap immediately disarms a trap, and makes it visible if it is hidden.

function DisarmTrap()
end

--- (457) ActivateOrBreakAlarm
-- @param param1 An entity 

function ActivateOrBreakAlarm(param1)
end

--- (458) IsDoorOpen
-- @return A boolean value 

function IsDoorOpen()
end

--- (459) IsDoorClosed
-- @return A boolean value 

function IsDoorClosed()
end

--- (460) IsLocked
-- @return A boolean value 

function IsLocked()
end

--- (461) CanDoorOpen
-- @param param1 An entity 
-- @return A boolean value 

function CanDoorOpen(param1)
end

--- (462) Vtable462DoorRelated
-- @param param1 An entity 
-- @return A boolean value 

function Vtable462DoorRelated(param1)
end

--- (463) SetupCombatFXCombatMessageStruct
-- @param param1 A @{CombatMessage} table 
-- @param param2 An integer 

function SetupCombatFXCombatMessageStruct(param1, param2)
end

--- (464) Vtable464NoOp
-- @param param1 An integer 
-- @param param2 An integer 
-- @param param3 An integer 
-- @param param4 An integer 
-- @param param5 An integer 
-- @param param6 An integer 

function Vtable464NoOp(param1, param2, param3, param4, param5, param6)
end

--- (465) Vtable465CombatFX
-- @param param1 A @{CombatMessage} table 

function Vtable465CombatFX(param1)
end

--- (466) Vtable466NoOp
-- @param param1 An integer 
-- @param param2 An integer 
-- @param param3 An integer 
-- @param param4 An integer 

function Vtable466NoOp(param1, param2, param3, param4)
end

--- (467) Vtable467CombatFX
-- @param param1 A @{Vector3} table 

function Vtable467CombatFX(param1)
end

--- (468) Vtable468CombatFX

function Vtable468CombatFX()
end

--- (469) Vtable469CombatFX
-- @param param1 A @{Vector3} table 

function Vtable469CombatFX(param1)
end

--- (470) CombatFXBloodSprayRelated
-- @param param1 A @{CombatMessage} table 

function CombatFXBloodSprayRelated(param1)
end

--- (471) CombatFXBloodTrailRelated
-- @param param1 A @{CombatMessage} table 
-- @param param2 An integer 

function CombatFXBloodTrailRelated(param1, param2)
end

--- (472) CombatFXBloodPoolRelated
-- @param param1 An entity 

function CombatFXBloodPoolRelated(param1)
end

--- (473) SetupScenaryMove
-- @param param1 A @{Vector3} table 
-- @param param2 An integer 

function SetupScenaryMove(param1, param2)
end

--- (474) Vtable474NoOp
-- @param param1 An integer 
-- @param param2 An integer 
-- @param param3 An integer 
-- @param param4 An integer 
-- @param param5 An integer 
-- @param param6 An integer 

function Vtable474NoOp(param1, param2, param3, param4, param5, param6)
end

--- (475) Vtable475NoOp
-- @param param1 An integer 
-- @param param2 An integer 
-- @param param3 An integer 
-- @param param4 An integer 
-- @param param5 An integer 
-- @param param6 An integer 

function Vtable475NoOp(param1, param2, param3, param4, param5, param6)
end

--- (476) Vtable476ReturnFloatZero
-- @param param1 An integer 
-- @param param2 An integer 
-- @return A floating-point value 

function Vtable476ReturnFloatZero(param1, param2)
end

--- (477) Vtable477ActorMaybeWeaponImpactKnockdownRelated
-- @param param1 An integer 
-- @param param2 A @{Vector3} table 
-- @return A floating-point value 

function Vtable477ActorMaybeWeaponImpactKnockdownRelated(param1, param2)
end

--- (478) Vtable478NoOp

function Vtable478NoOp()
end

--- (479) Vtable479ReturnFalse
-- @return A boolean value 

function Vtable479ReturnFalse()
end

--- (480) Vtable480DestructsStuff
-- @param param1 An integer 
-- @param param2 An integer 
-- @param param3 An integer 

function Vtable480DestructsStuff(param1, param2, param3)
end

--- (481) IsVehicleEmpty
-- @return A boolean value 

function IsVehicleEmpty()
end

--- (482) IsThisEntityTheDriver
-- @param param1 An entity 
-- @return A boolean value 

function IsThisEntityTheDriver(param1)
end

--- (483) IsThisEntityTheGunner
-- @param param1 An entity 
-- @return A boolean value 

function IsThisEntityTheGunner(param1)
end

--- (484) IsThisEntityInVehicle
-- @param param1 An entity 
-- @return A boolean value 

function IsThisEntityInVehicle(param1)
end

--- (485) GetVehicleDriver
-- @return An entity 

function GetVehicleDriver()
end

--- (486) GetVehiclePassengerVector
-- @return An array of entities 

function GetVehiclePassengerVector()
end

--- (487) VehicleBoundingBoxStuff1 is BROKEN. Do not use.

function VehicleBoundingBoxStuff1()
end

--- (488) IsInReverse
-- @return A boolean value 

function IsInReverse()
end

--- (489) VehiclePassengerDisembarkAllowed
-- @param param1 An entity 
-- @param param2 A @{Vector3} table 

function VehiclePassengerDisembarkAllowed(param1, param2)
end

--- (490) SetExitPoint
-- @return A @{Vector3} table 

function SetExitPoint()
end

--- (491) IsNotDeactivated
-- @return A boolean value 

function IsNotDeactivated()
end

--- (492) ActivateActor

function ActivateActor()
end

--- (493) DeactivateActor

function DeactivateActor()
end

--- (494) Vtable494ActorEffectsRelated
-- @param param1 An entity 

function Vtable494ActorEffectsRelated(param1)
end

--- (495) RemoveEffectID
-- @param param1 An entity 

function RemoveEffectID(param1)
end

--- (496) ApplyEffectsWithGivenTagName
-- @param param1 A string 

function ApplyEffectsWithGivenTagName(param1)
end

--- (497) BoundingBox2dSet
-- @return A boolean value 

function BoundingBox2dSet()
end

--- (498) GetListOfContainedEntities
-- @return An array of entities 

function GetListOfContainedEntities()
end

--- (499) ScriptConditionCheckMaybe
-- @param param1 A string 
-- @param param2 A @{ZoneDefinition} table 
-- @return A boolean value 

function ScriptConditionCheckMaybe(param1, param2)
end

--- (500) KillEntity immediately kills an @{Entity}.
-- @param param1 A string indicating the death type animation to use.

function KillEntity(param1)
end

--- (501) SetActivationState
-- @param param1 An integer 

function SetActivationState(param1)
end

--- (502) IsInActiveState
-- @return A boolean value 

function IsInActiveState()
end

--- (503) IsEnabledForScriptEvents
-- @return A boolean value 

function IsEnabledForScriptEvents()
end

--- (504) SetLockedState
-- @param param1 A boolean value 

function SetLockedState(param1)
end

--- (505) Vtable505ActorCallsWorldForSomething

function Vtable505ActorCallsWorldForSomething()
end

--- (506) GetRandomSpeechIDString
-- @return A string 

function GetRandomSpeechIDString()
end

--- (507) GetClickSpeechIDString
-- @return A string 

function GetClickSpeechIDString()
end

--- (508) SetRandomSpeechIDString
-- @param param1 A string 

function SetRandomSpeechIDString(param1)
end

--- (509) SetClickSpeechIDString
-- @param param1 A string 

function SetClickSpeechIDString(param1)
end

--- (510) ActOnControllerCommand
-- @param commandstruct A ControllerCommandStruct table.
-- @return None.
function ActOnControllerCommand(commandstruct)
end

--- (511) Vtable511ActorVehicle
-- @return A boolean value 

function Vtable511ActorVehicle()
end

--- (512) SetHavingTurn
-- @param param1 A boolean value 

function SetHavingTurn(param1)
end

--- (513) IsHavingTurn
-- @return A boolean value 

function IsHavingTurn()
end

--- (514) SetOverwatch
-- @param param1 A boolean value 

function SetOverwatch(param1)
end

--- (515) IsOverwatch
-- @return A boolean value 

function IsOverwatch()
end

--- (516) ResetBonusAC

function ResetBonusAC()
end

--- (517) SetBonusACForHTHEvade

function SetBonusACForHTHEvade()
end

--- (518) GetUnk23
-- @return An integer 

function GetUnk23()
end

--- (519) Vtable519EntityESSRelated
-- @param param1 An integer 
-- @return A boolean value 

function Vtable519EntityESSRelated(param1)
end

--- (520) DoSomethingWithHP
-- @param param1 An integer 
-- @return A boolean value 

function DoSomethingWithHP(param1)
end

--- (521) Vtable521ALLSomethingWithAllFields is BROKEN. Do not use.

function Vtable521ALLSomethingWithAllFields()
end

--- (522) Vtable522EntityDunno
-- @param param1 A string 

function Vtable522EntityDunno(param1)
end

--- (523) GetClassType is BROKEN. Do not use.

function GetClassType()
end

--- (524) ResetAndOrDestruct
-- @param param1 An integer 

function ResetAndOrDestruct(param1)
end

--- (525) IsSomethingMDCRelated
-- @return A boolean value 

function IsSomethingMDCRelated()
end

--- (526) ActorPostureRelated is BROKEN. Do not use. @{Actor} only.

function ActorPostureRelated()
end

--- (527) RenderUnconscious is BROKEN. Do not use. @{Actor} only.

function RenderUnconscious()
end

--- (528) Vtable528NoOp  @{Actor} only.
-- @param param1 An integer 
-- @param param2 An integer 
-- @return A floating-point value 

function Vtable528NoOp(param1, param2)
end

--- (529) AdjustRadLevelFromRadPoints  @{Actor} only.

function AdjustRadLevelFromRadPoints()
end

--- (530) ApplyRadLevelPenalties  @{Actor} only.
-- @param param1 An integer 

function ApplyRadLevelPenalties(param1)
end

--- (531) UndoExistingRadLevelPenalties  @{Actor} only.

function UndoExistingRadLevelPenalties()
end

--- (532) PoisonEntity2  @{Actor} only.
-- @param param1 A floating-point value 
-- @param param2 A boolean value 

function PoisonEntity2(param1, param2)
end

--- (533) ApplyPoisonDamage  @{Actor} only.

function ApplyPoisonDamage()
end

--- (534) ApplyOverdoseDamage  @{Actor} only.

function ApplyOverdoseDamage()
end

--- (535) ContestedSkillRollMaybe  @{Actor} only.
-- @param param1 An integer 
-- @param param2 An integer 
-- @return An integer 

function ContestedSkillRollMaybe(param1, param2)
end


--- Types.
-- @section Types

--- A floating-point array holding XYZ location information.
-- @field x X coordinate.
-- @field y Y coordinate.
-- @field z Z coordinate..
-- @table Vector3

--- A floating-point array holding RGB color information.
-- @field x X coordinate.
-- @field y Y coordinate.
-- @field z Z coordinate..
-- @table RGBColor

--- A table indicating the result of an inventory action.
-- @field status An integer indicating the result of the action. Generally uses zero value to mean success, and non-zero for some kind of failure.
-- @field unk_entity_1 An entity object. Typically the target object of the action.
-- @field unk2 An unknown integer field.
-- @field unk3 An unknown integer field.
-- @field errstring A string containing a locale string identifier for a message to give to the player, indicating why the action failed.
-- @field unk_entity4 An entity object. Use is still unknown.
-- @field unk_5 An unknown integer field.
-- @table InventoryActionResult

--- A table describing a zone within the current level.
-- @field bound_top A @{Vector3} indicating the top corner of the zone.
-- @field bound_bottom A @{Vector3} indicating the bottom corner of the zone.
-- @field zonecolor An @{RGBColor} indicating the color of the zone in the FT Tools editor.
-- @field zonename A string indicating the name of the zone.		
-- @field soundzone A boolean value indicating if the zone is a sound zone.
-- @field unk An unknown boolean value.
-- @table ZoneDefinition

--- A table used when determining ability to hit a target with a weapon.
-- @field hit_chance An integer indicating capped chance to hit. Seems unused by vtable functions.
-- @field raw_chance An integer indicating uncapped chance to hit. Seems unused by vtable functions.
-- @field ineligible_flags An integer indicating reasons why a shot is not possible. Zero if shot is clear, non-zero if blocked or out of range.
-- @field unk2 An unknown integer value.
-- @table ChanceToHit

--- A table used by the game when evaluating results of combat actions.
-- @field attacker An @{Entity} for the attacker in the combat action.
-- @field target An @{Entity} for the target in the combat action.
-- @field weapon An @{Entity} for the weapon used in the combat action.
-- @field unk1 An unknown integer field.
-- @field attacker_pos A @{Vector3} position for the attacker.
-- @field target_pos A @{Vector3} position for the target.
-- @field damage An integer indicating damage done to the target.
-- @field critflags An integer containing bitwise flags indicating which type(s) of critical hits were made.
-- @field unk3 An unknown integer field.
-- @field unk4 An unknown integer field.
-- @field aimedlocation An integer indicating location aimed at. Zero is unaimed, values 1-8 are corresponding body parts.
-- @field numshots An integer indicating the number of shots fired for the given target.
-- @field unk5 An unknown integer field.
-- @table CombatMessage

--- A table indicating the fields from the ActorStatus section of the @{Actor}.
-- @field hp An integer value indicating current HP for the Actor.
-- @field bandaged An integer value indicating number of times the Actor was treated with First Aid recently.
-- @field ap A **floating-point value** indicating the Actor's remaining action points.
-- @field radpoints An integer value indicating the Actor's current radiation points.
-- @field radlevel An integer value indicating the Actor's current radiation effect level.
-- @field poisonpoints An integer value indicating the Actor's current outstanding poison damage.
-- @field poisonlevel An integer value indicating the Actor's poison level value (not sure?).
-- @field overdosepoints An integer value indicating the Actor's current chem overdose points.
-- @field goneuplevel A boolean value indicating if the Actor has a pending level increase.
-- @table ActorStatus

--- A table indicating one or more attributes (stats, traits, skills, perks, etc.).
-- @field various Members of the AttributesTable object are comprised of the name of an attribute (any of the stats, skills, etc.), with a value indicating the value of that attribute.
-- @table AttributesTable

--- A table used in animation of an @{Actor}. Appears to be used to offset the origin point of a weapon attack relative to the Actor's @{Vector3} position.
-- @field x An integer value giving the X axis offset.
-- @field y An integer value giving the Y axis offset.
-- @field z An integer value giving the Z axis offset.
-- @table AnimOffset