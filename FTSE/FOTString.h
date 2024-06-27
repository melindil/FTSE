#pragma once
#include <string>



class FOTString
{
public:
	FOTString();
	FOTString(void*);
	FOTString(std::string const&);
	~FOTString();

	std::string get() const;
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
	wchar_t* ptr_;
};

/*
class FOTStringPtr
{
public:
	FOTStringPtr() : ptr_(nullptr) {}
	FOTStringPtr(FOTString* p) : ptr_(p) { ptr_->inc_ptr_ref(); }
	FOTStringPtr(FOTStringPtr const& p) : ptr_(p.ptr_) { ptr_->inc_ptr_ref(); }
	
	FOTStringPtr& operator=(FOTStringPtr const& rhs) { 
		if (ptr_ != nullptr)
		{
			if (ptr_->dec_ptr_ref())
				delete ptr_;
		}
		ptr_ = rhs.ptr_; 
		ptr_->inc_ptr_ref();
	}
	~FOTStringPtr() {
		if (ptr_ != nullptr)
		{
			if (ptr_->dec_ptr_ref())
				delete ptr_;
		}
	}
	FOTString* get() const { return ptr_; }

private:
	FOTString* ptr_;
};*/