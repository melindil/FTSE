#include "WFTSESpeech.h"

#include "Helpers.h"
#include "FOTString.h"

#include "WPortrait.h"
#include "WTextList.h"
#include "WFTSESpeechTextList.h"
#include "WBOSButton.h"

#include "World.h"
#include "LuaHelper.h"

//static 
CustomUIClass<WFTSESpeech> WFTSESpeech::customclass_("WFTSESpeech", 0x807d2c, 98);

WFTSESpeech::WFTSESpeech() //: log_("ftse_speech.txt")
{
}


WFTSESpeech::~WFTSESpeech()
{
	//log_ << "Destructor fired!" << std::endl;
}

WFTSESpeech::WFTSESpeech(void* ptr, std::string const& conversation, EntityID player, EntityID npc, lua_State* lua)
	: WWindow(ptr), conversation_(conversation), player_(player),npc_(npc),lua_(lua)//, log_("ftse_speech.txt")
{

}

size_t WFTSESpeech::GetVtableAddr()
{
	return customclass_.GetVtableAddr();
}

// static
std::shared_ptr<WFTSESpeech> WFTSESpeech::Construct(std::string const& conversation, EntityID player, EntityID npc, lua_State* lua)
{
	std::shared_ptr<WGameMain> parent = World::GetWGameMain();	

	wftsespeech_struct* new_window = (wftsespeech_struct*)Helpers::FOTHeapAlloc(0x14c);

	FOTString picfile("FTSE/modules/Dialogue/speechftse");
	wchar_t* ptr_to_string = picfile.getraw();
	WWindow::ScreenCoord topleft{ 0,0 };
	auto create_pic = (void(__thiscall *)(void*, void*, const WWindow::ScreenCoord*, wchar_t**))0x718a40;
	(*create_pic)(new_window, parent->Get(), &topleft, &ptr_to_string);

	WWindow::wwindow_header* hdr = reinterpret_cast<WWindow::wwindow_header*>(new_window);
	hdr->vtable_wwindow = customclass_.GetVtableAddr();
	hdr->vtable_eventhandler = customclass_.GetEventVtableAddr();

	// Init some flags and struct members. Not doing this results in a random crash.
	hdr->unk15_flags[2] |= 8;
	new_window->unk2[0] = *(char*)(parent->Get());
	new_window->unk2[1] = 0;
	new_window->unk2[2] = 0;
	new_window->unk2[3] = 0;
	new_window->unk2[4] = (int)parent->Get();
	new_window->unk2[5] = 0;
	new_window->unk2[6] = 0;
	new_window->unk2[7] = 0;

	new_window->player_ent = player;
	new_window->npc_ent = npc;

	std::shared_ptr<WFTSESpeech> ret = std::make_shared<WFTSESpeech>(new_window, conversation, player, npc, lua);
	customclass_.AddInstance(new_window, ret);

	//WBOSButton::Create(*ret, "DONE", WWindow::ScreenCoord{0x30b,0x50}, 0x4cffd0, 2);

	WPortrait::Create(*ret, WWindowRect{ {0x91,0xc},{0x91 + 0x4b,0xc + 100} }, npc);
	WPortrait::Create(*ret, WWindowRect{ {0x91,0xc+128},{0x91 + 0x4b,0xc + 228} }, player);

	size_t* defaultstyle_param18 = (size_t*)((char*)parent->Get()->default_style_ptr + 0x18);	// This is "AntiFont" type

	auto txtlist1 = WTextList::Create(*ret, WWindowRect{ {0x119,0x15},{0x2e9,0x75} }, *defaultstyle_param18);
	auto txtlist2 = WFTSESpeechTextList::Create(ret, WWindowRect{ {0x119,0x15+128},{0x2e9,0x75+128} }, *defaultstyle_param18);
	ret->SetControls(txtlist1, txtlist2);

	// Pause the world
	auto world_ptr = World::GetGlobal();
	auto force_pause_fxn = (void(__thiscall *)(void*, int))0x4fc800;
	(*force_pause_fxn)(world_ptr, 1);

	// Save prior view location and set focus location
	new_window->view_loc.v[0] = world_ptr->screen_center_loc.v[0];
	new_window->view_loc.v[1] = world_ptr->screen_center_loc.v[1];
	new_window->view_loc.v[2] = world_ptr->screen_center_loc.v[2];

	if (((char*)parent->GetGameMain()->gameworld)[0x193] == 0)
	{
		world_ptr->focus_object_loc.v[0] = world_ptr->screen_center_loc.v[0];
		world_ptr->focus_object_loc.v[1] = world_ptr->screen_center_loc.v[1];
		world_ptr->focus_object_loc.v[2] = world_ptr->screen_center_loc.v[2];
	}
	else
	{
		float* ptr = (float*)(((char*)parent->GetGameMain()->gameworld) + 0x16b);
		world_ptr->focus_object_loc.v[0] = ptr[0];
		world_ptr->focus_object_loc.v[1] = ptr[1];
		world_ptr->focus_object_loc.v[2] = ptr[2];

	}

	// Something related to the "player" list. TODO to find exactly what this is for.
	// Without these steps we crash outside the debugger.

	auto get_player_obj = (char* (__thiscall *)(void*))0x65af20;
	char* playerobj = (*get_player_obj)(parent->GetGameMain()->gameworld);
	if (playerobj != NULL)
	{
		new_window->some_entity = *(EntityID*)(((char*)playerobj) + 0x141);
		auto set_something = (void(__thiscall *)(void*, char*))0x4de360;
		(*set_something)(&new_window->unk2, playerobj);
	}

	// Steps done by 0x471ad0
	parent->SetSpeechMember(*ret);
	parent->SetWindowAlign(*ret, 2, 1);

	// Steps done by 0x477120 - scrolls game window to NPC
	auto call1 = (void(__thiscall *)(void*, EntityID*))0x49f920;
	(*call1)(parent->GetGameMain()->gameworld, &npc);
	auto call2 = (void(__thiscall *)(void*, EntityID*))0x49fa10;
	(*call2)(parent->GetGameMain()->gameworld, &npc);

	ret->RunNode("start");

	return ret;
}

