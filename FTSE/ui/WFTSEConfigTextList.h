#pragma once
#include "WWindow.h"

#include <string>
#include "ui/CustomUIClass.h"

#include "Logger.h"

class WFTSEConfig;

class WFTSEConfigTextList :
	public WWindow
{
public:
	WFTSEConfigTextList() //: log_("ftse_speech2.log") 
	{
		if (!init_custom_class) {
			init_custom_class = true;
			customclass_.InstallVtableHook(51, (size_t)ThunkMouseClick);
		}
	}
	virtual ~WFTSEConfigTextList() {}
	WFTSEConfigTextList(void* ptr, std::shared_ptr<WFTSEConfig> parent)
		: WWindow(ptr),
		//log_("ftse_speech2.log"),
		parent_(parent)
	{
		if (!init_custom_class) {
			init_custom_class = true;
			customclass_.InstallVtableHook(51, (size_t)ThunkMouseClick);
		}
	}

	void SetText(std::string const& txt, int param);
	void AppendText(std::string const& txt, int param);

	static std::shared_ptr<WFTSEConfigTextList> Create(std::shared_ptr<WFTSEConfig> parent, WWindow::WWindowRect const& rect, int32_t unkparam);

	void ClearItems();
	void InsertItem(std::string const& item, size_t idx, bool state);
	void FinalizeItems();
	int32_t GetCurrentLineCount();
	void Destruct();

#pragma pack (push,1)
	typedef struct
	{
		int32_t unk;
		void* root_node;
		int32_t num_lines;
	} wftsespeechtextlist_textheader;

	typedef struct
	{
		wwindow_header hdr_;
		void* scrollbar;
		wftsespeechtextlist_textheader* textheader;

	} wftsespeechtextlist_header;
#pragma pack (pop)

private:
	static CustomUIClass<WFTSEConfigTextList> customclass_;
	static bool init_custom_class;
	//static bool __stdcall ThunkMouseMove(void* ptr, int* loc, int unk);
	static bool __stdcall ThunkMouseClick(void* ptr, int* loc, bool unk1, int unk2);

	//Logger log_;

	typedef struct
	{
		std::string text;
		size_t index;
		bool state;
		int32_t startline;
	} Selection;

	std::vector<Selection> items_;
	int32_t selected_item_;
	std::shared_ptr<WFTSEConfig> parent_;

	bool MouseClick(int* loc, bool unk1, int unk2);


};

