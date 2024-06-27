#pragma once

#include "Helpers.h"
#include <stdint.h>

#pragma pack (push,1)
struct ZoneDefinition
{
	Vector3 bound_top;
	Vector3 bound_bottom;
	RGBColor zonecolor;
	wchar_t* zonename;
	bool soundzone;
	bool unk;

	ZoneDefinition() : bound_top(), bound_bottom(), zonecolor(), zonename(), soundzone(false), unk(false) {}
};

#pragma pack (pop)

