#include "WTextList.h"

#include "Helpers.h"
#include "FOTString.h"

std::shared_ptr<WTextList> WTextList::Create(WWindow const& parent, WWindow::WWindowRect const& rect, int32_t unkparam)
{
	char* new_textlist = Helpers::FOTHeapAlloc(0x10d);

	auto create_textlist = (void(__thiscall *)(void*, void*, const WWindow::WWindowRect*, int32_t))0x6c8020;
	(*create_textlist)(new_textlist, parent.Get(), &rect, unkparam);

	char* default_style_ptr = (char*)((WWindow::wwindow_header*)parent.Get())->default_style_ptr;
	auto init_fe = (void(__thiscall *)(void*, void*, int))0x6c86d0;
	(*init_fe)(new_textlist, default_style_ptr + 100, 0);

	auto top_justify = (void(__thiscall *)(void*, int))0x6c8820;
	(*top_justify)(new_textlist, 0);	// 0 = top justify, 1 = center justify

	return std::make_shared<WTextList>(new_textlist);
}

void WTextList::SetText(std::string const& txt,int param)
{
	FOTString wtxt(txt);
	wchar_t* ptrptr = wtxt.getraw();
	auto set_text = (void(__thiscall *)(void*, wchar_t**, int))0x6c8630;
	(*set_text)(Get(), &ptrptr, param);

}