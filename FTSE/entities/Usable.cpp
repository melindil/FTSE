#include "Usable.h"

#include "Helpers.h"
#include "LuaHelper.h"
#include "AttributesTable.h"

Usable::Usable(void * ptr)
	: Entity(ptr)
{
}

Usable::Usable(EntityID id)
	: Entity(id)
{
}

Usable::~Usable()
{
}

bool Usable::IsReusable()
{
	return GetUsableStruct()->reusable;
}

void Usable::SetLuaSubclass(lua_State * l)
{
	Entity::SetLuaSubclass(l);
	lua_pushboolean(l, true);
	lua_setfield(l, -2, "isUsable");

	lua_pushcfunction(l, (LuaHelper::THUNK<Usable, &Usable::IsReusable>()));
	lua_setfield(l, -2, "IsReusable");
}

Usable::UsableStructType* Usable::GetUsableStruct()
{
	char* ptr = ((char*)GetEntityPointer()) + OFFSET_STRUCT;
	return reinterpret_cast<UsableStructType*>(ptr);
}