std::shared_ptr<WFTSESpeech> WFTSESpeech::GetInstance(void * ptr)
{
	return customclass_.GetInstance(ptr);
}

void WFTSESpeech::RunNode(std::string const& nodename)
{
	//(log_) << "Running speech node " << conversation_ << ":" << nodename << std::endl;
	lua_getfield(lua_, LUA_REGISTRYINDEX, "SpeechObject");
	if (lua_istable(lua_, -1))
	{
		lua_getfield(lua_, -1, "RunNode");
		lua_remove(lua_, -2);
		lua_getfield(lua_, LUA_REGISTRYINDEX, "SpeechObject");
		lua_pushstring(lua_, conversation_.c_str());
		lua_pushstring(lua_, nodename.c_str());
		Entity::GetEntityByID(npc_)->MakeLuaObject(lua_);
		Entity::GetEntityByID(player_)->MakeLuaObject(lua_);
		if (lua_pcall(lua_, 5, 1, 0) == LUA_ERRRUN)
		{
			npc_text_->SetText(lua_tostring(lua_, -1),0);
			lua_pop(lua_, 1);
		}
		else
		{
			if (lua_istable(lua_, -1))
			{
				std::string txt = LuaHelper::GetTableString(lua_, -1, "text");
				npc_text_->SetText(txt,0);

				std::string audio = LuaHelper::GetTableString(lua_, -1, "audio");
				if (audio != "")
				{
					void* soundmanager = (void*)(0x8be368);
					auto stop_audio_fxn = (void(__thiscall *)(void*, int))0x71dc40;
					(*stop_audio_fxn)(soundmanager, 77777);
					auto start_audio_fxn = (void(__thiscall *)(void*, int, wchar_t**, int, int))0x71ccc0;
					FOTString audiofile(audio);
					wchar_t* widechar = audiofile.getraw();
					(*start_audio_fxn)(soundmanager, 77777, &widechar, 0, 1);
				}

				lua_getfield(lua_, -1, "choices");
				player_text_->ClearItems();

				struct Ordering{
					std::string node;
					std::string text;
					int priority;

					Ordering(std::string const&&n, std::string const&&t, int p) : node(n), text(t), priority(p) {}
				};
				std::vector<std::shared_ptr<Ordering>> choices;

				lua_pushnil(lua_);
				while (lua_next(lua_, -2))
				{
					auto new_ordering = std::make_shared<Ordering>(
						LuaHelper::GetTableString(lua_, -1, "node"),
						LuaHelper::GetTableString(lua_, -1, "text"),
						LuaHelper::GetTableInteger(lua_, -1, "priority"));
					//log_ << "Choice " << new_ordering->node << " = " << new_ordering->text << " prio " << new_ordering->priority << std::endl;
					std::vector<std::shared_ptr<Ordering>>::iterator it;
					for (it = choices.begin(); it != choices.end(); it++)
					{
						if ((*it)->priority > new_ordering->priority) break;
					}
					choices.insert(it, new_ordering);
					lua_pop(lua_, 1);
				}
				for (auto& it : choices)
				{
					player_text_->InsertItem(it->text, it->node);
				}
				player_text_->FinalizeItems();
				lua_pop(lua_, 2);
			}
			else if (lua_isstring(lua_, -1))
			{
				std::string cmd = lua_tostring(lua_, -1);
				lua_pop(lua_, -1);
				if (cmd == "exit" || cmd == "barter" || cmd == "gamble")
				{
					CloseWindow(cmd);
				}
			}
			else
			{
				npc_text_->SetText("Weird return type from RunNode", 0);
				lua_pop(lua_, 1);
			}
		}

	}
	else
	{
		npc_text_->SetText("Oops, something didn't load right!",0);
	}

}

void WFTSESpeech::SetControls(std::shared_ptr<WTextList> npc, std::shared_ptr<WFTSESpeechTextList> player)
{
	npc_text_ = npc;
	player_text_ = player;
}

void WFTSESpeech::CloseWindow(std::string const& cmd)
{
	void(__thiscall *fxn)(void*, int);
	if (cmd == "gamble")
		fxn = (void(__thiscall *)(void*, int))0x4d0090;
	else if (cmd == "barter")
		fxn = (void(__thiscall *)(void*, int))0x4d0000;
	else
		fxn = (void(__thiscall *)(void*, int))0x4cffd0;
	(*fxn)(ptr_, 0);
	player_text_->Destruct();
	player_text_.reset();
	customclass_.RemoveInstance(ptr_);
}