--- CombatFX is the @{Entity} definition for an effect animation object (usually a projectile).
-- Subclass of @{Entity}.
-- @entity CombatFX

--- GetAimpoint returns the aim location of the effect.
-- @return A table with values of "x", "y", and "z" for the aim location.
function CombatFX:GetAimpoint()
end

--- GetLastFrame returns the last frame of the effect (currently unknown).
-- @return An integer value of the last frame.
function CombatFX:GetLastFrame()
end

--- GetNumFlames returns the number of flames (??? - unknown).
-- @return An integer value of the number of flames.
function CombatFX:GetNumFlames()
end

--- GetType returns the type of the effect (currently unknown).
-- @return An integer value of the effect type.
function CombatFX:GetType()
end
