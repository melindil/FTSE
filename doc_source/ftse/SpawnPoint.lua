--- SpawnPoint is the @{Entity} definition for a spawn point.
-- Subclass of @{Entity}.
-- @entity SpawnPoint

--- GetDesiredTeam returns the player ID of this spawn point.
-- @return An integer indicating the player ID.
function SpawnPoint:GetDesiredTeam()
end

--- GetSpawnCounter returns the counter for this spawn point.
-- @return An integer indicating the counter value.
function SpawnPoint:GetSpawnCounter()
end

--- GetSpawnType returns the type string for this spawn point.
-- @return An integer indicating the type string.
function SpawnPoint:GetSpawnType()
end

--- GetMainDudeOnly returns true if only the main character should spawn.
-- @return A boolean indicating if only the main character spawns.
function SpawnPoint:GetMainDudeOnly()
end
