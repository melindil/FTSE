#pragma once
#include "WWindow.h"
#include <string>
class WPicture :
	public WWindow
{
public:
	WPicture();
	virtual ~WPicture();
	WPicture(void*);

	static std::shared_ptr<WPicture> Create(WWindow const& parent, std::string const& picname, WWindow::ScreenCoord const& topleft);
};

