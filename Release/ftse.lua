--[[
Leave these definitions alone - they are needed to interface
with the C++ DLL library.
--]]

ACTOR_TABLE_PERM = 0
ACTOR_TABLE_DERIVED = 1
ACTOR_TABLE_TEMPORARY = 2
ACTOR_TABLE_CURRENT = 3

COMBATLOG_SYSTEM = 0
COMBATLOG_FEW = 1
COMBATLOG_DEFAULT = 2
COMBATLOG_ALL = 3

--[[
The following code is used to convert between the internal
game timer (millisecond resolution) to in-game date and time.
--]]

month_end_days = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365}

function MsecToDayHMS(ms,scale)
    if scale > 0 then
      ms = ms * 3
    end
    d = (ms // 86400000)
    h = (ms // 3600000)%24
    m = (ms // 60000)%60
    s = (ms // 1000)%60
    ms = ms % 1000
    gametime = {
      year = 0,
      month = 0,
      day = d,
      hour = h,
      minute = m,
      second = s,
      msec = ms
    }
    return gametime
end

function AddBaseToGameTime(gametime)

    -- add base days, hours and minute
    gametime["minute"] = gametime["minute"] + 29
    gametime["hour"] = gametime["hour"] + 6
    gametime["day"] = gametime["day"] + 2197*365

    -- normalize hours and minutes
    if gametime["minute"] > 59 then
      gametime["minute"] = gametime["minute"] - 60
      gametime["hour"] = gametime["hour"] + 1
    end
    if gametime["hour"] > 23 then
      gametime["hour"] = gametime["hour"] - 24
      gametime["day"] = gametime["day"] + 1
    end
    
    -- calculate year and day within year
    gametime["year"] = gametime["day"]//365
    gametime["day"] = gametime["day"] % 365 + 1

    -- Get month and day within month
    mo = gametime["day"] // 30
    if mo == 0 or gametime["day"] > month_end_days[mo] then
      mo = mo + 1
    end
    if mo > 1 then
      gametime["day"] = gametime["day"] - month_end_days[mo-1]
    end
    gametime["month"] = mo
    return gametime
end

-- Burst bug fix code
function CalculateChance(chance,intended,angle)
  if intended == false then
    chance = math.min(chance,40)
    if angle > 5.0 then
      chance = math.floor(chance * (21.3 - angle) / 16.3)
    end
  end
  return chance
end

function CalculateShare(chance)
  return math.min(chance,70)   
end

function OnBurstAttack(attacker,shots,target_table)
  results = {}
  totalshare = 0
  for _,tgt in ipairs(target_table) do
    share = CalculateShare(CalculateChance(tgt["hit_chance"],tgt["intended"],tgt["angle"]))
    totalshare = totalshare + share
  end
  totalshare = math.max(totalshare,100)
  shotsleft = shots
  for _,tgt in ipairs(target_table) do
    chance = CalculateChance(tgt["hit_chance"],tgt["intended"],tgt["angle"])
    tgtshots = math.min(shotsleft,math.max(1,math.floor(shots * CalculateShare(chance) / totalshare)))
    shotsleft = shotsleft - tgtshots
    myhits = 0
    for i=1,tgtshots do
      roll = math.floor(math.random()*100)
      if roll < chance then
        myhits = myhits + 1
      end
    end
    result = {id=tgt["actor"]["id"], hits=myhits }
    table.insert(results, result)
  end
  return results
end

math.randomseed(os.time())
-- Modifiable functions begin below -----------------



