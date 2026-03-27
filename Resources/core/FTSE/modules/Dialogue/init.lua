local speech = {}
speech.conversations = {}
speech.text=""
speech.audio=""
speech.choices={}
speech.choicestack={}

function speech:RunNode(in_conversation,in_node,npc,player)
	if in_node == "start" then 
		self.choices={}
	else
		self.choices[in_node] = nil
	end
--	logger:log("RunNode " .. tostring(in_conversation) .. ":" .. tostring(in_node))
--	logger:log("npc " .. EntString(npc) .. " player " .. EntString(player))
	local conversation = self.conversations[in_conversation]
	if conversation == nil then return "exit" end
	if in_node == "exit" then return "exit" end
	if in_node == "barter" then return "barter" end
	if in_node == "gamble" then return "gamble" end
	local node = conversation[in_node]
	if node == nil then return "exit" end
	
	if node["code"] ~= nil then
		return node["code"](self, in_conversation,in_node,npc,player)
	end
	
	if node["text"] ~= nil then
		self.text = node["text"]
	else
		self.text = ""
	end
	
	if node["audio"] ~= nil then
		self.audio = node["audio"]
	else
		self.audio=""
	end
	
	if node["pushstack"] then
	  table.insert(self.choicestack, self.choices)
	  self.choices = {}
	elseif node["popstack"] and #self.choicestack > 0 then
	  self.choices = table.remove(self.choicestack)
	elseif node["clear"] then
	  self.choices = {}
	  logger:log("Clear fires here")
	end
	
	if node["choices"] ~= nil then
		for _,choice_node in ipairs(node["choices"]) do
		    if choice_node["condition"] ~= nil then
			  local cond_result = choice_node["condition"](in_conversation, in_node, npc, player)
			  if cond_result == false then goto continue end
			end
			if choice_node["remove"] then 
				self.choices[choice_node["node"]] = nil
			else
				local new_choice = {}
                                new_choice["text"]=string.gsub(choice_node["text"],"<player>",player:GetName())
				new_choice["text"]=string.gsub(new_choice["text"],"<rank>",player:CallVtable(208))
                                new_choice["priority"] = choice_node["priority"]
                                new_choice["node"] = choice_node["node"]
				logger:log("Adding choice " .. choice_node["node"])
				self.choices[choice_node["node"]] = new_choice
			end
			::continue::
		end
	end
	
	return speech
	
end

local function skillcheck(param,value)
  local function ret (conv,node,npc,player)
    return player:GetAttribute(param,ACTOR_TABLE_CURRENT) >= value
  end
  return ret
end
local function skillnotmet(param,value)
  local function ret (conv,node,npc,player)
    return player:GetAttribute(param,ACTOR_TABLE_CURRENT) < value
  end
  return ret
end
local function campaignvar(param,value)
  local function ret (conv,node,npc,player)
    return world:GetCampaignVar(param) == value
  end
  return ret
end
local function missionvar(param,value)
  local function ret (conv,node,npc,player)
    return world:GetMissionVar(param) == value
  end
  return ret
end

function speech:LoadConversation(tag,fil)
  local saved = {}
  saved["skillcheck"] = _G["skillcheck"]
  saved["skillnotmet"] = _G["skillnotmet"]
  saved["campaignvar"] = _G["campaignvar"]
  saved["missionvar"] = _G["missionvar"]
  _G["skillcheck"] = skillcheck
  _G["skillnotmet"] = skillnotmet
  _G["campaignvar"] = campaignvar
  _G["missionvar"] = missionvar
  speech.conversations[tag] = require(fil)
  _G["skillcheck"] = saved["skillcheck"]
  _G["skillnotmet"] = saved["skillnotmet"]
  _G["campaignvar"] = saved["campaignvar"]
  _G["missionvar"] = saved["missionvar"]
  
end

local function FTSE_OnSpeechHook(p,n,conv)
--    logger:log("Speech conversation " .. conv)
    if speech.conversations[conv] ~= nil then
      return speech
    end
    return false
end

hookexecutor:InstallHook("OnSpeechHook",FTSE_OnSpeechHook)

return speech