#pragma once

#include <stdint.h>
#include "lua.hpp"
#include "LuaHelper.h"
#include "Entity.h"
#include <map>

// Make large enough for the largest Vtable (Actor class = 536 entries)
typedef size_t *EntityVtableArray;

typedef int(*VtableCallTemplateFxn)(void*, size_t, lua_State*);

class EntityVtable
{
public:
	EntityVtable(lua_State* l);
	~EntityVtable();

	size_t GetVtableAddrForClass(lua_State* l);
	size_t GetOrigVtableAddrForClass(lua_State* l);

	VtableCallTemplateFxn GetVtableCallTemplateByIndex(int idx);

	void InstallVtableHook(std::string const& classname, size_t idx, size_t new_fxn);

private:
	void CopyVtable(std::string const& classname, size_t vtable_addr, int entries);
	void InitVtableCallTemplates();
	
	lua_State* lua_;

	std::map<size_t, EntityVtableArray> orig_vtables_;
	std::map<std::string, size_t> class_name_to_vtable_;

	// IMPORTANT NOTE:
	// There are multiple classes which extend beyond the "normal" Entity vtable of 525 entries.
	// The largest is Actor, with 536 total functions.  However, the format of the parameters
	// for the other classes which go beyond 525 are different.  For now, we will include
	// in the template all functions as they are used in the Actor class. If we need to handle
	// any vtable functions greater than 525 in other classes, we will need to add individual
	// special case code for each.
	// Confirmed OK to use vtables 525+ (types are same): Breakable, StateBreakable, Sneak, SentryAI
	VtableCallTemplateFxn vtable_call_templates_[536];

};

// **************************************************************************************
// VTABLE CALL TEMPLATES FOLLOW
//
// Naming patterns: Each patterned template uses the form "vtable_call_template_<PATTERN>",
// where <PATTERN> includes the following elements:
//
// First integer indicates the number of parameters in the vtable call. This includes
// return parameters in the "r1" and "r2" cases below.
//
// If the first integer is followed by "r", then the vtable call returns a value. The
// return type is included last in the template.
//
// If the first integer is followed by "r1", then the vtable call returns its value
// in the object pointed to by the first argument.  Template values should correspond
// to the parameter list, with the first parameter specified as the base return type,
// not pointer to return type.  
//
// If the first integer is followed by "r2", then the vtable call returns two values -
// the first parameter, which is always InventoryActionResult, is returned first. The
// third parameter, which is always an EntityID*, is returned second. Other types will
// vary depending on the template.
//
// If the pattern starts with "X", then the function pattern is unique enough to warrant
// a specialized function.  The value following the X indicates the vtable index for the
// function.
//  
// **************************************************************************************

template<int VTABLE_INDEX>
int vtable_call_template_noop(void* ent, size_t vtable_addr, lua_State* l)
{
	return 0;
}

template<int VTABLE_INDEX>
int vtable_call_template_0(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void(__thiscall *FXNType)(void* ent);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	(*fxn)(ent);

	return 0;

}

template<int VTABLE_INDEX, typename RET>
int vtable_call_template_0r(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef RET(__thiscall *FXNType)(void* ent);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	RET ret = (*fxn)(ent);

	LuaHelper::Return<RET>(l, ret);
	return 1;

}

template<int VTABLE_INDEX, typename PARAM1>
int vtable_call_template_1(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void (__thiscall *FXNType)(void* ent, PARAM1 param1);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1 = LuaHelper::Retrieve<PARAM1>(l, 3);

	(*fxn)(ent, param1);

	LuaHelper::Release<PARAM1>(param1);

	return 0;

}

template<int VTABLE_INDEX, typename PARAM1, typename RET>
int vtable_call_template_1r(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef RET(__thiscall *FXNType)(void* ent, PARAM1 param1);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1 = LuaHelper::Retrieve<PARAM1>(l, 3);

	RET ret = (*fxn)(ent, param1);

	LuaHelper::Return<RET>(l, ret);

	LuaHelper::Release<PARAM1>(param1);
	return 1;

}

template<int VTABLE_INDEX, typename PARAM1>
int vtable_call_template_1r1(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef PARAM1*(__thiscall *FXNType)(void* ent, PARAM1* param1);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1;

	(*fxn)(ent, &param1);

	LuaHelper::Return<PARAM1>(l, param1);
	return 1;

}

