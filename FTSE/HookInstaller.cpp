/* MIT License

Copyright (c) 2018 melindil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "HookInstaller.h"

#include <Windows.h>
#include <string>

size_t constexpr ConvertFunctionZeroParam(void(__thiscall HookExecutor::*fxn)())
{
	size_t* ret = reinterpret_cast<size_t*>(&fxn);
	return *ret;
}
size_t constexpr ConvertFunctionOneParam(void (__thiscall HookExecutor::*fxn)(void*))
{
	size_t* ret = reinterpret_cast<size_t*>(&fxn);
	return *ret;
}
size_t constexpr ConvertFunctionThreeParamRet(uint32_t(__thiscall HookExecutor::*fxn)(void*,void*,void*))
{
	size_t* ret = reinterpret_cast<size_t*>(&fxn);
	return *ret;
}
size_t constexpr ConvertFunctionOneParamRet(int(__thiscall HookExecutor::*fxn)(void*))
{
	size_t* ret = reinterpret_cast<size_t*>(&fxn);
	return *ret;
}
size_t constexpr ConvertFunctionFourParamRet(int(__thiscall HookExecutor::*fxn)(void*, void*, void*, uint32_t))
{
	size_t* ret = reinterpret_cast<size_t*>(&fxn);
	return *ret;
}
size_t constexpr ConvertFunctionTimeHook(int(__thiscall HookExecutor::*fxn)(uint64_t,uint32_t,void*))
{
	size_t* ret = reinterpret_cast<size_t*>(&fxn);
	return *ret;
}

HookInstaller::HookDefinition HookInstaller::hooks_[] =
{

	// Radiation hook
	{
		0x57b4ed,
		5,				// Replacing 5 bytes
		0,
		5,				// Append the instruction after we run our hook
		"\x56",			// push esi
		1,
		"",
		0,				// No cleanup needed
		
		ConvertFunctionOneParam(&HookExecutor::IsRadiated)

	},
	// Long tick hook
	{
		0x57cbb0,
		5,				// Replacing 5 bytes
		0,
		5,				// Append the instruction after we run our hook
		"\x56",			// push esi
		1,
		"",
		0,				// No cleanup needed

		ConvertFunctionOneParam(&HookExecutor::LongTickTrigger)

	},
	// DefaultStyle constructed hook
	{
		0x731ba2,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x56",			// push esi
		1,
		"",
		0,				// No cleanup needed

		ConvertFunctionOneParam(&HookExecutor::DefaultStyleConstructed)

	},
	{
		0x67ee30,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x51",			// push ecx
		1,
		"",
		0,				// No cleanup needed

		ConvertFunctionOneParam(&HookExecutor::SetVariableTrigger)

	},
	{
		0x6ad6a0,
		5,				// Replacing 5 bytes
		0,
		5,				// Append the instruction after we run our hook
		"\x51"				// push ecx
		"\x8b\x44\xe4\x1c"	// mov eax,dword ptr ss:[esp+1c]
	    "\x50"				// push eax
		"\x8b\x44\xe4\x1c"	// mov eax,dword ptr ss:[esp+1c]
		"\x50"				// push eax
		"\x8b\x44\xe4\x1c"	// mov eax,dword ptr ss:[esp+1c]
		"\x50",				// push eax
		16,
		"\x85\xc0"		// test eax,eax
		"\x74\x06"		// jz eip+8
		"\x5a"			// pop edx
		"\x59"			// pop ecx
		"\x58"			// pop eax
		"\x89\xc8"		// mov eax,ecx
		"\xc2\x0c\x00",	// ret 0c
	12,

		ConvertFunctionTimeHook(&HookExecutor::MsecTimerHook)

	},
	{
			0x6ada90,
			5,				// Replacing 5 bytes
			0,
			5,				// Append the instruction after we run our hook
			"\x51",				// push ecx
			1,
			"\x85\xc0"		// test eax,eax
			"\x74\x04"		// jz eip+4
			"\x5a"			// pop edx
			"\x59"			// pop ecx
			"\x58"			// pop eax
			"\xc3",			// ret
			8,

			ConvertFunctionOneParamRet(&HookExecutor::AddBaseTime)

	},
	{
		0x64eec1,
		5,				// Replacing 5 bytes
		0,
		5,				// Append the instruction after we run our hook
		"",
		0,
		"",
		0,

		ConvertFunctionZeroParam(&HookExecutor::OnLocaleLoad)

	},
	{
		0x614c3c,
		7,				// Replacing 5 bytes
		0,
		7,				// Append the instruction after we run our hook
		"\xff\xb4\xe4\x00\x01\x00\x00"	// push dword ptr ss:[esp+100]
		"\xff\xb4\xe4\x00\x01\x00\x00"	// push dword ptr ss:[esp+100]
		"\x56",							// push esi
		15,
		"\x85\xc0"						// test eax,eax
		"\x74\x0f"						// jz eip+0f
		"\x48"							// dec eax
		"\x88\x44\xe4\x1f"				// mov bute ptr ss:[esp+1f],al
		"\x5a"							// pop edx
		"\x59"							// pop ecx
		"\x58"							// pop eax
		"\xb8\xb5\x50\x61\x00"			// mov eax,006150b5
		"\xff\xe0",						// jmp eax

		19,

		ConvertFunctionThreeParamRet(&HookExecutor::OnBurstAttack)

	},
/*	{
		0x617a9f,
		5,				// Replacing 5 bytes
		5,
		0,				// Append the instruction before we run our hook
		"\x89\x38"					// mov dword ptr ds:[eax], edi
		"\x8b\x8c\xe4\x1c\x02\x00\x00"	// mov ecx,cmp dword ptr ss:[esp+21c]
		"\x51"						// push ecx
		"\x50"				// push eax
		"\x53"				// push ebx
		"\x56",				// push esi
		13,
		"\x85\xc0"					// test eax,eax
		"\x74\x0a"					// je eip+0a
		"\x5a"						// pop edx
		"\x59"						// pop ecx
		"\x58"						// pop eax
		"\xb9\xb7\x7a\x61\x00"		// mov ecx,00617ab7
		"\xff\xe1",					// jmp ecx
		14,

		ConvertFunctionFourParamRet(&HookExecutor::OnChanceToHitCalc)

	},
*/
	{0,0,0,0,0,0,0,0,0}
};

