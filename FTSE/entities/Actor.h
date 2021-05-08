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
#include "ActorStatus.h"
#include "AttributesTable.h"
#include "CombatMessage.h"

class Actor : public Entity
{
public:
	Actor(void* entityptr);
	Actor(EntityID id);
	virtual ~Actor();

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

	struct EffectListType
	{
		EffectListType* next;
		EffectListType* prev;
		EntityID entity_id;

	};


	typedef std::map<AlterTableLocation, int32_t> AlterTable;


	std::shared_ptr<Entity> GetInventory();
	int32_t GetAttribute(std::string const& name, int table);
	void SetAttribute(std::string const& name, int table,int32_t value);
	void GetField(lua_State* l, std::string const& name);
	void SetField(lua_State* l, std::string const& name);
	void ApplyBonus(AlterTable& alters, bool permanent);
	void RemoveBonus(AlterTable& alters, bool permanent);

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);

	bool TestFriendlyCrouched(Actor& tgt);
	int GetTeamReaction(Entity& tgt);

	std::pair<InventoryActionResult, std::shared_ptr<Entity>> EquipItem(std::shared_ptr<Entity> item, int slot);
	std::pair<InventoryActionResult, std::shared_ptr<Entity>> UnequipItem(int slot);
	std::shared_ptr<Entity> GetEquippedItem(int slot);

	void SwapFix();

	static const uint32_t VTABLE = 0x80c1d0;
private:
	std::string GetFieldString(std::string const& name);

	static const uint32_t FXN_FOTHEAPALLOC = 0x6c4dd0;
	static const uint32_t FXN_ATTRIBUTES_CONSTRUCTOR = 0x608d30;
	static const uint32_t FXN_ACTOR_APPLYBUFF = 0x56f300;
	static const uint32_t FXN_ACTOR_REMOVEBUFF = 0x56f510;

