--- hookexecutor contains functions which can modify the game executable during the startup hook functions @{Hooks:OnStart|OnStart} and @{Hooks:OnLocaleLoad|OnLocaleLoad}.
-- Functions defined here are temporary, and will likely move in the future once more of the internal game tables are exposed through the Lua interface.
-- @classmod hookexecutor

--- ReplacePerk changes the data for a perk in the Fallout: Tactics perk table.
-- This allows for the requirements and permanent bonuses from perks to be changed, and for currently disabled perk slots to be used for custom behavior.
-- @param perkTable A table formatted as @{PerkTable}, with the new parameters to use for the perk.
-- @param perkId The index into the perk table to modify. This can be determined by finding the location of the desired perk slot in the entity editor, and starts from 0.
-- @return None.
function hookexecutor:ReplacePerk(perkTable, perkId)
end

--- AddLocaleString adds a locale string mapping from within the Lua script.
-- This allows a script to provide a translation string without having to edit the BOS locale files.
-- @param key The key (name) of the locale string to add.
-- @param value The text (value) of the locale string to add.
-- @return None.
function hookexecutor:AddLocaleString(key, value)
end

--- Types.
-- @section Types

--- PerkTable format used in @{ReplacePerk}.
-- All fields are optional. If defaulted, integer parameters default to 0, and strings default to empty string, "".
-- @field minlevel The minimum character level required to be eligible to select this perk.
-- @field maxperktaken The number of times a player is allowed to select this perk for a character.
-- @field bonusstat If a perk applies a permanent bonus to a stat, provide the name of the stat here.  The name should match the name of the stat in the Entity editor. Leave as an empty string, "", if no bonus should be applied.
-- @field bonusamt How much to add or remove from the bonus stat.
-- @field requiredstat1 The first skill requirement, for perks which have minimum skill levels to be selected. If no skill requirement is needed for the perk, leave as an empty string, "".
-- @field requiredamt1 The minimum skill level of the skill listed in requiredstat1.
-- @field and_or_flag For perks with two skill requirements, this flag indicates whether only one of the two requirements must be met (value of 1), or if both requirements need to be met (value of 2).
-- @field requiredstat2 The second skill requirement, if any, for perks which have minimum skill levels to be selected. If no second skill requirement is needed for the perk, leave as an empty string, "".
-- @field requiredamt2 The minimum skill level of the skill listed in requiredstat2.
-- @field requiredST The minimum Strength attribute in order to use this perk.
-- @field requiredPE The minimum Perception attribute in order to use this perk.
-- @field requiredEN The minimum Endurance attribute in order to use this perk.
-- @field requiredCH The minimum Charisma attribute in order to use this perk.
-- @field requiredIN The minimum Intelligence attribute in order to use this perk.
-- @field requiredAG The minimum Agility attribute in order to use this perk.
-- @field requiredLK The minimum Luck attribute in order to use this perk.
-- @field prohibitedRace A comma-separated list of races which are prohibited from selecting the perk. If no prohibition, leave as an empty string, "".
-- @field requiredRace A string indicating the only race allowed to select the perk. If no limitation, leave as an empty string, "".
-- @table hookexecutor:PerkTable
