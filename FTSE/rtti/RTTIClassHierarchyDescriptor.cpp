#include "RTTIClassHierarchyDescriptor.h"

RTTIClassHierarchyDescriptor::RTTIClassHierarchyDescriptor(int32_t signature, int32_t attr)
	: signature_(signature), attr_(attr), num_base_classes_(0)
{
	base_class_array_.reserve(16);
	base_array_ptr_ = base_class_array_.data();

}

RTTIClassHierarchyDescriptor::~RTTIClassHierarchyDescriptor()
{
}

void RTTIClassHierarchyDescriptor::AddBaseClass(size_t addr)
{
	base_class_array_.push_back(addr);
	num_base_classes_++;
}