#pragma pack(push,1)
	typedef struct
	{
		/*
		OK, this is a long one ...
		*/

		bool deactivated;			// 0x01a5
		bool removeonexit;			// 0x01a6
		char unk1;					// 0x01a7
		int32_t gamblehate;			// 0x01a8
		int32_t npclevel;			// 0x01ac
		char unk2[0x15];			// 0x01b0
		void* unk3;					// 0x01c5
		void* unk4;					// 0x01c9
		void* unk5;					// 0x01cd
		char unk6;					// 0x01d1
		char unused1[3];			// 0x01d2
		void* kill_list;			// 0x01d5
		void* unk7;					// 0x01d9
		void* unk8;					// 0x01dd
		bool isturretarmed;			// 0x01e1
		char unk9;					// 0x01e2
		char unused2[3];			// 0x01e3
		EffectListType* effects_st;	// 0x01e6
		EffectListType* effects_end;// 0x01ea
		EffectListType* effects_cap;// 0x01ee
		int32_t guislot;			// 0x01f2
		char unused3[4];			// 0x01f6
		int32_t wantedstance;		// 0x01fa
		int32_t selectedstance;		// 0x01fe
		int32_t special_copy[8];	// 0x0202
		wchar_t* customname;		// 0x0222
		wchar_t* race;				// 0x0226
		wchar_t* dispname;			// 0x022a
		float unk11[15];			// 0x022e
		char unk12;					// 0x026a
		char unused4[3];			// 0x026b
		void* unk13;				// 0x026e
		void* unk14;				// 0x0272
		void* unk15;				// 0x0276
		char unk16[3];				// 0x027a
		ActorStatus actorstatus;	// 0x027d
		char primaryattr[0x339];	// 0x02a2
		char unkattr[0x339];		// 0x05db
		char tempattr[0x339];		// 0x0914
		char currattr[0x339];		// 0x0c4d
		char unk17[4];				// 0x0f86
		wchar_t* posture;			// 0x0f8a
		wchar_t* currentaction;		// 0x0f8e
		wchar_t* startposture;		// 0x0f92
		bool makedead;				// 0x0f96
		wchar_t* deathtype;			// 0x0f97
		char unused5;				// 0x0f9b
		bool alwaysfriend;			// 0x0f9c
		bool startburrowed;			// 0x0f9d
		EntityID unarmedleft;		// 0x0f9e
		int32_t unk18;				// 0x0fa2
		EntityID unarmedright;		// 0x0fa6
		int32_t unk19;				// 0x0faa
		float knockdownchance;		// 0x0fae
		bool injuries[9];			// 0x0fb2
		bool unconscious;			// 0x0fbb
		int32_t unconscioustime;	// 0x0fbc
		char unk20;					// 0x0fc0
		int32_t standheight;		// 0x0fc1
		uint64_t timesincerun;		// 0x0fc5
		uint64_t nextpoisonthink;	// 0x0fcd
		char unk21[8];				// 0x0fd5
		bool stunned;				// 0x0fdd
		uint64_t finishstun;		// 0x0fde
		bool immunefriendly;		// 0x0fe6
		bool havingturn;			// 0x0fe7
		bool overwatch;				// 0x0fe8
		int32_t xpreward;			// 0x0fe9
		int32_t reputation;			// 0x0fed
		int32_t rank;				// 0x0ff1
		int32_t missionscom;		// 0x0ff5
		int32_t startindex;			// 0x0ff9
		wchar_t* randomspeech;		// 0x0ffd
		wchar_t* clickspeech;		// 0x1001
		bool enablescriptevents;	// 0x1005
		EntityID inventory;			// 0x1006
		char unk22[4];				// 0x100a
		EntityID slotequip[3];		// 0x100e, hand1/hand2/armor
		EntityID hand1overrideequip;// 0x101a
		EntityID hand2overrideequip;// 0x101e
		EntityID arnoroverrideequip;// 0x1022
		bool hand1override;			// 0x1026
		bool hanad2override;		// 0x1027
		bool armoroverride;			// 0x1028
		char handselected;			// 0x1029
		char unk23[0x9];			// 0x102a
		EntityID sneaklink;			// 0x1033
		char unk24[0x1c];			// 0x1037
		bool isgeneral;				// 0x1053
		bool isrecruitmaster;		// 0x1054
		bool isquartermaster;		// 0x1055
		bool canbarter;				// 0x1056
		bool barterinbosscript;		// 0x1057
		bool cangamble;				// 0x1058
		bool istippablebrahmin;		// 0x1059
		char unk25[0x2c];			// 0x105a
		bool lonerapplied;			// 0x1086
		bool leaderapplied;			// 0x1087
		bool teamplayerapplied;		// 0x1088
		bool vatskinapplied;		// 0x1089
		uint64_t nextlongtick;		// 0x108a
		float attackdirx;			// 0x1092
		float attackdiry;			// 0x1096
		float attackdirz;			// 0x109a
		char unk26[0x13];			// 0x109e
		CombatMessage cmsg;			// 0x10b1
		char unk27[0x60];			// 0x1100

	} ActorStructType;
#pragma pack(pop)

	ActorStructType* GetStruct();
	static const uint32_t OFFSET_ACTOR_STRUCT = 0x1a5;

	static const uint32_t OFFSET_ACTOR_ATTRIBUTES = 0x2a2;
	static const uint32_t OFFSET_ACTOR_TEMP_ATTRIBUTES = 0x914;
	static const uint32_t ATTRIBUTES_SIZE = 0x339;
	static const uint32_t VTABLE_EQUIP_ITEM = 0x4f8;
	static const uint32_t VTABLE_UNEQUIP_SLOT = 0x4fc;
	static const uint32_t VTABLE_SWITCH_HAND = 0x538;

	static const std::map<std::string, ActorFOTOffset> offsets;


};

static bool operator<(Actor::AlterTableLocation lhs, Actor::AlterTableLocation rhs)
{
	return lhs.offset < rhs.offset;
}