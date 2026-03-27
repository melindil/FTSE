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

EQUIP_SLOT_LEFT = 0
EQUIP_SLOT_RIGHT = 1
EQUIP_SLOT_ARMOR = 2

ENTITY_COLOR_BASE = 0
ENTITY_COLOR_SKIN = 1
ENTITY_COLOR_HAIR = 2
ENTITY_COLOR_TEAM = 3

LOCALE_GUI_GUITEXT = 0
LOCALE_GAME_REASON = 1
LOCALE_GAME_COMBAT = 2
LOCALE_GAME_MISC = 3
LOCALE_GAME_AMMO = 4
LOCALE_GAME_WEAPON = 5
LOCALE_GAME_ITEMS = 6
LOCALE_GAME_BADWORDS = 7
LOCALE_GUI_HELPTEXT = 8
LOCALE_GAME_TAUNTS = 9
LOCALE_GAME_CHARACTERS = 10

--[[
Functions for Entity custom variable storage
--]]
function FTSE_Serialize(o)
  if type(o) == "string" then
    return string.format("%q", o)
  elseif type(o) == "table" then
    local s = " { "
      for k,v in pairs(o) do
        s = s .. " " .. tostring(k) .. " = " .. FTSE_Serialize(v) .. ","
      end
      return s .. " } "
  else
    return tostring(o)
  end
end

-- From StackOverflow, credit to "nobody"
local function condfail(cond,...)
  if not cond then return nil,(...) end
  return ...
end

function FTSE_Deserialize(s)
  local env = {}
  local f,_err = load("return  " .. s, "=FTSE_Deserialize","t",env)
  if not f then return nil,_err end
  local co = coroutine.create(f)
  local hook = function() debug.sethook(co,error,"c",1000000) end
  debug.sethook(co,hook,"c")
  return condfail(coroutine.resume(co))
end

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

require "FTSE.ftse"

--[[
chained_load = loadfile("./FTSE/ftse.lua")
if chained_load ~= nil then
	chained_load()
else
  chained_load = loadfile("ftse.lua")
  if chained_load ~= nil then
	chained_load()
  end
end
--]]

if OnRadiated ~= nil then hookexecutor:InstallHook("OnRadiated",OnRadiated) end
if OnLongTick ~= nil then hookexecutor:InstallHook("OnLongTick",OnLongTick) end
if OnVariableChanged ~= nil then hookexecutor:InstallHook("OnVariableChanged",OnVariableChanged) end
if OnStart ~= nil then hookexecutor:InstallHook("OnStart",OnStart) end
if DefaultStyleChanges ~= nil then hookexecutor:InstallHook("DefaultStyleChanges",DefaultStyleChanges) end
if OnLocaleLoad ~= nil then hookexecutor:InstallHook("OnLocaleLoad",OnLocaleLoad) end
if MsecToDayHMS ~= nil then hookexecutor:InstallHook("MsecToDayHMS",MsecToDayHMS) end
if AddBaseToGameTime ~= nil then hookexecutor:InstallHook("AddBaseToGameTime",AddBaseToGameTime) end
if OnChanceToHitCalc ~= nil then hookexecutor:InstallHook("OnChanceToHitCalc",OnChanceToHitCalc) end
if OnChanceToCritical1 ~= nil then hookexecutor:InstallHook("OnChanceToCritical1",OnChanceToCritical1) end
if OnChanceToCritical2 ~= nil then hookexecutor:InstallHook("OnChanceToCritical2",OnChanceToCritical2) end
if OnCriticalEffect1 ~= nil then hookexecutor:InstallHook("OnCriticalEffect1",OnCriticalEffect1) end
if OnCriticalEffect2 ~= nil then hookexecutor:InstallHook("OnCriticalEffect2",OnCriticalEffect2) end
if OnDamageCalc ~= nil then hookexecutor:InstallHook("OnDamageCalc",OnDamageCalc) end
if OnInventoryAdd ~= nil then hookexecutor:InstallHook("OnInventoryAdd",OnInventoryAdd) end
if OnInventoryRemove ~= nil then hookexecutor:InstallHook("OnInventoryRemove",OnInventoryRemove) end
if OnBurstAttack ~= nil then hookexecutor:InstallHook("OnBurstAttack",OnBurstAttack) end
if OnConeAttack ~= nil then hookexecutor:InstallHook("OnConeAttack",OnConeAttack) end
if OnSprayAttack ~= nil then hookexecutor:InstallHook("OnSprayAttack",OnSprayAttack) end
if OnRadialAttack ~= nil then hookexecutor:InstallHook("OnRadialAttack",OnRadialAttack) end
if OnAreaAttack ~= nil then hookexecutor:InstallHook("OnAreaAttack",OnAreaAttack) end
if OnStraightAttack ~= nil then hookexecutor:InstallHook("OnStraightAttack",OnStraightAttack) end
if OnProjectileAttack ~= nil then hookexecutor:InstallHook("OnProjectileAttack",OnProjectileAttack) end
if OnCheckItemAllowed ~= nil then hookexecutor:InstallHook("OnCheckItemAllowed",OnCheckItemAllowed) end
if OnEquip ~= nil then hookexecutor:InstallHook("OnEquip",OnEquip) end
if OnCheckEquip ~= nil then hookexecutor:InstallHook("OnCheckEquip",OnCheckEquip) end
if OnUnequip ~= nil then hookexecutor:InstallHook("OnUnequip",OnUnequip) end
if OnCheckUnequip ~= nil then hookexecutor:InstallHook("OnCheckUnequip",OnCheckUnequip) end
if OnMissionLoad ~= nil then hookexecutor:InstallHook("OnMissionLoad",OnMissionLoad) end



