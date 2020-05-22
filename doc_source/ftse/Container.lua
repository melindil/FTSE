--- Container is the @{Entity} definition for a container.
-- Subclass of @{Usable}.
-- @entity Container

--- GetMaxWeight returns the maximum carrying capacity of the container.
-- @return An integer indicating the max weight capacity of the container.
function Container:GetMaxWeight()
end

--- GetUnlockChance returns the chance to unlock for a locked container.
-- @return An integer indicating the unlock chance.
function Container:GetUnlockChance()
end

--- IsOpen returns true if the container is currently open.
-- @return A boolean indicating if the container is open.
function Container:IsOpen()
end

--- AlwaysOpen returns true if the container is always open.
-- @return A boolean indicating if the container is always open.
function Container:AlwaysOpen()
end

--- IsLocked returns true if the container is currently locked.
-- @return A boolean indicating if the container is locked.
function Container:IsLocked()
end

--- IsLockable returns true if the container is lockable.
-- @return A boolean indicating if the container is lockable.
function Container:IsLockable()
end

--- GetAction returns an action string for the container (used in animation).
-- @return A string indicating the current action for the container.
function Container:GetAction()
end

--- GetSound returns the sound effect played when the container is used.
-- @return A string indicating the sound effect name.
function Container:GetSound()
end

