--- Consumable is the @{Entity} definition for a consumable item (food, drug, healing).
-- Subclass of @{Collectable}.
-- @entity Consumable

--- GetEffectedMainAttribute returns the name of the attribute that this consumable increases. Most often "hitPoints" for healing items.
-- @return A string indicating the attribute affected. Strings match the attribute names from the editor.
function Consumable:GetEffectedMainAttribute()
end

--- GetCategory returns the category of the consumable item.
-- Typical values are "chems" and "food".
-- @return A string indicating the consumable category.
function Consumable:GetCategory()
end


--- IsAddicted returns true if the @{Consumable} item stores an addiction for an @{Actor}.
-- @return A boolean indicating if this @{Consumable} is an addiction list member.
function Consumable:IsAddicted()
end

--- GetEffectMinimum returns the minimum increase in the main attribute.
-- @return An integer value of the minimum attribute increase.
function Consumable:GetEffectMinimum()
end

--- GetEffectMaximum returns the maximum increase in the main attribute.
-- @return An integer value of the maximum attribute increase.
function Consumable:GetEffectMaximum()
end

--- GetOverdosePoints returns the number of overdose points this consumable adds.
-- @return An integer value of the number of overdose points.
function Consumable:GetOverdosePoints()
end

--- GetChanceAddiction returns the chance of a user being addicted to this consumable.
-- @return An integer value of the addiction chance.
function Consumable:GetChanceAddiction()
end

--- GetState returns the state of this @{Consumable} object (e.g. which state is currently active in the user, if any).
-- @return An integer value of the active state.
function Consumable:GetState()
end

--- GetEffectDuration returns the duration of the given effect number.
-- @param num The effect number of the item to get. Should be 0-3. with 3 being the addicted state.
-- @return An integer value of the number of minutes this effect number lasts.
function Consumable:GetEffectDuration(num)
end

--- GetEffectMultiplier returns the multiplier of the given effect number.
-- @param num The effect number of the item to get. Should be 0-3. with 3 being the addicted state.
-- @return A numeric value of the effect multiplier. (Unknown what this value means).
function Consumable:GetEffectMultiplier(num)
end

--- GetEffectAttribute returns the adjustment to the given attribute caused by this effect number.
-- @param num THe effect number of the item to get. Should be 0-3. with 3 being the addicted state.
-- @param attrib The attribute to check, as a string. Should match attribute strings from the editor.
-- @return An integer value of the positive or negative effect on the attribute the numbered effect has.
function Consumable:GetEffectAttribute(num,attrib)
end

--- IsEffectEnabled returns true if the given effect is active.
-- @param num The effect number of the item to get. Should be 0-3. with 3 being the addicted state.
-- @return A boolean indicating if the effect is active.
function Consumable:IsEffectEnabled(num)
end

