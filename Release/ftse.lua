function OnStart ()

    logger:log("Entered OnStart")

    newperk = {name = "hulkSmash",
               minlevel = 16,
               maxperktaken = 1,
               requiredstat1 = "doctor",
               requiredamt1 = 50,
               requiredEN = 5
              }
    hookexecutor:ReplacePerk(newperk, 18)
end

function DefaultStyleChanges(style)
--  style:SetColorDefaultText(0.9,0.6,0.0)
--  style:SetColorHighlightText(1.0,1.0,0.0)
--  style:SetColorOptionsPages(1.0,1.0,1.0)
--  style:SetColorPanelTitles(1.0,1.0,1.0)
--  style:SetColorBuffs(0.0,0.2,1.0)
--  style:SetColorDebuffs(0.0,1.0,0.0)
--  style:SetColorTags(0.45,0.3,0)
end

function OnRadiated(e)
    if e:HasPerk("hulkSmash") then
      if e:GetTempPerkValue("hulkSmash") == 0 then
        logger:log(e["name"] .. " gains hulkSmash")
        e:DisplayMessage("<Cg>HULK SMASH!")
        bonus = {strength=2, radiationResist=75}
        e:ApplyTempBonus(bonus)
      end
      e:SetTempPerkValue("hulkSmash",6)	-- # of big ticks to keep buff
    end
end

function OnLongTick(e)
    curr = e:GetTempPerkValue("hulkSmash")
    if curr > 0 then
      curr = curr - 1
      if curr == 0 then
        logger:log(e["name"] .. " hulkSmash expired")
        e:DisplayMessage("Normal")
        bonus = {strength=2, radiationResist=75}
        e:RemoveTempBonus(bonus)
      end
      e:SetTempPerkValue("hulkSmash",curr)
    end
end