HookInstaller::HookInstaller(Logger* logger, std::string const& luaname)
	: logger_(logger)
{
	hookExecutor_ = new HookExecutor(logger,luaname);
		 
}


HookInstaller::~HookInstaller()
{
	delete hookExecutor_;
}

void HookInstaller::installHooks()
{
	HookDefinition* hd = hooks_;
	while (hd->hookAddress != 0)
	{
		*logger_ << "Installing a hook at address 0x" << std::hex << hd->hookAddress << std::dec << std::endl;
		installOneHook(hd);
		hd++;
	}

	hookExecutor_->OnStart();
}

void HookInstaller::installOneHook(HookDefinition* def)
{
	hookCode_.push_back(std::vector<unsigned char>(0));
	std::vector<unsigned char>& newhook = hookCode_.back();

	// OK, now we're ready
	// Get length of hook code to create
	size_t hookLength = def->bytesPrecede				// Replaced instructions
		+ 3												// Save EAX,ECX,EDX
		+ def->precedeLength							// Custom instructions
		+ 5												// Set ECX for HookExecutor this ptr
		+ 5												// Size of CALL instruction
		+ def->followLength								// Custom instructions
		+ 3												// Replace EAX,ECX,EDX
		+ def->bytesFollow								// Replaced instructions
		+ 5;											// JMP back to original location

	// Allocate hook memory
	newhook.resize(hookLength);

	// Change execute privilege for hook code
	DWORD oldprotect;
	VirtualProtect(newhook.data(), hookLength, PAGE_EXECUTE_READWRITE, &oldprotect);

	// Build hook
	unsigned char* hookptr = newhook.data();

	// Copy preceding original bytes
	memcpy(hookptr, (void*)def->hookAddress, def->bytesPrecede);
	hookptr += def->bytesPrecede;

	// Save original registers
	memcpy(hookptr, "\x50\x51\x52", 3);		// push eax
											// push ecx
											// push edx
	hookptr += 3;

	// Copy custom preceding bytes
	memcpy(hookptr, def->precedeInstructions, def->precedeLength);
	hookptr += def->precedeLength;

	size_t hookExecutorThisPtr = (size_t)hookExecutor_;
	// Set ECX
	*hookptr = 0xb9;						// mov ecx
	hookptr++;
	memcpy(hookptr, (char*)&hookExecutorThisPtr, 4);
	hookptr += 4;

	// Call instruction
	*hookptr = 0xe8;
	hookptr++;

	size_t targetCallAsInt = (size_t)def->targetCall;
	// Note that CALL uses relative addressing, so we need to get the difference from
	// the end of this instruction to the start of our target
	int relativedist = targetCallAsInt - (int)(hookptr + 4);
	memcpy(hookptr, &relativedist, 4);
	hookptr += 4;

	// Following custom instructions
	memcpy(hookptr, def->followInstructions, def->followLength);
	hookptr += def->followLength;

	// Restore original registers
	memcpy(hookptr, "\x5a\x59\x58", 3);		// pop edx
											// pop ecx
											// pop eax
	hookptr += 3;

	// Following replaced instructions
	memcpy(hookptr, (char*)(def->hookAddress + def->bytesReplaced - def->bytesFollow), def->bytesFollow);
	hookptr += def->bytesFollow;

	// JMP back to original code
	// Note that JMP is also relative, so do the same as above
	*hookptr = 0xe9;
	hookptr++;
	relativedist = (int)(def->hookAddress + def->bytesReplaced) - (int)(hookptr + 4);
	memcpy(hookptr, &relativedist, 4);
	hookptr += 4;

	// OK, the hook code itself is done, so now we need to install the hook in
	// the original code.
	unsigned char* original_code = (unsigned char*)def->hookAddress;
	VirtualProtect(original_code, 5, PAGE_EXECUTE_READWRITE, &oldprotect);
	*original_code = 0xe9;
	relativedist = (int)(newhook.data()) - (int)(original_code + 5);
	memcpy(original_code + 1, &relativedist, 4);
	VirtualProtect(original_code, 5, oldprotect, &oldprotect);

}