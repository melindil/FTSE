#pragma once
#include "Entity.h"

class EffectSpawn :
	public Entity
{
public:
	EffectSpawn(EntityID id);
	EffectSpawn(void* ptr);
	virtual ~EffectSpawn();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x80fcc8;

	std::string GetSprite();
	std::string GetSequence();
	std::string GetLightAttach();
	std::string GetWavSound();
	int32_t GetRandomDelay();
	bool IsOn();

private:
	/*
	1a5 sprite wchar 1a9 sequence wchar
   1ad lightattach wchar 1b1 wavsound wchar 1b5 randomdelay int
   1b9 on bool	*/
#pragma pack (push,1)
	typedef struct
	{
		wchar_t* sprite;
		wchar_t* sequence;
		wchar_t* lightattach;
		wchar_t* wavsound;
		int32_t randomdelay;
		bool on;
	} EffectSpawnStructType;
#pragma pack(pop)
	EffectSpawnStructType* GetStruct();
	static const uint32_t OFFSET_EFFECTSPAWN_STRUCT = 0x1a5;

};
