#include "FTImprover.h"

#include <iostream>
#include <string>

#include <Windows.h>

#include "Helpers.h"
#include "Entity.h"
#include "Actor.h"
#include "Armour.h"

#include <Windows.h>

// static
FTI_GotoMissionStaticStruct FTImprover::static_struct;

FTImprover::FTImprover()
	: state(OUTSIDE), configtype(CONFIG), waiting_for_param_name(true), param_name(), param_value(),  param_on(0), ammo_types(), ammo_types_active(false), impact_sounds(), impact_sounds_active(false),
	animation_names(), animation_names_active(false), weapon_item_types(), weapon_item_types_active(false), weapon_sound_types(), weapon_sound_types_active(false),
	attack_mode_names(), attack_mode_names_active(false), ammo_variants(), ammo_variants_active(false), apct(), apct_active(false), armour_types(), armour_types_active(false),
	include_default_armour_types(false),include_default_armour_rules(true),armour_rules(),sound_rules(),enabled_features(),logger_(),disabled(false)
{

	static_struct.unk_zero = 0;
	static_struct.mylist = (FTI_GotoMissionLinkedList*)Helpers::FOTHeapAlloc(sizeof(FTI_GotoMissionLinkedList));
	static_struct.mylist->next = static_struct.mylist;
	static_struct.mylist->prev = static_struct.mylist;
	static_struct.ctr = 0;
	static_struct.flags_maybe = 1;
}


FTImprover::~FTImprover()
{
}

void FTImprover::Emit(std::string & token)
{
	const char* WHITESPACE = " \t\n\r\f\v";
	if (state != INSIDE_CONFIG && state != INSIDE_2)
	{
		token.erase(token.find_last_not_of(WHITESPACE) + 1);
		token.erase(0, token.find_first_not_of(WHITESPACE));
	}
	//std::cout << "Adjusted Token: " << token << std::endl;

	if (token == "{")
	{
		if (configtype == CONFIG && state == OUTSIDE)
			state = INSIDE_CONFIG;
		else if (state == OUTSIDE)
			state = INSIDE_1;
		else if (state == INSIDE_1)
			state = INSIDE_2;
		else throw std::runtime_error("Syntax error: Too many open braces");
	}
	else if (token == "}")
	{
		if (state == INSIDE_CONFIG)
		{
			state = OUTSIDE;
			if (!waiting_for_param_name)
			{
				HandleFeatureFlag(param_name, param_value);
				waiting_for_param_name = true;
			}
		}
		else if (state == INSIDE_2)
		{
			state = INSIDE_1;
			param_on = 0;
		}
		else if (state == INSIDE_1)
		{
			state = OUTSIDE;
			waiting_for_param_name = true;
			configtype = CONFIG;
		}
		else throw std::runtime_error("Syntax error: Too many close braces");
	}
	else if (token == "=")
	{
		waiting_for_param_name = false;
	}
	else if (state == INSIDE_CONFIG)
	{
		if (waiting_for_param_name == true)
			param_name = token;
		else param_value = token;
	}
	else if (state == OUTSIDE)
	{
		if (token == "AmmoTypes")
		{
			configtype = AMMOTYPES;
			ClearAmmoTypes();
		}
		else if (token == "AmmoVariant")
		{
			configtype = AMMOVARIANTS;
			ClearAmmoVariants();
		}
		else if (token == "AnimNames")
		{
			configtype = ANIMATIONNAMES;
			ClearAnimationNames();
		}
		else if (token == "APCT")
		{
			configtype = APCTSTRUCTURES;
			ClearAPCT();
		}
		else if (token == "ArmourTypes")
		{
			configtype = ARMOURTYPES;
			ClearArmourTypes();
		}
		else if (token == "AttackModeNames")
		{
			configtype = ATTACKMODENAMES;
		}
		else if (token == "ImpactSounds")
		{
			configtype = IMPACTSOUNDS;
			ClearImpactSounds();
		}
		else if (token == "WST")
		{
			configtype = WEAPONSOUNDTYPES;
			ClearWeaponSoundTypes();
		}
		else if (token == "WIT")
		{
			configtype = WEAPONITEMTYPES;
			ClearWeaponItemTypes();
		}
	}
	else
	{
		if (state == INSIDE_2)
		{
			if (configtype == AMMOTYPES && token  != "0")
			{
				AddAmmoType(token);
			}
			else if (configtype == ANIMATIONNAMES)
			{
				AddAnimationName(token);
			}
			else if (configtype == ATTACKMODENAMES)
			{
				// No-op the attack modes - not supported yet
			}
			else if (configtype == IMPACTSOUNDS)
			{
				AddImpactSound(token);
			}
			else if (configtype == WEAPONITEMTYPES)
			{
				AddWeaponItemType(token);
			}
			else if (configtype == WEAPONSOUNDTYPES)
			{
				AddWeaponSoundType(token);
			}
			else if (configtype == AMMOVARIANTS)
			{
				if (token == ";") param_on++;
				else if (param_on == 0)
					param_name = token;
				else if (param_on == 1)
					int_param_1 = std::stoi(token);
				else if (param_on == 2)
				{
					float dmg_mult = 1.0f + ((float)int_param_1) / 100.0f;
					int this_param = std::stoi(token);
					float resist_mult = 1.0f - ((float)this_param) / 100.0f;
					AddAmmoVariant(param_name, dmg_mult, resist_mult);
				}
			}
			else if (configtype == APCTSTRUCTURES)
			{
				if (token == ";") param_on++;
				else if (param_on == 0)
					param_name = token;
				else if (param_on == 1)
					int_param_1 = std::stoi(token);
				else if (param_on == 2)
					int_param_2 = std::stoi(token);
				else if (param_on == 3 && param_name != "0")
				{
					AddAPCT(param_name, int_param_1, int_param_2, std::stoi(token));
				}
			}
			else if (configtype == ARMOURTYPES)
			{
				if (token == ";") param_on++;
				else if (param_on == 0)
					param_name = token;
				else if (param_on == 1)
					param_value = token;
				else if (param_on == 2)
					int_param_1 = std::stoi(token);
				else if (param_on == 3)
					param_value_2 = token;
				else if (param_on == 4)
				{
					AddArmourTypeFTI(param_name, param_value, int_param_1, param_value_2);
				}
			}
		}
	}
}

