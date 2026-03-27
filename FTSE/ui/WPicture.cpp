#include "WPicture.h"
#include "Helpers.h"
#include "FOTString.h"

WPicture::WPicture()
	: WWindow()
{
}

WPicture::~WPicture()
{
}

WPicture::WPicture(void* ptr)
	: WWindow(ptr)
{

}

std::shared_ptr<WPicture> WPicture::Create(WWindow const& parent, std::string const & picname, WWindow::ScreenCoord const & topleft)
{
	char* new_pic = Helpers::FOTHeapAlloc(0x100);

	FOTString picfile(picname);
	wchar_t* ptr_to_string = picfile.getraw();

	auto create_pic = (void(__thiscall *)(void*, void*, const WWindow::ScreenCoord*, wchar_t**))0x718a40;
	(*create_pic)(new_pic, parent.Get(), &topleft, &ptr_to_string);

	return std::make_shared<WPicture>(new_pic);
}
