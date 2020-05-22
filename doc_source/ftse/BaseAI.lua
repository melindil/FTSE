--- BaseAI is the @{Entity} definition for the AI behavior of a computer-controlled @{Actor}.
-- Subclass of @{Entity}.
-- @entity BaseAI

--- GetCurrentTarget returns the current target of the @{Actor} controlled by this AI.
-- @return An @{Entity} object corresponding to the AI's current target.
function BaseAI:GetCurrentTarget()
end

--- GetController returns the @{Controller} entity for this AI. The @{Controller} holds waypoint and action information.
-- @return A @{Controller} entity for this AI's controller.
function BaseAI:GetController()
end

--- GetAction returns the current action for the AI-controlled entity.
-- @return A string indicating the current action.
function BaseAI:GetAction()
end

--- GetNature returns the nature for the AI-controlled entity.
-- @return A string indicating the nature.
function BaseAI:GetNature()
end

--- GetTactic returns the tactic for the AI-controlled entity.
-- @return A string indicating the tactic.
function BaseAI:GetTactic()
end

--- GetItemTag returns the item tag for the AI-controlled entity.
-- @return A string indicating the item tag.
function BaseAI:GetItemTag()
end

--- GetTaunts returns the taunt group used by the AI-controlled entity.
-- @return A string indicating the taunt group.
function BaseAI:GetTaunts()
end

--- UsesCover returns true if the AI will use nearby cover.
-- @return A boolean indicating if the AI will use cover.
function BaseAI:UsesCover()
end

--- UsesSneak returns true if the AI will sneak.
-- @return A boolean indicating if the AI will sneak.
function BaseAI:UsesSneak()
end

--- IsSleeping returns true if the controlled entity is currently sleeping.
-- @return A boolean indicating if the controlled entity is currently sleeping.
function BaseAI:IsSleeping()
end

--- IsSuppressed returns true if the controlled entity is currently suppressed.
-- @return A boolean indicating if the controlled entity is currently suppressed.
function BaseAI:IsSuppressed()
end

--- IsAttemptSuppressed returns true if the controlled entity has attempted to be suppressed.
-- @return A boolean indicating if the controlled entity has attempted to be suppressed.
function BaseAI:IsAttemptSuppressed()
end
