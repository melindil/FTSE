#include "Book.h"

#include "LuaHelper.h"
#include "Helpers.h"

Book::Book(EntityID id)
	: Collectable(id)
{
}

Book::Book(void * ptr)
	: Collectable(ptr)
{
}

Book::~Book()
{
}

void Book::MakeLuaObject(lua_State * l)
{
	lua_newtable(l);
	lua_pushinteger(l, GetID());
	lua_setfield(l, -2, "id");
	lua_getglobal(l, "BookMetaTable");
	lua_setmetatable(l, -2);
}

Book::BookStructType * Book::GetStruct()
{
	return (BookStructType*)(((uint32_t)GetEntityPointer()) + OFFSET_BOOK_STRUCT);
}

void Book::RegisterLua(lua_State * l, Logger * tmp)
{
	luaL_newmetatable(l, "BookMetaTable");
	Collectable::SetLuaSubclass(l);

	lua_pushcfunction(l, (LuaHelper::THUNK<Book, &Book::GetSkillAffected>()));
	lua_setfield(l, -2, "GetSkillAffected");

	lua_pushstring(l, "Book");
	lua_setfield(l, -2, "ClassType");

	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setglobal(l, "BookMetaTable");
}

std::string Book::GetSkillAffected()
{
	return Helpers::WcharToUTF8(GetStruct()->skill);
}
