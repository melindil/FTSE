--- Light is the @{Entity} definition for a light object.
-- Subclass of @{Entity}.
-- @entity Light

--- GetColor1 returns the first color used for the light.
-- @return A table with "r", "g", and "b" members indicating light color components (each is numeric from 0.0 to 1.0).
function Light:GetColor1()
end

--- GetColor2 returns the second color used for the light.
-- @return A table with "r", "g", and "b" members indicating light color components (each is numeric from 0.0 to 1.0).
function Light:GetColor2()
end

--- GetRateChange returns the rate of change of the light in msec.
-- @return An integer indicating the rate of change of the light.
function Light:GetRateChange()
end

--- GetArc returns the arc over which the light source shines.
-- @return An integer indicating the arc of the light in degrees.
function Light:GetArc()
end

--- GetIntensityStringPos returns the current location in the intensity string for the light.
-- @return An integer indicating the current offset in the intensity string.
function Light:GetIntensityStringPos()
end

--- GetIntensityString returns the string used to vary intensity of the light.
-- @return A string indicating the pattern of intensity change for the light.
function Light:GetIntensityStringPos()
end

--- GetRadius returns the radius of the light effect.
-- @return A numeric value indicating the radius.
function Light:GetRadius()
end

--- GetHighBound returns the high bound for the light (unknown).
-- @return A numeric value indicating the high bound.
function Light:GetHighBound()
end

--- GetLowBound returns the low bound for the light (unknown).
-- @return A numeric value indicating the low bound.
function Light:GetLowBound()
end

--- GetFogAlpha returns the intensity of fog effect.
-- @return A numeric value indicating the fog level.
function Light:GetFogAlpha()
end

--- OnOffSwitch returns true if the light is on.
-- @return A boolean indicating if the light is on.
function Light:OnOffSwitch()
end

--- IsNightOnly returns true if the light is on during night only.
-- @return A boolean indicating if the light is on during night only.
function Light:IsNightOnly()
end

--- IsRepeating returns true if the light pattern repeats.
-- @return A boolean indicating if the light pattern repeats.
function Light:IsRepeating()
end

--- IsUseColor2 returns true if the light uses the secondary color.
-- @return A boolean indicating if the light uses both defined colors.
function Light:IsUseColor2()
end

--- IsUseArc returns true if the light uses the arc value.
-- @return A boolean indicating if the light uses the arc value.
function Light:IsUseArc()
end

--- IsUseFog returns true if the light uses the fog value.
-- @return A boolean indicating if the light uses the fog value.
function Light:IsUseFog()
end

