#pragma once
#include "Entity.h"

class Light :
	public Entity
{
public:
	Light(EntityID id);
	Light(void* ptr);
	virtual ~Light();

	virtual void MakeLuaObject(lua_State* l);
	static void RegisterLua(lua_State* l, Logger* tmp);
	static const uint32_t VTABLE = 0x810524;

	bool OnOffSwitch();
	bool IsNightOnly();
	float GetRadius();
	int32_t GetRateChange();
	Vector3 GetColor1();
	std::string GetIntensityString();
	bool IsRepeating();
	bool IsUseColor2();
	Vector3 GetColor2();
	bool IsUseArc();
	int32_t GetArc();
	float GetHighBound();
	float GetLowBound();
	bool IsUseFog();
	float GetFogAlpha();
	int32_t GetIntensityStringPos();

private:
#pragma pack (push,1)
	typedef struct
	{
		float unk1[12];
		bool onoffswitch;
		bool nightonly;
		float radius;
		int32_t ratechange;
		float color1[3];
		wchar_t* intensitystring;
		bool repeating;
		bool usecolor2;
		float color2[3];
		bool usearc;
		int32_t arc;
		float highbound;
		float lowbound;
		bool usefog;
		float fogalpha;
		char unk2[10];
		int32_t intensitypos;
		wchar_t* unk3;
	} LightStructType;
#pragma pack(pop)
	LightStructType* GetStruct();
	static const uint32_t OFFSET_LIGHT_STRUCT = 0x1a5;
};