void FTImprover::Parse(std::vector<char> const & buf)
{
	try
	{

		const char EOBuf = 0;
		enum
		{
			OUTSIDE,
			IN_LINE_COMMENT
		} state;

		std::string curr_string;

		// Parse byte-at-a-time
		// Use buffer of 2 bytes - second is a lookahead for certain combinations
		char curr_buf[2];
		size_t offset = 0;

		if (buf.size() < 2)
			return;

		state = OUTSIDE;

		curr_buf[0] = (buf.size() > 0) ? buf[0] : EOBuf;
		curr_buf[1] = (buf.size() > 1) ? buf[1] : EOBuf;
		offset = 2;

		while (curr_buf[0] != EOBuf)
		{
			if (curr_buf[0] == '/' && curr_buf[1] == '/')	// Comment start
			{
				state = IN_LINE_COMMENT;

				curr_buf[1] = (buf.size() > offset) ? buf[offset] : EOBuf;
				offset++;
			}
			else if (curr_buf[0] == '\n')	// End any pending comment
			{
				state = OUTSIDE;
			}
			else if (state == OUTSIDE)
			{
				if (curr_buf[0] == '\\' && curr_buf[1] != 0)		// Allow backslashing any character
				{
					curr_string.push_back(curr_buf[1]);
					curr_buf[1] = (buf.size() > offset) ? buf[offset] : EOBuf;
					offset++;
				}
				else if (curr_buf[0] == ';' && curr_buf[1] == ';')		// Handle double semicolon as part of token
				{
					curr_string.push_back(curr_buf[1]);
					curr_buf[1] = (buf.size() > offset) ? buf[offset] : EOBuf;
					offset++;
				}
				else if (curr_buf[0] == '=' || curr_buf[0] == '{' || curr_buf[0] == '}' || curr_buf[0] == ';')	// Token split characters
				{
					// End of token
					if (curr_string != "")
						Emit(curr_string);
					curr_string.resize(0);
					curr_string.push_back(curr_buf[0]);
					Emit(curr_string);
					curr_string.resize(0);
				}
				else
				{
					curr_string.push_back(curr_buf[0]);
				}
				if (curr_buf[1] == 0 && curr_string != "") // EOF
				{
					Emit(curr_string);
					curr_string.resize(0);
				}
			}

			curr_buf[0] = curr_buf[1];
			curr_buf[1] = (buf.size() > offset) ? buf[offset] : EOBuf;
			offset++;
		}
	}
	catch (std::exception& e)
	{
		(*logger_) << e.what() << std::endl;
	}
}

void FTImprover::Patch(size_t offset, size_t value, size_t len)
{
	DWORD old_protect = 0, bogus_protect = 0;
	VirtualProtect((void*)offset, len, PAGE_EXECUTE_READWRITE, &old_protect);
	memcpy((void*)offset, &value, len);
	VirtualProtect((void*)offset, len, old_protect, &bogus_protect);
}

void FTImprover::ClearAmmoTypes()
{
	ammo_types_active = true;
	for (size_t i = 0; i < ammo_types.size(); i++)
	{
		delete[] ammo_types[i];
	}
	ammo_types.clear();
}

void FTImprover::AddAmmoType(std::string const& ammotype)
{
	char* newbuf;
	if (!ammo_types_active)
	{
		// We need to copy the existing ammo types
		ammo_types_active = true;
		
		for (char** arrayloc = (char**)0x8a8e0c; arrayloc != (char**)0x8a8e5c; arrayloc++)
		{
			newbuf = new char[strlen(*arrayloc) + 1];
			memcpy(newbuf, *arrayloc, strlen(*arrayloc) + 1);
			ammo_types.push_back(newbuf);
		}

	}
	newbuf = new char[ammotype.length() + 1];
	memcpy(newbuf, ammotype.c_str(), ammotype.length() + 1);
	ammo_types.push_back(newbuf);
}



void FTImprover::UpdateAmmoTypes()
{
	size_t beginloc = (size_t)(&ammo_types.front());
	size_t endloc = beginloc + (ammo_types.size() * sizeof(char*));
	Patch(0x5bdfa9, beginloc + 8);
	Patch(0x5be517, beginloc);
	Patch(0x5be570, endloc);
	Patch(0x5be6b4, beginloc + 8);
	Patch(0x5cce8e, beginloc);
	Patch(0x5d9500, beginloc);
	Patch(0x5dc29f, beginloc);
	Patch(0x5dc2f1, endloc);
	Patch(0x5dc477, beginloc);
}

void FTImprover::ClearImpactSounds()
{
	impact_sounds_active = true;
	for (size_t i = 0; i < impact_sounds.size(); i++)
	{
		delete[] impact_sounds[i];
	}
	impact_sounds.clear();
}

void FTImprover::AddImpactSound(std::string const& impactsound)
{
	char* newbuf;
	if (!impact_sounds_active)
	{
		// We need to copy the existing impact sounds
		impact_sounds_active = true;

		for (char** arrayloc = (char**)0x8a81d4; arrayloc != (char**)0x8a821c; arrayloc++)
		{
			newbuf = new char[strlen(*arrayloc) + 1];
			memcpy(newbuf, *arrayloc, strlen(*arrayloc) + 1);
			impact_sounds.push_back(newbuf);
		}

	}
	newbuf = new char[impactsound.length() + 1];
	memcpy(newbuf, impactsound.c_str(), impactsound.length() + 1);
	impact_sounds.push_back(newbuf);
}

