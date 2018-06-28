ACTOR_TABLE_PERM = 0
ACTOR_TABLE_DERIVED = 1
ACTOR_TABLE_TEMPORARY = 2
ACTOR_TABLE_CURRENT = 3


function OnStart ()

    logger:log("Entered OnStart")
--[[
-- Uncomment this code (remove --[[) to enable Hulk Smash perk
    newperk = {name = "hulkSmash",
               minlevel = 16,
               maxperktaken = 1,
               requiredstat1 = "doctor",
               requiredamt1 = 50,
               requiredEN = 5
              }
    hookexecutor:ReplacePerk(newperk, 18)
--]]
end

function DefaultStyleChanges(style)
--[[
  -- To modify text colors, uncomment (remove --[[) and
  -- modify the values below 
  -- colors are RGB, float between 0.0 and 1.0
  style:SetColorDefaultText(0.9,0.6,0.0)
  style:SetColorHighlightText(1.0,1.0,0.0)
  style:SetColorOptionsPages(1.0,1.0,1.0)
  style:SetColorPanelTitles(1.0,1.0,1.0)
  style:SetColorBuffs(0.0,0.2,1.0)
  style:SetColorDebuffs(0.0,1.0,0.0)
  style:SetColorTags(0.45,0.3,0)
--]]
end

function OnRadiated(e)
--[[
-- Uncomment (remove --[[) to enable Hulk Smash perk on radiated
    logger:log("Radiated: " .. e["name"])
    if e:GetAttribute("hulkSmash", ACTOR_TABLE_CURRENT) then
      logger:log("Radiated and has perk: " .. e["name"])
      if e:GetAttribute("hulkSmash", ACTOR_TABLE_TEMPORARY) == 0 then
        logger:log(e["name"] .. " gains hulkSmash")
        e:DisplayMessage("<Cg>HULK SMASH!")
        bonus = {strength=2, radiationResist=75}
        e:ApplyBonus(bonus,false)
      end
      e:SetAttribute("hulkSmash",ACTOR_TABLE_TEMPORARY,6)	-- # of big ticks to keep buff
    end
--]]
end

function OnLongTick(e)
--[[
-- Uncomment to enable expiration of Hulk Smash perk
    curr = e:GetAttribute("hulkSmash",ACTOR_TABLE_TEMPORARY)
    if curr > 0 then
      curr = curr - 1
      if curr == 0 then
        logger:log(e["name"] .. " hulkSmash expired")
        e:DisplayMessage("Normal")
        bonus = {strength=2, radiationResist=75}
        e:RemoveBonus(bonus,false)
      end
      e:SetAttribute("hulkSmash",ACTOR_TABLE_TEMPORARY,curr)
    end
--]]
end

month_end_days = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}

function MsecToGametime(ms)
    ms = ms + 2197*1000*86400*365 + 6*1000*3600 + 29*1000*60
    y = ms//(1000*86400*365)
    d = (ms//86400000%365)+1    
    h = (ms//3600000)%24
    m = (ms//60000)%60
    s = (ms//1000)%60
    mo = d // 30
    if mo == 0 or d > month_end_days[mo] then
      mo = mo + 1
    end
    if mo > 1 then
      d = d - month_end_days[mo-1]
    end
    gametime = {
      year = y,
      month = mo,
      day = d,
      hour = h,
      minute = m,
      second = s }
    return gametime
end