--- TestBed example map
-- Show examples for FTSE 0.60a features

-- To use: In core/FTSE/ftse.lua, add only the following statment: require "FTSE.examples.TestBed"
-- Then, load the custom TestBed mission from the FTSE section of the Custom Mission list.

--------------------------------------------------------------------------

-- To demo the FT Improver-based armour rule feature, we will
-- allow SuperMutants to use Metal armour

local function TestBed_OnCheckItemAllowed(ac,it)
  if ac:GetEntitySubType() == "Mutant" and it:GetEntitySubType() == "Metal" then
    return true
  end
  if ac:GetEntitySubType() == "Mutant" and it:GetEntitySubType() == "PipboyCostume" then
    return false
  end

  -- Empty return means to process other checks normally
  return
end

local function TestBed_OnStart()
  -- Set the sprite to use for mutant wearing Metal armour. If this is not done,
  -- the sprite will revert to the human version.
  improver:AddArmourRule("Metal","*","Mutant","sprites/characters/MutantArmour.spr")

  -- We will also make a new Pipboy Costume armour type, and set sprite and sound overrides
  improver:AddArmourType("PipboyCostume")
  improver:AddArmourRule("PipboyCostume","*","*","sprites/characters/Pipboy.spr")
  improver:AddSoundRule("PipboyCostume","*","*","Leath")
end

-- Create the locale text for the Pipboy Costume and Mr. Tutor
local function TestBed_OnLocaleLoad()
  hookexecutor:AddLocaleString("name_ftse_costume","Pipboy Costume",LOCALE_GAME_ITEMS)
  hookexecutor:AddLocaleString("desc_ftse_costume","A strange costume.",LOCALE_GAME_ITEMS)
  -- This overrides the existing name
  hookexecutor:AddLocaleString("name_CORE_bosElder", "Mr. Tutor", LOCALE_GAME_CHARACTERS)
end

-- Load the Dialogue module and register the Mr. Tutor conversation
speech = require "FTSE.modules.Dialogue"
speech:LoadConversation("tutor","FTSE.examples.TestBed.tutor")

-- Install the hooks as if we're a module
hookexecutor:InstallHook("OnStart",TestBed_OnStart)
hookexecutor:InstallHook("OnLocaleLoad",TestBed_OnLocaleLoad)
hookexecutor:InstallHook("OnCheckItemAllowed",TestBed_OnCheckItemAllowed)
