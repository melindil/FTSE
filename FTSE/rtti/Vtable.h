#pragma once
#include <vector>
#include <stdint.h>

class Vtable
{
public:
	Vtable(size_t rtti, size_t* old_vtable, size_t entries)
		: table_(entries + 1, rtti)
	{
		for (size_t i = 0; i < entries; i++)
			table_[i + 1] = old_vtable[i];
	}
	~Vtable() {}

	size_t Get() { return (size_t)(&table_.at(1)); }

	void Set(int idx, size_t addr) { table_[idx] = addr; }

private:
	std::vector<size_t> table_;
};

