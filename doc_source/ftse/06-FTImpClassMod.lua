--- improver provides a set of functions that allow Lua to configure the built-in FTImprover functionality in FTSE.
-- Note that functions in this class must be called as part of the code executed by the @{Hooks:OnStart} hook, otherwise they will not have the intended effect.
-- @classmod improver

--- FTImprover module helper functions.
-- These functions are used by the FTImprover module to handle the data files in the original format.
-- @section Helper

--- ParseConfig is used by the FTImprover module to process an FTImprover-formatted configuration or definition file.
-- A mod using the Lua interface to FTImprover features should not need to use this function.
-- @param data The contents of a FTImprover configuration or definition file.
-- @return None.
function ParseConfig(data)
end

--- IsFeatureEnabled will return true if the specified feature was enabled in the FTImprover module configuration.
-- This is primarily used by the FTImprover module to keep track of which data files need to be processed,
-- but could also be useful if a modification provides configuration options that would change the FTImprover
-- feature set.
-- @param feature The name of the feature to check. Can be one of APCT, AnimationNames, WeaponItemTypes, WeaponSoundTypes, AmmoTypes, AmmoVariants, ArmourTypes, or ImpactSounds.
-- @return True if the feature was enabled.
function IsFeatureEnabled(feature)
end

--- Feature toggle flags.
-- These functions will enable specific FTImprover features.
-- @section Toggle

--- EnableScriptTriggers will enable the "allTriggerConditions" patch behavior as was implemented in FTImprover.
-- @return None.
function EnableScriptTriggers()
end

--- EnableGotoMissionFix will enable the "GOTO_MISSION trigger action" patch behavior as was implemented in FTImprover.
-- @return None.
function EnableGotoMissionFix()
end

--- EnableMultiProcessFix will enable the "multipleBOSprocesses" patch behavior as was implemented in FTImprover.
-- @return None.
function EnableMultiProcessFix()
end

--- List management functions.
-- These functions will add or clear items from the internal game lists.
-- @section Lists

--- AddAmmoType will add a new ammo type to the current list.
-- @param name The name of the new ammo type.
-- @return None.
function AddAmmoType(name)
end

--- ClearAmmoTypes will clear the default list of ammo types in the game, allowing a modification to redefine all available types.
-- A mod will typically use this function to redefine or replace existing ammo types in the game.
-- @return None.
function ClearAmmoTypes()
end

--- AddImpactSound will add a new impact sound to the current list.
-- @param name The name of the new ammo type.
-- @return None.
function AddImpactSound(name)
end

--- ClearImpactSounds will clear the default list of impact sounds in the game, allowing a modification to redefine all available sound entries.
-- A mod will typically use this function to redefine or replace existing impact sounds in the game.
-- @return None.
function ClearImpactSounds()
end

--- AddAnimationName will add a new animation name to the current list.
-- @param name The name of the new animation.
-- @return None.
function AddAnimationName(name)
end

--- ClearAnimationNames will clear the default list of animation names in the game, allowing a modification to redefine all available animations.
-- A mod will typically use this function to redefine or replace existing animation names in the game.
-- @return None.
function ClearAnimationNames()
end

--- AddWeaponItemType will add a new weapon item type to the current list.
-- @param name The name of the new weapon item type.
-- @return None.
function AddWeaponItemType(name)
end

--- ClearWeaponItemTypes will clear the default list of weapon item types in the game, allowing a modification to redefine all available weapon types.
-- A mod will typically use this function to redefine or replace existing weapon item types in the game.
-- @return None.
function ClearWeaponItemTypes()
end

--- AddWeaponSoundType will add a new weapon sound type to the current list.
-- @param name The name of the new weapon sound type.
-- @return None.
function AddWeaponSoundType(name)
end

--- ClearWeaponSoundTypes will clear the default list of weapon sound types in the game, allowing a modification to redefine all available weapon sounds.
-- A mod will typically use this function to redefine or replace existing weapon sound types in the game.
-- @return None.
function ClearWeaponSoundTypes()
end

--- AddAmmoVariant will add a new ammo variant to the current list.
-- @param name The name of the new ammo variant.
-- @param damage The damage multiplier percentage adjustment to use for the new ammo variant (positive to increase damage, negative to reduce).
-- @param penetration The penetration multiplier percentage adjustment to use for the new ammo variant (positive to increase penetration, negative to increase resistance).
-- @return None.
function AddAmmoVariant(name,damage,penetration)
end

--- ClearAmmoVariants will clear the default list of ammo variants in the game, allowing a modification to redefine all available ammo variants.
-- A mod will typically use this function to redefine or replace existing ammo variants in the game.
-- @return None.
function ClearAmmoVariants()
end