template<int VTABLE_INDEX, typename PARAM1,typename PARAM2>
int vtable_call_template_2(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void(__thiscall *FXNType)(void* ent, PARAM1 param1,PARAM2 param2);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1(LuaHelper::Retrieve<PARAM1>(l, 3));
	PARAM2 param2(LuaHelper::Retrieve<PARAM2>(l, 4));

	(*fxn)(ent, param1,param2);

	LuaHelper::Release<PARAM1>(param1);
	LuaHelper::Release<PARAM2>(param2);

	return 0;

}

template<int VTABLE_INDEX, typename PARAM1, typename PARAM2, typename RET>
int vtable_call_template_2r(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef RET(__thiscall *FXNType)(void* ent, PARAM1 param1,PARAM2 param2);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1 = LuaHelper::Retrieve<PARAM1>(l, 3);
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 4);

	RET ret = (*fxn)(ent, param1,param2);

	LuaHelper::Return<RET>(l, ret);
	LuaHelper::Release<PARAM1>(param1);
	LuaHelper::Release<PARAM2>(param2);
	return 1;

}

template<int VTABLE_INDEX, typename PARAM1,typename PARAM2>
int vtable_call_template_2r1(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef PARAM1*(__thiscall *FXNType)(void* ent, PARAM1* param1,PARAM2 param2);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1;
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 3);

	(*fxn)(ent, &param1,param2);

	LuaHelper::Return<PARAM1>(l, param1);
	LuaHelper::Release<PARAM2>(param2);
	return 1;

}

template<int VTABLE_INDEX, typename PARAM1, typename PARAM2,typename PARAM3>
int vtable_call_template_3(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void(__thiscall *FXNType)(void* ent, PARAM1 param1, PARAM2 param2,PARAM3 param3);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1 = LuaHelper::Retrieve<PARAM1>(l, 3);
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 4);
	PARAM3 param3 = LuaHelper::Retrieve<PARAM3>(l, 5);

	(*fxn)(ent, param1, param2,param3);

	LuaHelper::Release<PARAM1>(param1);
	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM3>(param3);

	return 0;

}

template<int VTABLE_INDEX, typename PARAM1, typename PARAM2, typename PARAM3, typename RET>
int vtable_call_template_3r(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef RET(__thiscall *FXNType)(void* ent, PARAM1 param1, PARAM2 param2,PARAM3 param3);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1 = LuaHelper::Retrieve<PARAM1>(l, 3);
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 4);
	PARAM3 param3 = LuaHelper::Retrieve<PARAM3>(l, 5);

	RET ret = (*fxn)(ent, param1, param2,param3);

	LuaHelper::Return<RET>(l, ret);

	LuaHelper::Release<PARAM1>(param1);
	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM3>(param3);

	return 1;

}

template<int VTABLE_INDEX, typename PARAM1, typename PARAM2,typename PARAM3>
int vtable_call_template_3r1(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef PARAM1*(__thiscall *FXNType)(void* ent, PARAM1* param1, PARAM2 param2,PARAM3 param3);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1;
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 3);
	PARAM3 param3 = LuaHelper::Retrieve<PARAM3>(l, 4);

	(*fxn)(ent, &param1, param2,param3);

	LuaHelper::Return<PARAM1>(l, param1);

	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM3>(param3);
	return 1;

}

template<int VTABLE_INDEX, typename PARAM1, typename PARAM2, typename PARAM3,typename PARAM4>
int vtable_call_template_4(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void(__thiscall *FXNType)(void* ent, PARAM1 param1, PARAM2 param2, PARAM3 param3,PARAM4 param4);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1 = LuaHelper::Retrieve<PARAM1>(l, 3);
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 4);
	PARAM3 param3 = LuaHelper::Retrieve<PARAM3>(l, 5);
	PARAM4 param4 = LuaHelper::Retrieve<PARAM4>(l, 6);

	(*fxn)(ent, param1, param2, param3,param4);

	LuaHelper::Release<PARAM1>(param1);
	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM3>(param3);
	LuaHelper::Release<PARAM4>(param4);

	return 0;

}

