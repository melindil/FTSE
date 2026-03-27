#pragma once

#include "Logger.h"

class FOTLocale
{
public:
	FOTLocale(Logger* l);
	~FOTLocale();

	enum LocaleDictionaries
	{
		LOCALE_GUI_GUITEXT,
		LOCALE_GAME_REASON,
		LOCALE_GAME_COMBAT,
		LOCALE_GAME_MISC,
		LOCALE_GAME_AMMO,
		LOCALE_GAME_WEAPON,
		LOCALE_GAME_ITEMS,
		LOCALE_GAME_BADWORDS,
		LOCALE_GUI_HELPTEXT,
		LOCALE_GAME_TAUNTS,
		LOCALE_GAME_CHARACTERS
	};

	struct FOTLocaleTreeNode
	{
		FOTLocaleTreeNode* left;
		FOTLocaleTreeNode* parent;
		FOTLocaleTreeNode* right;
		wchar_t* key;
		wchar_t* value;
	};

	void AddToDictionary(LocaleDictionaries dict_num, std::string const& key, std::string const& value);
	FOTLocaleTreeNode* FindInDictionary(LocaleDictionaries dict_num, std::string const& key);

	std::string GetDictionary(LocaleDictionaries dict_num, std::string const& key);

private:
	Logger* logger_;


	struct FOTLocaleDictionary
	{
		int32_t unk1;
		FOTLocaleTreeNode* dict_tree;
		int32_t unk2;
		int32_t unk3;
		void* some_ptr;
	};
	struct FOTLocaleManagerStruct
	{
		FOTLocaleDictionary dicts[10];
		wchar_t* LocaleDirName;
		FOTLocaleDictionary dict_game_characters;
	};

	const FOTLocaleManagerStruct* DICT_GLOBAL_PTR = (const FOTLocaleManagerStruct*)0x8bd8b8;
	const uint32_t FXN_ADD_DICTIONARY = 0x703260;

};

