--- ScenarySpawn (sic) is the @{Entity} definition for a scenery spawner.
-- Subclass of @{Entity}.
-- @entity ScenarySpawn

--- GetSpeed returns the speed of the spawned entities.
-- @return A numeric value indicating the spawned object speed.
function ScenarySpawn:GetSpeed()
end

--- GetFrequency returns the frequency of spawning an object.
-- @return A numeric value indicating the spawn frequency.
function ScenarySpawn:GetFrequency()
end

--- GetMaxSpawn returns the maximum number of spawned objects.
-- @return An integer indicating the maximum spawned objects.
function ScenarySpawn:GetMaxSpawn()
end

--- GetSpawnSprite returns the name of the sprite this spawn generates.
-- @return An integer indicating the sprite file name.
function ScenarySpawn:GetSpawnSprite()
end
