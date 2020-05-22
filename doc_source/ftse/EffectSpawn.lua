--- EffectSpawn is the @{Entity} definition for an effect spawner.
-- Subclass of @{Entity}.
-- @entity EffectSpawn

--- GetSprite returns the name of the sprite effect to use.
-- @return A string indicating the sprite file.
function EffectSpawn:GetSprite()
end

--- GetSequence returns the effect emission sequence string.
-- @return A string indicating the effect emission string.
function EffectSpawn:GetSequence()
end

--- GetLightAttach returns the name of an attached light?
-- @return A string indicating the attached light.
function EffectSpawn:GetLightAttach()
end

--- GetWavSound returns the name of a sound file for the effect.
-- @return A string indicating the sound file to use.
function EffectSpawn:GetWavSound()
end

--- GetRandomDelay returns an amount of random delay for the effect spawn.
-- @return An integer indicating the random delay amount.
function EffectSpawn:GetRandomDelay()
end

--- IsOn returns true if the spawner is on.
-- @return A boolean indicating the spawner is on.
function EffectSpawn:IsOn()
end
