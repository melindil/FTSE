--- Ammo is the @{Entity} definition for an ammunition object.
-- Subclass of @{Collectable}.
-- @entity Ammo

--- GetType returns the type of ammo this entity provides (e.g. "9mm").
-- @return A string indicating the ammo type.
function Ammo:GetType()
end

--- GetVariant returns the variant of ammo this entity provides (e.g. "JHP").
-- @return A string indicating the ammo variant.
function Ammo:GetVariant()
end

--- GetPrimaryDamageType returns the main damage type this ammo will cause (e.g. "energy").
-- @return A string indicating the main dmaage type.
function Ammo:GetPrimaryDamageType()
end

--- GetSecondaryDamageType returns the secondary damage type this ammo will cause, if any (e.g. "poison").
-- @return A string indicating the secondary dmaage type, or "normal" if none.
function Ammo:GetSecondaryDamageType()
end

--- GetImpactFX returns the sprite effect shown on impact when this ammo is used.
-- @return A string indicating the impact FX type.
function Ammo:GetImpactFX()
end

--- GetImpactSoundFX returns the sound effect played on impact when this ammo is used.
-- @return A string indicating the impact sound FX.
function Ammo:GetImpactSoundFX()
end
