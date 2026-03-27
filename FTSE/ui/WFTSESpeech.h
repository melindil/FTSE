#pragma once

#include "CustomUIClass.h"

#include "WGameMain.h"

#include "EntityID.h"

#include "Helpers.h"

#include "Logger.h"

class WTextList;
class WFTSESpeechTextList;

struct lua_State;

class WFTSESpeech : public WWindow
{
public:
	WFTSESpeech();
	virtual ~WFTSESpeech();
	WFTSESpeech(void* ptr, std::string const& dialogue, EntityID player, EntityID npc, lua_State* lua);

	void RunNode(std::string const& nodename);

	void SetControls(std::shared_ptr<WTextList> npc, std::shared_ptr<WFTSESpeechTextList> player);

	size_t GetVtableAddr();

	void CloseWindow(std::string const& cmd);

	static std::shared_ptr<WFTSESpeech> Construct(std::string const& conversation, EntityID player, EntityID npc, lua_State* lua);
	static std::shared_ptr<WFTSESpeech> GetInstance(void* ptr);

#pragma pack (push,1)
	typedef struct
	{
		WWindow::wwindow_header hdr_;
		void* imagezar;
		uint16_t unk1;
		EntityID player_ent;
		EntityID npc_ent;
		EntityID some_entity;
		WWindow::wwindow_header* barter_button;
		WWindow::wwindow_header* gamble_button;
		WWindow::wwindow_header* recruit_button;
		WWindow::wwindow_header* text_list;
		Vector3 view_loc;
		int32_t unk2[8];
		WWindow::wwindow_header* portrait;

	} wftsespeech_struct;
#pragma pack (pop)

private:
	
	// Make this a static
	static CustomUIClass<WFTSESpeech> customclass_;

	// Which dialogue tree are we running?
	std::string conversation_;
	
	// Entities in the conversation
	EntityID player_;
	EntityID npc_;

	// saved controls
	std::shared_ptr<WTextList> npc_text_;
	std::shared_ptr<WFTSESpeechTextList> player_text_;

	// Hold LUA state
	lua_State* lua_;

	//Logger log_;

};

