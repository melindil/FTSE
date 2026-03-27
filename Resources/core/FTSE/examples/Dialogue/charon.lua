-- This is the original dialogue engine demo for Charon (Village Elder) on mission 1.

return {
	start={
		code=function(speech, conversation, node, npc, player)
			if world:GetMissionVar("M01_Raider_Leader_Dead") == "T" then
				if world:GetMissionVar("M01_Tribals_Attacked") == "T" then
					return speech:RunNode(conversation,"End_Partial_1",npc,player)
				else
					return speech:RunNode(conversation,"End_Success_1",npc,player)
				end
			else 
                if world:GetMissionVar("M01_Charon_Talk") == "T" then
                    return speech:RunNode(conversation,"Repeat_Visit_1",npc,player)
                else
                    world:SetMissionVar("M01_Charon_Talk","T")
                    return speech:RunNode(conversation,"First_Visit_1",npc,player)
                end
			end
		end
	},
	First_Visit_1={
		text = "Greetings to the Brotherhood of Steel. You are speaking with Charon, Elder and tribal Father to Brahmin Wood.",
		audio = "locale/missions/mission01/Charon_First_Visit_1.mp3",
		choices = {
			{ 
				node = "Greet",
				text = "Greetings to you, I am <rank> <player>. I have been sent here to help you.",
				priority = 1
			},
			{
				node = "Demand",
				text = "Uh huh. Look, the Brotherhood expects to be well compensated for our help.",
				priority = 2
			},
			{
				node = "Special",
				text = "Is there anyplace to trade or gamble here?",
				priority=3
			},
			{
				node = "exit",
				text = "I don't have time for this. Get lost.",
				priority = 99
			}
		}
	},
	Special = {
	    text = "Yes, what would you like to do?",
		pushstack = true,
		choices = { 
			{
				node = "gamble",
				text = "Hey, how about a quick game?",
				priority=1
			},
			{
				node = "barter",
				text = "Can I trade for anything?",
				priority = 2
			},
			{
				node = "Special2",
				text = "Can we go back to the previous discussion?",
				priority = 3
			}

		}
	},
	Special2 = {
	    text = "Yes, what do you need?",
		popstack = true,
		choices = {
			{
				node = "Special",
				text = "Is there anyplace to trade or gamble here?",
				priority=3
			}
		}
	},
	Greet = {
		text = "You have arrived none too soon. Our village has fallen to raiders who rode out of the darkness. Some of my people escaped death or bondage by fleeing into the wasteland, but without tools and shelter. This must be the doing of the Brahmin God of Fate.",
		audio = "locale/missions/mission01/Charon_Greet.mp3",
		choices = {
			{
				node = "Help",
				text = "Where are your people now?",
				priority = 2
			},
			{
				node = "Demand_soft",
				text = "Do not worry, we can handle the raiders. However, our leadership expects to be compensated for our help. We need new recruits to help with our growing borders.",
				priority = 1
			},
			{
				node = "Demand",
				remove = true
			},
			{
				node = "exit",
				text = "Understood. We will clear out the raiders and protect your people. Wait here until we return.",
				priority = 99
            }
		}
	},
	Demand = {
		text = "With the bandits we trade away our present, in the Brotherhood we trade away our future. *Sigh*... very well Initiate, I fully agree to your terms, but I fear this decision will haunt me till I reach the Eternal Grazing Field in the heavens.",
		audio = "locale/missions/mission01/Charon_Demand.mp3",
		choices = {
			{
				node = "Greet",
				remove = true
			},
			{
				node = "Help",
				text = "Yeah, yeah. So what do we gotta do?",
				priority = 1
			},
			{
				node = "exit",
				text = "Just stay out of our way. (Leave.)",
				priority=99
			}
		}
	},
	Demand_soft = {
		text = "With the bandits we trade away our present, in the Brotherhood we trade away our future. *Sigh*... very well Initiate, I fully agree to your terms, but I fear this decision will haunt me till I reach the Eternal Grazing Field in the heavens.",
		audio = "locale/missions/mission01/Charon_Demand.mp3",
		choices = {
		}
	},
	Help = {
		text = "Our villagers are being held prisoner in the camp. Save them, and you will have more people to aid your cause. Lose them, and you will be cutting Brahmin from your own herd.",
		audio = "locale/missions/mission01/Charon_Help.mp3",
		choices = {}
	},
	
	Repeat_Visit_1 = {
		text = "Please hurry. My people are in grave danger.",
		choices = {
			{
				node = "exit",
				text = "(Leave.)",
				priority = 1
			}
		}
	},
	
	End_Success_1 = {
		text = "Hah hah hah. Your grace in battle is wondrous to behold Initiate. The death screams of the bandits reminded me of angels singing. They will think twice before attacking this village again.",
		audio = "locale/missions/mission01/Charon_End_Success_1.mp3",
		choices = {
			{
				node = "Recruits",
				text = "The Brotherhood of Steel keeps its word. Do you keep yours?",
				priority = 1
			},
			{
				node = "Reward",
				text = "Is there any reward for the squad who saved your people?",
				priority = 2
			},
			{
				node = "exit",
				text = "Until we meet again, elder.",
				priority = 99
			}
		}
	},
	Recruits = {
		text = "Of course. All the recruits will be delivered to your base as agreed. Well done. You have our thanks.",
		audio = "locale/missions/mission01/Charon_Recruits.mp3",
		choices = {}
	},
	Reward = {
		code = function(speech, conversation, node, npc, player)
			local ent
			for _,it in pairs(npc:GetInventory()) do
				if it:GetName() == "Detonator" then
					local it2 = npc:RemoveInventory(it,1)
					player:AddToInventory(it2,1)
					return speech:RunNode(conversation,"Reward_given",npc,player)
				end
			end
			return speech:RunNode(conversation,"Reward_already",npc,player)
		end
	},
	
	Reward_given = {
		text = "Initiate, you defended my people as fiercely as a Deathclaw protects her cubs. Please accept this ancient gift from us. You are now considered family in my humble village.",
		audio = "locale/missions/mission01/Charon_Reward.mp3",
		choices = {}
	},
	Reward_already = {
		text = "You have our precious gift already. We can not part with any more.",
		choices = {}
	},
	
	End_Partial_1 = {
		text = "The raiders have been dealt a heavy blow. They will need weeks to fill their ranks. While our losses are smaller, they are infinitely graver. Only much time and \"hiding the spear\" will replenish our ranks.",
		audio = "locale/missions/mission01/Charon_End_Partial_1.mp3",
		choices = {
			{
				node = "Recruits_2",
				text = "We did what we could for your people. Will you still provide recruits for the Brotherhood?",
				priority = 1
			},
			{
				node = "Reward_2",
				text = "We did save many of your people. Isn't that worth some reward?",
				priority = 2
			},
			{
				node = "exit",
				text = "Until we meet again, elder.",
				priority = 99
			}
		}
	},
	Recruits_2 = {
		text = "You will get only half the agreed recruits. You have our ... thanks.",
		audio = "locale/missions/mission01/Charon_Recruits_2.mp3",
		choices = {}
	},
	Reward_2 = {
		text = "I am thankful many of my people survived, Initiate, but no reward can be offered. We must preserve what little we have left if we are to survive this tragedy.",
		choices = {}
	}
		
}
