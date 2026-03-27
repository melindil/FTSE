#pragma once
#include "WWindow.h"

#include <string>
class WTextList :
	public WWindow
{
public:
	WTextList() {}
	virtual ~WTextList() {}
	WTextList(void* ptr) : WWindow(ptr) {}

	void SetText(std::string const& txt,int param);

	static std::shared_ptr<WTextList> Create(WWindow const& parent, WWindow::WWindowRect const& rect, int32_t unkparam);

};

