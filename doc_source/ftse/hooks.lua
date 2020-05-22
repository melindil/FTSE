--- Defines the hooks provided by FTSE, for the Lua script to be notified
-- when certain game events occur.  The FTSE engine will call the corresponding
-- function in ftse.lua at the time the event occurs, with parameters corresponding
-- to the state of the Fallout: Tactics game engine.
-- Most hooks have one or more parameters, which will contain either information
-- from the Fallout: Tactics executable (translated through the FTSE engine), or
-- class objects corresponding to game state (world, entities, etc.). Some hooks also
-- have an optional or required return value, which will often result in a change
-- in game state depending on the hook.
-- @module Hooks

--- Startup hooks.
-- These hooks are generally called once at the start of the game process.
-- @section Startup

--- OnStart is called immediately after start of the FTSE engine.
-- This hook allows the Lua script a chance to update the internal tables
-- (currently only perks are supported) prior to the game actually starting.
-- @return None.
function OnStart()
end

--- DefaultStyleChanges is called when the Fallout: Tactics executable sets
-- up the default style object, used for text coloring.  The Lua script can use
-- the given DefaultStyle object to customize text colors.
-- @param style The @{DefaultStyle} object, which can be called to make the desired text color customization.
-- @return None.
function DefaultStyleChanges(style)
end

--- OnLocaleLoad is called after the Fallout: Tactics executable has set up the
-- table for locale strings.  The Lua script can use the AddLocaleString function
-- of the hookexecutor global object to add new locale strings.
-- @return None.
function OnLocaleLoad()
end

--- Utility hooks.
-- These hooks are called when the game needs to calculate some value for which
-- a Lua script may want to override the result.
-- @section Utility

--- MsecToDayHMS is called whenever Fallout: Tactics needs to convert the internal
-- game time millisecond counter to an in-game date/time value.  The conversion takes
-- place in two steps.  MsecToDayHMS converts the millisecond counter to days, hours,
-- minutes, seconds, and milliseconds, optionally scaling it by the normal game
-- scaling factor (by default, 3 game seconds to 1 real second).  This is then added
-- to the mission/campaign start offset by @{AddBaseToGameTime}.
-- @param msec The current time on the game millisecond timer.
-- @param scale Whether this conversion should use the scaling factor.
-- @return (required) A table formatted as a @{TimeStamp} value, with only days, hours, minutes, seconds, and milliseconds converted.
function MsecToDayHMS(msec, scale)
end

--- AddBaseToGameTime is called whenever Fallout: Tactics needs to convert a
-- day+hour+minute+second timestamp to a year/month/day hh:mm:ss time.
-- The conversion takes place in two steps.  @{MsecToDayHMS} first converts the
-- millisecond timer to a day+hour+minute+second format.  Then, AddBaseToGameTime
-- adds starting year, month, day, etc. to that to give a final date/time.
-- @param timestamp The @{TimeStamp} value returned from @{MsecToDayHMS}.
-- @return (required) A table formatted as a @{TimeStamp} value with all fields adjusted based on desired starting date/time for the campaign.
function AddBaseToGameTime(timestamp)
end

--- Event hooks.
-- These hooks are called when game events occur.
-- @section Event

--- OnLongTick is called once per 10 seconds of game time, per entity.
-- This interval is used internally for checking radiation exposure, and nearby
-- entities for Leader, Team Player, Loner, and Vat Skin perks/traits.
-- @param ent The entity whose long tick is being triggered.
-- @return None.

function OnLongTick(ent)
end

--- OnRadiated is called whenever an entity suffers radiation damage.
-- This function is deprecated and will be replaced in the future with a
-- more general damage notification hook.
-- @param ent Entity which suffered radiation damage.
-- @return None.
function OnRadiated(ent)
end

--- OnVariableChanged is called whenever a mission or campaign variable is updated.
-- Mission and campaign variables are stored as key/value pairs, with both
-- key and value being strings.  Fallout: Tactics will use these variables for
-- updating its own triggers and actions.  This hook allows Lua to be notified
-- similarly whenever a variable is changed.
-- @param key The key (name) of the variable which changed.
-- @param value The new value for the variable.
-- @param isCampaign Boolean field indicating if the changed variable is a campaign variable (true) or mission variable (false).
-- @return None.
function OnVariableChanged(key, value, isCampaign)
end

