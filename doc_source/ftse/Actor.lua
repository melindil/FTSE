--- Actor contains functions for operating on Actor entity types.
-- Subclass of @{Entity}.
-- @entity Actor

--- GetAttribute returns the value of the named attribute from the Actor, in the requested table.
-- Each actor has four tables, reflecting permanent, derived, temporary, and current values for each attribute.
-- @param attrib The name of the attribute, as shown in the Fallout: Tactics entity editor.
-- @param tab The table to retrieve from. One of ACTOR\_TABLE\_PERM, ACTOR\_TABLE\_DERIVED, ACTOR\_TABLE\_TEMPORARY, or ACTOR\_TABLE\_CURRENT.
-- @return The value of the requested attribute. May be a boolean for tag\_* fields or traits; otherwise is an integer value.
function Actor:GetAttribute(attrib, tab)
end

--- SetAttribute adjusts the value of the named attribute for the Actor, in the requested table.
-- Each actor has four tables, reflecting permanent, derived, temporary, and current values for each attribute.
-- Prefer to use ApplyBonus or RemoveBonus for temporary adjustments wherever possible, as the resulting
-- value will be calculated immediately.
-- @param attrib The name of the attribute, as shown in the Fallout: Tactics entity editor.
-- @param tab The table to set. One of ACTOR\_TABLE\_PERM, ACTOR\_TABLE\_DERIVED, ACTOR\_TABLE\_TEMPORARY, or ACTOR\_TABLE\_CURRENT.
-- @param value The value to set, Should be a boolean for tag\_* or trait fields, otherwise an integer.
-- @return None.
function Actor:SetAttribute(attrib, tab, value)
end

--- ApplyBonus adds a temporary or permanent bonus to the attributes of the Actor.
-- Temporary bonuses are often highlighted in blue (for positive) or red (for negative), by default, on the character screen.
-- @param attribs A table containing the bonus to apply. Table fields should be named as for the attributes to grant bonus, and each field's value should be the amount to increase the attribute by.
-- @param perm A boolean flag indicating if the adjustment should be permanent.
-- @return None.
function Actor:ApplyBonus(attribs, perm)
end

--- RemoveBonus subtracts a temporary or permanent bonus from the attributes of the Actor.
-- Temporary bonuses are often highlighted in blue, by default, on the character screen.
-- @param attribs A table containing the bonus to remove. Table fields should be named as for the attributes to grant bonus, and each field's value should be the amount to decrease the attribute by. NOTE that the value should be what was granted by the initial bonus (e.g. to remove a bonus of positive 3, specify positive 3 in the RemoveBonus call as well).
-- @param perm A boolean flag indicating if the adjustment should be permanent.
-- @return None.
function Actor:RemoveBonus(attribs, perm)
end

--- GetInventory returns the inventory of the Actor.
-- @return An array of @{Collectable} objects comprising the Actor's inventory.
function Actor:GetInventory()
end

--- DisplayMessage shows the given message above the Actor for a short time.
-- @param msg The message to display.
-- @return None.
function Actor:DisplayMessage(msg)
end

