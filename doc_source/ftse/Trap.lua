--- Trap is the @{Entity} definition for a trap.
-- Subclass of @{Collectable}.
-- @entity Trap

--- GetOwner returns the owner @{Entity} for an active trap.
-- @return An @{Entity}, usually type @{Actor}, indicating the trap owner.
function Trap:GetOwner()
end

--- GetHolder returns the current holder @{Entity} for an active trap.
-- @return An @{Entity} indicating the trap holder.
function Trap:GetHolder()
end

--- GetSkillRoll returns the skill roll value when the trap was placed.
-- @return An integer indicating the skill roll.
function Trap:GetSkillRoll()
end

--- IsActivated returns true if the trap is active.
-- @return A boolean indicating if the trap is active.
function Trap:IsActivated()
end

--- IsHidden returns true if the trap is hidden.
-- @return A boolean indicating if the trap is hidden.
function Trap:IsHidden()
end

--- GetDetonationTime returns the millisecond timer value for the trap to explode.
-- @return A large integer indicating the explosion time.
function Trap:GetDetonationTime()
end

--- GetFrequency returns the frequency of the trap.
-- @return A numeric value indicating the trap's frequency.
function Trap:GetFrequency()
end

--- IsReusable returns true if the trap is reusable.
-- @return A boolean value indicating if the trap is reusable.
function Trap:IsReusable()
end

--- GetDifficulty returns the disarm difficulty of the trap.
-- @return An integer indicating the trap difficulty.
function Trap:GetDifficulty()
end

--- GetCriticalChance returns the critical hit chance of the trap.
-- @return An integer indicating the trap critical hit chance.
function Trap:GetCriticalChance()
end

--- GetMinBaseDamage returns the minimum base damage of the trap.
-- @return An integer indicating the trap minimum damage.
function Trap:GetMinBaseDamage()
end

--- GetMaxBaseDamage returns the maximum base damage of the trap.
-- @return An integer indicating the trap maximum damage.
function Trap:GetMaxBaseDamage()
end

--- GetPrimaryDamageType returns the primary type of damage for the trap.
-- @return A string indicating the trap primary damage type.
function Trap:GetPrimaryDamageType()
end

--- GetSecondaryDamageType returns the secondary effect for the trap, if any.
-- @return A string indicating the trap secondary effect type.
function Trap:GetSecondaryDamageType()
end

--- GetAmmoVariant returns the ammo variant to use when calculating damage and penetration of the trap.
-- @return A string indicating the trap ammo variant.
function Trap:GetAmmoVariant()
end

--- GetDamageRadius returns the blast radius of the trap.
-- @return A numeric value indicating the trap radius.
function Trap:GetDamageRadius()
end

--- GetImpactFX returns the effect to use to show impact to a character.
-- @return A string indicating the impact effect.
function Trap:GetImpactFX()
end

