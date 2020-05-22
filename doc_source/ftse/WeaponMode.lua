--- WeaponMode is a special class denoting a mode of a @{Weapon}.
-- @entity WeaponMode

--- GetName returns the name of this weapon mode.
-- @return A string indicating the weapon mode.
function WeaponMode:GetName()
end

--- IsValid returns true if this weapon mode is used.
-- @return A boolean value indicating the weapon mode is valid.
function WeaponMode:IsValid()
end

--- GetWeaponSkill returns the weapon skill to use for this mode.
-- @return A string indicating the weapon mode skill (e.g. "smallGuns").
function WeaponMode:GetWeaponSkill()
end

--- GetAnimation returns the animation set to use for this mode.
-- @return A string indicating the animation.
function WeaponMode:GetAnimation()
end

--- GetRangeType returns the range class for the weapon.
-- Examples are "Melee", "Short", and "Long".
-- @return A string indicating the range type.
function WeaponMode:GetRangeType()
end

--- GetBaseMaxRange returns the max range for the mode.
-- @return A numeric value indicating the max range.
function WeaponMode:GetBaseMaxRange()
end

--- GetSpreadType returns the spread type for the mode.
-- Examples are "Straight", "Burst", and "Cone".
-- @return A string indicating the spread type.
function WeaponMode:GetSpreadType()
end

--- GetAccuracyBonus returns the accuracy bonus for the mode.
-- @return An integer indicating the accuracy bonus.
function WeaponMode:GetAccuracyBonus()
end

--- GetDamageMultiplier returns the damage multiplier.
-- @return A numeric value indicating the damage multiplier.
function WeaponMode:GetDamageMultiplier()
end

--- GetAmmoUsage returns the number of shots used in the mode.
-- @return An integer indicating the number of shots.
function WeaponMode:GetAmmoUsage()
end

--- GetActionCostType returns the action cost type for the mode.
-- @return A string indicating the action cost type.
function WeaponMode:GetActionCostType()
end

--- GetImpactFX returns the impact effect from this mode hitting a target.
-- @return A string indicating the impact effect.
function WeaponMode:GetImpactFX()
end

--- GetSoundFX returns the impact sound from this mode hitting a target.
-- @return A string indicating the impact sound.
function WeaponMode:GetImpactFX()
end

--- GetLightFX returns the impact light effect from this mode hitting a target.
-- @return A string indicating the impact light effect.
function WeaponMode:GetLightFX()
end

--- CanCallShot returns true if this mode allows aimed shots.
-- @return A boolean indicating if aimed shots are allowed.
function WeaponMode:CanCallShot()
end

--- HasProjectile returns true if this mode uses a visible projectile.
-- @return A boolean indicating if this mode uses a visible projectile.
function WeaponMode:HasProjectile()
end

--- DestroyOnUse returns true if this mode destroys one instance of the weapon on use.
-- @return A boolean indicating if this mode destroys the weapon on use.
function WeaponMode:DestroyOnUse()
end

--- GetKnockoverMultiplier returns the multiplier for knockover chance for this mode.
-- @return A numeric value indicating the knockover chance multiplier.
function WeaponMode:GetKnockoverMultiplier()
end

--- GetCriticalBonus returns the bonus to critical chance for this mode.
-- @return An integer indicating critical chance bonus.
function WeaponMode:GetCriticalBonus()
end
