
function FTImprover_OnStart()

local function FTImprover_ProcessFile(feature, filename)
  logger:log("FTImprover Enabled: " .. feature)
  local file_handle,errmsg = io.open(filename)
  if file_handle == nil then
    logger:log("Could not open " .. feature .. " file: " .. errmsg)
  else
    local file_content = file_handle:read("*all")
    improver:ParseConfig(file_content)
  end
end

  logger:log("FTImprover module loading configuration file Improver_data/Improver.cfg")
  local config_file,errmsg = io.open("Improver_data/Improver.cfg")
  if config_file == nil then
    logger:log("Could not open FT Improver config file: " .. errmsg)
  else
    local config_content = config_file:read("*all")
    improver:ParseConfig(config_content)
    if improver:IsFeatureEnabled("APCT") then
      FTImprover_ProcessFile("APCT","Improver_data/APCTstructures.txt")
    end
    if improver:IsFeatureEnabled("AnimationNames") then
      FTImprover_ProcessFile("AnimationNames","Improver_data/AnimationNames.txt")
    end
    if improver:IsFeatureEnabled("WeaponItemTypes") then
      FTImprover_ProcessFile("WeaponItemTypes","Improver_data/WeaponItemTypes.txt")
    end
    if improver:IsFeatureEnabled("WeaponSoundTypes") then
      FTImprover_ProcessFile("WeaponSoundTypes","Improver_data/WeaponSoundTypes.txt")
    end
    if improver:IsFeatureEnabled("AmmoTypes") then
      FTImprover_ProcessFile("AmmoTypes","Improver_data/AmmoTypes.txt")
    end
    if improver:IsFeatureEnabled("AmmoVariants") then
      FTImprover_ProcessFile("AmmoVariants","Improver_data/AmmoVariants.txt")
    end
    if improver:IsFeatureEnabled("ArmourTypes") then
      FTImprover_ProcessFile("ArmourTypes","Improver_data/ArmourTypes.txt")
    end
    if improver:IsFeatureEnabled("ImpactSounds") then
      FTImprover_ProcessFile("ImpactSounds","Improver_data/ImpactSounds.txt")
    end
  end

  -- FTI defaults to always having GotoMissionFix enabled
  improver:EnableGotoMissionFix()

end

logger:log("Installing FT Improver start")
hookexecutor:InstallHook("OnStart", FTImprover_OnStart)

return {}