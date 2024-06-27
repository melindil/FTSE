--[[

This example shows how to use Entity Vtable hooks and calls to add
functionality to an item. In this case, we modify the bullhorn trap item
so that, if it is on the ground and the Science skill is used on it
(with a character at 50% or higher skill), then all traps within a 40 unit
radius are disarmed. (In practice, this is probably overpowered, and in any
event, should probably be more of a single-use item, but it's OK as an
example.)

--]]

-- Utility function to disarm traps within a given distance of a point.
-- Uses Entity vtable 456 to disarm, which requires 0 parameters.

function DisarmTrapsAround(center_pos,maxdist)
  local ret = false
  for _,ent in pairs(world:GetAllEntities()) do
    if ent.ClassType == "Trap" and ent:IsActivated() then
      local entpos = ent:GetPosition()
      local distx = entpos["x"]-center_pos["x"]
      local disty = entpos["y"]-center_pos["y"]
      local distz = entpos["z"]-center_pos["z"]
      local dist = math.sqrt(distx*distx+disty*disty+distz*distz)
      if dist < maxdist then
        ent:CallVtable(456)
        ret = true
      end  
    end
  end
  return ret
end

--[[

Hook for Entity vtable 244 (Use skill on entity)
Parameters are: 
  ent: Entity the skill is used on
  skill: String indicating which skill was used
  target1: The Actor using the skill
  target2: Empty (not set)

If the object is our desired object, then we make the skill check, and if passed,
we call DisarmTrapsAround to actually disarm the traps. If we fail the skill
check, then we use the return UseItemResponse to give an appropriate message to
the player.

If the object is not our desired object, then we call the original vtable 244
function for the entity, and use its return value as ours.

--]]

function Hook244(ent, skill, target1, target2)
  if ent:GetName() == "Bull Horn Trap" and skill == "science" then
    if target1:GetAttribute("science", ACTOR_TABLE_CURRENT) < 50 then
      local ret = { status=1, unk_entity_1 = ent, errstring="trapFailWarning" }
      return ret
    end
    local ret = { status=0 }
    local disarmed_one = DisarmTrapsAround(target1:GetPosition(),40)
    if disarmed_one then
      world:CombatLog(COMBATLOG_SYSTEM, "You see some traps have been disarmed.")
    end
    return ret
  else
    return ent:CallOrigVtable(244,skill,target1,target2)
  end
end

--[[
Hook for Entity vtable 249 (Is skill allowed on entity)
Parameters are: 
  ent: Entity the skill is used on
  skill: String indicating which skill was used

We need to override this in order for Science skill to be allowed on the
bullhorn item (Science is not an allowed skill to use on traps by default). If the
item is bullhorn and skill is "science", then we return true to allow the use.
Otherwise, we return the result of calling the original 249 vtable function.
--]]

function Hook249(ent, skill)
  if ent:GetName() == "Bull Horn Trap" and skill == "science" then
    return true
  end
  return ent:CallOrigVtable(249,skill)
end

--[[
In OnStart, we use InstallVtableHook to install the new hooks for Entity vtable
244 and 249.  We only want the new function to take effect for "Trap" entity
types, as that is what the bullhorn object is.
--]]

function OnStart()
  hookexecutor:InstallVtableHook("Trap",244,Hook244)
  hookexecutor:InstallVtableHook("Trap",249,Hook249)
end