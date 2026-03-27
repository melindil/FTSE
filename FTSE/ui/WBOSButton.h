#pragma once
#include "WWindow.h"
#include <string>

class WBOSButton :
	public WWindow
{
public:
	WBOSButton();
	virtual ~WBOSButton();

	WBOSButton(void*);

	static std::shared_ptr<WBOSButton> Create(WWindow const& parent, std::string const& text, WWindow::ScreenCoord const& topleft, size_t handler,int button_type);

private:
	uint8_t bb_unk[0x61];
};

