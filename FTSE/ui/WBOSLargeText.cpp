#include "WBOSLargeText.h"
#include "Helpers.h"
#include "FOTString.h"
#include "DefaultStyle.h"

WBOSLargeText::WBOSLargeText()
	: WWindow()
{
}


WBOSLargeText::~WBOSLargeText()
{
}

WBOSLargeText::WBOSLargeText(void* ptr)
	: WWindow(ptr)
{
}

//static
std::shared_ptr<WBOSLargeText> WBOSLargeText::Create(WWindow const& parent, std::string const& text, WWindow::WWindowRect const& rect)
{
	WWindow::wwindow_header* new_text = (WWindow::wwindow_header*)Helpers::FOTHeapAlloc(0x102);


	FOTString textstring1(text);
	wchar_t* ptr_to_string = textstring1.getraw();

	auto wbostext_constructor = (void(__thiscall *)(void*, void*, const WWindow::WWindowRect*, wchar_t**, int32_t))0x468cd0;
	(*wbostext_constructor)(new_text, parent.Get(), &rect, &ptr_to_string, 0);

	new_text->vtable_wwindow = VTABLE;
	new_text->vtable_eventhandler = VTABLE_EVENTHANDLER;

	auto setcolor = (void(__thiscall *)(void*, float*))0x6c6390;
	(*setcolor)(new_text, parent.Get()->default_style_ptr->colorPanelTitles);
	auto setfont = (void(__thiscall *)(void*, void*))0x6c8010;
	(*setfont)(new_text, parent.Get()->default_style_ptr->StyleObjects[6]);

	return std::make_shared<WBOSLargeText>(new_text);
}