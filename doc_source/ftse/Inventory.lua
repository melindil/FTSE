--- Inventory is the @{Entity} definition for another entity's inventory.
-- Note that it is not recommended to use this class; instead, use
-- the inventory functions available in the @{Entity} class.
--
-- Subclass of @{Entity}.
-- @entity Inventory

--- GetItemList returns the items in the inventory.
-- @return An array of @{Collectable} objects.
function Inventory:GetItemList()
end

--- GetNumItems returns the number of items in the inventory.
-- @return An integer value indicating the number of objects in the inventory.
function Inventory:GetNumItems()
end
