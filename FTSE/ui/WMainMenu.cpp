#include "WMainMenu.h"
#include "Helpers.h"
#include "WBOSButton.h"
#include "WPicture.h"
#include "WSingleMain.h"

#include "WFTSEConfig.h"

// static
std::shared_ptr<GenericPatcher> WMainMenu::patcher_;

WMainMenu::WMainMenu()
{
}

WMainMenu::~WMainMenu()
{
}

void WMainMenu::PrepAddedButtons(std::shared_ptr<GenericPatcher> patcher)
{
	patcher_ = patcher;
	WBOSButton::Create(this, std::string("EDITOR TOOLS"), WWindow::ScreenCoord{ 550, 346 }, Helpers::ConvertFunction(&WMainMenu::Handler_EditorTools),0);
	WBOSButton::Create(this, std::string("FTSE CONFIG"), WWindow::ScreenCoord{ 550, 392 }, Helpers::ConvertFunction(&WMainMenu::Handler_FTSEConfig), 0);
}

void WMainMenu::Handler_EditorTools(int32_t unused)
{
	WWindow::ScreenCoord button_coord{ 550,50 };
	std::shared_ptr<WSingleMain> new_window = WSingleMain::Create(WWindow::GetGblMenuManager()->Get(), WWindow::WWindowRect{ {0, 0}, { 800,600 }});
	WPicture::Create(new_window->Get(), "gui/wait/wait1.jpg", WWindow::ScreenCoord{ 0,0 });
	WBOSButton::Create(new_window->Get(), "CAMPAIGN EDITOR", button_coord, 0x6c0d60, 0);
	button_coord.y += 46;
	WBOSButton::Create(new_window->Get(), "LEVEL EDITOR", button_coord, 0x6c0d50, 0);
	button_coord.y += 46;
	WBOSButton::Create(new_window->Get(), "ENTITY EDITOR", button_coord, 0x6c0f00, 0);
	button_coord.y += 46;
	WBOSButton::Create(new_window->Get(), "LOG", button_coord, 0x6c1040, 0);
	button_coord.y += 46;
	WBOSButton::Create(new_window->Get(), "SPRITE EDITOR", button_coord, 0x6c0dc0, 0);
	button_coord.y += 46;
	WBOSButton::Create(new_window->Get(), "GRAPHER", button_coord, 0x6c0fa0, 0);
	button_coord.y += 46;
	WBOSButton::Create(new_window->Get(), "BACK", button_coord, 0x6c62c0, 0);

}

void WMainMenu::Handler_FTSEConfig(int32_t unused)
{
	std::shared_ptr<WFTSEConfig> new_window = WFTSEConfig::Create(WWindow::GetGblMenuManager()->Get(), WWindow::WWindowRect{ {0, 0}, { 800,600 } }, patcher_);
}