--- AddAPCT will add a new action point cost type to the current list.
-- @param name The name of the new action point cost type.
-- @param purpose The action type purpose value. 0 is general, 1 is combat, 2 is treatment or repair.
-- @param type The type of action. 0 is none, 1 is H2H, 2 is melee, 3 is throwing, 4 is ranged, single attack, or setting trap, 5 is ranged, burst and spray, and 6 is equip.
-- @param cost The cost of the action.
-- @return None.
function AddAPCT(name,purpose,type,cost)
end

--- ClearAPCT will clear the default list of action point cost types in the game, allowing a modification to redefine all available action point types.
-- A mod will typically use this function to redefine or replace existing action point cost types in the game.
-- @return None.
function ClearAPCT()
end

--- AddAPCT will add a new action point cost type to the current list.
-- @param name The name of the new action point cost type.
-- @param purpose The action type purpose value. 0 is general, 1 is combat, 2 is treatment or repair.
-- @param type The type of action. 0 is none, 1 is H2H, 2 is melee, 3 is throwing, 4 is ranged, single attack, or setting trap, 5 is ranged, burst and spray, and 6 is equip.
-- @param cost The cost of the action.
-- @return None.
function AddAPCT(name,purpose,type,cost)
end

--- ClearAPCT will clear the default list of action point cost types in the game, allowing a modification to redefine all available action point types.
-- A mod will typically use this function to redefine or replace existing action point cost types in the game.
-- @return None.
function ClearAPCT()
end

--- AddArmourType will add a new armour type to the current list.
-- Note that in the Lua interface, the other parameters are not set here; they are instead set by using @{AddArmourRule} and @{AddSoundRule}. See those functions for
-- information on how to define rules for the new armour type.
-- @param name The name of the new armour type.
-- @return None.
function AddArmourType(name)
end

--- ClearArmourTypes will clear the default list of armour types in the game, allowing a modification to redefine all available armour types.
-- A mod will typically use this function to redefine or replace existing armour types in the game.
-- @return None.
function ClearArmourTypes()
end

