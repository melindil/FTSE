#pragma once
#pragma pack (push,1)

#include <stdint.h>
#include <memory>

#include "DefaultStyle.h"

class WWindow
{
public:
	WWindow();
	virtual ~WWindow();
	WWindow(void*);

	struct ScreenCoord
	{
		int32_t x;
		int32_t y;
	};

	struct WWindowRect
	{
		ScreenCoord topleft;
		ScreenCoord bottomright;
	};
protected:
	typedef struct
	{
		uint32_t vtable_wwindow;
		uint8_t unk1;
		uint8_t unused[3];
		void* event_handler_list;
		int32_t event_handler_count;
		uint32_t vtable_eventhandler;
		WWindow* parent;
		WWindow* first_child;
		WWindow* last_child;
		WWindow* prev_sibling;
		WWindow* next_sibling;
		int32_t unk5;
		wchar_t* unk6;
		wchar_t* unk7;
		uint8_t unk8[7];
		int32_t unk_struct[30];
		int32_t unk9;
		WWindow* manager;
		ScreenCoord last_mouse;
		WWindowRect border;
		int32_t unk11;
		DefaultStyle::DataStruct* default_style_ptr;
		void* font_ptr;
		float unk12[3];
		wchar_t* unk13;
		int32_t unk14;
		WWindow* background_maybe;
		uint8_t unk15_flags[3];

	} wwindow_header;

	wwindow_header* ptr_;

	static const size_t GBL_MENUMANAGER = 0x8bddb8;

public:
	void HandleEvent(int32_t bogus);

	static std::shared_ptr<WWindow> GetGblMenuManager();

	wwindow_header* Get() const { return ptr_; }

};
#pragma pack (pop)