void FTImprover::UpdateImpactSounds()
{
	size_t beginloc = (size_t)(&impact_sounds.front());
	size_t endloc = beginloc + (impact_sounds.size() * sizeof(char*));
	Patch(0x5be644, beginloc);
	Patch(0x5be69b, endloc);
	Patch(0x5be6ae, beginloc);
	Patch(0x5d85d9, beginloc);
	Patch(0x5d862d, endloc);
	Patch(0x5d864d, beginloc);
}

void FTImprover::ClearAnimationNames()
{
	animation_names_active = true;
	for (size_t i = 0; i < animation_names.size(); i++)
	{
		delete[] animation_names[i];
	}
	animation_names.clear();
}

void FTImprover::AddAnimationName(std::string const& animationname)
{
	char* newbuf;
	if (!animation_names_active)
	{
		// We need to copy the existing impact sounds
		animation_names_active = true;

		for (char** arrayloc = (char**)0x8a8cfc; arrayloc != (char**)0x8a8d24; arrayloc++)
		{
			newbuf = new char[strlen(*arrayloc) + 1];
			memcpy(newbuf, *arrayloc, strlen(*arrayloc) + 1);
			animation_names.push_back(newbuf);
		}

	}
	newbuf = new char[animationname.length() + 1];
	memcpy(newbuf, animationname.c_str(), animationname.length() + 1);
	animation_names.push_back(newbuf);
}

void FTImprover::UpdateAnimationNames()
{
	size_t beginloc = (size_t)(&animation_names.front());
	size_t endloc = beginloc + (animation_names.size() * sizeof(char*));
	Patch(0x5d8516, beginloc);
	Patch(0x5d856a, endloc);
	Patch(0x5d866d, beginloc);
}

void FTImprover::ClearWeaponItemTypes()
{
	weapon_item_types_active = true;
	for (size_t i = 0; i < weapon_item_types.size(); i++)
	{
		delete[] weapon_item_types[i];
	}
	weapon_item_types.clear();
}

void FTImprover::AddWeaponItemType(std::string const& weapontype)
{
	char* newbuf;
	if (!weapon_item_types_active)
	{
		// We need to copy the existing impact sounds
		weapon_item_types_active = true;

		for (char** arrayloc = (char**)0x8a8d24; arrayloc != (char**)0x8a8d50; arrayloc++)
		{
			newbuf = new char[strlen(*arrayloc) + 1];
			memcpy(newbuf, *arrayloc, strlen(*arrayloc) + 1);
			weapon_item_types.push_back(newbuf);
		}

	}
	newbuf = new char[weapontype.length() + 1];
	memcpy(newbuf, weapontype.c_str(), weapontype.length() + 1);
	weapon_item_types.push_back(newbuf);
}

void FTImprover::UpdateWeaponItemTypes()
{
	size_t beginloc = (size_t)(&weapon_item_types.front());
	size_t endloc = beginloc + (weapon_item_types.size() * sizeof(char*));
	Patch(0x5dc3bb, beginloc);
	Patch(0x5dc40f, endloc);
	Patch(0x5dc490, beginloc);
}

void FTImprover::ClearWeaponSoundTypes()
{
	weapon_sound_types_active = true;
	for (size_t i = 0; i < weapon_sound_types.size(); i++)
	{
		delete[] weapon_sound_types[i];
	}
	weapon_sound_types.clear();
}

void FTImprover::AddWeaponSoundType(std::string const& weaponsound)
{
	char* newbuf;
	if (!weapon_sound_types_active)
	{
		// We need to copy the existing impact sounds
		weapon_sound_types_active = true;

		for (char** arrayloc = (char**)0x8a8134; arrayloc != (char**)0x8a81c0; arrayloc++)
		{
			newbuf = new char[strlen(*arrayloc) + 1];
			memcpy(newbuf, *arrayloc, strlen(*arrayloc) + 1);
			weapon_sound_types.push_back(newbuf);
		}

	}
	newbuf = new char[weaponsound.length() + 1];
	memcpy(newbuf, weaponsound.c_str(), weaponsound.length() + 1);
	weapon_sound_types.push_back(newbuf);
}

void FTImprover::UpdateWeaponSoundTypes()
{
	size_t beginloc = (size_t)(&weapon_sound_types.front());
	size_t endloc = beginloc + (weapon_sound_types.size() * sizeof(char*));
	Patch(0x5d957c, beginloc);
	Patch(0x5dc416, beginloc);
	Patch(0x5dc46a, endloc);
	Patch(0x5dc4a9, beginloc);
}

void FTImprover::ClearAmmoVariants()
{
	ammo_variants_active = true;
	for (size_t i = 0; i < ammo_variants.size(); i++)
	{
		delete[] ammo_variants[i].name;
	}
	ammo_variants.clear();
}

void FTImprover::AddAmmoVariant(std::string const& ammovariant, float dmg_mult, float resist_mult)
{
	char* newbuf;
	if (!ammo_variants_active)
	{
		// We need to copy the existing impact sounds
		ammo_variants_active = true;

		for (FTI_AmmoVariant* arrayloc = (FTI_AmmoVariant*)0x8a8e60; arrayloc != (FTI_AmmoVariant*)0x8a8ef0; arrayloc++)
		{
			newbuf = new char[strlen(arrayloc->name) + 1];
			memcpy(newbuf, arrayloc->name, strlen(arrayloc->name) + 1);

			ammo_variants.emplace_back(newbuf, arrayloc->dmg_mult, arrayloc->resist_mult );
		}

	}
	newbuf = new char[ammovariant.length() + 1];
	memcpy(newbuf, ammovariant.c_str(), ammovariant.length() + 1);
	ammo_variants.emplace_back(newbuf, dmg_mult, resist_mult);
}

