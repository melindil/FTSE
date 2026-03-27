#pragma once
#include "WWindow.h"

#include "EntityID.h"

class WPortrait :
	public WWindow
{
public:
	WPortrait() {}
	virtual ~WPortrait() {}
	WPortrait(void* ptr) : WWindow(ptr) {}

	static std::shared_ptr<WPortrait> Create(WWindow const& parent, WWindow::WWindowRect const& rect, EntityID ent);

};

