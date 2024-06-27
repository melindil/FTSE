# EXE Patches

This section describes the patches included with FTSE by default.

## Sniper fix

In Fallout: Tactics 1.27, the Sniper perk behavior is broken. Due to a typo in the game's source, the extra roll is made as if the attacker's Luck stat is 0.  This results in the extra Sniper check nearly always failing.

With the fix, the luck roll is properly checked against the attacker's Luck stat.

Note that the game only performs a check of the Sniper perk for weapons that have range type of Long or Scoped.  This covers the sniper rifle type weapons, the M1 Garand, the laser rifle, and the gauss rifle. All other weapons use the normal critical hit checking, without the bonus Luck roll.

## Music blocked fix

When a conversation or briefing window is displayed during gameplay, music is normally paused. THis patch will allow music to continue during the conversation.

## Tag name fix

Due to a game engine bug, the tag names for some entities will be removed on spawn.  This fix corrects that behavior.

## Team Player / Loner fix

In Fallout: Tactics 1.27, the Team Player and Loner perks are broken.  Loner is always applied, and Team Player never is.

With the fix, both perks work as intended.  Team player applies whenever any teammate is in range, and Loner only applies when not.

## Color character portraits

For some mods, it was desired to use full-color character portraits, instead of the green-scale portraits used in the original game. With this patch, the character portraits show in the colors defined in the graphics file, not converted to green.

## High res patch

This is a patch that was included on the NMA forums, to enable higher resolution with Fallout: Tactics to work correctly.

## Encounter color fix

This patch fixes some color issues with entities that spawn in random encounters.

## Fix melee and unarmed crit tables

In Fallout: Tactics 1.27, the critical effect tables for melee and unarmed attacks do not work correctly.  Certain effects are applied far more often than intended, the most noticeable of which is a high chance of a devastating hit when using unarmed attacks on left legs.

With this fix, the check into the melee and unarmed critical effects tables is fixed.

## Fix Bonus H2H Damage perks

In Fallout: Tactics 1.27, the game mistakenly gives the 15% damage bonus intended for the Bonus H2H Damage perk, to the Bonus H2H Attacks perk.

With this fix, the 15% bonus applies to the Bonus H2H Damage perk as intended.

## Special encounter crash fix

In Fallout: Tactics 1.27, there is a chance of the game crashing on the world map if a special encounter is triggered after all special encounters have been visited.

With this fix, the game will not crash, and will instead correctly load a random encounter once all special encounters have been visited.

## Radiation Resistance fix

In Fallout: Tactics 1.27, radiation resistance has zero effect unless it is 100% or greater.

With this fix, radiation resistance works as expected/documented.

## Poison Resistance fix

In Fallout: Tactics 1.27, poison resistance has zero effect unless it is 100% or greater.

With this fix, poison resistance works as expected/documented.

## Fix non-random injury on crits

In Fallout: Tactics 1.27, critical hits can cause injury even for non-aimed attacks. When this happens, a random roll is performed to see which body part is injured. Due to a bug, the selection was unintentionally biased - torso is 2x as likely to be selected, and right leg is never selected.

With this fix, the random selection of body part for a non-aimed critical is equally weighted as (presumably) intended.

## Old burst bug fix

This patch is an older version of the burst bug fix, as posted on the NMA forums. The bug involves a burst attack at a far-away target having a high chance of hitting anything standing near that target.

With the new scripts in ftse_base.lua, a better patch for the burst attack bug was provided, so this fix is obsolete.

## doNightPerson fix

This patch is from the NMA forums, and states that it will help the Night Person trait work.  Testing has still shown spotty results, and the behavior of the patch has not yet been confirmed.

## No starting inventory

This patch is used by some mods, to remove the items that the player starts with at the beginning of a campaign.
