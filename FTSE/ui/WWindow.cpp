#include "WWindow.h"



WWindow::WWindow()
	: ptr_(nullptr)
{
}


WWindow::~WWindow()
{
}

WWindow::WWindow(void* ptr)
	: ptr_(reinterpret_cast<wwindow_header*>(ptr))
{

}

void WWindow::HandleEvent(int32_t bogus)
{

}

// static
std::shared_ptr<WWindow> WWindow::GetGblMenuManager()
{
	void** ptr = (void**)(0x8bddb8);
	return std::make_shared<WWindow>(*ptr);
}