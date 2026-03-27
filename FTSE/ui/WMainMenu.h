#pragma once
#include "WWindow.h"

class GenericPatcher;
class WMainMenu :
	public WWindow
{
public:
	WMainMenu();
	virtual ~WMainMenu();

	void PrepAddedButtons(std::shared_ptr<GenericPatcher> patcher);
	void Handler_EditorTools(int32_t unused);
	void Handler_FTSEConfig(int32_t unused);

private:
	// Kludge, consider refactoring this to not be a static when WMainMenu becomes a fully-supported custom class
	static std::shared_ptr<GenericPatcher> patcher_;
};

