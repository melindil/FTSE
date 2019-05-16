#pragma once
#include <string>
class FOTString
{
public:
	FOTString();
	FOTString(void*);
	~FOTString();

	std::string get();
	wchar_t* getraw();
	void incref();
	void decref();

private:
#pragma pack (push,1)
	typedef struct {
		uint32_t refcount;
		uint32_t stringlen;
		uint32_t stringcap;
		wchar_t str[1];
	} FOTStringType;
#pragma pack(pop)
	FOTStringType* ptr_;
};

