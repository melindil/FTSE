#include "WGameMain.h"



void WGameMain::SetSpeechMember(WWindow const& ptr)
{
	wgamemain_header* myptr = reinterpret_cast<wgamemain_header*>(ptr_);
	myptr->speech_ptr_ = ptr.Get();
}

void WGameMain::SetWindowAlign(WWindow const& ptr, int horiz_align, int vert_align)
{
	wgamemain_header* myptr = reinterpret_cast<wgamemain_header*>(ptr_);
	struct {
		void* new_window;
		int32_t unk1;
		int32_t unk2;
		int32_t horiz_align;
		int32_t vert_align;
		uint8_t unk3;
	} window_init_struct;
	window_init_struct.new_window = ptr.Get();
	window_init_struct.unk1 = 0;
	window_init_struct.unk2 = 0;
	window_init_struct.unk3 = 0;
	window_init_struct.horiz_align = horiz_align;
	window_init_struct.vert_align = vert_align;

	auto fxn = (void(__thiscall*)(void*, void*, void*))(0x47b340);
	(*fxn)(&myptr->this_for_47b340, myptr->param_for_47b340, &window_init_struct);
	auto fxn2 = (void(__thiscall*)(void*))(0x472c30);
	(*fxn2)(myptr);

}