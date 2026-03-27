#pragma once
#include "WWindow.h"
#include <string>

class WBOSLargeText :
	public WWindow
{
public:
	WBOSLargeText();
	virtual ~WBOSLargeText();

	WBOSLargeText(void*);

	static std::shared_ptr<WBOSLargeText> Create(WWindow const& parent, std::string const& text, WWindow::WWindowRect const& rect);
	static const uint32_t VTABLE = 0x8008d4;
	static const uint32_t VTABLE_EVENTHANDLER = 0x8008cc;

};

