--- Switch is the @{Entity} definition for a switch.
-- Subclass of @{Usable}.
-- @entity Switch

--- IsOn returns true if the switch is triggered.
-- @return A boolean indicating if the switch is triggered.
function Switch:IsOn()
end

--- IsBrainExtractor returns true if the switch is a brain extractor.
-- @return A boolean indicating if the switch is a brain extractor.
function Switch:IsBrainExtractor()
end

--- GetKeyTagName returns the tag name for this switch to open.
-- @return A string indicating the switch tag name.
function Switch:GetKeyTagName()
end

--- Indicates that the object is a switch.
-- @field isSwitch
Switch.isSwitch = true
