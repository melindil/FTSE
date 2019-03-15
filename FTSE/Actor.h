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
#include "Helpers.h"
#include "Entity.h"

class Actor : public Entity
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

	enum class FieldType
	{
		INVALID,
		BOOLEAN,
		INTEGER,
		FLOAT,
		FLOATVECTOR,
		COLOR,
		WCHAR_STRING,
		SHORT
	};

	struct ActorFOTOffset
	{
		uint32_t offset;
		FieldType type;
	};



	typedef std::map<AlterTableLocation, int32_t> AlterTable;



	int32_t GetAttribute(std::string const& name, int table);
	void SetAttribute(std::string const& name, int table,int32_t value);
	void GetField(lua_State* l, std::string const& name);
	void SetField(lua_State* l, std::string const& name);
	void DisplayMessage(std::string const& msg);
	void ApplyBonus(AlterTable& alters, bool permanent);
	void RemoveBonus(AlterTable& alters, bool permanent);

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);

	bool TestFriendlyCrouched(Actor& tgt);
	int GetTeamReaction(Actor& tgt);
	float GetHeight();

	static const uint32_t VTABLE = 0x80c1d0;
private:
	std::string GetFieldString(std::string const& name);
	float GetBoundingBoxSum();

	typedef struct
	{
		void* entityptr;
		uint16_t unknown_1;
		uint16_t unknown_2;
	} EntityTableEntryType;


	static const uint32_t OFFSET_ENTITY_NAME_PTR = 0x0222;
	static const uint32_t FXN_FOTHEAPALLOC = 0x6c4dd0;
	static const uint32_t FXN_ENTITY_SHOWMESSAGE = 0x5e6d20;
	static const uint32_t FXN_ATTRIBUTES_CONSTRUCTOR = 0x608d30;
	static const uint32_t FXN_ACTOR_APPLYBUFF = 0x56f300;
	static const uint32_t FXN_ACTOR_REMOVEBUFF = 0x56f510;



	static const uint32_t OFFSET_ACTOR_ATTRIBUTES = 0x2a2;
	static const uint32_t OFFSET_ACTOR_TEMP_ATTRIBUTES = 0x914;
	static const uint32_t ATTRIBUTES_SIZE = 0x339;

	static const std::map<std::string, ActorFOTOffset> offsets;


};

static bool operator<(Actor::AlterTableLocation lhs, Actor::AlterTableLocation rhs)
{
	return lhs.offset < rhs.offset;
}