template<int VTABLE_INDEX, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4>
int vtable_call_template_4r1(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef PARAM1*(__thiscall *FXNType)(void* ent, PARAM1* param1, PARAM2 param2, PARAM3 param3, PARAM4 param4);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1;
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 3);
	PARAM3 param3 = LuaHelper::Retrieve<PARAM3>(l, 4);
	PARAM4 param4 = LuaHelper::Retrieve<PARAM4>(l, 5);

	(*fxn)(ent, &param1, param2, param3, param4);
	LuaHelper::Return<PARAM1>(l, param1);

	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM3>(param3);
	LuaHelper::Release<PARAM4>(param4);

	return 1;

}
template<int VTABLE_INDEX, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4,typename PARAM5>
int vtable_call_template_5(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void(__thiscall *FXNType)(void* ent, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4,PARAM5 param5);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1 = LuaHelper::Retrieve<PARAM1>(l, 3);
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 4);
	PARAM3 param3 = LuaHelper::Retrieve<PARAM3>(l, 5);
	PARAM4 param4 = LuaHelper::Retrieve<PARAM4>(l, 6);
	PARAM5 param5 = LuaHelper::Retrieve<PARAM5>(l, 7);

	(*fxn)(ent, param1, param2, param3, param4,param5);

	LuaHelper::Release<PARAM1>(param1);
	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM3>(param3);
	LuaHelper::Release<PARAM4>(param4);
	LuaHelper::Release<PARAM5>(param5);

	return 0;

}
template<int VTABLE_INDEX, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4,typename PARAM5,typename PARAM6>
int vtable_call_template_6(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef void(__thiscall *FXNType)(void* ent, PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4,PARAM5 param5,PARAM6 param6);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1 = LuaHelper::Retrieve<PARAM1>(l, 3);
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 4);
	PARAM3 param3 = LuaHelper::Retrieve<PARAM3>(l, 5);
	PARAM4 param4 = LuaHelper::Retrieve<PARAM4>(l, 6);
	PARAM5 param5 = LuaHelper::Retrieve<PARAM5>(l, 7);
	PARAM6 param6 = LuaHelper::Retrieve<PARAM6>(l, 8);

	(*fxn)(ent, param1, param2, param3, param4,param5,param6);

	LuaHelper::Release<PARAM1>(param1);
	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM3>(param3);
	LuaHelper::Release<PARAM4>(param4);
	LuaHelper::Release<PARAM5>(param5);
	LuaHelper::Release<PARAM6>(param6);

	return 0;
}

template<int VTABLE_INDEX, typename PARAM1,typename PARAM2,typename PARAM3>
int vtable_call_template_3r2(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef PARAM1*(__thiscall *FXNType)(void* ent, PARAM1* param1,PARAM2 param2,PARAM3* param3);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1;
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 3);
	PARAM3 param3;

	(*fxn)(ent, &param1,param2,&param3);

	LuaHelper::Return<PARAM1>(l, param1);
	LuaHelper::Return<PARAM3>(l, param3);

	LuaHelper::Release<PARAM2>(param2);

	return 2;

}
template<int VTABLE_INDEX, typename PARAM1, typename PARAM2, typename PARAM3,typename PARAM4>
int vtable_call_template_4r2(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef PARAM1*(__thiscall *FXNType)(void* ent, PARAM1* param1, PARAM2 param2, PARAM3* param3,PARAM4 param4);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1;
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 3);
	PARAM3 param3;
	PARAM4 param4 = LuaHelper::Retrieve<PARAM4>(l,4);

	(*fxn)(ent, &param1, param2, &param3,param4);

	LuaHelper::Return<PARAM1>(l, param1);
	LuaHelper::Return<PARAM3>(l, param3);

	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM4>(param4);

	return 2;

}
template<int VTABLE_INDEX, typename PARAM1, typename PARAM2, typename PARAM3, typename PARAM4, typename PARAM5>
int vtable_call_template_5r2(void* ent, size_t vtable_addr, lua_State* l)
{
	typedef PARAM1*(__thiscall *FXNType)(void* ent, PARAM1* param1, PARAM2 param2, PARAM3* param3, PARAM4 param4, PARAM5 param5);
	FXNType* fxn = (FXNType*)(vtable_addr + VTABLE_INDEX * sizeof(size_t));

	PARAM1 param1;
	PARAM2 param2 = LuaHelper::Retrieve<PARAM2>(l, 3);
	PARAM3 param3;
	PARAM4 param4 = LuaHelper::Retrieve<PARAM4>(l, 4);
	PARAM5 param5 = LuaHelper::Retrieve<PARAM5>(l, 5);

	(*fxn)(ent, &param1, param2, &param3, param4, param5);

	LuaHelper::Return<PARAM1>(l, param1);
	LuaHelper::Return<PARAM3>(l, param3);

	LuaHelper::Release<PARAM2>(param2);
	LuaHelper::Release<PARAM4>(param4);
	LuaHelper::Release<PARAM5>(param5);
	return 2;

}

int vtable_call_template_X32(void* ent, size_t vtable_addr, lua_State* l);