--- Armour rule functions.
-- These functions are used to set rules for which sprite and sound effect prefix to use, based on a character's armour, sex/gender, and race.
-- Each rule contains a string that is matched against the three attributes listed, with an asterisk '*' as a wildcard to match any value.
-- Rules are processed in the order they are created, with internal default rules coming first, unless they are disabled. The first rule
-- that matches successfully is the one which determines the final sprite or sound name, based on the output string of that rule. In
-- the output string, the character's armour type, sex/gender, and race values can be inserted in the string using '%a', '%s', and '%r' respectively.
-- 
-- @usage The default sprite rules are listed below, in the format "(armour,sex,race)=output":
-- (None,*,*)=sprites/characters/%a.spr
-- (Leather,*,*)=sprites/characters/%a%s.spr
-- (Metal,*,*)=sprites/characters/%a%s.spr
-- (Environmental,*,*)=sprites/characters/%a.spr
-- (Power,*,*)=sprites/characters/%a.spr
-- (Raider,*,*)=sprites/characters/%a%s.spr
-- (Reaver,*,*)=sprites/characters/%a%s.spr
-- (Ghoul,*,*)=sprites/characters/GhoulArmour.spr
-- (Mutant,*,*)=sprites/characters/MutantArmour.spr
-- (Brahmin,*,BOS)=sprites/critters/TwoHeadedBrahmin.spr
-- (Brahmin,*,Tribal)=sprites/critters/TwoHeadedBrahmin.spr
-- (Brahmin,*,Raider)=sprites/critters/TwoHeadedBrahmin.spr
-- (Brahmin,*,Reaver)=sprites/critters/TwoHeadedBrahmin.spr
-- (Brahmin,*,CitizenAlpha)=sprites/critters/TwoHeadedBrahmin.spr
-- (Brahmin,*,CitizenThin)=sprites/critters/TwoHeadedBrahmin.spr
-- (Brahmin,*,CitizenMedium)=sprites/critters/TwoHeadedBrahmin.spr
-- (Brahmin,*,VaultZero)=sprites/critters/TwoHeadedBrahmin.spr
-- (*,*,BOS)=sprites/characters/Tribal%s.spr
-- (*,*,Raider)=sprites/characters/Raider%s.spr
-- (*,*,Reaver)=sprites/characters/Reaver%s.spr
-- (*,*,Ghoul)=sprites/characters/Ghoul.spr
-- (*,*,Mutant)=sprites/characters/Mutant.spr
-- (*,*,TribalLarge)=sprites/characters/TribalMaleLarge.spr
-- (*,*,RaiderLarge)=sprites/characters/RaiderMaleLarge.spr
-- (*,*,RaiderHuge)=sprites/characters/RaiderMaleHuge.spr
-- (*,*,ReaverHuge)=sprites/characters/Reaver%s.spr
-- (*,*,BOSScribe)=sprites/characters/BOSScribe.spr
-- (*,*,BOSElder)=sprites/characters/BOSElder.spr
-- (*,*,CitizenAlpha)=sprites/characters/CitizenAlpha.spr
-- (*,*,CitizenThin)=sprites/characters/CitizenThin%s.spr
-- (*,*,CitizenMedium)=sprites/characters/CitizenMedium%s.spr
-- (*,*,VaultZero)=sprites/characters/Vault%s.spr
-- (*,*,PipBoy)=sprites/characters/Pipboy.spr
-- (*,*,Dummy)=sprites/characters/Dummy.spr
-- (*,*,Deathclaw)=sprites/critters/DeathClaw.spr
-- (*,*,DeathclawBaby)=sprites/critters/DeathClawBaby.spr
-- (*,*,BDCSmall)=sprites/critters/SDC.spr
-- (*,*,BDCLarge)=sprites/critters/BDC.spr
-- (*,*,Dog)=sprites/critters/MutantDog.spr
-- (*,*,Wasp)=sprites/critters/Wasp.spr
-- (*,*,Radscorpion)=sprites/critters/RadScorpion.spr
-- (*,*,Rat)=sprites/critters/GiantRat.spr
-- (*,*,Brahmin)=sprites/critters/TwoHeadedBrahmin.spr
-- (*,*,Komodo)=sprites/critters/MutantLizard.spr
-- (*,*,Wolf)=sprites/critters/Wolf.spr
-- (*,*,RobotHumanoid)=sprites/robots/Humanoid.spr
-- (*,*,RobotPacification)=sprites/robots/Pacification.spr
-- (*,*,RobotLifter)=sprites/robots/LoadLifter.spr
-- (*,*,RobotHover)=sprites/robots/SmallHover.spr
-- (*,*,RobotTank)=sprites/robots/MTT.spr
-- (*,*,RobotBehemoth)=sprites/robots/Behemoth.spr
-- (*,*,RobotSecurity)=sprites/robots/Security.spr
-- (*,*,RobotScurry)=sprites/robots/Scurry.spr
--
-- The default sound prefix rules are listed below:
-- (None,*,*)=Leath
-- (Leather,*,*)=Leath
-- (Metal,*,*)=Metal
-- (Environmental,*,*)=Metal
-- (Power,*,*)=Pow
-- (Raider,*,*)=Leath
-- (Reaver,*,*)=Metal
-- (Ghoul,*,*)=Leath
-- (Mutant,*,*)=Metal
-- (Brahmin,*,*)=Leath
-- (*,*,Reaver)=Metal
-- (*,*,ReaverHuge)=Metal
-- (*,*,Raider)=Leath
-- (*,*,RaiderLarge)=Leath
-- (*,*,RaiderHuge)=Leath
--
-- There are other sound prefix rules that are completely hardcoded, and applied if there is no match:
-- The CyborgGeneral sprite uses Pow
-- The BrainJar sprite uses Metal
-- The GunTurret01 race uses Metal
-- Otherwise, a living target uses Flesh, and a non-living target uses Pow
-- @section ArmourRules

--- AddArmourRule will add a new armour sprite rule.
-- @param armour The name of the armour type for the rule, or * to match all armour types.
-- @param sex The sex/gender of the character to apply the rule to, or * to match all.
-- @param race The race of the character to apply the rule to, or * to match all.
-- @param output A string indicating the name of the sprite to use (with wildcards as described).
-- @return None.
function AddArmourRule(armour,sex,race,output)
end

--- AddSoundRule will add a new armour sound rule.
-- @param armour The name of the armour type for the rule, or * to match all armour types.
-- @param sex The sex/gender of the character to apply the rule to, or * to match all.
-- @param race The race of the character to apply the rule to, or * to match all.
-- @param output A string indicating the name of the sprite to use (with wildcards as described).
-- @return None.
function AddSoundRule(armour,sex,race,output)
end

--- DisableDefaultArmourRules will remove the built-in armour sprite and sound rules defined in the game code.
-- This is useful if a mod wants to completely change how sprites and sounds are applied depending on armour,
-- sex/gender, and race.
-- @return None.
function DisableDefaultArmourRules()
end
