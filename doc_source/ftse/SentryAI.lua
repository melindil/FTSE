--- SentryAI is the @{Entity} definition for a player-controller character's sentry AI.
-- Subclass of @{Entity}.
-- @entity SentryAI

--- GetCurrentTarget returns the current target of the sentry AI.
-- @return An @{Entity} object indicating the current target.
function SentryAI:GetCurrentTarget()
end

--- GetUserTarget returns the user's target.
-- @return An @{Entity} object indicating the user-defined target.
function SentryAI:GetUserTarget()
end

--- GetRepathTime returns the animation clock time for the next repath to the target.
-- @return A numeric value indicating the repath time.
function SentryAI:GetRepathTime()
end

--- GetAimLocation returns the aimed shot location for the sentry mode.
-- @return An integer indicating the aimed shot location (0-8).
function SentryAI:GetAimLocation()
end

--- GetAccuracy returns the minimum accuracy for the sentry AI to shoot.
-- @return An integer indicating the minimum accuracy.
function SentryAI:GetAccuracy()
end

--- GetMode returns the current sentry mode.
-- @return An integer indicating the sentry mode (values not known).
function SentryAI:GetMode()
end
