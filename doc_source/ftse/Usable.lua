--- Usable is the parent class of all world @{Entity} types which can be interacted with.
-- This does not include items which can be picked up, only those which are visible on the map.
--
-- Subclass of @{Entity}.
-- @entity Usable

--- IsReusable returns true if the entity can be used more than once.
-- @return A boolean indicating if the entity can be used more than once.
function Usable:IsReusable()
end

--- Flag indicating a class is a subclass of Usable.
-- @field isUsable
Usable:isUsable = true
