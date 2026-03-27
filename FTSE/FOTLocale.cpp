#include "FOTLocale.h"

#include "Helpers.h"
#include "FOTString.h"

FOTLocale::FOTLocale(Logger* l)
	: logger_(l)
{
}


FOTLocale::~FOTLocale()
{
}

void FOTLocale::AddToDictionary(LocaleDictionaries dict_num, std::string const & key, std::string const & value)
{
	wchar_t* wvalue = Helpers::UTF8ToWcharFOTHeap(value, 1);

	auto exist_node = FindInDictionary(dict_num, key);
	if (exist_node)
	{
		FOTString destructme(exist_node->value);
		exist_node->value = wvalue;
		return;
	}

	typedef void(__thiscall *fxntype)(void*, wchar_t**, wchar_t**);
	auto fxn = (fxntype)FXN_ADD_DICTIONARY;
	wchar_t* wkey = Helpers::UTF8ToWcharFOTHeap(key, 1);
	if (dict_num == LocaleDictionaries::LOCALE_GAME_CHARACTERS)
	{
		(*fxn)((void*)&DICT_GLOBAL_PTR->dict_game_characters, &wkey, &wvalue);
	}
	else
	{
		int offset = (int)dict_num;
		(*fxn)((void*)(DICT_GLOBAL_PTR->dicts+offset), &wkey, &wvalue);
	}
}

FOTLocale::FOTLocaleTreeNode * FOTLocale::FindInDictionary(LocaleDictionaries dict_num, std::string const & key)
{
	FOTLocaleTreeNode* head;
	FOTString s_key(key);
	wchar_t* w_key = s_key.getraw();

	if (dict_num == LocaleDictionaries::LOCALE_GAME_CHARACTERS)
	{
		head = DICT_GLOBAL_PTR->dict_game_characters.dict_tree;
	}
	else
	{
		head = DICT_GLOBAL_PTR->dicts[dict_num].dict_tree;
	}

	head = head->parent;
	while (head->parent != nullptr)
	{
		int result = wcscmp(w_key, head->key);
		if (result == 0)
			return head;
		else if (result < 0)
			head = head->left;
		else
			head = head->right;
	}

	return nullptr;
}

std::string FOTLocale::GetDictionary(LocaleDictionaries dict_num, std::string const& key)
{
	auto node = FindInDictionary(dict_num, key);
	if (node)
	{
		return Helpers::WcharToUTF8(node->value);
	}
	return std::string();
}