#pragma once

#include "RTTITypeDescriptor.h"
#include "RTTIClassHierarchyDescriptor.h"
#include <stdint.h>


class RTTICompleteObjectLocator
{
public:
	RTTICompleteObjectLocator(int32_t signature, int32_t vbtable_offset, int32_t cons_displace, RTTITypeDescriptor const& typedesc, RTTIClassHierarchyDescriptor const& hierdesc);
	~RTTICompleteObjectLocator();

	void const* Get() { return this; }

private:
	int32_t signature_;
	int32_t vbtable_offset_;
	int32_t cons_displace_;
	void const* typedesc_;
	void const* hierdesc_;
};

