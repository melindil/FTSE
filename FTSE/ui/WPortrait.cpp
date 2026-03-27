#include "WPortrait.h"

#include "Helpers.h"

// static
std::shared_ptr<WPortrait> WPortrait::Create(WWindow const& parent, WWindow::WWindowRect const& rect, EntityID ent)
{
	char* new_portrait = Helpers::FOTHeapAlloc(0x169);

	auto create_portrait = (void(__thiscall *)(void*, void*, const WWindow::WWindowRect*))0x4bba10;
	(*create_portrait)(new_portrait, parent.Get(), &rect);

	auto load_portrait = (void(__thiscall *)(void*, EntityID*))0x4bc020;
	(*load_portrait)(new_portrait, &ent);

	return std::make_shared<WPortrait>(new_portrait);
}
