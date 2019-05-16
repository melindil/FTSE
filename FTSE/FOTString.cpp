#include "FOTString.h"
#include "Helpers.h"


FOTString::FOTString() : ptr_(nullptr)
{
}

// Adjust received pointer to account for the preceding struct
FOTString::FOTString(void* ptr) : ptr_(reinterpret_cast<FOTStringType*>(((uint32_t)ptr)-0x0c))
{
}

std::string FOTString::get()
{
	if (ptr_ == nullptr)
		return "";
	return Helpers::WcharToUTF8(ptr_->str);
}

wchar_t* FOTString::getraw()
{
	return ptr_->str;
}

void FOTString::incref()
{
	ptr_->refcount++;
}

void FOTString::decref()
{
	ptr_->refcount--;
}

FOTString::~FOTString()
{
	if (ptr_ != nullptr)
	{
		ptr_->refcount--;
		if (ptr_->refcount == 0)
		{
			// Need to free the string
			auto fxn = (void (__cdecl *)(void*))0x6c4e50;
			fxn(reinterpret_cast<void*>(ptr_));
		}
	}
}
