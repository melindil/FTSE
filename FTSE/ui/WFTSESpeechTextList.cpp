#include "WFTSESpeechTextList.h"

#include "Helpers.h"
#include "FOTString.h"
#include "WFTSESpeech.h"

//static 
CustomUIClass<WFTSESpeechTextList> WFTSESpeechTextList::customclass_("WFTSESpeechTextList", 0x82e8b8, 98);
bool WFTSESpeechTextList::init_custom_class = false;

std::shared_ptr<WFTSESpeechTextList> WFTSESpeechTextList::Create(std::shared_ptr<WFTSESpeech> parent, WWindow::WWindowRect const& rect, int32_t unkparam)
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

	auto new_shared = std::make_shared <WFTSESpeechTextList>(new_textlist, parent);
	customclass_.AddInstance(new_textlist, new_shared);

	return new_shared;
}

void WFTSESpeechTextList::SetText(std::string const& txt, int param)
{
	FOTString wtxt(txt);
	wchar_t* ptrptr = wtxt.getraw();
	auto set_text = (void(__thiscall *)(void*, wchar_t**, int))0x6c8630;
	(*set_text)(Get(), &ptrptr, param);

}
void WFTSESpeechTextList::AppendText(std::string const& txt, int param)
{
	FOTString wtxt(txt);
	wchar_t* ptrptr = wtxt.getraw();
	auto set_text = (void(__thiscall *)(void*, wchar_t**, int))0x6c8680;
	(*set_text)(Get(), &ptrptr, param);

}

int32_t WFTSESpeechTextList::GetCurrentLineCount()
{
	wftsespeechtextlist_header* hdr = (wftsespeechtextlist_header*)Get();
	return hdr->textheader->num_lines;
}

// static
bool __stdcall WFTSESpeechTextList::ThunkMouseMove(void* ptr, int* loc, int unk)
{
	auto instance = customclass_.GetInstance(ptr);
	return instance->MouseMove(loc, unk);
}
// static
bool __stdcall WFTSESpeechTextList::ThunkMouseClick(void* ptr, int* loc, bool unk1,int unk2)
{
	auto instance = customclass_.GetInstance(ptr);
	return instance->MouseClick(loc, unk1,unk2);
}

bool WFTSESpeechTextList::MouseMove(int* loc, int unk)
{
	//log_ << "Mouse Move " << loc[0] << "," << loc[1] << std::endl;
	//log_ << "Num items at mousemove = " << items_.size() << std::endl;

	int32_t last_item = selected_item_;
	if (loc[0] < 440 && loc[1] < GetCurrentLineCount()*11)
	{
		int32_t line = loc[1] / 11;
		selected_item_ = items_.size() - 1;
		while (selected_item_ > 0 && items_[selected_item_].startline > line ) selected_item_--;
	
	}
	else
	{
		selected_item_ = -1;
	}
	if (selected_item_ != last_item) FinalizeItems();
	return true;
}

bool WFTSESpeechTextList::MouseClick(int* loc, bool unk1, int unk2)
{
	//log_ << "Mouse Click " << loc[0] << "," << loc[1] << std::endl;

	if (loc[0] < 440 && loc[1] < GetCurrentLineCount() * 11)
	{
		int32_t line = loc[1] / 11;
		int32_t item = items_.size() - 1;
		while (item > 0 && items_[item].startline > line) item--;
		parent_->RunNode(items_[item].nodename);
	}
	return true;
}

void WFTSESpeechTextList::ClearItems()
{ 
	//log_ << "Clear items" << std::endl;
	items_.clear(); 
	selected_item_ = -1;
}
void WFTSESpeechTextList::InsertItem(std::string const& item, std::string const& node)
{
	//log_ << "Insert item: " << item << std::endl;
	items_.push_back(Selection{ item, node, 0 });
}

void WFTSESpeechTextList::FinalizeItems()
{
	//log_ << "Num items at finalize = " << items_.size() << std::endl;
	SetText("", 0);
	for (uint32_t i=0;i<items_.size();i++)
	{
		items_[i].startline = GetCurrentLineCount();
		//log_ << "Startline for item " << i << " is " << items_[i].startline << std::endl;
		std::string head = (i == selected_item_) ? "<b+>" : "<b->";
		std::string this_entry = head+std::to_string(i+1) + ". " + items_[i].text + "<b->\n";
		AppendText(this_entry, 0);
	}
}

void WFTSESpeechTextList::Destruct()
{
	ClearItems();
	customclass_.RemoveInstance(ptr_);
}