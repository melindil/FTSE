#pragma once
#include "Entity.h"

class BaseAI :
	public Entity
{
public:
	BaseAI(EntityID id);
	BaseAI(void* ptr);
	virtual ~BaseAI();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x809ea8;

	std::string GetAction();
	std::string GetNature();
	std::string GetTactic();
	bool UsesCover();
	bool UsesSneak();
	bool IsSleeping();
	std::string GetItemTag();
	std::string GetTaunts();
	std::shared_ptr<Entity> GetCurrentTarget();
	std::shared_ptr<Entity> GetController();
	bool IsSuppressed();
	bool IsAttemptSuppressed();


private:
	/*
1a5 unkflag bool 1a6 uninit char[3] 1a9 vector of actions? int32*3
   1b5 unkflag bool 1b6 uninit char[3] 1b9 vector of actions? int32*3
   1c5 "MoveWander" str
   1c9 nature str 1cd unknown (0) int32 1d1 tactic string 1d5 ? int
   1d9 usecover bool 1da usesneak bool 1db sleeping bool
   1dc itemtag str 1e0 taunts str 1e4 uninit char 1e5 currenttarget entity
   1e9 ? int 1ed controller ent 1f1 ?? float 1f5 thinkrate float
   1f9 ?? float 1fd pathattempts int
   201 highestdesire float 205 targetrating float? 209 ? float
   20c unknown? char[0x25] (TODO: IS THIS OFF BY ONE?)
   231 blockobj ? (int)
   235 ? (int)
   239 suppressed.bool 23a.attemptsuppressed bool
   23b lastammo int? 23f ? char[0x14] 253 vislist VECTOR
   25f newthink uint64 	*/
#pragma pack (push,1)
	typedef struct
	{
		bool unkflag1;
		char unused1[3];
		wchar_t** actions_start;
		wchar_t** actions_end;
		wchar_t** actions_cap;
		bool unkflag2;
		char unused2[3];
		wchar_t** other_actions_start;
		wchar_t** other_actions_end;
		wchar_t** other_actions_cap;
		wchar_t* currentaction;
		wchar_t* nature;
		int32_t unknown3;
		wchar_t* tactic;
		int32_t unknown4;
		bool usecover;
		bool usesneak;
		bool sleeping;
		wchar_t* itemtag;
		wchar_t* taunts;
		char unused3;
		EntityID currenttarget;
		int32_t unknown5;
		EntityID controller;
		float unknown6;
		float thinkrate;
		float unknown7;
		int pathattempts;
		float highestdesire;
		float targetrating;
		float unknown8;
		char unknown9[0x25];
		int32_t blockobj;
		int32_t unknown10;
		bool suppressed;
		bool attemptsuppressed;
		int32_t lastammo;
		char unknown11[0x14];
		uint32_t* vislist_start;
		uint32_t* vislist_end;
		uint32_t* vislist_cap;
		uint64_t newthink;
	} BaseAIStructType;
#pragma pack(pop)
	BaseAIStructType* GetStruct();
	static const uint32_t OFFSET_BASEAI_STRUCT = 0x1a5;

};
