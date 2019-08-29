#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include "Logger.h"
#include "Helpers.h"


struct lua_State;

class Entity
{
public:
	Entity(void* ptr);
	Entity(uint16_t id);
	virtual ~Entity();

	virtual int16_t GetID() { return ((EntityHeaderType*)entity_ptr_)->id; }
	virtual void MakeLuaObject(lua_State* l);

	static std::shared_ptr<Entity> GetEntityByID(uint16_t id);
	static std::shared_ptr<Entity> GetEntityByPointer(void* ptr);

	virtual std::string GetEntityName();
	virtual bool isAlive();
	virtual void ShotAtMissed(void* cmsg);
	virtual float GetMaxRange(Entity& holder);
	virtual std::string GetTag();
	virtual std::string GetEntitySubType();
	virtual int32_t GetMinEffectiveDamage(Entity& holder);
	virtual int32_t GetMaxEffectiveDamage(Entity& holder);


	static void RegisterLua(lua_State* l, Logger* tmp);
	static void SetLuaSubclass(lua_State* l);
	virtual void* GetEntityPointer()
	{
		return entity_ptr_;
	}
	virtual Vector3 GetLocation();
	virtual uint16_t GetSeqnum();
	virtual uint32_t GetVtable();
	virtual uint32_t GetVtableFxn(uint32_t offset);
	float GetHeight();
	float GetBoundingBoxSum();
	void DisplayMessage(std::string const& msg);

protected:
#pragma pack(push,1)
	typedef struct
	{
		uint32_t vtable;
		uint16_t id;
		uint16_t motileflags;
		uint32_t boundbox[6];
		int32_t imagerect[4];
		char pad2[0x6a];
		float pos[3];
		char pad3[0xc];
		char unkflags;
		char basecolor[0x14];
		char skincolor[0x14];
		char haircolor[0x14];
		char teamcolor[0x14];
		int16_t unknown2;
		wchar_t* spritename;
		wchar_t* animname;
		char*    animnameascii;
		char unkflags2[3];
		char unused1[3];
		int16_t* unknown3;
		int16_t* unknown4;
		int16_t* unknown5;
		char unkflag3;
		char unused2[3];
		wchar_t* unknown6;
		wchar_t* unknown7;
		wchar_t* unknown8;
		int16_t sequenceindex;
		int16_t sequenceframe;
		int16_t sequencedelay;
		int16_t animationframe;
		char animationrotation;
		char unknown9[6];
		uint32_t vtable_entitystructstream;
		uint16_t entityseqnum;
		uint16_t entityid_2;
		wchar_t* entitytype;
		wchar_t* customname;
		wchar_t* internalname;
		wchar_t* subtype;
		char unknown10[9];
		int32_t playerindex;
		char unknown11;
		int32_t gsid;
		float dofadedestination;
		char unknown12;
		float nextenginethink;
		uint32_t nextgamethink;
		char unknown13[8];
		uint16_t attachedseqnum;
		uint16_t attachedid;
		uint32_t seed;
		bool xpflag;
		bool broken;
		bool trap;
		char unknown14[7];
		uint16_t vehicleseqnum;
		uint16_t vehicleid;
		uint16_t unknown15;
		wchar_t* entitypath;
		uint32_t unknown16;
		wchar_t* tag;
	} EntityHeaderType;
#pragma pack(pop)

	static const uint32_t OBJECT_ENTITY_GETNAME = 0x8bd8b8;
	static const uint32_t FXN_ENTITY_GETNAME = 0x64f5d0;
	static const uint32_t FXN_ENTITY_SHOWMESSAGE = 0x5e6d20;
	static const uint32_t VTABLE_OFFSET_ISALIVE = 0x480;
	static const uint32_t VTABLE_OFFSET_SHOTATMISSED = 0x4ac;
	static const uint32_t VTABLE_OFFSET_MINDAMAGE = 0x664;
	static const uint32_t VTABLE_OFFSET_MAXDAMAGE = 0x668;
	static const uint32_t VTABLE_OFFSET_GETRANGE = 0x6e0;

	void* entity_ptr_;
};

