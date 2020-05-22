--- Door is the @{Entity} definition for a door.
-- Subclass of @{Usable}.
-- @entity Door

--- GetSounds returns the name of the sound file to use for the door.
-- @return A string indicating the sound to use for the door.
function Door:GetSounds()
end

--- GetSensorRange returns the opening sensor range for the door.
-- @return A numeric value indicating the door sensor range.
function Door:GetSensorRange()
end

--- GetChanceUnlock returns the unlock chance for a locked door.
-- @return An integer indicating the door unlock chance.
function Door:GetChanceUnlock()
end

--- GetTimeOpen returns the time taken for a door to open.
-- @return An integer indicating the door opening time.
function Door:GetTimeOpen()
end

--- IsLaserDoor returns true if the door is a laser-bar door.
-- @return A boolean indicating a laser-bar door.
function Door:IsLaserDoor()
end

--- IsAutoclose returns true if the door automatically closes.
-- @return A boolean indicating if the door automatically closes.
function Door:IsAutoclose()
end

--- CanBreak returns true if the door can break.
-- @return A boolean indicating if the door can break.
function Door:CanBreak()
end

--- IsSensorOpen returns true if the door can be opened by sensor.
-- @return A boolean indicating if the door can be opened by sensor.
function Door:IsSensorOpen()
end

--- IsTrapDoor returns true if the door is trapped.
-- @return A boolean indicating if the door is trapped.
function Door:IsTrapDoor()
end

--- IsOpen returns true if the door is currently opened.
-- @return A boolean indicating if the door is opened.
function Door:IsOpen()
end

--- IsOpenOutwards returns true if the door opens outwards.
-- @return A boolean indicating if the door opens outwards.
function Door:IsOpenOutwards()
end

--- IsLocked returns true if the door is currently locked.
-- @return A boolean indicating if the door is locked.
function Door:IsLocked()
end

--- IsLockable returns true if the door is lockable.
-- @return A boolean indicating if the door is lockable.
function Door:IsLockable()
end

--- Indicates that the object is a door.
-- @field isDoor
Door.isDoor = true


