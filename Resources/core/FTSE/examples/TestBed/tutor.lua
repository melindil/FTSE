--- tutor.lua: TestBed example for the Dialogue module

--[[
   The Dialogue module will load conversation files via the Lua "require"
   command. This serves two purposes. First, it ensures the path traversal
   is correct (FTSE updates the Lua environment to follow the directory
   specified in -path at the command line, or the regular "core" directory
   if -path is not specified). Second, local functions can be defined
   here and used in the conversation table if desired.
--]]

-- Dialogue files always return the conversation table
return {

  --[[
     In many cases, an NPC may start from a different point in the dialogue tree
     based on the current player, NPC, or world condition. When this is needed,
     it is generally best to use a code node as start, so that it can redirect
     to the correct dialogue start point. Here, we redirect to a special path
     when the player is wearing the new Pipboy Costume item.
  --]]
  
  start={
    code=function(speech,conv,node,npc,plyr)
	  if plyr:GetEquippedItem(2):GetEntitySubType() == "PipboyCostume" then
	    return speech:RunNode(conv,"SawPipboy",npc,plyr)
	  else
	    -- Keep track of number of visits
		local visits = (npc:GetCustomValue("NumTalked") or 0)+1
		npc:SetCustomValue("NumTalked",visits)
		if visits > 1 then 
		    return speech:RunNode(conv,"ReturnedStart",npc,plyr)
		else
			return speech:RunNode(conv,"NormalStart",npc,plyr)
		end
	  end
	end
  },
  
  ------------------------------------------------------------------------------
  -- Pipboy Costume cases
  SawPipboy={
    text="Whoa, I guess I had better start laying off of the rotgut.",
	choices={
		-- First choice will trigger a reaction from the NPC.
		{
		  node="Scared",
		  text="BOO!",
		  priority=1
		},
		
		-- Second choice will exit conversation
		{
		  node="exit",
		  text="(Leave.)",
		  priority=99
		}
	}
	
  },
  
  Scared={
	-- Dies of fright
	code=function(speech,conv,node,npc,plyr)
	  npc:ApplyDamage(1000)
	  return speech:RunNode(conv,"Scared2",npc,plyr)
	end
  },
  
  Scared2={
	-- Give one last message before we get out of the conversation.
    text="WAAAHH!!",
	choices={
		{
			node="exit",
			text="{Whoops.}",
			priority=99
		}
	}
  },

  ------------------------------------------------------------------------------
  -- Normal conversation starts here
  NormalStart={
    text="Hello. I am Mr. Tutor. I am here to help guide you on how to use the Dialogue module.",
    choices={
      {
        node="WhatIs",
        text="What is the Dialogue module?",
        priority=5
      },
      {
        node="Require",
        text="How can I start using the Dialogue module?",
        priority=20
      },
	  -- Here, use a missionvar condition check to see if the player has completed a "quest"
      {
        node="QuestDone",
        text="I destroyed that practice dummy. Do I get anything?",
        condition=missionvar("DummyGone","T"),
        priority=90
      },
	  -- Similarly, offer an option that requires a Charisma score of 8 or higher
      {
        node="CharCheck",
        text="(Charisma) Can you tell me anything about yourself?",
        condition=skillcheck("charisma",8),
        priority=98
      },
      {
        node="PreExit",
        text="I don't have any more questions right now.",
        priority=99
      }
	}
  },
  
  WhatIs={
    text="The Dialogue module allows a mission or campaign creator to add interactive dialogue to NPC characters, like I am talking with you now.",
    choices={
      {
        node="Features",
        text="What features are in the Dialogue module?",
        priority=5
      }
    }
  },
  
  Require={
    text="To enable the module, use the Lua \"require\" command to load the module. The returned object should be assigned to a variable for use in calling member functions. It should look like the following:\n\n<b+>dlg=require \"FTSE.modules.Dialogue\"<b->",
    choices={
      {
        node="Register",
        text="What should I do after loading the module?",
        priority=15
      }
    }
  },
  
  Features={
    text="A dialogue conversation can contain both conversation nodes and code scripts. Conversation nodes present dialogue text from an NPC to the player, and provide choices for how the player can respond. Conversations can be nested, and conditions can be used to control if a player is allowed certain choices. Code scripts allow game world or character conditions to affect the dialogue, and allow the dialogue process to affect the world as well.",
    choices={
      {
        node="Dialogue",
        text="How does a dialogue node work?",
        priority=25
      },
      {
        node="Nested",
        text="What is a nested conversation?",
        priority=26
      },
      {
        node="Conditional",
        text="How can choices be limited by conditions?",
        priority=27
      },
      {
        node="Code",
        text="How do I use a code node?",
        priority=40
      }
    }
  },

  Register={
    text="A conversation is registered by calling the LoadConversation function within the Dialogue module object. The call takes two parameters. The first is a tag string to associate with the conversation, and the second is to specify the Lua file containing the conversation.\n\n<b+>dlg:LoadConversation(\"tutor\",\"FTSE.examples.TestBed.tutor\")<b->",
    choices={
      {
        node="Tag",
        text="What is a tag string, and how do I use it?",
        priority=16
      },
      {
        node="Lua",
        text="How do I specify the Lua file name?",
        priority=17
      }
    }
  },
  Dialogue={
    text="A dialogue node contains a field, named \"text\", with the value being the string of text the NPC will \"say\" when that node is reached. It may also contain an \"audio\" field with the name of the audio file to play. There is also a field, \"choices\", which is an array of Choice objects, containing any new conversation choices offered to the player. An example dialogue node called \"MyNode\" would look like this:\n\n<b+>MyNode={\n  text=\"Hi there!\",\n  choices={\n    {\n      node=\"exit\",\n      text=\"(Done.)\",\n      priority=1\n    }\n  }\n}<b->",
    choices={
      {
        node="Choice",
        text="How do choices work?",
        priority=25
      }
    }
  },
  Choice={
    text="A choice is an option given to the player for them to interact with the conversation. Each dialogue node can add (or remove) options to let the conversation develop. An added choice will stay in the list until it is selected (at which point it is removed), or until it is specifically removed.\n\nChoices contain a \"node\" field indicating which node should run if the player selects the choice, a \"text\" field containing the player's \"words\" for that choice, and a \"priority\" value indicating how choices should be ordered for the player (lowest value first).",
    choices={ }
  },
  
  -- Use a nested conversation to describe nested conversations
  Nested={
    text="In some cases, a particular topic may deserve further focus by the player. In this case, the mod writer may want to limit player choices to that topic, and only return to the original topics once they are done with the new one. A nested conversation facilitates this.",
    pushstack=true,
    choices={
      {
        node="Nested1",
        text="How do I start a nested conversation?",
        priority=1
      },
      {
        node="Nested2",
        text="How do I complete a nested conversation and go back to the original choices?",
        priority=2
      },
      {
        node="NestedDone",
        text="I'm finished talking about nested conversations, can we go back to something else?",
        priority=3
      }
    }
  },
  Nested1={
    text="For the node which starts the nested conversation, include the field \"pushstack\", with a value of true. The current set of choices available to the player will be pushed onto a saved stack, and a new empty choice list will be created. The node which starts the nested conversation will need to add new choices for the new topic.",
    choices={ }
  },
  Nested2={
    text="The node which exits the nested conversation should include the field \"popstack\", with a value of true. All curtent choices will be discarded, and the saved choice list at the top of the saved stack will be restored.",
    choices={ }
  },
  NestedDone={
    text="Very well. What else do you need?",
    popstack=true,
    choices={ }
  },
  
  Conditional={
    text="Each choice has an optional field named \"condition\". There are built-in shortcuts for skill checks and mission and campaign variable checks. Otherwise, the field can be set to a Lua function that is evaluated when the choice is processed - if the function returns true, then the choice is included.",
    choices={ }
  },
  
  Code={
    text="A code node contains a single field named \"code\". This field is set to a Lua function containing the desired script functionality. The script can do any Lua commands normally done by a hook function, and should eventually return with a call to the RunNode function of the Dialogue module, so that the conversation can continue from there.",
    choices={ }
  },
  Lua={
    text="The file name should be in the same format as a Lua \"require\" statement, using \".\" to separate paths. A good practice would be to include conversation Lua files in the project's \"FTSE\\Dialogue\" directory; in that case, the format for a conversation file named MyNPC.lua would be \"FTSE.Dialogue.MyNPC\".",
    choices={ }
  },
  Tag={
    text="The tag string is a creator-set name for a particular conversation. This allows the conversation to be tied to a speech action in-game. \n\nTo assign the conversation to an NPC, in the level editor, go to the \"Speech...\" button under the Level tab. Add a new Speech entry, with a name matching the tag string, and ensure the \"Windowed\" option is selected. Then, go to the desired Actor, and set its Click Speech to that new string.",
    choices={ }
  },
  
  QuestDone={
    code=function(speech,conv,node,npc,player)
	  if npc:GetCustomValue("Given") == nil then
		player:AddXP(5000000)
		npc:SetCustomValue("Given",true)
		return speech:RunNode(conv,"QuestDone2",npc,player)
	  end
	  return speech:RunNode(conv,"QuestDoneAlready",npc,player)
    end
  },
  QuestDone2={
    text="I grant you the wisdom of the Whachutu.",
    choices={ }
  },
  QuestDoneAlready={
    text="Greed is not a virtue of the Whachutu.",
    choices={ }
  },
  CharCheck={
    text="Well, I'm not all that interesting. Some people say I do a good Ron Perlman impression, but I don't really think so.",
    pushstack=true,
    choices={
      {
        node="CharCheck2",
        text="Really? Can I hear it?",
        priority=1
      }
    }
  },
  CharCheck2={
    text="War. War never changes.",
    audio="FTSE/examples/TestBed/tutor.mp3",
    choices={
      {
        node="CharCheckAsk",
        text="How do you do that?",
        priority=1
      },
      {
        node="CharCheckDone",
        text="Yeah... I don't know about that.",
        priority=2
      }
    }
  },
  CharCheckAsk={
    text="It's simple. In the dialogue node, add the field \"audio\", and give it a sound filename. That sound will be played when the node is reached.",
    popstack=true,
    choices={ }
  },  
  CharCheckDone={
    text="You're right, never mind.",
    popstack=true,
    choices={ }
  },  
  ------------------------------------------------------------------------------
  -- Second and later conversation starts here
  ReturnedStart={
    text="Welcome back. Is there more I can help you with?",
    choices={
      {
        node="WhatIs",
        text="Can you describe the Dialogue module?",
        priority=5
      },
      {
        node="Require",
        text="How can I start using the Dialogue module?",
        priority=20
      },
      {
        node="QuestDone",
        text="I destroyed that practice dummy. Do I get anything?",
        condition=missionvar("DummyGone","T"),
        priority=90
      },
      {
        node="CharCheck",
        text="(Charisma) Can you tell me anything about yourself?",
        condition=skillcheck("charisma",8),
        priority=98
      },
      {
        node="PreExit",
        text="I don't have any more questions right now.",
        priority=99
      }
	}
  },
  
  ------------------------------------------------------------------------------
  -- Exit the conversation
  
  -- You can use a "pre-exit" node like this to have the NPC say one final thing
  -- before gameplay continues.
  -- The "clear" optional field will remove all other conversation options so that
  -- the player's only choice is to select the final "exit" choice.
  PreExit={
    text="Very well. Please return if you need my help with anything else.",
	clear=true,
	choices={
	  {
	    node="exit",
		text="(Leave.)",
		priority=1
	  }
	}
  }
}