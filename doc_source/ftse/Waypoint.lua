--- Waypoint is the @{Entity} definition for an AI waypoint.
-- Subclass of @{Entity}.
-- @entity Waypoint

--- GetNextWaypointList returns the next waypoint in the list.
-- @return A string indicating the next waypoint.
function Waypoint:GetNextWaypointList()
end

--- GetAction returns the action at the waypoint.
-- @return A string indicating the waypoint action.
function Waypoint:GetAction()
end

--- GetPauseTime returns the animation clock time to wait at the waypoint.
-- @return A numeric value indicating the pause time.
function Waypoint:GetPauseTime()
end

--- IsUseDirection returns true if the waypoint uses the direction from the @{Controller} object.
-- @return A boolean value indicating whether the waypoint uses the @{Controller} direction.
function Waypoint:IsUseDirection()
end

--- IsRepeat returns true if the waypoint should repeat.
-- @return A boolean value indicating whether the waypoint should repeat.
function Waypoint:IsRepeat()
end

--- GetWaypointIndex returns this waypoint index.
-- @return An integer indicating the waypoint index.
function Waypoint:GetWaypointIndex()
end
