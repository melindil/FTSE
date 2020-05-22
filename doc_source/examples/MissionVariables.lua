--[[

In this example, we will add a bonus perk that is granted
on a mission trigger. We will have Felix, from Quincy,
give the player the following perk if he is rescued:

Veteran's Knowledge

An old veteran has taught you some new tricks. +10% to
Small Guns and Repair.

--]]

-- @{Hooks:OnStart|OnStart} hook function
function OnStart()

  -- Add the perk. Setting maxperktaken to 0 prevents the perk from
  -- appearing in the list of perks at level up.
  newperk = { name = "perkFelix", maxperktaken = 0 }

  -- Replace unused perk 37: "mysteriousStranger"
  hookexecutor:ReplacePerk(newperk, 37)
end

-- @{Hooks:OnLocaleLoad|OnLocaleLoad} hook function
function OnLocaleLoad()
  hookexecutor:AddLocaleString("name_perkFelix", "Veteran's Knowledge")
  hookexecutor:AddLocaleString("desc_perkFelix", "An old veteran has taught you some new tricks. +10% to Small Guns and Repair.")

end

-- @{Hooks:OnVariableChanged|OnVariableChanged} hook function
function OnVariableChanged(key,value,campaign)

  -- Check if our variable is set to the value we want
  if key == "Felix_Rescued" and value == "True" then

    -- Get the player with @{world:GetPlayer}
    player = world:GetPlayer()

    -- Safety check: ensure the player  doesn't already have the perk
    -- This will catch cases where the variable trigger accidentally fires twice
    if player:GetAttribute("perkFelix", ACTOR_TABLE_CURRENT) == 0 then

      -- Apply the perk
      player:SetAttribute("perkFelix", ACTOR_TABLE_PERM, 1)

      -- Apply bonuses granted by the perk
      bonus = { smallGuns=10, repair=10}
      player:ApplyBonus(bonus, true)

      -- Notify the player: message in the combat log with @{world:CombatLog}:
      world:CombatLog(COMBATLOG_FEW,"Felix has taught " .. player:GetName() .. " something new.")

    end
  end
end

