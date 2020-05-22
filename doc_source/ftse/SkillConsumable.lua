--- SkillConsumable is the @{Entity} definition for a skill-required item (health kits, repair tools).
-- Subclass of @{Collectable}.
-- @entity SkillConsumable

--- GetTotalUses returns the total number of uses this item offers.
-- @return An integer indicating the total number of uses.
function SkillConsumable:GetTotalUses()
end

--- GetRemainingUses returns the remaining number of uses this item offers.
-- @return An integer indicating the remaining uses.
function SkillConsumable:GetRemainingUses()
end
