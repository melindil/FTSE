#include "WBOSButton.h"
#include "Helpers.h"
#include "FOTString.h"

WBOSButton::WBOSButton()
	: WWindow()
{
}

WBOSButton::~WBOSButton()
{
}

WBOSButton::WBOSButton(void* ptr)
	: WWindow(ptr)
{

}

//static
std::shared_ptr<WBOSButton> WBOSButton::Create(WWindow const& parent, std::string const& text, WWindow::ScreenCoord const& topleft, size_t handler, int button_type)
{
	char* new_button = Helpers::FOTHeapAlloc(0x15b);

	uint32_t event_handler_array[5];
	auto setup_event_handler = (void* (__thiscall *)(void*, uint32_t*, size_t, uint32_t))0x6c2fd0;
	(*setup_event_handler)(parent.Get(), event_handler_array, handler, 0);

	FOTString buttonstring1(text);
	wchar_t* ptr_to_string = buttonstring1.getraw();

	auto create_button = (void(__thiscall *)(void*, void*, const WWindow::ScreenCoord*, uint32_t*, wchar_t**, int32_t))0x65bcb0;
	(*create_button)(new_button, parent.Get(), &topleft, event_handler_array, &ptr_to_string, button_type);

	auto finalize_event_handler = (void(__thiscall *)(void*, uint32_t*))0x6c7d40;
	(*finalize_event_handler)((void*)event_handler_array[0], event_handler_array);

	return std::make_shared<WBOSButton>(new_button);
}