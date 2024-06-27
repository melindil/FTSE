--- world contains functions which can query or alter the state of the game world.
-- Functions are available to create or list entities, get or set campaign variables, get the current game time, or write to the combat log.
-- The class is accessed from a global object named "world".
-- @classmod world

--- Game state functions.
-- Functions which get or set overall game state.
-- @section Gamestatefunctions

--- GetTime returns the current time for the game world.
-- The time value comes from the game millisecond timer, converted into year/month/day hour:minute:second.
-- @return A table formatted as a @{Hooks:TimeStamp|TimeStamp} value, containing the current world time.
function world:GetTime()
end

--- AdvanceTime moves time forward for the given amount.
-- @param timestruct A table formatted as a @{Hooks:TimeStamp|TimeStamp} value. The "year" and "month" fields are
-- ignored; instead pass the equivalent number of days in the "day" field.
-- @return None.
function world:AdvanceTime(timestruct)
end

--- GetMissionVar gets the current value of a mission variable.
-- @param key The key (name) of the mission variable to retrieve.
-- @return The value of the mission variable's value as a string. If the variable does not exist, returns an empty string.
function world:GetMissionVar(key)
end

--- GetCampaignVar gets the current value of a campaign variable.
-- @param key The key (name) of the campaign variable to retrieve.
-- @return The value of the campaign variable's value as a string. If the variable does not exist, returns an empty string.
function world:GetCampaignVar(key)
end

--- SetMissionVar sets a new value for a mission variable.
-- @param key The key (name) of the mission variable to set.
-- @param value The new value for the mission variable. If it does not exist, the variable will be created.
-- @return None.
function world:SetMissionVar(key, value)
end

--- SetCampaignVar sets a new value for a campaign variable.
-- @param key The key (name) of the campaign variable to set.
-- @param value The new value for the campaign variable. If it does not exist, the variable will be created.
-- @return None.
function world:SetCampaignVar(key, value)
end

--- Entity functions.
-- Functions which get or create entities.
-- @section Entity_functions

--- GetPlayer retrieves the main character entity.
--
-- @return An Entity object corresponding to the main character. Should be of type Actor.
function world:GetPlayer()
end

--- GetSquad retrieves all characters and vehicles belonging to the player's squad.
-- @return An array of Entity objects corresponding to the squad members. Should be of types Actor or Vehicle.
function world:GetSquad()
end

--- GetAllEntities retrieves all entities existing on the current map.
-- @return An array of Entity objects corresponding to all mission entities.
function world:GetAllEntitites()
end

--- GetEntitiesByTag retrieves all entities existing on the current map which have the given value as the tag name.
-- @param tagname The tag name to search for in the entity list.
-- @return An array of Entity objects corresponding to all mission entities matching the given tag name.
function world:GetEntitiesByTag(tagname)
end

--- CreateEntity creates a new entity.
-- At present, this only works for inventory items - subclasses of Collectable.
-- @param entityFile The file name of the entity to create.
-- @param quantity The number of that item to create.
-- @return An Entity object that can be passed to AddToInventory.
function world:CreateEntity(entityFile, quantity)
end


--- UI functions.
-- UI and other miscellaneous functions.
-- @section UI

--- CobatLog adds a string to the combat log.
-- @param level The minimum logging level for this message to be displayed in the combat log.  Can be COMBATLOG_SYSTEM, COMBATLOG_FEW, COMBATLOG_DEFAULT, or COMBATLOG_ALL.
-- @param str The string to display in the combat log.
function world:CombatLog(level, str)
end
