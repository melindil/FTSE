--[[

For this example, we will be creating the following
custom perk:

Hulk Smash!

Requirements: Level 16, EN 5, Doctor 50%

Perk Levels: 1

Description: Being radiated makes you very angry.
Gain +2 Strength and +75 Radiation Resistance when
in a radiated area.

Change note: This example has been modified for FTSE 0.60a to show how to use module-based hooks and custom variable storage.
To use: Copy to core\FTSE\modules and add require "FTSE.modules.HulkSmash" to ftse.lua

--]]

-- @{Hooks:OnStart|OnStart} hook function
local function HS_OnStart ()
    -- We create the new perk by using the @{hookexecutor:ReplacePerk}
    -- function at game startup.
    newperk = {name = "hulkSmash",
               minlevel = 16,
               maxperktaken = 1,
               requiredstat1 = "doctor",
               requiredamt1 = 50,
               requiredEN = 5
              }
    -- We replace unused perk 18, "fortuneFinder"
    hookexecutor:ReplacePerk(newperk, 18)
end

-- @{Hooks:OnLocaleLoad|OnLocaleLoad} hook function
local function HS_OnLocaleLoad()
  -- Locale strings can either be added directly to the game's .txt
  -- files, or included in the OnLocaleLoad function. Here, we
  -- define the name and description strings for our new perk.
  -- Use @{hookexecutor:AddLocaleString} to add the strings.
  hookexecutor:AddLocaleString("name_hulkSmash","Hulk Smash!")
  hookexecutor:AddLocaleString("desc_hulkSmash","Being radiated makes you very angry. Gain +2 Strength and +75 Radiation Resistance when in a radiated area.")
end

-- @{Hooks:OnRadiated|OnRadiated} hook function
local function HS_OnRadiated(e)
    -- OnRadiated was defined as a trigger specifically for this perk
    -- example. This is called anytime an actor has its radiation
    -- counter increased.

    -- Check if the actor has the perk using @{Actor:GetAttribute}
    if e:GetAttribute("hulkSmash", ACTOR_TABLE_CURRENT) > 0 then
      -- Check the saved timer variable with @{Entity:GetCustomValue} to see if the perk is already active
      if e:GetCustomValue("HulkSmashTimer") ==nil or e:GetCustomValue("HulkSmashTimer") == 0 then
        -- Actor has the perk but not the bonus yet, apply it

        -- For debugging purposes, we can log a message to FTSE.log using @{logger:log}
        logger:log(e:GetName() .. " gains hulkSmash")
        -- Give a visual indication to the player - text above the
        -- character's head, using @{Actor:DisplayMessage}:
        e:DisplayMessage("<Cg>HULK SMASH!")
        -- Apply the bonus. The second parameter to @{Actor:ApplyBonus|ApplyBonus}
        -- indicates if the bonus is temporary (false) or permanent (true)
        bonus = {strength=2, radiationResist=75}
        e:ApplyBonus(bonus,false)
      end
      -- Set the variable indicating the bonus is applied, and set or
      -- refresh its value to 6 (60 seconds - see OnLongTick below)
	  -- Use @{Entity:SetCustomValue}:
      e:SetCustomValue("HulkSmashTimer",6)
    end
end

-- @{Hooks:OnLongTick} hook function
local function HS_OnLongTick(e)
  -- This is called for every actor, every 10 seconds of game time
  -- Check if character has the Hulk Smash bonus applied
  curr = e:GetCustomValue("HulkSmashTimer")
  if curr ~= nil and curr > 0 then
    -- The bonus is applied. Reduce the counter by 1
    curr = curr - 1
    if curr == 0 then
      -- The counter has expired, remove the bonus
      logger:log(e:GetName() .. " hulkSmash expired")
      -- Indicate to the player
      e:DisplayMessage("Normal")
      -- Remove the bonus
      bonus = {strength=2, radiationResist=75}
      e:RemoveBonus(bonus,false)
    end
    -- Update the counter with the new value
    e:SetCustomValue("HulkSmashTimer",curr)
  end
end

hookexecutor:InstallHook("OnStart", HS_OnStart)
hookexecutor:InstallHook("OnLocaleLoad", HS_OnLocaleLoad)
hookexecutor:InstallHook("OnRadiated", HS_OnRadiated)
hookexecutor:InstallHook("OnLongTick", HS_OnLongTick)

return {}