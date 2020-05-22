--- Breakable is the @{Entity} definition for a breakable world object (e.g. barricade).
-- Subclass of @{Entity}.
-- @entity Breakable

--- GetAttribute returns the value of the named attribute from the Breakable entity.
-- @param attrib The name of the attribute, as shown in the Fallout: Tactics entity editor.
-- @return The value of the requested attribute. May be a boolean for tag\_* fields or traits; otherwise is an integer value.
function Breakable:GetAttribute(attrib)
end

--- GetHP returns the current hit points of the Breakable entity.
-- @return The current HP of the breakable.
function Breakable:GetHP()
end

--- GetMinDamage returns the minimum damage done to nearby entities when this Breakable is destroyed.
-- @return The minimum damage done to nearby entities.
function Breakable:GetMinDamage()
end

--- GetMaxDamage returns the maximum damage done to nearby entities when this Breakable is destroyed.
-- @return The maximum damage done to nearby entities.
function Breakable:GetMaxDamage()
end

--- GetDamageRadius returns the radius of damage done to nearby entities when this Breakable is destroyed.
-- @return The radius of damage done to nearby entities.
function Breakable:GetDamageRadius()
end

--- GetVariant returns the ammo variant type used when calculating damage from this Breakable to nearby entities.
-- @return A string indicating the ammo variant type.
function Breakable:GetVariant()
end

--- GetPrimaryDamageType returns the main damage type this breakable will cause (e.g. "energy").
-- @return A string indicating the main dmaage type.
function Breakable:GetPrimaryDamageType()
end

--- GetSecondaryDamageType returns the secondary damage type this breakable will cause, if any (e.g. "poison").
-- @return A string indicating the secondary dmaage type, or "normal" if none.
function Breakable:GetSecondaryDamageType()
end


--- EtherealWhenDead returns true if the Breakable is able to be moved through when destroyed.
-- @return A boolean indicating if the Breakable is ethereal when destroyed.
function Breakable:EtherealWhenDead()
end

--- IsMetal returns true if the Breakable is to be treated as if made of metal.
-- @return A boolean indicating if the Breakable is made of metal.
function Breakable:IsMetal()
end
