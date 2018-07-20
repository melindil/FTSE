#include "Helpers.h"
#include <Windows.h>
#include <memory>

static const DWORD FXN_FOTHEAPALLOC = 0x6c4dd0;
static char* (*FOTHeapAlloc)(DWORD) = (char* (*)(DWORD))FXN_FOTHEAPALLOC;

std::string Helpers::WcharToUTF8(wchar_t* str)
{
	int needed = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str, -1, nullptr, 0, nullptr, nullptr);
	std::unique_ptr<char[]> buf(new char[needed]);
	WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str, -1, buf.get(), needed, nullptr, nullptr);
	return std::string(buf.get());
}
std::wstring Helpers::UTF8ToWchar(std::string const& str)
{
	int needed = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, NULL,0);
	std::unique_ptr<wchar_t[]> buf(new wchar_t[needed]);
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, buf.get(), needed);
	return std::wstring(buf.get());
}
wchar_t* Helpers::UTF8ToWcharFOTHeap(std::string const& str,int start_ref)
{
	int needed = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, NULL, 0);
	char* buf = FOTHeapAlloc(needed * 2 + 12);
	*((int*)buf) = start_ref;
	*((int*)buf + 1) = needed - 1;
	*((int*)buf + 2) = needed - 1;
	buf += 12;

	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), -1, (WCHAR*)buf, needed);
	return (wchar_t*)buf;

}
