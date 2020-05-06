#pragma once
#include "Entity.h"

class Waypoint :
	public Entity
{
public:
	Waypoint(EntityID id);
	Waypoint(void* ptr);
	virtual ~Waypoint();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x81612c;

	float GetPauseTime();
	int32_t GetWaypointIndex();
	std::string GetNextWaypointList();
	std::string GetAction();
	bool IsRepeat();
	bool IsUseDirection();

private:
	/*
1a5 pausetime float 1a9 waypointindex int
	1ad nextwaypointlist wchar (csv) 1b1 action wchar 1b5 repeat bool
	1b6 usedirection bool 	*/
#pragma pack (push,1)
	typedef struct
	{
		float pausetime;
		int32_t waypointindex;
		wchar_t* nextwaypointlist;
		wchar_t* action;
		bool repeat;
		bool usedirection;
	} WaypointStructType;
#pragma pack(pop)
	WaypointStructType* GetStruct();
	static const uint32_t OFFSET_WAYPOINT_STRUCT = 0x1a5;

};

