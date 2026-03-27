#include "RTTICompleteObjectLocator.h"



RTTICompleteObjectLocator::RTTICompleteObjectLocator(int32_t signature, int32_t vbtable_offset, int32_t cons_displace, RTTITypeDescriptor const& typedesc, RTTIClassHierarchyDescriptor const& hierdesc)
	: signature_(signature), vbtable_offset_(vbtable_offset), cons_displace_(cons_displace), typedesc_(typedesc.Get()), hierdesc_(hierdesc.Get())
{
}

RTTICompleteObjectLocator::~RTTICompleteObjectLocator()
{
}
