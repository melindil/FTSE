#include "FOTString.h"
#include "Helpers.h"

FOTString::FOTString() : ptr_(nullptr)
{
}

// Adjust received pointer to account for the preceding struct
FOTString::FOTString(void* ptr) : ptr_(static_cast<wchar_t*>(ptr))
{
}

FOTString::FOTString(std::string const& s)
{
	ptr_ = Helpers::UTF8ToWcharFOTHeap(s, 1);
}

std::string FOTString::get() const
{
	if (ptr_ == nullptr)
		return "";
	return Helpers::WcharToUTF8(ptr_);
}

wchar_t* FOTString::getraw()
{
	return ptr_;
}

void FOTString::incref()
{
	FOTStringType* base = (FOTStringType*)(ptr_ - 6);
	base->refcount++;
}

void FOTString::decref()
{
	FOTStringType* base = (FOTStringType*)(ptr_ - 6);
	if (base->refcount > 0)
	{
		base->refcount--;
		if (base->refcount == 0)
		{
			// Need to free the string
			auto fxn = (void(__cdecl *)(void*))0x6c4e50;
			fxn(reinterpret_cast<void*>(base));
			ptr_ = nullptr;
		}
	}
}

FOTString::~FOTString()
{
	if (ptr_ != nullptr)
	{
		FOTStringType* base = (FOTStringType*)(ptr_ - 6);
		base->refcount--;
		if (base->refcount == 0)
		{
			// Need to free the string
			auto fxn = (void (__cdecl *)(void*))0x6c4e50;
			fxn(reinterpret_cast<void*>(base));
		}
		else
		{
		}
	}
}