void FTImprover::UpdateAmmoVariants()
{
	size_t beginloc = (size_t)(&ammo_variants.front());
	size_t endloc = beginloc + (ammo_variants.size() * sizeof(FTI_AmmoVariant));
	Patch(0x5908eb, beginloc);
	Patch(0x5926b9, beginloc);
	Patch(0x5bdfbb, beginloc);
	Patch(0x5be6bb, beginloc);
	Patch(0x5ccee1, beginloc);
	Patch(0x5d387c, beginloc);
	Patch(0x5d9547, beginloc);
	Patch(0x5dc49c, beginloc);
	Patch(0x62f97c, beginloc);
	Patch(0x62f9cd, endloc);
	Patch(0x62fa16, beginloc);
	Patch(0x62fa8c, endloc);
	Patch(0x62fac6, beginloc + 4);
	Patch(0x62faf6, beginloc);
	Patch(0x62fb6c, endloc);
	Patch(0x62fba6, beginloc + 8);
}

void FTImprover::ClearAPCT()
{
	apct_active = true;
	for (size_t i = 0; i < apct.size(); i++)
	{
		delete[] apct[i].name;
	}
	apct.clear();
}

void FTImprover::AddAPCT(std::string const& apctname, int purpose, int actiontype, int cost)
{
	char* newbuf;
	if (!apct_active)
	{
		// We need to copy the existing impact sounds
		apct_active = true;

		for (FTI_APCT* arrayloc = (FTI_APCT*)0x8a8a30; arrayloc != (FTI_APCT*)0x8a8cc0; arrayloc++)
		{
			newbuf = new char[strlen(arrayloc->name) + 1];
			memcpy(newbuf, arrayloc->name, strlen(arrayloc->name) + 1);

			apct.emplace_back(arrayloc->purpose, arrayloc->actiontype, newbuf, arrayloc->cost);
		}

	}
	newbuf = new char[apctname.length() + 1];
	memcpy(newbuf, apctname.c_str(), apctname.length() + 1);
	apct.emplace_back(purpose,actiontype,newbuf, cost);
}

void FTImprover::UpdateAPCT()
{
	size_t beginloc = (size_t)(&apct.front());
	size_t endloc = beginloc + (apct.size() * sizeof(FTI_APCT));
	Patch(0x5d863b, beginloc  +8);
	Patch(0x62ef5c, beginloc + 8);
	Patch(0x62efb3, endloc + 8);
	Patch(0x62efed, beginloc + 8);
	Patch(0x62f062, endloc + 8);
	Patch(0x62f0b8, beginloc + 8);
	Patch(0x62f12e, endloc + 8);
	Patch(0x62f17f, beginloc + 12);
	Patch(0x62f218, beginloc + 4);
	Patch(0x62f23b, beginloc + 4);
	Patch(0x62f261, beginloc + 4);
	Patch(0x62f26a, beginloc + 12);
	Patch(0x62f289, beginloc + 4);
	Patch(0x62f2e6, beginloc + 8);
	Patch(0x62f39a, beginloc + 8);
	Patch(0x62f3a0, beginloc + 8);
}

void FTImprover::AddArmourTypeFTI(std::string const & name, std::string const & soundprefix, int32_t sex, std::string const & overrd)
{
	AddArmourType(name);
	if (overrd != "0" && overrd != "")
	{
		AddArmourRule(name, "*", "*", overrd);
	}
	else if (sex)
	{
		AddArmourRule(name, "*", "*", "sprites/characters/%a%s.spr");
	}
	else
	{
		AddArmourRule(name, "*", "*", "sprites/characters/%a.spr");
	}
	AddSoundRule(name, "*", "*", soundprefix);
}

void FTImprover::AddArmourType(std::string const& name)
{
	char* newbuf;
	if (!armour_types_active)
	{
		include_default_armour_types = true;
		armour_types_active = true;
	}
	newbuf = new char[name.length() + 1];
	memcpy(newbuf, name.c_str(), name.length() + 1);
	armour_types.push_back(newbuf);
}

void FTImprover::AddArmourRule(std::string const& armour, std::string const& sex, std::string const& race, std::string const& output)
{
	if (!armour_types_active)
	{
		include_default_armour_types = true;
		armour_types_active = true;
	}
	armour_rules.emplace_back(armour, sex, race, output);
}

void FTImprover::AddSoundRule(std::string const& armour, std::string const& sex, std::string const& race, std::string const& output)
{
	if (!armour_types_active)
	{
		include_default_armour_types = true;
		armour_types_active = true;
	}
	sound_rules.emplace_back(armour, sex, race, output);
}

void FTImprover::ClearArmourTypes()
{
	armour_types_active = true;
	for (size_t i = 0; i < armour_types.size(); i++)
	{
		delete[] armour_types[i];
	}
	armour_types.clear();

}

void FTImprover::UpdateArmourTypes()
{
	if (include_default_armour_types)
	{
		BuildDefaultArmourTypes();
	}

	if (include_default_armour_rules)
	{
		BuildDefaultArmourSoundRules();
	}
	size_t beginloc = (size_t)(&armour_types.front());
	size_t endloc = beginloc + (armour_types.size() * sizeof(char*));
	Patch(0x5bfb45, beginloc);
	Patch(0x5bfb93, endloc);

}

