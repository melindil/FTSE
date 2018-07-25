/* MIT License

Copyright (c) 2018 melindil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include<map>
#include <string>
#include "lua.hpp"
#include "Logger.h"

class Actor
{
public:
	Actor(void* entityptr);
	Actor(uint16_t id);
	~Actor();

	struct AlterTableLocation
	{
		uint32_t offset;
		uint32_t size;
	};
	typedef std::map<AlterTableLocation, int32_t> AlterTable;

	int16_t GetID() { return entity_id_; }
	void MakeLuaObject(lua_State* l);
	int32_t GetAttribute(std::string const& name, int table);
	void SetAttribute(std::string const& name, int table,int32_t value);
	void DisplayMessage(std::string const& msg);
	void ApplyBonus(AlterTable& alters, bool permanent);
	void RemoveBonus(AlterTable& alters, bool permanent);

	static Actor* GetActorByID(uint16_t id);
	static void RegisterLua(lua_State* l, Logger* tmp);

	// DummyClass is used whenever we call a FoT function which requires
	// the "this" pointer be set.  We override both the this pointer and
	// the function pointer when the call is made.
	class DummyClass
	{
	public:
		void Entity_ShowMessage(uint32_t, uint32_t) {}
		void AttributeTable_Constructor() {}
		void ApplyBuff(void* attributes, uint32_t unknown, float multiplier) {}
		void GetActorName(wchar_t** ptrtobuf, void* entity) {}

	};

private:
	void* GetEntityPointer();
	std::string GetActorName();

	typedef struct
	{
		void* entityptr;
		uint16_t unknown_1;
		uint16_t unknown_2;
	} EntityTableEntryType;

	static const uint32_t OFFSET_ENTITY_ID = 0x004;
	static const uint32_t OFFSET_ENTITY_NAME_PTR = 0x0222;
	static const uint32_t FXN_FOTHEAPALLOC = 0x6c4dd0;
	static const uint32_t FXN_ENTITY_SHOWMESSAGE = 0x5e6d20;
	static const uint32_t FXN_ATTRIBUTES_CONSTRUCTOR = 0x608d30;
	static const uint32_t FXN_ACTOR_APPLYBUFF = 0x56f300;
	static const uint32_t FXN_ACTOR_REMOVEBUFF = 0x56f510;
	static const uint32_t FXN_ACTOR_GETNAME = 0x64f5d0;
	static const uint32_t OBJECT_ACTOR_GETNAME = 0x8bd8b8;

	static const uint32_t OFFSET_ACTOR_ATTRIBUTES = 0x2a2;
	static const uint32_t OFFSET_ACTOR_TEMP_ATTRIBUTES = 0x914;
	static const uint32_t ATTRIBUTES_SIZE = 0x339;

	uint16_t entity_id_;

};

static bool operator<(Actor::AlterTableLocation lhs, Actor::AlterTableLocation rhs)
{
	return lhs.offset < rhs.offset;
}