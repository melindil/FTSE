#include "WFTSEConfig.h"

#include "Helpers.h"
#include "WPicture.h"
#include "WBOSButton.h"
#include "WTextList.h"
#include "WBOSLargeText.h"
#include "Version.h"
#include "WFTSEConfigTextList.h"
#include "FOTString.h"

CustomUIClass<WFTSEConfig> WFTSEConfig::customclass_("WFTSEConfig", 0x807868, 98);


WFTSEConfig::WFTSEConfig()
	: WWindow()
{
}


WFTSEConfig::~WFTSEConfig()
{
}

WFTSEConfig::WFTSEConfig(void* ptr, std::shared_ptr<GenericPatcher> patcher)
	: WWindow(ptr), patcher_(patcher)
{
}
void WFTSEConfig::SetupVtable()
{
	auto my_struct = GetStruct();
	my_struct->hdr_.vtable_wwindow = VTABLE;
	my_struct->hdr_.vtable_eventhandler = VTABLE_EVENTHANDLER;
}

void WFTSEConfig::SetupControls(std::shared_ptr<WFTSEConfigTextList> patches_list, std::shared_ptr<WTextList> description)
{
	patches_list_ = patches_list;
	description_ = description;
	patch_data_ = patcher_->GetPatchDescriptors();

	UpdatePatchesList();

}

void WFTSEConfig::ToggleState(size_t item)
{
	patch_data_[item].new_apply = !patch_data_[item].new_apply;
}

void WFTSEConfig::PopulateDescription(size_t item)
{
	description_->SetText(patch_data_[item].description,0);
}

void WFTSEConfig::UpdatePatchesList()
{
	std::string newtext;
	patches_list_->ClearItems();
	for (size_t i = 0; i < patch_data_.size(); i++)
	{
		patches_list_->InsertItem(patch_data_[i].name, i, patch_data_[i].new_apply);
	}
	patches_list_->FinalizeItems();
}


std::shared_ptr<WFTSEConfig> WFTSEConfig::Create(WWindow const& parent, WWindow::WWindowRect const& border, std::shared_ptr<GenericPatcher> patcher)
{
	void* new_window = Helpers::FOTHeapAlloc(0xfa);

	auto wwindow_constructor = (WWindow* (__thiscall *)(void*, void*, const WWindow::WWindowRect*))0x6c5d00;
	*(wwindow_constructor)(new_window, parent.Get(), &border);

	//new_window->border = border;
	auto newobj = std::make_shared<WFTSEConfig>(new_window, patcher);
	customclass_.AddInstance(new_window, newobj);

	newobj->SetupVtable();

	WPicture::Create(newobj->Get(), "FTSE/gui/back/FTSEOptions.png", WWindow::ScreenCoord{ 0,0 });

	WWindow::ScreenCoord button_coord{ 550,498 };
	WBOSButton::Create(newobj->Get(), "DONE", button_coord, Helpers::ConvertFunction(&WFTSEConfig::Handler_Done), 0);
	button_coord.y += 46;
	WBOSButton::Create(newobj->Get(), "CANCEL", button_coord, 0x6c62c0, 0);
	button_coord.y += 46;

	//std::shared_ptr<WGameMain> parent = World::GetWGameMain();
	size_t* defaultstyle_param18 = (size_t*)((char*)newobj->Get()->default_style_ptr + 0x18);	// This is console font
	size_t* defaultstyle_param2c = (size_t*)((char*)newobj->Get()->default_style_ptr + 0x2c);	// This is "Large" font

	auto txtlist1 = WFTSEConfigTextList::Create(newobj, WWindowRect{ {14,39},{391,446} }, *defaultstyle_param18);
	auto txtlist2 = WTextList::Create(*newobj, WWindowRect{ {409,39},{786,446} }, *defaultstyle_param18);
	auto txtlist3 = WTextList::Create(*newobj, WWindowRect{ {100,520},{540,580} }, *defaultstyle_param18);
	//txtlist1->SetText("<img:gui/btn/check_dn.zar><img:gui/btn/check_up.zar>This is a test string that is really long, I mean so really long, like yeah this long, that I want to wrap around please, so can you please wrap?", 0);
	txtlist2->SetText("This is a test string that is really long, I mean so really long, like yeah this long, that I want to wrap around please, so can you please wrap? 2", 0);
	txtlist3->SetText(std::string("<rgb:255,255,255>FTSE " FTSE_VERSION " (C) 2018-2026 Melindil\nThis product is open source; see LICENSE file in Github repo for usage terms.\nPortions based on FT Improver (C) Jaroslaw Szatkowski"), 0);

	newobj->SetupControls(txtlist1, txtlist2);

	auto header1 = WBOSLargeText::Create(*newobj, "PATCHES", WWindowRect{ {160,10},{395,30} });
	auto header2 = WBOSLargeText::Create(*newobj, "DESCRIPTION", WWindowRect{ {535,10},{790,30} });


	return newobj;

}

void WFTSEConfig::Handler_Done(int32_t unused)
{
	// BE CAREFUL IN THIS FUNCTION: the "this" pointer is set to a Fallout Tactics object, not an FTSE one.

	// Do a message box (eventually make this a static fxn in WBOSMessageBox?)
	auto mboxfxn = (int(__cdecl*)(void*, wchar_t**, int32_t, int32_t*))0x65f900;
	int32_t mboxarray[8] = { 0,0 };
	{
		FOTString msg(std::string("The game must be restarted for changes to take effect."));
		wchar_t* msgw = msg.getraw();
		(*mboxfxn)(this, &msgw, 4, mboxarray);

	}

	customclass_.GetInstance(this)->CommitChanges();

	auto closefxn = (void(__thiscall*)(void*, int32_t))0x6c62c0;
	(*closefxn)(this,unused);
}

void WFTSEConfig::CommitChanges()
{
	patcher_->WriteNewConfig(patch_data_);
	customclass_.RemoveInstance(ptr_);
}