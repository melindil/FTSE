---Powernode is the @{Entity} definition for a power node.
-- This item comes from the Canyon City mission, where power nodes need to be disabled at the same time.
--
-- Subclass of @{Usable}.
-- @entity Powernode

--- GetHealRate returns the heal rate of the power node.
-- @return A numeric value indicating heal rate.
function Powernode:GetHealRate()
end

--- GetDifficulty returns the difficulty of disabling the power node with the Science skill.
-- @return An integer indicating Science difficulty.
function Powernode:GetDifficulty()
end

--- GetArmorClass returns the armor class of the power node.
-- @return An integer indicating the node's armor class.
function Powernode:GetArmorClass()
end

--- GetMaxHP returns the maximum HP of the power node.
-- @return An integer indicating the node's maximum HP.
function Powernode:GetMaxHP()
end

--- GetCurrentHP returns the current HP of the power node.
-- @return An integer indicating the node's current HP.
function Powernode:GetCurrentHP()
end

--- GetLastHealTime returns the time on the millisecond timer that the node last healed.
-- @return A large integer indicating the node's last heal time.
function Powernode:GetLastHealTime()
end

--- GetCurrentAction returns the current animation action of the power node.
-- @return A string indicating the node's current animation.
function Powernode:GetCurrentAction()
end
