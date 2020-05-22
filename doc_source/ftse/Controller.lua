--- Controller is the @{Entity} definition for a "player" AI in the mission.
-- Subclass of @{Entity}.
-- @entity Controller

--- GetControllerDir returns the direction of the controller - unknown.
-- @return A table with "x", "y", and "z" of a direction.
function Controller:GetControllerDir()
end

--- GetStartWaypoint returns the @{Waypoint} object to start for this AI.
-- @return A @{Waypoint} entity indicating the start of the waypoint path.
function Controller:GetStartWaypoint()
end

--- GetFollowTarget returns the @{Entity} object this AI is following.
-- @return An @{Entity} indicating the follow target.
function Controller:GetFollowTarget()
end

--- GetControllerType returns the type of AI controller for this AI player.
-- @return A string indicating the controller type.
function Controller:GetControllerType()
end

--- GetMoveTargetTag returns the tag name of the AI's move target.
-- @return A string indicating the target tag name.
function Controller:GetMoveTargetTag()
end

--- GetWorkingZone returns the name of the AI's working zone.
-- @return A string indicating the working zone name.
function Controller:GetWorkingZone()
end

--- GetRetreatZone returns the name of the AI's retreat zone.
-- @return A string indicating the retreat zone name.
function Controller:GetRetreatZone()
end

--- GetMoveTypePriority returns the move type priority. (Unknown)
-- @return An integer indicating the priority.
function Controller:GetMoveTypePriority()
end

--- GetAggression returns the aggression of the AI player.
-- @return An integer indicating the aggression.
function Controller:GetAggression()
end

--- GetMobility returns the mobility of the AI player.
-- @return An integer indicating the mobility.
function Controller:GetMobility()
end

--- GetCommunicate returns the communicate value of the AI player.
-- @return An integer indicating the communicate value.
function Controller:GetCommunicate()
end

--- IsGroupMove returns true if group mode is selected for this AI player.
-- @return A boolean indicating the group move flag.
function Controller:IsGroupMove()
end

--- IsRandMove returns true if random move mode is selected for this AI player.
-- @return A boolean indicating the random move flag.
function Controller:IsRandMove()
end
--- IsNeverLeaveZone returns true if the never leave zone flag is selected for this AI player.
-- @return A boolean indicating the never leave zone flag.
function Controller:IsNeverLeaveZone()
end
