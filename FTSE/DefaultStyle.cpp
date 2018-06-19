#include "DefaultStyle.h"



DefaultStyle::DefaultStyle(void* style, Logger* logger)
	: style_((DataStruct*)style), logger_(logger)
{
}


DefaultStyle::~DefaultStyle()
{
}

int l_setcoloroptionspages(lua_State* l);
int l_setcolorpaneltitles(lua_State* l);
int l_setcolordefaulttext(lua_State* l);
int l_setcolorhighlighttext(lua_State* l);
int l_setcolorbuffs(lua_State* l);
int l_setcolordebuffs(lua_State* l);
int l_setcolortags(lua_State* l);

void DefaultStyle::MakeLuaObject(lua_State* l)
{
	DefaultStyle** heptrptr = (DefaultStyle**)lua_newuserdata(l, sizeof(DefaultStyle*));
	*heptrptr = this;
	luaL_newmetatable(l, "DefaultStyle");
	lua_pushcfunction(l, l_setcoloroptionspages);
	lua_setfield(l, -2, "SetColorOptionsPages");
	lua_pushcfunction(l, l_setcolorpaneltitles);
	lua_setfield(l, -2, "SetColorPanelTitles");
	lua_pushcfunction(l, l_setcolordefaulttext);
	lua_setfield(l, -2, "SetColorDefaultText");
	lua_pushcfunction(l, l_setcolorhighlighttext);
	lua_setfield(l, -2, "SetColorHighlightText");
	lua_pushcfunction(l, l_setcolorbuffs);
	lua_setfield(l, -2, "SetColorBuffs");
	lua_pushcfunction(l, l_setcolordebuffs);
	lua_setfield(l, -2, "SetColorDebuffs");
	lua_pushcfunction(l, l_setcolortags);
	lua_setfield(l, -2, "SetColorTags");
	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setmetatable(l, -2);

}

void DefaultStyle::RegisterLua(lua_State* l)
{
	// No-op (for now)
}

void DefaultStyle::SetColorOptionsPages(float r, float g, float b)
{
	style_->colorOptionsPages[0] = r;
	style_->colorOptionsPages[1] = g;
	style_->colorOptionsPages[2] = b;
}
void DefaultStyle::SetColorPanelTitles(float r, float g, float b)
{
	style_->colorPanelTitles[0] = r;
	style_->colorPanelTitles[1] = g;
	style_->colorPanelTitles[2] = b;
}
void DefaultStyle::SetColorDefaultText(float r, float g, float b)
{
	style_->colorDefaultText[0] = r;
	style_->colorDefaultText[1] = g;
	style_->colorDefaultText[2] = b;

	float* CharSheetDefaultText = (float*)OFFSET_CHAR_SHEET_DEFAULT_TEXT;
	CharSheetDefaultText[0] = r;
	CharSheetDefaultText[1] = g;
	CharSheetDefaultText[2] = b;
}
void DefaultStyle::SetColorHighlightText(float r, float g, float b)
{
	style_->colorHighlightText[0] = r;
	style_->colorHighlightText[1] = g;
	style_->colorHighlightText[2] = b;

	float* CharSheetHighlightText = (float*)OFFSET_CHAR_SHEET_HIGHLIGHT_TEXT;
	CharSheetHighlightText[0] = r;
	CharSheetHighlightText[1] = g;
	CharSheetHighlightText[2] = b;
}

void DefaultStyle::SetColorBuffs(float r, float g, float b)
{
	float* CharSheetBuff = (float*)OFFSET_CHAR_SHEET_BUFFS;
	CharSheetBuff[0] = r;
	CharSheetBuff[1] = g;
	CharSheetBuff[2] = b;

}
void DefaultStyle::SetColorDebuffs(float r, float g, float b)
{
	float* CharSheetBuff = (float*)OFFSET_CHAR_SHEET_DEBUFFS;
	CharSheetBuff[0] = r;
	CharSheetBuff[1] = g;
	CharSheetBuff[2] = b;

}

void DefaultStyle::SetColorTags(float r, float g, float b)
{
	float* CharSheetTags = (float*)OFFSET_CHAR_SHEET_TAGS;
	CharSheetTags[0] = r;
	CharSheetTags[1] = g;
	CharSheetTags[2] = b;
}

int l_setcoloroptionspages(lua_State* l)
{
	DefaultStyle** ds = (DefaultStyle**)luaL_checkudata(l, 1, "DefaultStyle");
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3) && lua_isnumber(l, 4))
	{
		(*ds)->SetColorOptionsPages((float)lua_tonumber(l,2), (float)lua_tonumber(l,3), (float)lua_tonumber(l,4));
	}
	return 0;
}
int l_setcolorpaneltitles(lua_State* l)
{
	DefaultStyle** ds = (DefaultStyle**)luaL_checkudata(l, 1, "DefaultStyle");
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3) && lua_isnumber(l, 4))
	{
		(*ds)->SetColorPanelTitles((float)lua_tonumber(l, 2), (float)lua_tonumber(l, 3), (float)lua_tonumber(l, 4));
	}
	return 0;
}
int l_setcolordefaulttext(lua_State* l)
{
	DefaultStyle** ds = (DefaultStyle**)luaL_checkudata(l, 1, "DefaultStyle");
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3) && lua_isnumber(l, 4))
	{
		(*ds)->SetColorDefaultText((float)lua_tonumber(l, 2), (float)lua_tonumber(l, 3), (float)lua_tonumber(l, 4));
	}
	return 0;
}
int l_setcolorhighlighttext(lua_State* l)
{
	DefaultStyle** ds = (DefaultStyle**)luaL_checkudata(l, 1, "DefaultStyle");
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3) && lua_isnumber(l, 4))
	{
		(*ds)->SetColorHighlightText((float)lua_tonumber(l, 2), (float)lua_tonumber(l, 3), (float)lua_tonumber(l, 4));
	}
	return 0;
}
int l_setcolorbuffs(lua_State* l)
{
	DefaultStyle** ds = (DefaultStyle**)luaL_checkudata(l, 1, "DefaultStyle");
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3) && lua_isnumber(l, 4))
	{
		(*ds)->SetColorBuffs((float)lua_tonumber(l, 2), (float)lua_tonumber(l, 3), (float)lua_tonumber(l, 4));
	}
	return 0;
}
int l_setcolordebuffs(lua_State* l)
{
	DefaultStyle** ds = (DefaultStyle**)luaL_checkudata(l, 1, "DefaultStyle");
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3) && lua_isnumber(l, 4))
	{
		(*ds)->SetColorDebuffs((float)lua_tonumber(l, 2), (float)lua_tonumber(l, 3), (float)lua_tonumber(l, 4));
	}
	return 0;
}
int l_setcolortags(lua_State* l)
{
	DefaultStyle** ds = (DefaultStyle**)luaL_checkudata(l, 1, "DefaultStyle");
	if (lua_isnumber(l, 2) && lua_isnumber(l, 3) && lua_isnumber(l, 4))
	{
		(*ds)->SetColorTags((float)lua_tonumber(l, 2), (float)lua_tonumber(l, 3), (float)lua_tonumber(l, 4));
	}
	return 0;
}