void FTImprover::BuildDefaultArmourSoundRules()
{
	// Default sound rules
	// NOTE that flag-based overrides are to be handled in GetSoundPrefixName.
	AddSoundRule("*", "*", "Reaver", "Metal");
	AddSoundRule("*", "*", "ReaverHuge", "Metal");
	AddSoundRule("*", "*", "Raider", "Leath");
	AddSoundRule("*", "*", "RaiderLarge", "Leath");
	AddSoundRule("*", "*", "RaiderHuge", "Leath");

	// Default armour rules
	AddArmourRule("*", "*", "BOS", "sprites/characters/Tribal%s.spr");
	AddArmourRule("*", "*", "Raider", "sprites/characters/Raider%s.spr");
	AddArmourRule("*", "*", "Reaver", "sprites/characters/Reaver%s.spr");
	AddArmourRule("*", "*", "Ghoul", "sprites/characters/Ghoul.spr");
	AddArmourRule("*", "*", "Mutant", "sprites/characters/Mutant.spr");
	AddArmourRule("*", "*", "TribalLarge", "sprites/characters/TribalMaleLarge.spr");
	AddArmourRule("*", "*", "RaiderLarge", "sprites/characters/RaiderMaleLarge.spr");
	AddArmourRule("*", "*", "RaiderHuge", "sprites/characters/RaiderMaleHuge.spr");
	AddArmourRule("*", "*", "ReaverHuge", "sprites/characters/Reaver%s.spr");
	AddArmourRule("*", "*", "BOSScribe", "sprites/characters/BOSScribe.spr");
	AddArmourRule("*", "*", "BOSElder", "sprites/characters/BOSElder.spr");
	AddArmourRule("*", "*", "CitizenAlpha", "sprites/characters/CitizenAlpha.spr");
	AddArmourRule("*", "*", "CitizenThin", "sprites/characters/CitizenThin%s.spr");
	AddArmourRule("*", "*", "CitizenMedium", "sprites/characters/CitizenMedium%s.spr");
	AddArmourRule("*", "*", "VaultZero", "sprites/characters/Vault%s.spr");
	AddArmourRule("*", "*", "PipBoy", "sprites/characters/Pipboy.spr");
	AddArmourRule("*", "*", "Dummy", "sprites/characters/Dummy.spr");
	AddArmourRule("*", "*", "Deathclaw", "sprites/critters/DeathClaw.spr");
	AddArmourRule("*", "*", "DeathclawBaby", "sprites/critters/DeathClawBaby.spr");
	AddArmourRule("*", "*", "BDCSmall", "sprites/critters/SDC.spr");
	AddArmourRule("*", "*", "BDCLarge", "sprites/critters/BDC.spr");
	AddArmourRule("*", "*", "Dog", "sprites/critters/MutantDog.spr");
	AddArmourRule("*", "*", "Wasp", "sprites/critters/Wasp.spr");
	AddArmourRule("*", "*", "Radscorpion", "sprites/critters/RadScorpion.spr");
	AddArmourRule("*", "*", "Rat", "sprites/critters/GiantRat.spr");
	AddArmourRule("*", "*", "Brahmin", "sprites/critters/TwoHeadedBrahmin.spr");
	AddArmourRule("*", "*", "Komodo", "sprites/critters/MutantLizard.spr");
	AddArmourRule("*", "*", "Wolf", "sprites/critters/Wolf.spr");
	AddArmourRule("*", "*", "RobotHumanoid", "sprites/robots/Humanoid.spr");
	AddArmourRule("*", "*", "RobotPacification", "sprites/robots/Pacification.spr");
	AddArmourRule("*", "*", "RobotLifter", "sprites/robots/LoadLifter.spr");
	AddArmourRule("*", "*", "RobotHover", "sprites/robots/SmallHover.spr");
	AddArmourRule("*", "*", "RobotTank", "sprites/robots/MTT.spr");
	AddArmourRule("*", "*", "RobotBehemoth", "sprites/robots/Behemoth.spr");
	AddArmourRule("*", "*", "RobotSecurity", "sprites/robots/Security.spr");
	AddArmourRule("*", "*", "RobotScurry", "sprites/robots/Scurry.spr");

}

void FTImprover::BuildDefaultArmourTypes()
{
	AddArmourTypeFTI("None", "Leath", 0, "");
	AddArmourTypeFTI("Leather", "Leath", 1, "");
	AddArmourTypeFTI("Metal", "Metal", 1, "");
	AddArmourTypeFTI("Environmental", "Metal", 0, "");
	AddArmourTypeFTI("Power", "Pow", 0, "");
	AddArmourTypeFTI("Raider", "Leath", 1, "");
	AddArmourTypeFTI("Reaver", "Metal", 1, "");
	AddArmourTypeFTI("Ghoul", "Leath", 0, "sprites/characters/GhoulArmour.spr");
	AddArmourTypeFTI("Mutant", "Metal", 0, "sprites/characters/MutantArmour.spr");

	// Special: Existing behavior of Brahmin armor only replaces the sprite for Human
	// characters. Don't change that by default - let the player/modder decide if need to
	// override.
	AddArmourType("Brahmin");
	AddSoundRule("Brahmin", "*", "*", "Leath");
	AddArmourRule("Brahmin", "*", "BOS", "sprites/critters/TwoHeadedBrahmin.spr");
	AddArmourRule("Brahmin", "*", "Tribal", "sprites/critters/TwoHeadedBrahmin.spr");
	AddArmourRule("Brahmin", "*", "Raider", "sprites/critters/TwoHeadedBrahmin.spr");
	AddArmourRule("Brahmin", "*", "Reaver", "sprites/critters/TwoHeadedBrahmin.spr");
	AddArmourRule("Brahmin", "*", "CitizenAlpha", "sprites/critters/TwoHeadedBrahmin.spr");
	AddArmourRule("Brahmin", "*", "CitizenThin", "sprites/critters/TwoHeadedBrahmin.spr");
	AddArmourRule("Brahmin", "*", "CitizenMedium", "sprites/critters/TwoHeadedBrahmin.spr");
	AddArmourRule("Brahmin", "*", "VaultZero", "sprites/critters/TwoHeadedBrahmin.spr");
}

