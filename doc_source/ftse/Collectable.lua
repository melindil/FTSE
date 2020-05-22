--- Collectable is the parent class of all @{Entity} types which can be carried in an inventory.
-- Subclass of @{Entity}.
-- @entity Collectable

--- GetRequiredAttribute returns the minimum reqiurement for a given attribute in order to use this item.
-- @param attrib The name of the attribute, as shown in the Fallout: Tactics entity editor.
-- @return The minimum required value of the requested attribute for this object.
function Collectable:GetRequiredAttribute(attrib)
end

--- GetEffectAttribute returns the bonus this item grants to the the given attribute.
-- @param attrib The name of the attribute, as shown in the Fallout: Tactics entity editor.
-- @return The bonus value this item grants for that attribute.
function Collectable:GetEffectAttribute(attrib)
end

--- GetWeightPerUnit returns the weight of a single unit of this entity.
-- @return The weight of one unit of this entity, as a numeric value.
function Collectable:GetWeightPerUnit()
end

--- GetWeightTotal returns the weight of all units of this entity.
-- @return The weight of all units of this entity, as a numeric value.
function Collectable:GetWeightTotal()
end

--- GetValuePerUnit returns the monetary value of a single unit of this entity.
-- @return The monetaryvalue of one unit of this entity, as a numeric value.
function Collectable:GetValuePerUnit()
end

--- GetValueTotal returns the monetary value of all units of this entity.
-- @return The monetary value of all units of this entity, as a numeric value.
function Collectable:GetValueTotal()
end

--- GetCount returns the number of lootable/usable items of this entity that are being carried.
-- @return The number of lootable items.
function Collectable:GetCount()
end

--- GetCountNonLootable returns the number of non-lootable items of this entity that are being carried.
-- @return The number of non-lootable items.
function Collectable:GetCountNonLootable()
end

--- GetCountTotal returns the total number of items of this entity that are being carried.
-- @return The total number of items.
function Collectable:GetCountTotal()
end

--- IsUnique returns true if the item is listed as unique in the game.
-- @return A boolean indicating if the item is unique.
function Collectable:IsUnique()
end

--- CanBeRemoved returns true if the item can be removed from its equipped slot.
-- @return A boolean indicating if the item can be removed.
function Collectable:CanBeRemoved()
end

--- Lootable returns true if the item can be looted from the @{Actor} carrying it.
-- @return A boolean indicating if the item is lootable.
function Collectable:Lootable()
end

--- BeenApplied returns true if the item's effects have been applied to the @{Actor} equipping it.
-- @return A boolean indicating if the item effects have been applied.
function Collectable:BeenApplied()
end
