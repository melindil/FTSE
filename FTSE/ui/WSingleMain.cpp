#include "WSingleMain.h"
#include "WPicture.h"
#include "WBOSButton.h"
#include "Helpers.h"

WSingleMain::WSingleMain()
	: WWindow()
{
}

WSingleMain::~WSingleMain()
{
}

WSingleMain::WSingleMain(void* ptr)
	: WWindow(ptr)
{

}

void WSingleMain::SetupVtable()
{
	auto my_struct = GetStruct();
	my_struct->hdr_.vtable_wwindow = VTABLE;
	my_struct->hdr_.vtable_eventhandler = VTABLE_EVENTHANDLER;
}

std::shared_ptr<WSingleMain> WSingleMain::Create(WWindow const& parent, WWindow::WWindowRect const& border)
{
	void* new_window = Helpers::FOTHeapAlloc(0xfa);

	auto wwindow_constructor = (WWindow* (__thiscall *)(void*, void*, const WWindow::WWindowRect*))0x6c5d00;
	*(wwindow_constructor)(new_window, parent.Get(), &border);

	//new_window->border = border;
	auto newobj = std::make_shared<WSingleMain>(new_window);

	newobj->SetupVtable();
	return newobj;

}
