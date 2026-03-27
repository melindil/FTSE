#pragma once
#include "WWindow.h"

#include <string>
#include "ui/CustomUIClass.h"

#include "Logger.h"

class WFTSESpeech;

class WFTSESpeechTextList :
	public WWindow
{
public:
	WFTSESpeechTextList() //: log_("ftse_speech2.log") 
	{ 
		if (!init_custom_class) {
			init_custom_class = true; 
			customclass_.InstallVtableHook(50, (size_t)ThunkMouseMove); 
			customclass_.InstallVtableHook(51, (size_t)ThunkMouseClick);
		} 
	}
	virtual ~WFTSESpeechTextList() {}
	WFTSESpeechTextList(void* ptr, std::shared_ptr<WFTSESpeech> parent) 
		: WWindow(ptr),
		//log_("ftse_speech2.log"),
		parent_(parent)
	{
		if (!init_custom_class) {
			init_custom_class = true; 
			customclass_.InstallVtableHook(50, (size_t)ThunkMouseMove); 
			customclass_.InstallVtableHook(51, (size_t)ThunkMouseClick);
		}
	}

	void SetText(std::string const& txt, int param);
	void AppendText(std::string const& txt, int param);

	static std::shared_ptr<WFTSESpeechTextList> Create(std::shared_ptr<WFTSESpeech> parent, WWindow::WWindowRect const& rect, int32_t unkparam);

	void ClearItems();
	void InsertItem(std::string const& item, std::string const& node);
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
	static CustomUIClass<WFTSESpeechTextList> customclass_;
	static bool init_custom_class;
	static bool __stdcall ThunkMouseMove(void* ptr, int* loc, int unk);
	static bool __stdcall ThunkMouseClick(void* ptr, int* loc, bool unk1, int unk2);

	//Logger log_;

	typedef struct
	{
		std::string text;
		std::string nodename;
		int32_t startline;
	} Selection;

	std::vector<Selection> items_;
	int32_t selected_item_;
	std::shared_ptr<WFTSESpeech> parent_;

	bool MouseMove(int* loc, int unk); 
	bool MouseClick(int* loc, bool unk1, int unk2);


};

