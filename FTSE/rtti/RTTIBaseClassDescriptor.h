#pragma once

#include <stdint.h>
#include "rtti/RTTITypeDescriptor.h"

class RTTIBaseClassDescriptor
{
public:
	RTTIBaseClassDescriptor(RTTITypeDescriptor const& typedesc, int32_t parent_classes, int32_t mdisp, int32_t pdisp, int32_t vdisp, int32_t attr)
		: typedesc_(typedesc.Get()), parent_classes_(parent_classes), mdisp_(mdisp), pdisp_(pdisp), vdisp_(vdisp), attr_(attr)
	{}

	~RTTIBaseClassDescriptor() {}

	void const* Get() const { return this; }

private:
	void const* typedesc_;
	int32_t parent_classes_;
	int32_t mdisp_;
	int32_t pdisp_;
	int32_t vdisp_;
	int32_t attr_;
};