--- Combat hooks.
-- These hooks are called whenever combat results are being calculated.
-- @section Combat

--- OnChanceToHitCalc is called whenever the game needs to calculate the chance
-- of an attack hitting a desired target. This is used in showing the percentage
-- chance on the mouse cursor prior to an attack, in determining whether an attack
-- meets the necessary chance for the sentry mode, and finally on the attack
-- calculation itself. Note that the raw chance is important even if greater than 95%, as the reduction in chance due to aim location is taken into account after this function is processed.
-- @param attacker An Entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param target The target being aimed at. Can be an Actor, Vehicle, Trap, Breakable, or StateBreakable.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @param chanceInfo A table holding the fields "hit\_chance" (game calculated chance to hit), "raw\_chance" (game calculated chance to hit, not capped at 95%), "location" (text string indicating the aimed location), and "ineligible\_flags" (an integer, non-zero if the target can't be hit due to obstruction or range limit).
-- @return (optional) The chanceInfo struct can be modified and returned by the function. Modifications to hit chance or eligibility are passed to Fallout: Tactics.
function OnChanceToHitCalc(attacker, target, weapon, chanceInfo)
end

--- OnChanceToCritical1 is called whenever an attack hits a target, and a check needs
-- to be made to determine if the attack is a critical hit.  Version 1 of this function
-- is called for every attack type.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param target The target being aimed at.  Can be an Actor, Vehicle, Trap, Breakable, or StateBreakable.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @param loc The location of an aimed attack, or an empty string for an unaimed attack.
-- @param chance The calculated chance for critical hit as determined by the game's default behavior.
-- @return (required) The modified chance of a critical hit. Return the value of "chance" parameter to make no changes to critical chance.
function OnChanceToCritical1(attacker, target, weapon, loc, chance)
end

--- OnChanceToCritical2 is called whenever an attack hits a target, and a check needs
-- to be made to determine if the attack is a critical hit.  Version 2 of this function
-- is called for unarmed or melee attacks only, and uses a separate set of criteria to determine
-- critical chance.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param target The target being aimed at.  Can be an Actor, Vehicle, Trap, Breakable, or StateBreakable.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @param loc The location of an aimed attack, or an empty string for an unaimed attack.
-- @param chance The calculated chance for critical hit as determined by the game's default behavior.
-- @return (required) The modified chance of a critical hit. Return the value of "chance" parameter to make no changes to critical chance.
function OnChanceToCritical2(attacker, target, weapon, loc, chance)
end

--- OnCriticalEffect1 is called whenever an attack critically hits a target, and a check needs
-- to be made to determine the effects of the critical hit. Version 1 of this function
-- is called when the critical check for all attack types (version 1 of @{OnChanceToCritical1})
-- succeeds.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param target The target being aimed at.  Can be an Actor, Vehicle, Trap, Breakable, or StateBreakable.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @param loc The location of an aimed attack, or an empty string for an unaimed attack.
-- @param roll The modified roll value for the critical attack. The value is not capped at 100.
-- @return[1] (either-or required) A modified value for the critical effect roll, to be used in the default lookup table. If no modifications are desired, return the value of the "roll" parameter.
-- @return[2] (either-or required) A table formatted as a @{CriticalEffects} value, with each desired critical effect to apply set to boolean true.
function OnCriticalEffect1(attacker, target, weapon, loc, roll)
end

--- OnCriticalEffect2 is called whenever an attack critically hits a target, and a check needs
-- to be made to determine the effects of the critical hit. Version 1 of this function
-- is called when the critical check for unarmed/melee attack types (version 2 of @{OnChanceToCritical2})
-- succeeds.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param target The target being aimed at.  Can be an Actor, Vehicle, Trap, Breakable, or StateBreakable.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @param loc The location of an aimed attack, or an empty string for an unaimed attack.
-- @param roll The modified roll value for the critical attack. The value is not capped at 100.
-- @return[1] (either-or required) A modified value for the critical effect roll, to be used in the default lookup table. If no modifications are desired, return the value of the "roll" parameter.
-- @return[2] (either-or required) A table formatted as a @{CriticalEffects} value, with each desired critical effect to apply set to boolean true.
function OnCriticalEffect2(attacker, target, weapon, loc, roll)
end

--- OnDamageCalc is called whenever the game needs to calculate the final damage of an attack.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param target The target being aimed at.  Can be an Actor, Vehicle, Trap, Breakable, or StateBreakable.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @param loc The location of an aimed attack, or an empty string for an unaimed attack.
-- @param damage The total damage as calculated by the game's default behaviors.
-- @param critTable A table formatted as a @{CriticalEffects} value, indicating what critical effects are being applied.
-- @param rollArray An array of integers providing the individual damage rolls perfomed by Fallout: Tactics for each shot in a multi-shot weapon, prior to any resistances being applied. These can be used for more complex damage calculation, or for overriding resistances.
-- @return (optional) A new damage value to pass to Fallout: Tactics for the given attack scenario. This damage will override that calculated by the game.
function OnDamageCalc(attacker, target, weapon, loc, damage, critTable, rollArray)
end

--- OnStraightAttack is called whenever the game needs to calculate the effects of a "Straight" or "Normal" arc weapon.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param shots The number of shots in the attack. Usually 1, but can be different for some weapons (e.g. triple-fire pistols use 3).
-- @param targetsArray A Lua array containing a @{TargetInfo} value for each eligible target. For Straight/Normal attacks, this is the intended target only.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @return (required) An array of @{TargetResults} values for each target to be hit by the attack. Can be empty.
function OnStraightAttack(attacker, shots, targetsArray, weapon)
end

--- OnAreaAttack is called whenever the game needs to calculate the effects of an area-effect hit, usually an explosion (grenades, rockets, etc).
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param shots The number of shots in the attack. Unsure what effect this has for area attacks.
-- @param targetsArray A Lua array containing a @{TargetInfo} value for each eligible target in range.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @return (required) An array of @{TargetResults} values for each target to be hit by the attack. Can be empty.
function OnAreaAttack(attacker, shots, targetsArray, weapon)
end

--- OnRadialAttack is called whenever the game needs to calculate the effects of a "Radial" arc weapon.
-- Note that this does not include explosions. One example is the electrical effect of the Pacifier Bot.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param shots The number of shots in the attack.
-- @param targetsArray A Lua array containing a @{TargetInfo} value for each eligible target in range.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @return (required) An array of @{TargetResults} values for each target to be hit by the attack. Can be empty.
function OnRadialAttack(attacker, shots, targetsArray, weapon)
end

--- OnSprayAttack is called whenever the game needs to calculate the effects of a "Spray" arc weapon.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param shots The number of shots in the attack.
-- @param targetsArray A Lua array containing a @{TargetInfo} value for each eligible target.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @return (required) An array of @{TargetResults} values for each target to be hit by the attack. Can be empty.
function OnSprayAttack(attacker, shots, targetsArray, weapon)
end

--- OnConeAttack is called whenever the game needs to calculate the effects of a "Cone" arc weapon (usually shotguns)
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param shots The number of shots in the attack.
-- @param targetsArray A Lua array containing a @{TargetInfo} value for each eligible target.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @return (required) An array of @{TargetResults} values for each target to be hit by the attack. Can be empty.
function OnConeAttack(attacker, shots, targetsArray, weapon)
end

--- OnBurstAttack is called whenever the game needs to calculate the effects of a "Burst" arc weapon.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param shots The number of shots in the attack.
-- @param targetsArray A Lua array containing a @{TargetInfo} value for each eligible target.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @return (required) An array of @{TargetResults} values for each target to be hit by the attack. Can be empty.
function OnBurstAttack(attacker, shots, targetsArray, weapon)
end

--- OnProjectileAttack is called whenever the game needs to calculate the effects of an attack which draws a projectile. This includes thrown weapons, energy weapons with visible laser/energy bursts, and rocket launcher attacks, among others.
-- @param attacker An entity corresponding to the attacker. Usually an instance of the Actor class.
-- @param shots The number of shots in the attack. The game will only animate a single projectile, but will calculate effects for all attacks.
-- @param targetsArray A Lua array containing a @{TargetInfo} value for each eligible target. For projectile attacks, this array includes only the intended target.
-- @param weapon The weapon being used in the attack. Usually a Weapon or VehicleWeapon, but could possibly be a Trap or a Breakable.
-- @return (required) An array of @{TargetResults} values for each target to be hit by the attack. Can be empty.
function OnProjectileAttack(attacker, shots, targetsArray, weapon)
end

--- Inventory hooks.
-- These hooks are called whenever inventory operations or events occur in the game.
-- @section Inventory

--- OnInventoryAdd is called whenever the game is preparing to add an item to an entity's inventory.
-- The call occurs once it has been determined that the item can be added to inventory successfully, but before the item is actually added.
-- NOTE that the "id" value of the item will change once it is moved to the inventory of the receiver, so do not save the "id" value for any purpose.
-- @param receiver The Entity receiving the item.  Can be an Actor, Container, or Vehicle.
-- @param item The item being received. Must be a subclass of Collectable.
-- @param quantity How many of the indicated item will be added to inventory.
-- @return None.
function OnInventoryAdd(receiver, item, quantity)
end

--- OnInventoryRemove is called whenever the game is preparing to remove an item from an entity's inventory.
-- The call occurs once it has been determined that the item can be remvoed from the inventory successfully, but before the item is actually removed.
-- NOTE that the "id" value of the removed entity will change once it is removed, so do not save the "id" value for any purpose.
-- @param source The Entity losing the item  Can be an Actor, Container, or Vehicle.
-- @param item The item being removed. Must be a subclass of Collectable.
-- @param quantity How many of the indicated item will be removed from inventory.
-- @return None.
function OnInventoryRemove(source, item, quantity)
end

--- OnCheckItemAllowed is called whenever the game needs to check if an Actor can use the given item.
-- This is called for weapons and armours. Unsure if it is called for skill items or consumables.
-- @param actor The Actor to check for ability to use the item.
-- @param item The item being checked for eligibility.
-- @return (optional) A boolean value indicating if the item can be used (true) or not (false). If not specified, the game will use its default behavior.
function OnCheckItemAllowed(actor, item)
end

--- Types.
-- @section Types

--- Timestamp format used in @{MsecToDayHMS} and @{AddBaseToGameTime}.
-- @field year Year value in timestamp.
-- @field month Month value in timestamp.
-- @field day Day value in timestamp.
-- @field hour Hour value in timestamp.
-- @field minute Minute value in timestamp.
-- @field second Second value in timestamp.
-- @field msec Leftover milliseconds after timestamp conversion.
-- @table TimeStamp

--- CriticalEffects table format used in @{OnCriticalEffect1} and @{OnCriticalEffect2}, and given as a parameter in @{OnDamageCalc}.
-- @field bypassdefenses Set to true to ignore target resistances.
-- @field wickedhit Set to true to double damage.
-- @field knockdown Set to true to knock target down.
-- @field injure Set to true to injure the target at the aimed location, or a random location for unaimed attacks.
-- @field disarmright Set to true to force target to drop its right hand weapon (currently broken).
-- @field disarmleft Set to true to force target to drop its left hand weapon (currently broken).
-- @field knockout Set to true to knock target unconscious.
-- @field breakweapon Set to true to break the target's currently active weapon.
-- @field tornapart Set to true to cause a massive critical on the target. Depending on the result of a luck roll by the target, the damage is either multiplied by 4, or by a random value between 4 and 103.
-- @table CriticalEffects

--- TargetInfo table format passed as a parameter to @{OnStraightAttack} et al, describing eligible targets for an attack.
-- @field actor The Entity object of the eligible target. Most often an Actor, but can be other Entity types as well.
-- @field hit_chance The normal chance of that target being hit from the attacker's current position and weapon.
-- @field distance The distance between attacker and this target.
-- @field angle (optional) The angle between the attacker's shot direction (to its intended target), and this particular target. May be used by Cone or Burst attack types to adjust chance to hit based on distance from the actual angle of fire. Not present for @{OnProjectileAttack}.
-- @field intended A boolean field indicating if this target is the intended target of the attack.
-- @table TargetInfo

--- TargetResults table format returned by @{OnStraightAttack} et al, describing actual hits made on any eligible targets for an attack.
-- @field id The "id" value from the Entity that is being hit.
-- @field hits The number of hits applied to this target.
-- @field mult A multiplier to the damage applied to this target. Used normally in the Cone and Radial attack types (shotguns, explosions) to make close shots more damaging.
-- @field deltax (optional) For @{OnProjectileAttack}, how far to show the attack missing the target in the X direction. (Used in the animation of the projectile, but also has an effect if the projectile causes radial damage.)
-- @field deltaz (optional) For @{OnProjectileAttack}, how far to show the attack missing the target in the Z direction. (Used in the animation of the projectile, but also has an effect if the projectile causes radial damage.)
-- @table TargetResults