/*void FTImprover::ClearAttackModeNames()
{
	attack_mode_names_active = true;
	for (size_t i = 0; i < attack_mode_names.size(); i++)
	{
		delete[] attack_mode_names[i];
	}
	attack_mode_names.clear();
}

void FTImprover::AddAttackModeName(std::string const& attackmode)
{
	char* newbuf;
	if (!attack_mode_names_active)
	{
		// We need to copy the existing impact sounds
		attack_mode_names_active = true;
	}
	newbuf = new char[attackmode.length() + 1];
	memcpy(newbuf, attackmode.c_str(), attackmode.length() + 1);
	attack_mode_names.push_back(newbuf);
}*/

void FTImprover::EnableScriptTriggers()
{
	if (disabled) return;
	Patch(0x695bc1, 0, 1);
	Patch(0x695c61, 0, 1);
	Patch(0x695cf1, 0, 1);
	Patch(0x695d91, 0, 1);
}

void FTImprover::UpdateMemoryLists()
{
	if (disabled) return;
	if (ammo_types.size() > 0)
	{
		UpdateAmmoTypes();
	}
	if (impact_sounds.size() > 0)
	{
		UpdateImpactSounds();
	}
	if (animation_names.size() > 0)
	{
		UpdateAnimationNames();
	}
	if (weapon_item_types.size() > 0)
	{
		UpdateWeaponItemTypes();
	}
	if (weapon_sound_types.size() > 0)
	{
		UpdateWeaponSoundTypes();
	}
	if (ammo_variants.size() > 0)
	{
		UpdateAmmoVariants();
	}
	if (apct.size() > 0)
	{
		UpdateAPCT();
	}

	if (armour_types_active)
	{
		UpdateArmourTypes();
	}

}

void FTImprover::EnableGotoMissionFix()
{
	if (disabled) return;
	Patch(0x82b368, Helpers::ConvertFunction(&FTI_GotoMissionStaticStruct::FTI_GotoMission_Called1));
}
void FTImprover::EnableMultiProcessFix()
{
	if (disabled) return;
	
	Patch(0x72ee94, 0x33ebed33);
	Patch(0x72f253, 0x9090c033);
	Patch(0x72f257, 0x90, 1);
}
bool FTImprover::IsArmourReplaceEnabled()
{
	return armour_types_active;
}

std::string FTImprover::BuildStringFromElements(std::string const& output, std::string const& armour, std::string const& sex, std::string const& race)
{
	std::string ret;
	ret.reserve(output.length() + armour.length() + sex.length() + race.length());
	for (size_t i = 0; i < output.length(); i++)
	{
		if (output.at(i) == '%' && i != output.length() - 1)
		{
			i++;
			if (output.at(i) == 'a')
			{
				ret.append(armour);
			}
			else if (output.at(i) == 's')
			{
				ret.append(sex);
			}
			else if (output.at(i) == 'r')
			{
				ret.append(race);
			}
			else
			{
				ret.append(1,output.at(i));
			}
		}
		else
		{
			ret.append(1, output.at(i));
		}
	}
	return ret;
}

void FTImprover::HandleFeatureFlag(std::string const & name, std::string const & value)
{
	if (disabled) return;
	if (value == "1")
	{
		// Check for supported parameters
		if (name == "patch.allTriggerConditions")
		{
			EnableScriptTriggers();
		}
		if (name == "patch.multipleBOSprocesses")
		{
			EnableMultiProcessFix();
		}
		else if (name == "patch.actionPointCostTypes")
		{
			enabled_features.insert("APCT");
		}
		else if (name == "patch.animationNames")
		{
			enabled_features.insert("AnimationNames");
		}
		else if (name == "patch.weaponItemTypes")
		{
			enabled_features.insert("WeaponItemTypes");
		}
		else if (name == "patch.weaponSoundTypes")
		{
			enabled_features.insert("WeaponSoundTypes");
		}
		else if (name == "patch.ammoTypes")
		{
			enabled_features.insert("AmmoTypes");
		}
		else if (name == "patch.ammoVariants")
		{
			enabled_features.insert("AmmoVariants");
		}
		else if (name == "patch.armourTypes")
		{
			enabled_features.insert("ArmourTypes");
		}
		else if (name == "patch.impactSounds")
		{
			enabled_features.insert("ImpactSounds");
		}
	}
}

void FTImprover::GetArmourSpriteName(wchar_t ** w_dest, wchar_t ** w_sex, wchar_t ** w_race, wchar_t ** w_armour)
{
	std::string armour = Helpers::WcharToUTF8(*w_armour);
	std::string sex = Helpers::WcharToUTF8(*w_sex);
	std::string race = Helpers::WcharToUTF8(*w_race);
	std::string dest;
	for (auto rule : armour_rules)
	{
		if ((rule.armour == armour || rule.armour == "*") &&
			(rule.sex == sex || rule.sex == "*") &&
			(rule.race == race || rule.race == "*"))
		{
			dest = BuildStringFromElements(rule.output, armour, sex, race);

			break;
		}
	}
	*w_dest = Helpers::UTF8ToWcharFOTHeap(dest, 1);
}

