#pragma once
#include "Entity.h"

class Controller :
	public Entity
{
public:
	Controller(EntityID id);
	Controller(void* ptr);
	virtual ~Controller();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x80a778;

	std::string GetControllerType();
	std::string GetMoveTargetTag();
	int32_t GetMoveTypePriority();
	bool IsGroupMove();
	bool IsRandMove();
	std::string GetWorkingZone();
	std::string GetRetreatZone();
	int32_t GetAggression();
	int32_t GetCommunicate();
	int32_t GetMobility();
	std::shared_ptr<Entity> GetStartWaypoint();
	std::shared_ptr<Entity> GetFollowTarget();
	bool IsNeverLeaveZone();
	Vector3 GetControllerDir();

private:
	/*
 	*/
#pragma pack (push,1)
	typedef struct
	{
		char unknown1;
		char unused1[3];
		void* unknown_list_2;
		int32_t unknown3;
		wchar_t* controllertype;
		wchar_t* movetargettag;
		wchar_t* unknown4;
		int32_t movetypepriority;
		bool groupmove;
		bool randmove;
		char unknown5[0x24];
		wchar_t* workingzone;
		char unknown6[0x26];
		wchar_t* retreatzone;
		int16_t unknown7;
		int32_t aggression;
		int32_t communicate;
		int32_t mobility;
		EntityID startwaypoint;
		EntityID followtarget;
		char unknown8[0x38];
		bool neverleavezone;
		char unknown9[0x08];
		float dir[3];
	} ControllerStructType;
#pragma pack(pop)
	ControllerStructType* GetStruct();
	static const uint32_t OFFSET_CONTROLLER_STRUCT = 0x1a5;

};

