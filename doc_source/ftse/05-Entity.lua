--- Entity contains functions which control game entities. It is the parent
-- class of all entity types in Fallout: Tactics. Functions described here are
-- available to be called for any type of entity.
-- @entity Entity

--- GetName returns the printable name of the entity.
-- @return A string containing the entity's name.
function Entity:GetName()
end

--- GetTag returns the TagName value for the entity.
-- @return A string containing the entity's TagName.
function Entity:GetTag()
end

--- GetBaseID returns the ID value of the entity from the World entity table.
-- NOTE that saving ID values is not advised for any purpose, as they will
-- change frequently (inventory items change any time they are dropped or equipped,
-- for example).
-- @return An integer value containing the current value of the entity's ID.
function Entity:GetBaseID()
end

--- GetPosition returns the position of the entity.
-- @return A table containing fields "x", "y", and "z" for the entity's position.
function Entity:GetPosition()
end

--- GetEntitySubType retrns the subtype of the given entity.
-- The meaning of the subtype depends on the type of entity. For example,
-- Actor uses this field as the creature type or race, Armour uses it as the
-- armour material type, etc.
-- @return A string containing the entity's subtype.
function Entity:GetEntitySubType()
end

--- GetMaxRange returns the range of a weapon, if held by the given Actor.
-- This function takes as a parameter an Actor object. The range will be
-- calculated as if the Actor has equipped this weapon.
-- While this field is implemented for Entity types, it may only return reasonable
-- values for Weapon type entities. Return values for other types of entities are
-- currently unknown.
-- @param a An Actor object to use when determining the weapon's range.
-- @return A numeric value containing the Weapon's maximum range.
function Entity:GetMaxRange(a)
end

--- GetMinEffectiveDamage returns the minimum damage of a weapon, if held by the given Actor.
-- This function takes as a parameter an Actor object. The damage will be
-- calculated as if the Actor has equipped this weapon.
-- While this field is implemented for Entity types, it may only return reasonable
-- values for Weapon type entities. Return values for other types of entities are
-- currently unknown.
-- @param a An Actor object to use when determining the weapon's minimum damage.
-- @return A numeric value containing the Weapon's minimum damage.
function Entity:GetMinEffectiveDamage(a)
end

--- GetMaxEffectiveDamage returns the maximum damage of a weapon, if held by the given Actor.
-- This function takes as a parameter an Actor object. The damage will be
-- calculated as if the Actor has equipped this weapon.
-- While this field is implemented for Entity types, it may only return reasonable
-- values for Weapon type entities. Return values for other types of entities are
-- currently unknown.
-- @param a An Actor object to use when determining the weapon's maximum damage.
-- @return A numeric value containing the Weapon's maximum damage.
function Entity:GetMaxEffectiveDamage(a)
end

--- AddInventory adds the given entity to this entity's inventory.
-- This function is only implemented for Actor, Container, and Vehicle entity types.
-- @param e The Entity to add to the inventory of this Entity.
-- @param count (optional) How many objects from the Entity to add to inventory (allows partial success).
-- @return[1] If successful, nothing is returned.
-- @return[2] If failed to add to inventory, the first part of the return is the error string returned by AddInventory.
-- @return[2] If failed to add to inventory, the second part of the return is the remainder of the Entity that was not added (in case of partial success).
function Entity:AddInventory(e, count)
end

--- RemoveInventory removes the given entity from this entity's inventory.
-- This function is only implemented for Actor, Container, and Vehicle entity types.
-- @param e The Entity to remove from the the inventory of this Entity.
-- @param count (optional) How many objects to remove from the inventory.
-- @return[1] If successful, returns the Entity that was removed.
-- @return[2] If failed, returns a string indicating the reason for failure.
function Entity:RemoveInventory(e, count)
end

--- Destruct removes the entity from the game state.
-- This should be called only on entities which have been removed from an
-- inventory. If called on a world item, or on an object still in an inventory,
-- results are unknown.
-- @return None.
function Entity:Destruct()
end

--- Indicates that the object is an entity.
-- This value will be true for all Entity subclasses.
-- @field isEntity
Entity.isEntity = true

--- Contains a string value indicating the type of entity.
-- This will be overwritten by all Entity subclasses.
-- @field ClassType
Entity.ClassType = "Entity"

