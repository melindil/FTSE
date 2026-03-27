#include "RTTITypeDescriptor.h"

#include <stdint.h>

RTTITypeDescriptor::RTTITypeDescriptor(std::string const& name)
{
	int32_t len = name.length() + 15;
	data_.resize(len, 0);
	data_[0] = 0x24; data_[1] = 0x7b; data_[2] = 0x83;					// Address for type_info in the exe
	data_[8] = '.';  data_[9] = '?'; data_[10] = 'A'; data_[11] = 'V';	// Pre-name mangling for class name
	memcpy(&data_.at(12), name.c_str(), name.length());
	data_[name.length() + 12] = '@'; data_[name.length() + 13] = '@';	// Post-name mangling for class name

	// Other bytes are left at zero
}


RTTITypeDescriptor::~RTTITypeDescriptor()
{
}

void const* RTTITypeDescriptor::Get() const
{
	return data_.data();
}