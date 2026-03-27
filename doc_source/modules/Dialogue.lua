--- Dialogue provides the interface to the interactive dialogue feature of FTSE.
-- To use, include the module using Lua's "require" statement, saving the returned object:
-- @usage speech=require "FTSE.modules.Dialogue"
-- @module Dialogue

--- The LoadConversation function will load a Lua file, and register it as a conversation.
-- @param tag A tag string identifying the conversation. This tag must be included in a level as a "windowed" speech node, and assigned to an NPC's click speech.
-- @param file A string indicating the location of the file, in Lua "require" format. For example, if a conversation file named MyConv.lua is in the core\FTSE\Dialogue directory, the file string would be "FTSE.Dialogue.MyConv".
-- @usage speech=require "FTSE.modules.Dialogue"
-- @usage speech:LoadConversation("MyConv","FTSE.Dialogue.MyConv")
-- @return None.
function Dialogue:LoadConversation(tag,file)
end

--- The RunNode function is used internally by FTSE to initiate transitions within the dialogue tree.
-- It is not necessary to call this function directly to start a conversation; however, it can be
-- useful within code-type nodes to move to a new node in the conversation.
-- @param conv The tag string for the conversation being processed.
-- @param node The name of the node to transition to.
-- @param npc The Entity corresponding to the NPC taking part in the conversation.
-- @param plyr The Entity corresponding to the player's squad member taking part in the conversation
-- @return Returns the Dialogue module object.
function Dialogue:RunNode(conv,node,npc,plyr)
end

--- Helper functions for Choice conditions.
-- @section Helpers

--- skillcheck is a shortcut function for the condition field in @{Choice} objects, returning true if the squad member meets the given skill check.
-- @param skillparam The name of the parameter as normally used in @{Actor:GetAttribute}.
-- @param value The minimum value in the specified parameter.
-- @return A function object suitable for the condition field.
function Dialogue:skillcheck(skillparam,value)
end

--- skillnotmet is a shortcut function for the condition field in @{Choice} objects, returning true if the squad member does not meet the given skill check.
-- @param skillparam The name of the parameter as normally used in @{Actor:GetAttribute}.
-- @param value The value at which the choice will no longer be presented.
-- @return A function object suitable for the condition field.
function Dialogue:skillnotmet(skillparam,value)
end

--- campaignvar is a shortcut function for the condition field in @{Choice} objects, returning true if the listed campaign variable has the given value.
-- @param var The name of the campaign variable to check.
-- @param value The value at which the choice will be presented to the player.
-- @return A function object suitable for the condition field.
function Dialogue:campaignvar(var,value)
end

--- missionvar is a shortcut function for the condition field in @{Choice} objects, returning true if the listed mission variable has the given value.
-- @param var The name of the mission variable to check.
-- @param value The value at which the choice will be presented to the player.
-- @return A function object suitable for the condition field.
function Dialogue:missionvar(var,value)
end

--- Dialogue object fields.
-- These fields are used in the C++-to-Lua linkage, and should generally not be modified by the script.
-- @section fields

--- The conversations field is a table which includes all registered conversation objects.
-- @field conversations
Dialogue.conversations={}

--- The text field is a string which indicates the NPC text to show in the FTSE Dialogue window.
-- @field text
Dialogue.text=""

--- The audio field is a string which indicates which audio file should be played, if any.
-- @field audio
Dialogue.audio=""

--- The choices field is an array which contains @{Choice} type objects, indicating which dialogue choices will be presented to the player.
-- @field choices
Dialogue.choices={}

--- The choicestack field is an array which contains subtables of @{Choice} objects, as a stack structure. This is used when nesting conversations.
-- @field choicestack
Dialogue.choicestack={}

--- Dialogue object types.
-- These types describe the parameters which can be set in @{Node} and @{Choice} tables.
-- @section fields

--- The Node type defines each node in a conversation tree.
-- A conversation tree object is a Lua table containing multiple @{Node} objects, each with a key value indicating the node name.
-- @field code For a code-type node, this field is required, and should be set to a Lua function, taking five arguments: Dialogue module handle, Converation name, Node name, NPC @{Entity}, and Player @{Entity}. This function will be called if/when the node is reached. For text-type nodes, this field should not be included.
-- @field text The text string to display in the NPC's conversation window when this node is reached. Required for text-type nodes.
-- @field audio The audio file to play when the node is reached. Optional.
-- @field clear If set to true, the list of player choices will be cleared prior to inserting any choices provided by this node. Optional.
-- @field pushstack If set to true, the current set of player choices will be saved in the choicestack, and a new empty table will be created for a nested conversation. This occurs prior to adding any choices provided by this node. Optional.
-- @field popstack If set to true, the current set of player choices will be discarded, and the most recently saved set of choices in choicestack will be restored (and removed from the stack). This occurs prior to adding any choices provided by this node. Optional.
-- @field choices An array of @{Choice} tables which will be added to the set of converstaion options presented to the player. Note that only one active choice can point to any conversation node at a time; if a new choice is provided pointing to a node that is already in the list, the existing choice is replaced with the new one. Optional.
-- @table Node

--- The Choice type defines a conversation choice which will be presented to the player.
-- @field node The name of the node that this choice should transition to if selected. Required.
-- @field text The text that is shown as the player's words for this choice. The strings <player> and <rank>, if placed in the string, will be substituted with the squad member's name or rank. Required unless the remove field is set.
-- @field priority The display priority for this choice. Lower-numbered choices will be displayed first. For example, it is common for the choice to leave a conversation to be the last one presented; setting the priority for that choice to the highest value will ensure that is the case. Required unless the remove field is set.
-- @field remove If set to true, the current choice leading to the specified node will be removed. Optional.
-- @field condition A function that will be called to determine if the choice should be presented to the player. This is typically used for skill-check choices, or choices which only occur if certain game state conditions apply. This should be set to a Lua function taking parameters: (Conversation Name, Node Name, NPC, Player), and returning true if the choice should be presented. There are shortcuts for common functions as described above.
-- @table Choice


