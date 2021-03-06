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

template<typename T>
size_t constexpr ConvertFunction(T fxn)
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
		
		ConvertFunction(&HookExecutor::IsRadiated)

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

		ConvertFunction(&HookExecutor::LongTickTrigger)

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

		ConvertFunction(&HookExecutor::DefaultStyleConstructed)

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

		ConvertFunction(&HookExecutor::SetVariableTrigger)

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

		ConvertFunction(&HookExecutor::MsecTimerHook)

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

			ConvertFunction(&HookExecutor::AddBaseTime)

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

		ConvertFunction(&HookExecutor::OnLocaleLoad)

	},
	{
		0x614c3c,
		7,				// Replacing 7 bytes
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

		ConvertFunction(&HookExecutor::OnBurstAttack)

	},
	{
		0x61522f,
		13,				// Replacing 13 bytes
		0,
		13,				// Append the instruction after we run our hook
		"\x50"	// push eax 
		"\x51"	// push ecx
		"\x56",							// push esi
		3,
		"\x85\xc0"						// test eax,eax
		"\x74\x0f"						// jz eip+0f
		"\x48"							// dec eax
		"\x88\x44\xe4\x27"				// mov bute ptr ss:[esp+27],al
		"\x5a"							// pop edx
		"\x59"							// pop ecx
		"\x58"							// pop eax
		"\xb8\x6b\x57\x61\x00"			// mov eax,0061576b
		"\xff\xe0",						// jmp eax

			19,

			ConvertFunction(&HookExecutor::OnConeAttack)

	},
	{
		0x6146bb,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x51"	// push ecx
		"\x50"	// push eax
		"\x56",							// push esi
		3,
		"\x85\xc0"						// test eax,eax
		"\x74\x0f"						// jz eip+0f
		"\x48"							// dec eax
		"\x88\x44\xe4\x1f"				// mov bute ptr ss:[esp+1f],al
		"\x5a"							// pop edx
		"\x59"							// pop ecx
		"\x58"							// pop eax
		"\xb8\x82\x4a\x61\x00"			// mov eax,0061576b
		"\xff\xe0",						// jmp eax

		19,

		ConvertFunction(&HookExecutor::OnSprayAttack)

	},
		{
			0x6158e2,
			7,				// Replacing 7 bytes
			7,				// Append instruction before the hook
			0,
			"\x50"	// push eax
			"\x51"	// push ecx
			"\x56",							// push esi
			3,
			"\x85\xc0"						// test eax,eax
			"\x74\x0f"						// jz eip+0f
			"\x48"							// dec eax
			"\x88\x44\xe4\x23"				// mov bute ptr ss:[esp+23],al
			"\x5a"							// pop edx
			"\x59"							// pop ecx
			"\x58"							// pop eax
			"\xb8\x32\x5e\x61\x00"			// mov eax,00615e32
			"\xff\xe0",						// jmp eax

			19,

			ConvertFunction(&HookExecutor::OnRadialAttack)

		},
		{
			0x6133a7,
			5,				// Replacing 5 bytes
			0,				// Append instruction after the hook
			5,
			"\x51"	// push ecx
			"\x50"	// push eax
			"\x53",							// push ebx
			3,
			"\x85\xc0"						// test eax,eax
			"\x74\x0e"						// jz eip+0e
			"\x48"							// dec eax
			"\x88\x45\xeb"				    // mov bute ptr ss:[ebp-15],al
			"\x5a"							// pop edx
			"\x59"							// pop ecx
			"\x58"							// pop eax
			"\xb8\x09\x3a\x61\x00"			// mov eax,00613a09
			"\xff\xe0",						// jmp eax

			18,

			ConvertFunction(&HookExecutor::OnAreaAttack)

		},
	{
		0x613a66,
		5,				// Replacing 5 bytes
		5,				// Append instruction before the hook
		0,
		"\x57",	// push edi
		1,
		"\x85\xc0"						// test eax,eax
		"\x74\x0a"						// jz eip+0a
		"\x5a"							// pop edx
		"\x59"							// pop ecx
		"\x58"							// pop eax
		"\xb8\x98\x3b\x61\x00"			// mov eax,00613b98
		"\xff\xe0",						// jmp eax

		14,

		ConvertFunction(&HookExecutor::OnStraightAttack)

	},
	{
		0x613c5c,
		6,				// Replacing 6 bytes
		0,
		6,		// after the hook
		"\x57"	// push edi
		"\x56",	// push esi
		2,
		"\x85\xc0"						// test eax,eax
		"\x74\x0a"						// jz eip+0a
		"\x5a"							// pop edx
		"\x59"							// pop ecx
		"\x58"							// pop eax
		"\xb8\x29\x45\x61\x00"			// mov eax,00614529
		"\xff\xe0",						// jmp eax

		14,

		ConvertFunction(&HookExecutor::OnProjectileAttack)

	},
	{
		0x617b2f,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x8b\x55\x1c"		// mov edx,dword ptr ss:[ebp+1c]
		"\x8d\x4d\x84"		// lea ecx,dword ptr ss:[ebp-7c]
		"\x8b\x45\x18"		// mov eax,dword ptr ss:[ebp+18]
		"\x52"				// push edx
		"\x51"				// push ecx
		"\x50"				// push eax
		"\x53"				// push ebx
		"\x56",				// push esi
		14,
		"",
		0,

		ConvertFunction(&HookExecutor::OnChanceToHitCalc)

	},
	{
		0x615f0f,
		5,				// Replacing 5 bytes
		0,
		5,				// Append the instruction after we run our hook
		"\x57",			// push edi
		1,
		"",
		0,

		ConvertFunction(&HookExecutor::OnChanceToHitCalc_SaveWeapon)

	},
	{
		0x61a620,
		5,				// Replacing 5 bytes
		5,
		0,				// Append the instruction before we run our hook
		"\x8b\x54\x24\x28"	// mov edx,dword ptr ss:[esp+28]
		"\x52"				// push edx
		"\x50"				// push eax
		"\x56"				// push esi
		"\x53"				// push ebx
		"\x57",				// push edi
		9,
		"\x89\x44\x24\x08",	// mov dword ptr ss:[esp+8],eax (Need to fix "pop eax", it overwrites the value)
		4,

		ConvertFunction(&HookExecutor::OnChanceToCritical1)

	},
	{
		0x61b4bd,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x57"				// push edi
		"\x55",				// push ebp
		2,
		"\x89\xc7",			// mov edi,eax
		2,

		ConvertFunction(&HookExecutor::OnChanceToCritical2)

	},
	{
		0x61a986,
		11,				// Replacing 11 bytes
		0,
		11,				// Append the instruction after we run our hook
		"\x56"							// push esi
		"\xff\xb4\x24\x88\x00\x00\x00",	// push dword ptr ss:[esp+88]
		8,
		"\x85\xc0"						// test eax,eax
		"\x75\x0a"						// jne [eip+a]
		"\x5a"							// pop edx
		"\x59"							// pop ecx
		"\x58"							// pop eax
		"\xb8\xbf\xa9\x61\x00"			// mov eax,0061a9bf
		"\xff\xe0"						// jmp eax
		"\x8b\xf0"						// mov esi,eax
		"\x89\x74\x24\x20",				// mov dword ptr [esp+20],esi
		
		20,

		ConvertFunction(&HookExecutor::OnCriticalEffect1)

	},
	{
		0x61b5c7,
		11,				// Replacing 11 bytes
		0,
		11,				// Append the instruction after we run our hook
		"\x57"							// push edi
		"\x55",							// push ebp
		2,
		"\x85\xc0"						// test eax,eax
		"\x75\x0a"						// jne [eip+a]
		"\x5a"							// pop edx
		"\x59"							// pop ecx
		"\x58"							// pop eax
		"\xb8\x07\xb6\x61\x00"			// mov eax,0061b607
		"\xff\xe0"						// jmp eax
		"\x8b\xf8"						// mov edi,eax
		"\x89\x7c\x24\x20",				// mov dword ptr [esp+20],edi
		20,

		ConvertFunction(&HookExecutor::OnCriticalEffect2)

	},
	{
		0x612ead,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x50",							// push eax
		1,
		"",
		0,

		ConvertFunction(&HookExecutor::OnDamageCalcSaveHit)

	},
	{
		0x6131ca,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x56",							// push esi
		1,
		"",
		0,

		ConvertFunction(&HookExecutor::OnDamageCalc)

	},
	{
		0x5552b0,
		5,				// Replacing 6 bytes
		0,
		5,				// Append the instruction after we run our hook
		"\x50"			// push eax
		"\x8d\x44\x24\x74"	// lea eax,dword ptr ss:[esp+74]
		"\x50"			// push eax
		"\x57",			// push edi
		7,
		"",
		0,

		ConvertFunction(&HookExecutor::OnInventoryAdd)	// Actor

	},
	{
		0x5ac21d,
		5,				// Replacing 6 bytes
		0,
		5,				// Append the instruction after we run our hook
		"\x52"			// push edx
		"\x50"			// push eax
		"\x57",			// push edi
		3,
		"",
		0,

		ConvertFunction(&HookExecutor::OnInventoryAdd)	// Container

	},
	{
		0x58c2cd,
		5,				// Replacing 6 bytes
		0,
		5,				// Append the instruction after we run our hook
		"\x50"			// push eax
		"\x51"			// push ecx
		"\x57",			// push edi
		3,
		"",
		0,

		ConvertFunction(&HookExecutor::OnInventoryAdd)	// Vehicle

	},
	{
		0x555553,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x52"							// push edx
		"\x8d\x94\x24\x84\x00\x00\x00"	// lea edx,dword ptr ss:[esp+84]
		"\x52"							// push edx
		"\x57",							// push edi
		10,
		"",
		0,

		ConvertFunction(&HookExecutor::OnInventoryRemove)	// Actor

	},
	{
		0x5ac45b,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x51"				// push ecx
		"\x50"				// push eax
		"\x57",				// push edi
		3,
		"",
		0,

		ConvertFunction(&HookExecutor::OnInventoryRemove)	// Container

	},
	{
		0x58c507,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x50"				// push eax
		"\x52"				// push edx
		"\x57",				// push edi
		3,
		"",
		0,

		ConvertFunction(&HookExecutor::OnInventoryRemove)	// Vehicle

	},
	{
		0x559013,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x50"				// push eax
		"\x56",				// push esi
		2,
		"\x84\xc0"			// test al,al
		"\x7c\x0a"			// jl [eip+0a]
		"\x5a"				// pop edx
		"\x59"				// pop ecx
		"\x58"				// pop eax
		"\xba\x31\x90\x55\x00"	// mov edx,00559031
		"\xff\xe2",			// jmp edx
		14,

		ConvertFunction(&HookExecutor::OnCheckItemAllowed)

	},
	{
		0x55490b,
		5,				// Replacing 5 bytes
		0,
		5,					// Append the instruction after we run our hook
		"\x55"				// push ebp
		"\x8d\x44\x24\x78"	// lea eax,dword ptr ss:[esp+78]
		"\x50"				// push eax
		"\x57",				// push edi
		7,
		"\x85\xc0"			// test eax,eax
		"\x74\x36"			// jz eip+0x36 
		// (we have to copy a lot of ASM code here since the existing code to
		//  return an error doesn't quite fit)
		"\x8b\x74\x24\x70"	// mov esi,dword ptr ss:[esp+70]
		"\xc7\x06\x01\x00"
		"\x00\x00"			// mov dword ptr ds:[esi],1
		"\x89\x46\x10"		// mov dword ptr ds:[esi+10],eax
		"\x8b\x44\x24\x2c"	// mov eax,dword ptr ss:[esp+2c]
		"\x8b\x54\x24\x30"	// mov edx,dword ptr ss:[esp+30]
		"\x89\x46\x04"		// mov dword ptr ds:[esi+4],eax
		"\x89\x56\x08"		// mov dword ptr ds:[esi+8],edx
		"\x8b\x44\x24\x34"	// mov eax,dword ptr ss:[esp+34]
		"\x8b\x54\x24\x40"	// mov edx,dword ptr ss:[esp+40]
		"\x89\x46\x0c"		// mov dword ptr ds:[esi+c],eax
		"\x89\x56\x18"		// mov dword ptr ds:[esi+18],edx
		"\x89\x5e\x14"		// mov dword ptr ds:[esi+14],ebx
		"\x5a"				// pop edx
		"\x59"				// pop ecx
		"\x58"				// pop eax
		"\xb8\x70\x4b\x55\x00"	// mov eax,0x554b70
		"\xff\xe0",			// jmp eax
		58,

		ConvertFunction(&HookExecutor::OnCheckEquip)

	},
	{
		0x554a62,
		10,				// Replacing 10 bytes
		0,
		10,				// Append the instruction after we run our hook
		"\x55"				// push ebp
		"\x51"				// push ecx
		"\x57",				// push edi
		3,
		"",
		0,

		ConvertFunction(&HookExecutor::OnEquip)

	},
	{
		0x554df4,
		12,				// Replacing 12 bytes
		12,				// Execute those instructions before our hook
		0,
		"\x57"				// push edi
		"\x51"				// push ecx
		"\x56",				// push esi
		3,
		"\x85\xc0"			// test eax,eax
		"\x74\x36"			// jz eip+0x36 
		// (we have to copy a lot of ASM code here since the existing code to
		//  return an error doesn't quite fit)
		"\x8b\x74\x24\x54"	// mov esi,dword ptr ss:[esp+54]
		"\xc7\x06\x01\x00"
		"\x00\x00"			// mov dword ptr ds:[esi],1
		"\x89\x46\x10"		// mov dword ptr ds:[esi+10],eax
		"\x8b\x44\x24\x2c"	// mov eax,dword ptr ss:[esp+2c]
		"\x8b\x54\x24\x30"	// mov edx,dword ptr ss:[esp+30]
		"\x89\x46\x04"		// mov dword ptr ds:[esi+4],eax
		"\x89\x56\x08"		// mov dword ptr ds:[esi+8],edx
		"\x8b\x44\x24\x34"	// mov eax,dword ptr ss:[esp+34]
		"\x8b\x54\x24\x40"	// mov edx,dword ptr ss:[esp+40]
		"\x89\x46\x0c"		// mov dword ptr ds:[esi+c],eax
		"\x89\x56\x18"		// mov dword ptr ds:[esi+18],edx
		"\x89\x5e\x14"		// mov dword ptr ds:[esi+14],ebx
		"\x5a"				// pop edx
		"\x59"				// pop ecx
		"\x58"				// pop eax
		"\xb8\x19\x4f\x55\x00"	// mov eax,0x554f19
		"\xff\xe0",			// jmp eax
		58,

		ConvertFunction(&HookExecutor::OnCheckUnequip)

	},
	{
		0x554e39,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x57"				// push edi
		"\x8b\x44\x24\x5c"	// mov eax,dword ptr ss:[esp+5c]
		"\x50"				// push eax
		"\x56",				// push esi
		7,
		"",
		0,

		ConvertFunction(&HookExecutor::OnUnequip)

	},
	{
		0x573dd8,
		6,				// Replacing 6 bytes
		0,
		6,				// Append the instruction after we run our hook
		"\x8b\x44\x24\x78"	// mov eax,dword ptr ss:[esp+78]
		"\x50"				// push eax
		"\x51",				// push ecx
		6,
		"\x85\xc0"			// test eax,eax
		"\x74\x0a"			// jz eip+0x07
		"\x5a"				// pop edx
		"\x59"				// pop ecx
		"\x5b"				// pop ebx
		"\xbb\x78\x42\x57\x00"	// mov ebx,0x574278
		"\xff\xe3"			// jmp ebx
			,
		14,

		ConvertFunction(&HookExecutor::SwapFix)

	},
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