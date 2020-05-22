--- Used in the hook DefaultStyleChanges to change default text colors.
-- All parameters are color component values, which are decimal values between
-- 0.0 and 1.0.
-- @classmod DefaultStyle

--- SetColorOptionsPages sets the default text color for the tabs in the
-- options screen.
-- @param r Red component of color.
-- @param g Green component of color.
-- @param b Blue component of color.
-- @return None.
function DefaultStyle:SetColorOptionsPages(r,g,b)
end

--- SetColorPanelTitles sets the default text color for the text on the
-- Skilldex, character bar, weapon mode, and a few other locations.
-- @param r Red component of color.
-- @param g Green component of color.
-- @param b Blue component of color.
-- @return None.
function DefaultStyle:SetColorPanelTitles(r,g,b)
end

--- SetColorDefaultText sets the default text color for the normal text
-- on the character screen.
-- @param r Red component of color.
-- @param g Green component of color.
-- @param b Blue component of color.
-- @return None.
function DefaultStyle:SetColorDefaultText(r,g,b)
end

--- SetColorHighlightText sets the default text color for a highlighted
-- skill or attribute nn the character screen.
-- @param r Red component of color.
-- @param g Green component of color.
-- @param b Blue component of color.
-- @return None.
function DefaultStyle:SetColorHighlightText(r,g,b)
end

--- SetColorBuffs sets the default text color for attributes with
-- temporary positive modifiers on the character screen.
-- @param r Red component of color.
-- @param g Green component of color.
-- @param b Blue component of color.
-- @return None.
function DefaultStyle:SetColorBuffs(r,g,b)
end

--- SetColorDebuffs sets the default text color for attributes with
-- temporary negative modifiers on the character screen.
-- @param r Red component of color.
-- @param g Green component of color.
-- @param b Blue component of color.
-- @return None.
function DefaultStyle:SetColorDebuffs(r,g,b)
end

--- SetColorTags sets the default text color for the tagged skills in
-- the character screen.
-- @param r Red component of color.
-- @param g Green component of color.
-- @param b Blue component of color.
-- @return None.
function DefaultStyle:SetColorTags(r,g,b)
end
