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
	DefaultStyle(Logger* logger);
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
		void* vftable;				// 0x00
		uint32_t unknown;			// 0x04
		float blinkRate;			// 0x08
		float actionDelay;			// 0x0c
		float repeatRate;			// 0x10
		void* StyleObjects[11];		// 0x14 - 0x3c
		float colorTBD[3];			// 0x40 - 0x48
		float colorOptionsPages[3];	// 0x4c - 0x54
		float colorPanelTitles[3];	// 0x58 - 0x60
		float colorDefaultText[3];	// 0x64 - 0x6c
		float colorHighlightText[3];// 0x70 - 0x78
		float colorMediumGray[3];	// 0x7c - 0x84
		float colorDarkGray[3];		// 0x88 - 0x90
		float colorVeryDarkGray[3];	// 0x94 - 0x9c
		float colorBlack[3];		// 0xa0 - 0xa8
		float colorUnknown[3];		// 0xac - 0xb4
		uint32_t unused[4];			// 0xb8 - 0xc4
		float colorGreen[3];		// 0xc8 - 0xd0
		float colorUnknown2[3];		// 0xd4 - 0xdc
		float colorUnknown3[3];		// 0xe0 - 0xe8
		float colorLightGray[3];	// 0xec - 0xf4
		float colorGreen2[3];		// 0xf8 - 0x100
		float colorUnknown4[3];		// 0x104 - 0x10c
		void* image;				// 0x110
		float wallpaperTint;		// 0x114
		void* unused2[3];			// 0x118 - 0x120
	};

	struct FOTLinkedListDefaultStyle
	{
		FOTLinkedListDefaultStyle* next;
		FOTLinkedListDefaultStyle* prev;
		DataStruct* DefaultStyleObject;
	};
	
	struct StyleManager
	{
		void* vftable;
		int32_t unk;
		FOTLinkedListDefaultStyle* style_list;
	};

	DataStruct* GetDataStruct() { return style_; }

private:

	static const uint32_t OFFSET_CHAR_SHEET_DEFAULT_TEXT = 0x8bc2c0;
	static const uint32_t OFFSET_CHAR_SHEET_HIGHLIGHT_TEXT = 0x8bc220;
	static const uint32_t OFFSET_CHAR_SHEET_BUFFS = 0x8bc290;
	static const uint32_t OFFSET_CHAR_SHEET_DEBUFFS = 0x8bc240;
	static const uint32_t OFFSET_CHAR_SHEET_TAGS = 0x8bc2d0;

	static const uint32_t OFFSET_STYLEMANAGER = 0x8be4c8;

	DataStruct* style_;
	Logger* logger_;
};

