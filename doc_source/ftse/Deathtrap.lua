--- Deathtrap is the @{Entity} definition for a death trap object.
-- Subclass of @{Entity}.
-- @entity Deathtrap

--- IsRepeat returns true if the effect repeats.
-- @return A boolean indicating repeatable.
function Deathtrap:IsRepeat()
end

--- IsAnimated returns true if the effect animates.
-- @return A boolean indicating animating.
function Deathtrap:IsAnimated()
end

--- IsScriptState returns true if the script state for the trap is set.
-- @return A boolean indicating the script state.
function Deathtrap:IsScriptState()
end

--- GetDamageType returns the damage type for the trap.
-- @return A string indicating the damage type.
function Deathtrap:IsScriptState()
end

--- GetDamage returns the damage amount for the trap.
-- @return An integer indicating the damage amount.
function Deathtrap:GetDamage()
end

--- GetRepeatInterval returns the repeat interval for the trap.
-- @return A numeric value indicating the repeat interval.
function Deathtrap:GetRepeatInterval()
end

--- GetNextActivateTime returns the next activation time on the animation clock.
-- @return A numeric value indicating the next activation time.
function Deathtrap:GetNextActivateTime()
end
