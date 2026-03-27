#pragma once

#include <vector>

class RTTITypeDescriptor
{
public:
	RTTITypeDescriptor(std::string const& classname);
	~RTTITypeDescriptor();

	void const* Get() const;

private:
	std::vector<unsigned char> data_;
};