void FTImprover::GetSoundPrefixName(wchar_t ** w_dest, Entity * ent)
{
	std::string dest;
	Actor* e = dynamic_cast<Actor*>(ent);
	if (e)
	{
		std::string race = e->GetRace();
		std::string sex = e->GetSex();
		std::string armour;
		auto armour_ent = e->GetEquippedItem(2);
		if (armour_ent->GetBaseID() != 0)
		{
			armour = armour_ent->GetEntitySubType();
		}
		for (auto rule : sound_rules)
		{
			if ((rule.armour == armour || rule.armour == "*") &&
				(rule.sex == sex || rule.sex == "*") &&
				(rule.race == race || rule.race == "*"))
			{
				dest = BuildStringFromElements(rule.output, armour, sex, race);
				break;
			}
		}

		if (dest == "")
		{
			// There are some additional default rules, check those too

			// Cyborg General and Brain Jar depend on sprite name
			// Just check it here instead of calling vtable
			std::string ent_sprite = ent->GetSpriteName();
			if (ent_sprite.find(std::string("CyborgGeneral")) != std::string::npos)
			{
				dest = "Pow";
			}
			// Brain jar too
			else if (ent_sprite.find(std::string("BrainJar")) != std::string::npos)
			{
				dest = "Metal";
			}
			// Gun turret
			else if (race == "GunTurret01")
			{
				dest = "Metal";
			}

			// Final check is living or not. Living gets Flesh, non gets Pow
			else if (e->IsLivingType())
			{
				dest = "Flesh";
			}
			else
			{
				dest = "Pow";
			}

		}

	}
	*w_dest = Helpers::UTF8ToWcharFOTHeap(dest, 1);
}

void FTImprover::DisableDefaultArmourTypes()
{
	include_default_armour_types = false;
}
void FTImprover::DisableDefaultArmourRules()
{
	include_default_armour_rules = false;
}

bool FTImprover::IsFeatureEnabled(std::string const& feature)
{
	auto x = enabled_features.find(feature);
	if (x == enabled_features.end())
	{
		return false;
	}
	return true;
}

int l_parseconfig(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2))
	{
		std::vector<char> inbuf;
		std::string instr = lua_tostring(l, 2);
		inbuf.resize(instr.size());
		memcpy(&inbuf.at(0), instr.c_str(), instr.size());
		(*ds)->Parse(inbuf);
	}
	return 0;
}

int l_addammotype(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2))
	{
		std::string instr = lua_tostring(l, 2);
		(*ds)->AddAmmoType(instr);
	}
	return 0;

}

int l_clearammotypes(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->ClearAmmoTypes();
	return 0;
}

int l_addimpactsound(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2))
	{
		std::string instr = lua_tostring(l, 2);
		(*ds)->AddImpactSound(instr);
	}
	return 0;

}

int l_clearimpactsounds(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->ClearImpactSounds();
	return 0;
}
int l_addanimationname(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2))
	{
		std::string instr = lua_tostring(l, 2);
		(*ds)->AddAnimationName(instr);
	}
	return 0;

}

int l_clearanimationnames(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->ClearAnimationNames();
	return 0;
}

int l_addweaponitemtype(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2))
	{
		std::string instr = lua_tostring(l, 2);
		(*ds)->AddWeaponItemType(instr);
	}
	return 0;

}

int l_clearweaponitemtypes(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->ClearWeaponItemTypes();
	return 0;
}

int l_addweaponsoundtype(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2))
	{
		std::string instr = lua_tostring(l, 2);
		(*ds)->AddWeaponSoundType(instr);
	}
	return 0;

}

int l_clearweaponsoundtypes(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->ClearWeaponSoundTypes();
	return 0;
}

int l_addammovariant(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2) && lua_isinteger(l, 3) && lua_isinteger(l, 4))
	{
		std::string instr = lua_tostring(l, 2);
		float dmg_mult = 1.0f + ((float)lua_tointeger(l, 3)) / 100.0f;
		float resist_mult = 1.0f - ((float)lua_tointeger(l, 4)) / 100.0f;
		(*ds)->AddAmmoVariant(instr, dmg_mult, resist_mult);
	}
	return 0;

}

int l_clearammovariants(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->ClearAmmoVariants();
	return 0;
}

int l_addapct(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2) && lua_isinteger(l, 3) && lua_isinteger(l, 4) && lua_isinteger(l, 5))
	{
		std::string instr = lua_tostring(l, 2);
		(*ds)->AddAPCT(instr, (int)lua_tointeger(l, 3), (int)lua_tointeger(l, 4), (int)lua_tointeger(l, 5));
	}
	return 0;

}

int l_clearapct(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->ClearAPCT();
	return 0;
}

int l_enablescripttriggers(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->EnableScriptTriggers();
	return 0;

}

int l_enablegotomissionfix(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->EnableGotoMissionFix();
	return 0;

}
int l_enablemultiprocessfix(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->EnableMultiProcessFix();
	return 0;

}
int l_addarmourtype(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2))
	{
		std::string instr = lua_tostring(l, 2);
		(*ds)->AddArmourType(instr);
	}
	return 0;
}
int l_cleararmourtypes(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->ClearArmourTypes();
	(*ds)->DisableDefaultArmourTypes();
	return 0;
}
int l_disabledefaultarmourrules(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	(*ds)->DisableDefaultArmourRules();
	return 0;
}

int l_addarmourrule(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2) && lua_isstring(l, 3) && lua_isstring(l, 4) && lua_isstring(l,5))
	{
		std::string armour = lua_tostring(l, 2);
		std::string sex = lua_tostring(l, 3);
		std::string race = lua_tostring(l, 4);
		std::string output = lua_tostring(l, 5);
		(*ds)->AddArmourRule(armour,sex,race,output);
	}

	return 0;
}
int l_addsoundrule(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2) && lua_isstring(l, 3) && lua_isstring(l, 4) && lua_isstring(l, 5))
	{
		std::string armour = lua_tostring(l, 2);
		std::string sex = lua_tostring(l, 3);
		std::string race = lua_tostring(l, 4);
		std::string output = lua_tostring(l, 5);
		(*ds)->AddSoundRule(armour, sex, race, output);
	}

	return 0;
}

