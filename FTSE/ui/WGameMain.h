#pragma once
#include "WWindow.h"
#pragma pack (push,1)
class WGameMain :
	public WWindow
{
public:
	WGameMain() {}
	virtual ~WGameMain() {}
	WGameMain(void* ptr) : WWindow(ptr) {}

	typedef struct
	{
		WWindow::wwindow_header hdr_;
		void* worldhandler_vtable;
		void* client;
		void* gameworld;
		char unk[0x30];
		void* speech_ptr_;
		char unk2[0x12f];
		void* this_for_47b340;
		void* unk3;
		void* param_for_47b340;
		void* unk4;
	} wgamemain_header;

	void SetSpeechMember(WWindow const& ptr);
	void SetWindowAlign(WWindow const& ptr, int horiz, int vert);

	wgamemain_header* GetGameMain() { return (wgamemain_header*)ptr_; }
};

#pragma pack(pop)