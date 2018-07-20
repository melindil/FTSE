#pragma once

#include <string>

class Helpers
{
public:
	static std::string WcharToUTF8(wchar_t* str);
	static std::wstring UTF8ToWchar(std::string const& str);
	static wchar_t* UTF8ToWcharFOTHeap(std::string const& str,int start_ref);

};