int l_isfeatureenabled(lua_State* l)
{
	FTImprover** ds = (FTImprover**)luaL_checkudata(l, 1, "FTImprover");
	if (lua_isstring(l, 2))
	{
		std::string feature = lua_tostring(l, 2);
		lua_pushboolean(l, (*ds)->IsFeatureEnabled(feature));
	}
	else
	{
		lua_pushboolean(l, false);
	}

	return 1;
}
void FTImprover::RegisterLua(lua_State* l, Logger* logger)
{
	logger_ = logger;
	HMODULE ret;
	if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, "Improver_shmem.dll", &ret))
	{
		(*logger) << "FT Improver detected, disabling FTSE built-in FTI module" << std::endl;
		disabled = true;
	}
	FTImprover** heptrptr = (FTImprover**)lua_newuserdata(l, sizeof(FTImprover*));
	*heptrptr = this;
	luaL_newmetatable(l, "FTImprover");
	lua_pushcfunction(l, l_parseconfig);
	lua_setfield(l, -2, "ParseConfig");
	lua_pushcfunction(l, l_addammotype);
	lua_setfield(l, -2, "AddAmmoType");
	lua_pushcfunction(l, l_clearammotypes);
	lua_setfield(l, -2, "ClearAmmoTypes");
	lua_pushcfunction(l, l_addimpactsound);
	lua_setfield(l, -2, "AddImpactSound");
	lua_pushcfunction(l, l_clearimpactsounds);
	lua_setfield(l, -2, "ClearImpactSounds");
	lua_pushcfunction(l, l_addanimationname);
	lua_setfield(l, -2, "AddAnimationName");
	lua_pushcfunction(l, l_clearanimationnames);
	lua_setfield(l, -2, "ClearAnimationNames");
	lua_pushcfunction(l, l_addweaponitemtype);
	lua_setfield(l, -2, "AddWeaponItemType");
	lua_pushcfunction(l, l_clearweaponitemtypes);
	lua_setfield(l, -2, "ClearWeaponItemTypes");
	lua_pushcfunction(l, l_addweaponsoundtype);
	lua_setfield(l, -2, "AddWeaponSoundType");
	lua_pushcfunction(l, l_clearweaponsoundtypes);
	lua_setfield(l, -2, "ClearWeaponSoundTypes");
	lua_pushcfunction(l, l_addammovariant);
	lua_setfield(l, -2, "AddAmmoVariant");
	lua_pushcfunction(l, l_clearammovariants);
	lua_setfield(l, -2, "ClearAmmoVariants");
	lua_pushcfunction(l, l_addapct);
	lua_setfield(l, -2, "AddAPCT");
	lua_pushcfunction(l, l_clearapct);
	lua_setfield(l, -2, "ClearAPCT");
	lua_pushcfunction(l, l_enablescripttriggers);
	lua_setfield(l, -2, "EnableScriptTriggers");
	lua_pushcfunction(l, l_addarmourtype);
	lua_setfield(l, -2, "AddArmourType");
	lua_pushcfunction(l, l_cleararmourtypes);
	lua_setfield(l, -2, "ClearArmourTypes");
	lua_pushcfunction(l, l_addarmourrule);
	lua_setfield(l, -2, "AddArmourRule");
	lua_pushcfunction(l, l_addsoundrule);
	lua_setfield(l, -2, "AddSoundRule");
	lua_pushcfunction(l, l_disabledefaultarmourrules);
	lua_setfield(l, -2, "DisableDefaultArmourRules");
	lua_pushcfunction(l, l_enablegotomissionfix);
	lua_setfield(l, -2, "EnableGotoMissionFix");
	lua_pushcfunction(l, l_enablemultiprocessfix);
	lua_setfield(l, -2, "EnableMultiProcessFix");
	lua_pushcfunction(l, l_isfeatureenabled);
	lua_setfield(l, -2, "IsFeatureEnabled");
	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setmetatable(l, -2);
	lua_setglobal(l, "improver");
}

void FTI_GotoMissionStaticStruct::FTI_GotoMission_Called2(int32_t unk)
{
	auto fxn = (void(__thiscall*)(void*))0x681290;
	(*fxn)(mylist->next->event->orig_action_msg);
	auto freefxn = (void(__cdecl *)(void*))0x6c4e50;
	freefxn(reinterpret_cast<void*>(mylist->next->event->alloc_struct));
}

void FTI_GotoMissionStaticStruct::FTI_GotoMission_Called1()
{
	FTI_GotoMissionAllocatedStruct* alloc_struct = (FTI_GotoMissionAllocatedStruct*)(Helpers::FOTHeapAlloc(sizeof(FTI_GotoMissionAllocatedStruct)));
	alloc_struct->zerobyte = 0;
	alloc_struct->one_int = 1;
	
	auto timestamp_fxn = (long long(__cdecl *)(void))0x705b60;
	alloc_struct->timestamp = (*timestamp_fxn)() + 1;
	alloc_struct->static_struct = &FTImprover::static_struct;
	alloc_struct->fxnptr = Helpers::ConvertFunction(&FTI_GotoMissionStaticStruct::FTI_GotoMission_Called2);
	alloc_struct->zero_int = 0;
	alloc_struct->self = alloc_struct;
	alloc_struct->orig_action_msg = (void*)this;

	int32_t unused_ret;
	auto fxn = (void(__thiscall *)(void*, int32_t*, void*, FTI_GotoMissionAllocatedStruct**))0x426940;
	(*fxn)((void*)0x8be1b5, &unused_ret, (void*)*((void**)0x8be1b9), &alloc_struct);
}
