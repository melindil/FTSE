#pragma once
#include "Entity.h"
#include "ActorStatus.h"
#include "CombatMessage.h"
#include "Helpers.h"

#include <memory>
using std::shared_ptr;

class Vehicle :
	public Entity
{
public:
	Vehicle(EntityID id);
	Vehicle(void* ptr);
	virtual ~Vehicle();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static void SetLuaSubclass(lua_State* l);
	static const uint32_t VTABLE = 0x80db28;

	int32_t GetAttribute(std::string const& name);
	int32_t GetHP();

	shared_ptr<Entity> GetDriver();
	shared_ptr<Entity> GetGunner();
	shared_ptr<Entity> GetController();
	shared_ptr<Entity> GetPassenger(int num);
	int GetNumPassengers();
	int GetTotalOccupants();
	int GetMaxPassengers();
	int GetMaxTotalOccupants();
	Vector3 GetExitPoint();
	shared_ptr<Entity> GetRepairObject();
	bool NeedsRepairObject();
	bool HasWeapon();
	shared_ptr<Entity> GetVehicleWeapon();
	shared_ptr<Entity> GetDriverLeftHandItem();
	shared_ptr<Entity> GetGunnerLeftHandItem();
	shared_ptr<Entity> GetInventory();
	bool IsRunSpeed();
	bool IsReverse();
	std::string GetCurrentAction();
	Vector3 GetDestination();
	bool IsDestinationReached();
	bool IsTurnReached();
	int32_t GetTurnRate();
	float GetDamageMultiplier();
	int32_t GetCriticalChance();
	float GetAcceleration();
	float GetMaxSlowSpeed();
	float GetMaxHighSpeed();
	Vector3 GetVelocity();
	Vector3 GetDimensions();
	float GetEndTurnTime();
	std::string GetButtonSprite();
	bool IsHavingTurn();


private:
#pragma pack(push,1)
	typedef struct
	{
		wchar_t* material;
		float unkbox[6];
		int32_t unk2;
		EntityID driver;
		EntityID controller;
		EntityID gunner;
		int32_t maxpassengers;
		EntityID passenger[7];
		float exitx;
		float exity;
		float exitz;
		char attributes[0x339];
		ActorStatus actorstatus;
		EntityID repairobject;
		bool repairneeded;
		bool hasweapon;
		EntityID weapon;
		EntityID driver_lefthand;
		EntityID gunner_lefthand;
		EntityID inventory;
		bool runspeed;
		bool inreverse;
		int32_t unk3;
		int32_t unk4;
		wchar_t* action;
		float destx;
		float desty;
		float destz;
		bool unk5;
		char unused1[3];
		void* unk_list;
		int32_t unk6;
		bool unk7;
		char unused2[3];
		void* unk_list2;
		int32_t unk8;
		bool destreached;
		int32_t turnrate;
		bool turnreached;
		float damagemult;
		int32_t critchance;
		float acceleration;
		float maxspeed;
		float maxrunspeed;
		float velx;
		float vely;
		float velz;
		float width;
		float height;
		float length;
		float endturntime;
		CombatMessage cmsg;
		wchar_t* buttonsprite;
		bool havingturn;

	} VehicleStructType;
#pragma pack(pop)
	VehicleStructType* GetStruct();
	static const uint32_t OFFSET_VEHICLE_STRUCT = 0x1a5;
};

