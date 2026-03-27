#pragma once
#include "WWindow.h"
class WSingleMain :
	public WWindow
{
public:

	typedef struct
	{
		WWindow::wwindow_header hdr_;
	} wsinglemain_header;

	WSingleMain();
	virtual ~WSingleMain();

	WSingleMain(void*);

	static std::shared_ptr<WSingleMain> Create(WWindow const& parent, WWindow::WWindowRect const& border);

	void SetupVtable();

private:
	wsinglemain_header* GetStruct() { return reinterpret_cast<wsinglemain_header*>(ptr_); }
	static const uint32_t VTABLE = 0x807868;
	static const uint32_t VTABLE_EVENTHANDLER = 0x807860;
};

