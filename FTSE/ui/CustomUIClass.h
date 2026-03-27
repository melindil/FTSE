#pragma once

#include <memory>
#include <map>
#include <string>

#include <Windows.h>

#include "rtti/RTTITypeDescriptor.h"
#include "rtti/RTTIBaseClassDescriptor.h"
#include "rtti/RTTIClassHierarchyDescriptor.h"
#include "rtti/RTTICompleteObjectLocator.h"
#include "rtti/Vtable.h"

template <typename T>
class CustomUIClass
{
public:
	CustomUIClass(std::string const& name, size_t old_vtable_addr, size_t old_vtable_entries)
	{
		typedesc_ = std::make_shared<RTTITypeDescriptor>(name);
		hierdesc_ = std::make_shared<RTTIClassHierarchyDescriptor>(0, 1);

		size_t* old_objloc = *((size_t**)(old_vtable_addr - 4));
		size_t* old_event_vtable = (size_t*)(old_vtable_addr - 8);
		size_t* old_hier = (size_t*)(old_objloc[4]);
		size_t old_base_class_ct = old_hier[2];
		size_t* old_base_array = (size_t*)(old_hier[3]);

		basedesc_ = std::make_shared<RTTIBaseClassDescriptor>(*typedesc_, old_base_class_ct, 0, 0xffffffff, 0, 0);
		hierdesc_->AddBaseClass((size_t)basedesc_->Get());
		for (size_t i = 0; i < old_base_class_ct; i++)
			hierdesc_->AddBaseClass(old_base_array[i]);

		comploc_main_ = std::make_shared<RTTICompleteObjectLocator>(0, 0, 0, *typedesc_, *hierdesc_);
		comploc_event_ = std::make_shared<RTTICompleteObjectLocator>(0, 0x10, 0, *typedesc_, *hierdesc_);

		size_t* old_vtable = (size_t*)old_vtable_addr;
		vtable_main_ = std::make_shared<Vtable>((size_t)comploc_main_->Get(), old_vtable, old_vtable_entries);
		vtable_event_ = std::make_shared<Vtable>((size_t)comploc_event_->Get(), old_event_vtable, 1);

	}
	~CustomUIClass() {}

	size_t GetVtableAddr() { return vtable_main_->Get(); }
	size_t GetEventVtableAddr() { return vtable_event_->Get(); }

	void AddInstance(void* ptr, std::shared_ptr<T> instance) { instances_[ptr] = instance; }
	void RemoveInstance(void* ptr) { instances_.erase(ptr); }
	std::shared_ptr<T> GetInstance(void* ptr) { return instances_[ptr]; }

	void InstallVtableHook(int fxnid, size_t fxnaddr)
	{
		// Hook stub: converts __thiscall of Entity to __thiscall of HookExecutor, and adds
		//            the index of the hook as a parameter, so that we can use that to look
		//            up the correct Lua function in the Lua registry.

		// pop eax                 58
		// push ecx                51
		// push eax                50
		// jmp <fxnaddr-rel>       e9 xx xx xx xx
		static const int HOOKSIZE = 8;

		// Allocate memory for the hook stub
		unsigned char* newhook = new unsigned char[HOOKSIZE];


		// Change execute privilege for hook stub
		DWORD oldprotect;
		VirtualProtect(newhook, HOOKSIZE, 0x40, &oldprotect);

		// build the stub code
		newhook[0] = 0x58;
		newhook[1] = 0x51;
		newhook[2] = 0x50;
		newhook[3] = 0xe9;

		size_t relative_address = fxnaddr - (((size_t)newhook) + 8);
		memcpy(newhook + 4, &relative_address, sizeof(size_t));

		// Add one to idx because of the RTTI entry
		vtable_main_->Set(fxnid+1, (size_t)newhook);


	}
	

private:
	std::shared_ptr<RTTITypeDescriptor> typedesc_;
	std::shared_ptr<RTTIBaseClassDescriptor> basedesc_;
	std::shared_ptr<RTTIClassHierarchyDescriptor> hierdesc_;
	std::shared_ptr<RTTICompleteObjectLocator> comploc_main_;
	std::shared_ptr<RTTICompleteObjectLocator> comploc_event_;
	std::shared_ptr<Vtable> vtable_main_;
	std::shared_ptr<Vtable> vtable_event_;

	std::map<void*, std::shared_ptr<T>> instances_;

};

