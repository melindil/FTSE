#include "WFTSEConfigTextList.h"

#include "Helpers.h"
#include "FOTString.h"
#include "WFTSEConfig.h"

//static 
CustomUIClass<WFTSEConfigTextList> WFTSEConfigTextList::customclass_("WFTSEConfigTextList", 0x82e8b8, 98);
bool WFTSEConfigTextList::init_custom_class = false;

std::shared_ptr<WFTSEConfigTextList> WFTSEConfigTextList::Create(std::shared_ptr<WFTSEConfig> parent, WWindow::WWindowRect const& rect, int32_t unkparam)
{
	wftsespeechtextlist_header* new_textlist = (wftsespeechtextlist_header*)Helpers::FOTHeapAlloc(0x10d);

	auto create_textlist = (void(__thiscall *)(void*, void*, const WWindow::WWindowRect*, int32_t))0x6c8020;
	(*create_textlist)(new_textlist, parent->Get(), &rect, unkparam);

	new_textlist->hdr_.vtable_wwindow = customclass_.GetVtableAddr();
	new_textlist->hdr_.vtable_eventhandler = customclass_.GetEventVtableAddr();

	char* default_style_ptr = (char*)((WWindow::wwindow_header*)parent->Get())->default_style_ptr;
	auto init_fe = (void(__thiscall *)(void*, void*, int))0x6c86d0;
	(*init_fe)(new_textlist, default_style_ptr + 100, 0);

	auto top_justify = (void(__thiscall *)(void*, int))0x6c8820;
	(*top_justify)(new_textlist, 0);	// 0 = top justify, 1 = center justify

	auto new_shared = std::make_shared <WFTSEConfigTextList>(new_textlist, parent);
	customclass_.AddInstance(new_textlist, new_shared);

	return new_shared;
}

void WFTSEConfigTextList::SetText(std::string const& txt, int param)
{
	FOTString wtxt(txt);
	wchar_t* ptrptr = wtxt.getraw();
	auto set_text = (void(__thiscall *)(void*, wchar_t**, int))0x6c8630;
	(*set_text)(Get(), &ptrptr, param);

}
void WFTSEConfigTextList::AppendText(std::string const& txt, int param)
{
	FOTString wtxt(txt);
	wchar_t* ptrptr = wtxt.getraw();
	auto set_text = (void(__thiscall *)(void*, wchar_t**, int))0x6c8680;
	(*set_text)(Get(), &ptrptr, param);

}

int32_t WFTSEConfigTextList::GetCurrentLineCount()
{
	wftsespeechtextlist_header* hdr = (wftsespeechtextlist_header*)Get();
	return hdr->textheader->num_lines;
}


// static
bool __stdcall WFTSEConfigTextList::ThunkMouseClick(void* ptr, int* loc, bool unk1, int unk2)
{
	auto instance = customclass_.GetInstance(ptr);
	return instance->MouseClick(loc, unk1, unk2);
}

bool WFTSEConfigTextList::MouseClick(int* loc, bool unk1, int unk2)
{
	//log_ << "Mouse Click " << loc[0] << "," << loc[1] << std::endl;

	if (loc[1] < GetCurrentLineCount() * 15)
	{
		int32_t line = loc[1] / 15;
		int32_t item = items_.size() - 1;
		while (item > 0 && items_[item].startline > line) item--;
		selected_item_ = item;
		parent_->PopulateDescription(items_[item].index);
		if (line == items_[item].startline && loc[0] < 12)
		{
			items_[item].state = !items_[item].state;
			parent_->ToggleState(items_[item].index);
		}
		FinalizeItems();
	}
	return true;
}

void WFTSEConfigTextList::ClearItems()
{
	//log_ << "Clear items" << std::endl;
	items_.clear();
	selected_item_ = -1;
}
void WFTSEConfigTextList::InsertItem(std::string const& item, size_t idx, bool state)
{
	//log_ << "Insert item: " << item << std::endl;
	items_.push_back(Selection{ item, idx, state, 0 });
}

void WFTSEConfigTextList::FinalizeItems()
{
	//log_ << "Num items at finalize = " << items_.size() << std::endl;
	SetText("", 0);
	for (uint32_t i = 0; i < items_.size(); i++)
	{
		items_[i].startline = GetCurrentLineCount();
		//log_ << "Startline for item " << i << " is " << items_[i].startline << std::endl;
		std::string head = (i == selected_item_) ? "<b+>" : "<b->";
		std::string check = (items_[i].state) ? "<img:gui/btn/check_dn.zar> " : "<img:gui/btn/check_up.zar> ";
		std::string this_entry = head + check + items_[i].text + "<b->\n";
		AppendText(this_entry, 0);
	}
}

void WFTSEConfigTextList::Destruct()
{
	ClearItems();
	customclass_.RemoveInstance(ptr_);
}