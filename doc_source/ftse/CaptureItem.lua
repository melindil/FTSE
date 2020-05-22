--- CaptureItem is the @{Entity} definition for the in-world version of the multiplayer flag.
-- Subclass of @{Entity}.
-- @entity CaptureItem

--- GetCaptureTeam returns the ID of the team that owns the capture flag.
-- @return An integer value of the team ID owning the flag.
function CaptureItem:GetCaptureTeam()
end

--- GetInventoryItem returns the @{CaptureInvItem} item of the flag.
-- @return An object of type @{CaptureInvItem} corresponding to the carried version of the flag.
function CaptureItem:GetInventoryItem()
end
