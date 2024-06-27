#pragma once

#include <stdint.h>

#pragma pack(push,1)
struct EntityID
{
	uint16_t seqnum;
	uint16_t id;

	EntityID(uint32_t inttype)
		: seqnum(inttype & 0xffffu), id((inttype >> 16) & 0xffffu)
	{

	}

	EntityID() :seqnum(0), id(0) {}
	bool operator==(EntityID const& rhs)
	{
		return seqnum == rhs.seqnum && id == rhs.id;
	}

	operator uint32_t() { return seqnum | (id << 16); }
};
#pragma pack(pop)