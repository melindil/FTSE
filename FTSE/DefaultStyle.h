#pragma once

#include<map>
#include <string>
#include <stdint.h>
#include "lua.hpp"
#include "Logger.h"

class DefaultStyle
{
public:
	DefaultStyle(void* style, Logger* logger);
	~DefaultStyle();

	void MakeLuaObject(lua_State* l);

	static void RegisterLua(lua_State* l);
	void SetColorOptionsPages(float r, float g, float b);
	void SetColorPanelTitles(float r, float g, float b);
	void SetColorDefaultText(float r, float g, float b);
	void SetColorHighlightText(float r, float g, float b);

	void SetColorBuffs(float r, float g, float b);
	void SetColorDebuffs(float r, float g, float b);
	void SetColorTags(float r, float g, float b);

	struct DataStruct
	{
		void* vftable;
		uint32_t unknown;
		float blinkRate;
		float actionDelay;
		float repeatRate;
		void* StyleObjects[11];
		float colorTBD[3];
		float colorOptionsPages[3];
		float colorPanelTitles[3];
		float colorDefaultText[3];
		float colorHighlightText[3];
		float colorMediumGray[3];
		float colorDarkGray[3];
		float colorVeryDarkGray[3];
		float colorBlack[3];
		float colorUnknown[3];
		uint32_t unused[4];
		float colorGreen[3];
		float colorUnknown2[3];
		float colorUnknown3[3];
		float colorLightGray[3];
		float colorGreen2[3];
		float colorUnknown4[3];
		void* image;
		float wallpaperTint;
		void* unused2[3];
	};

private:

	static const uint32_t OFFSET_CHAR_SHEET_DEFAULT_TEXT = 0x8bc2c0;
	static const uint32_t OFFSET_CHAR_SHEET_HIGHLIGHT_TEXT = 0x8bc220;
	static const uint32_t OFFSET_CHAR_SHEET_BUFFS = 0x8bc290;
	static const uint32_t OFFSET_CHAR_SHEET_DEBUFFS = 0x8bc240;
	static const uint32_t OFFSET_CHAR_SHEET_TAGS = 0x8bc2d0;

	DataStruct* style_;
	Logger* logger_;
};

