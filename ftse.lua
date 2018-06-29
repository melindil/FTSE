--[[
Leave these definitions alone - they are needed to interface
with the C++ DLL library.
--]]

ACTOR_TABLE_PERM = 0
ACTOR_TABLE_DERIVED = 1
ACTOR_TABLE_TEMPORARY = 2
ACTOR_TABLE_CURRENT = 3

--[[
The following code is used to convert between the internal
game timer (millisecond resolution) to in-game date and time.
In the future, this code will be added to a hook to allow it
to change the way in-game dates are calculated, but for now,
do not modify the below.
--]]

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

-- Modifiable functions begin below -----------------

--[[
OnStart is called at process start. At this point, no
game has yet been loaded. The main purpose here is to
make modifications to in-game tables before any other part
of the game initializes.  So far, only modifying the perk list
is supported, but additional capabilities will be added.
--]]

function OnStart ()

--[[
Logging can be performed with the logger:log("") function
--]]

    logger:log("Entered OnStart")

--[[
hookexecutor:ReplacePerk is the function used to overwrite a
perk.  The following prerequisites are needed:
1) Create a Lua table containing the perk information, described
   below. Note that the "name" field must be set, and used below
2) Ensure that core/locale/game/perk.txt contains an entry for
   name_"name" and desc_"name", where "name" is the name value
   given in the Lua table.
3) Ensure that core/gui/pip contains a .txt file and .zar file
   named with the perk's "name" value.

Supported parameters in the perk information table are:

  name: Name to use for the perk (referenced in locale and gui
        files)

  minlevel: The minimum level at which the perk can be taken

  maxperktaken: The maximum number of times the perk can be
                selected

  bomusstat: A character attribute which is permanently
             boosted by the perk. This can be a stat (e.g.
             strength), a skill (smallGuns), or a "Derived"
             field (maxActionPoints). This field is currently
             case-sensitive, so double-check the desired stat
             in the FT Tools entity editor.

  bonusamt: Amount to increase for bonus (negative to decrease)

  requiredstat1: Indicates a skill requirement that must be
                 met in order to select the perk.
  requiredamt1:  Amount of that stat that is needed

  and_or_flag: If two skill requirements are given, this value
               is set to "1" to indicate that only one of the
               two requirements must be met (like Medic!) or
               "2" if both must be met (like Silent Death)

  requiredstat2: Second skill requirement, if any
  requiredamt2:  

  requiredST: Minimum stat values for SPECIAL attributes.
  requiredPE
  requiredEN
  requiredCH
  requiredIN
  requiredAG
  requiredLK

  prohibitedRace: A space-separated list of race types which are
                   prohibited from selecting the perk.  Example
                   is "Dog Deathclaw".
  requiredRace: A race type which is required in order to select
                this perk.

Two example perks are listed below.  Uncomment to enable them.
Note that the changes to locale and gui described above must
be done for them to work properly. To uncomment, remove the
 "--[[" and "--]]" lines around the code snippet.
--]]

--[[
-- Night person as a perk, and working correctly
    newperk = {name = "nightPersonPerk",
               minlevel = 3,
               maxperktaken = 1
              }
    hookexecutor:ReplacePerk(newperk, 18)
--]]

--[[ 
-- Hulk Smash example
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

--[[
OnRadiated is a trigger which fires whenever an actor takes
radiation damage from a map source (unknown if other radiation
sources trigger it). This is used for the Hulk Smash POC.
The function takes an actor object, which can be used
to get and set actor attributes.

Actor functions:

GetAttribute can retrieve any character attribute
value for an actor. First parameter is the attribute
name (case-sensitive), second is the table which the
attribute should be read from (usually ACTOR_TABLE_CURRENT
or ACTOR_TABLE_TEMPORARY).

SetAttribute is the corollary to GetAttribute, to update
a specific attribute.  This usually should only be used
for perk entries, and only in the "temporary" table, to
keep track of values needed by the rest of the script.
Example: in Hulk Smash, the number of long ticks that the
buff lasts is kept in the temporary attribute for the perk.

DisplayMessage shows a message above the actor's head.

ApplyBonus takes two parameters: a table of bonus attributes
and values to add, and a boolean indicating true if the
modifier is intended to be permanent.  The entries in the
table are the same attributes as normal, and again case-
sensitive.

RemoveBonus is the direct opposite of ApplyBonus.


--]]


--[[
-- Uncomment this for Hulk Smash to work

function OnRadiated(e)

    if e:GetAttribute("hulkSmash", ACTOR_TABLE_CURRENT) then
      if e:GetAttribute("hulkSmash", ACTOR_TABLE_TEMPORARY) == 0 then
        logger:log(e["name"] .. " gains hulkSmash")
        e:DisplayMessage("<Cg>HULK SMASH!")
        bonus = {strength=2, radiationResist=75}
        e:ApplyBonus(bonus,false)
      end
      e:SetAttribute("hulkSmash",ACTOR_TABLE_TEMPORARY,6)	-- # of big ticks to keep buff
    end
end

--]]

--[[
OnLongTick is called for every actor, every 10 seconds
of "game" time. This is 10 actual seconds in CTB. For turn-
based, one complete turn for all actors takes 7 seconds.

In the Night Person example, the call to world:GetTime returns
a Lua table with the current date and time.  Fields are "year",
"month", "day", "hour", "minute", and "second".
--]]

function OnLongTick(e)

--[[
-- Uncomment this for the Night Person sample to work

    if e:GetAttribute("nightPersonPerk", ACTOR_TABLE_CURRENT) > 0 then
      -- use perk entry in temporary table to save info about
      -- penalty (=1), bonus (=2), or neither (=0) is applied.
      curr = e:GetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY)
      now = world:GetTime()
      if now["hour"] < 8 or now["hour"] > 19 then
        -- night time: apply bonus if none yet or on penalty
        if curr == 0 then
          bonus = { perception=1, intelligence=1 }
          e:ApplyBonus(bonus,false)
          e:SetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY, 2)
        elseif curr == 1 then
          bonus = { perception=2, intelligence=2 }
          e:ApplyBonus(bonus,false)
          e:SetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY, 2)
        end
      else
        -- day time: apply penalty if none yet or on bonus
        if curr == 0 then
          bonus = { perception=1, intelligence=1 }
          e:RemoveBonus(bonus,false)
          e:SetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY, 1)
        elseif curr == 2 then
          bonus = { perception=2, intelligence=2 }
          e:RemoveBonus(bonus,false)
          e:SetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY, 1)
        end
      end
    end
--]]

--[[
-- Uncomment this section for the Hulk Smash example to work
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

