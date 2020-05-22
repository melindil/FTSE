--[[

In this example, we will recreate the Night Person trait as a perk,
 with the following differences:

* The perk will work as described, giving -1 to PE and IN during daytime and +1 to PE and IN during night time.
* The perk will apply the +1/-1 stats as temporary modifiers rather than permanent ones. Among other things, this will result in them being highlighted in blue or red, and will make it so that the character's perk selection is (correctly) not affected by the bonus/penalty.

--]]

-- @{Hooks:OnStart|OnStart} hook function
function OnStart ()
  -- We create the new perk by using the @{hookexecutor:ReplacePerk}
  -- function at game startup.
  -- Very simple requirement for testing, level 3 only
  newperk = {name = "nightPersonPerk",
               minlevel = 3,
               maxperktaken = 1
              }
  -- Replace unused perk 18 "fortuneFinder"
  hookexecutor:ReplacePerk(newperk, 18)

end

-- @{Hooks:OnLocaleLoad|OnLocaleLoad} hook function
function OnLocaleLoad()
  -- Locale strings can either be added directly to the game's .txt
  -- files, or included in the OnLocaleLoad function. Here, we
  -- define the name and description strings for our new perk.
  -- Use @{hookexecutor:AddLocaleString} to add the strings.
    hookexecutor:AddLocaleString("name_nightPersonPerk","Night Person")
    hookexecutor:AddLocaleString("desc_nightPersonPerk","Perk demonstrating game timer routines")

end

-- @{Hooks:OnLongTick} hook function
function OnLongTick(e)

  -- Check if character has the Night Person
  if e:GetAttribute("nightPersonPerk", ACTOR_TABLE_CURRENT) > 0 then

    -- use perk entry in temporary table to save info about
    -- penalty (=1), bonus (=2), or neither (=0) is applied.
    curr = e:GetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY)
    now = world:GetTime()

    if now["hour"] < 8 or now["hour"] > 19 then

      -- night time: apply bonus if none yet or on penalty

      if curr == 0 then

        -- Nothing applied yet - apply the bonus. The second
        -- parameter to @{Actor:ApplyBonus|ApplyBonus}
        -- indicates if the bonus is temporary (false) or permanent (true)
        bonus = { perception=1, intelligence=1 }
        e:ApplyBonus(bonus,false)

        -- Use @{Actor:SetAttribute} to save bonus applied:
        e:SetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY, 2)

      elseif curr == 1 then

        -- Penalty currently applied. Override it with extra bonus
        bonus = { perception=2, intelligence=2 }
        e:ApplyBonus(bonus,false)

        -- Use @{Actor:SetAttribute} to save bonus applied:
        e:SetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY, 2)
      end
    else

      -- day time case : apply penalty if none yet or on bonus
      if curr == 0 then

        -- Nothing applied yet
        bonus = { perception=1, intelligence=1 }
        e:RemoveBonus(bonus,false)

        -- Use @{Actor:SetAttribute} to save penalty applied:
        e:SetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY, 1)

      elseif curr == 2 then

        -- Bonus is applied - apply extra penalty to counter it
        bonus = { perception=2, intelligence=2 }
        e:RemoveBonus(bonus,false)

        -- Use @{Actor:SetAttribute} to save penalty applied:
        e:SetAttribute("nightPersonPerk", ACTOR_TABLE_TEMPORARY, 1)
      end
    end
  end
end

