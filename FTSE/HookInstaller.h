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

#pragma once
#include <list>
#include <vector>

#include "HookExecutor.h"
#include "Logger.h"

class HookInstaller
{
public:
	HookInstaller(Logger* logger,std::string const& luaname);
	~HookInstaller();

	void installHooks();
	typedef struct 
	{
		size_t hookAddress;					// Address of instruction(s) to replace with the hook
		size_t bytesReplaced;				// How many instruction bytes will be replaced (min 5)
		size_t bytesPrecede;				// How many instruction bytes to execute before hook
		size_t bytesFollow;					// How many instruction bytes to execute after hook
		char const* precedeInstructions;	// Custom instructions to precede hook
		size_t precedeLength;				// Custom instruction length
		char const* followInstructions;		// Custom instructions to follow hook
		size_t followLength;				// Custom instruction length
		size_t targetCall;					// Address of the target function to call
	} HookDefinition;

private:

	static HookDefinition hooks_[];

	void installOneHook(HookDefinition* def);

	std::list<std::vector<unsigned char> > hookCode_;
	Logger* logger_;

	HookExecutor* hookExecutor_;
};

