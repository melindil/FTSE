--- Weapon is the @{Entity} definition for a weapon.
-- Subclass of @{Collectable}.
-- @entity Weapon

--- GetEquippedAmmo returns the ammo object loaded into this weapon.
-- @return An @{Ammo} object if the weapon is loaded, else an empty @{Entity} object (or nil).
function Weapon:GetEquippedAmmo()
end

--- GetMagCapacity returns the maximum rounds the weapon can hold.
-- @return An integer indicating the weapon magazine capacity.
function Weapon:GetMagCapacity()
end

--- GetMinBaseDamage returns the minimum base damage for the weapon.
-- @return An integer indicating the weapon minimum damage.
function Weapon:GetMinBaseDamage()
end

--- GetMaxBaseDamage returns the maximum base damage for the weapon.
-- @return An integer indicating the weapon maximum damage.
function Weapon:GetMaxBaseDamage()
end

--- GetSelectedModeID returns the ID of the currently selected mode.
-- @return An integer indicating the weapon mode ID.
function Weapon:GetSelectedModeID()
end

--- GetDamageRadius returns the default damage radius for the weapon.
-- @return A numeric value indicating the weapon damage radius.
function Weapon:GetDamageRadius()
end

--- IsCalledShot returns true if the weapon is set to use an aimed shot.
-- @return A boolean value indicating the weapon is set to aimed shot.
function Weapon:IsCalledShot()
end

--- DestroyUser returns true if the weapon destroys the using @{Actor}.
-- @return A boolean value indicating the weapon kills its user.
function Weapon:DestroyUser()
end

--- GetProjectileSprite returns the name of the projectile sprite.
-- @return A string indicating the projectile sprite file name.
function Weapon:GetProjectileSprite()
end

--- GetAmmoType returns the base ammo type of the weapon ("9mm" for example).
-- @return A string indicating the weapon's ammo type.
function Weapon:GetAmmoType()
end

--- GetDefaultAmmoVariant returns the ammo variant ("JHP" for example) to use for non-ammo weapons.
-- @return A string indicating the weapon's default ammo variant.
function Weapon:GetDefaultAmmoVariant()
end

--- GetWeaponPerk returns any active weapon perk (currently only "None" or "Night Scope").
-- @return A string indicating the weapon's special perk.
function Weapon:GetWeaponPerk()
end

--- GetPrimaryDamageType returns the primary damage type for a non-ammo weapon.
-- @return A string indicating the weapon's damage type (e.g. "energy").
function Weapon:GetPrimaryDamageType()
end

--- GetSecondaryDamageType returns the secondary effect type for a non-ammo weapon.
-- @return A string indicating the weapon's secondary effect type (e.g. "knockdown").
function Weapon:GetSecondaryDamageType()
end

--- GetSoundType returns the sound type for the weapon.
-- @return A string indicating the weapon's sound type.
function Weapon:GetSoundType()
end

--- GetMode returns the given numbered mode for the weapon.
-- @param num The weapon mode to retrieve.
-- @return A @{WeaponMode} object for that mode.
function Weapon:GetMode(num)
end

--- GetCurrentMode returns the current mode for the weapon.
-- @return A @{WeaponMode} object for the current mode.
function Weapon:GetCurrentMode()
end
