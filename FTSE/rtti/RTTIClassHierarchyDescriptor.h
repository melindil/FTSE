#pragma once

#include <stdint.h>
#include <vector>

#include "rtti/RTTIBaseClassDescriptor.h"

class RTTIClassHierarchyDescriptor
{
public:
	RTTIClassHierarchyDescriptor(int32_t signature, int32_t attr);
	~RTTIClassHierarchyDescriptor();

	void const* Get() const { return this; }

	void AddBaseClass(size_t addr);

private:
	int32_t signature_;
	int32_t attr_;
	int32_t num_base_classes_;
	void const* base_array_ptr_;


	std::vector<size_t> base_class_array_;
};

