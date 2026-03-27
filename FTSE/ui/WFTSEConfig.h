#pragma once
#include <memory>
#include "WWindow.h"
#include "CustomUIClass.h"
#include "GenericPatcher.h"

class WTextList;
class WFTSEConfigTextList;
class WFTSEConfig : public WWindow
{
public:

	WFTSEConfig();
	virtual ~WFTSEConfig();

	WFTSEConfig(void*, std::shared_ptr<GenericPatcher>);

	static std::shared_ptr<WFTSEConfig> Create(WWindow const& parent, WWindow::WWindowRect const& border, std::shared_ptr<GenericPatcher> patcher);
	void SetupVtable();
	void SetupControls(std::shared_ptr<WFTSEConfigTextList> patches_list, std::shared_ptr<WTextList> description);

	void ToggleState(size_t item);

	void Handler_Done(int32_t unused);
	void CommitChanges();
	void PopulateDescription(size_t item);

	typedef struct
	{
		WWindow::wwindow_header hdr_;
	} wftseconfig_header;

private:
	std::vector<GenericPatcher::PatchDescriptor> patch_data_;
	std::shared_ptr<WFTSEConfigTextList> patches_list_;
	std::shared_ptr<WTextList> description_;
	std::shared_ptr<GenericPatcher> patcher_;

	wftseconfig_header* GetStruct() { return reinterpret_cast<wftseconfig_header*>(ptr_); }
	void UpdatePatchesList();
	static const uint32_t VTABLE = 0x807868;
	static const uint32_t VTABLE_EVENTHANDLER = 0x807860;

	// Make this a static
	static CustomUIClass<WFTSEConfig> customclass_;

};

