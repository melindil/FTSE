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

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <stdexcept>

#include <Windows.h>
using namespace std;

typedef struct
{
	unsigned char const* source_data;
	unsigned char const* target_data;
	size_t len;
	size_t offset;
} PatchType;

PatchType patches[] = {
	{ 
		// source_data 1
		(unsigned char const*)
		"\x84\xdb\xbe\xa0\xf3\x8a\x00\x74\x27\x68\xb8\xf5\x8a\x00\xc7\x05"
		"\x80\xdf\x8b\x00\x70\xdf\x8b\x00\x89\x35\x78\xdf\x8b\x00\xc7\x05"
		"\x7c\xdf\x8b\x00\x03\x01\x00\x00\xe8\x28\x1f\x01\x00\x83\xc4\x04",

		// target_data 1
		(unsigned char const*)
		"\xeb\x2e"						// jmp short 6c43bb
		"\x68\xae\x43\x6c\x00"			// push offset aFtsedll
		"\xff\x15\x64\xc0\x7f\x00"		// call ds:LoadLibraryA
		"\x85\xc0"						// test eax,eax
		"\x74\x0e"						// jz short 006c43a8
		"\x68\xb7\x43\x6c\x00"			// push offset aBnd
		"\x50"							// push eax
		"\xff\x15\x94\xc1\x7f\x00"		// call ds:GetProcAddress
		"\xff\xd0"						// call eax
		"\xc3"							// ret
		"\x90\x90\x90"					// NOPs
		"\x46\x54\x53\x45\x2e\x44\x4c\x4c\x00"	// aFtsedll: db 'FTSE.DLL',0
		"\x42\x6e\x64\x00",				// aBnd: db 'Bnd',0

		// Length of 1
		0x30,

		// Offset of 1
		0x2c438b

	},
	{
		// source_data 2
		(unsigned char const*)
		"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90",

		// target_data 2
		(unsigned char const*)
		"\x51"						// push ecx
		"\x56"						// push esi
		"\xe8\x92\x60\x00\x00"		// call 6c438d
		"\x83\xcf\xff"				// or edi,ffffffff
		"\x83\xc4\x08"				// add esp,8
		"\xeb\x30",					// jmp short 006be330

		// Length of 2
		0x0f,

		// Offset of 2
		0x2be2f1
	},
	{
		// source_data 3
		(unsigned char const*)
		"\x83\xcf\xff",				// or edi,ffffffff
		
		// target_data 3
		(unsigned char const*)
		"\xeb\xc2"					// jmp short 6be2f1
		"\x90",						// nop

		// Length of 3
		0x03,

		// Offset of 3
		0x2be32d
	},
	{
		NULL,NULL,0,0
	}
};

void BackupFile(std::string const& inputfilename)
{
	string outputfilename(inputfilename);
	outputfilename.append(".ftse.bak");
	if (!CopyFile(inputfilename.c_str(), outputfilename.c_str(), FALSE))
	{
		throw std::runtime_error("Could not produce backup file. Aborting for safety.");
	}
}

void GetFileContent(std::string const& inputfilename, size_t offset, vector<unsigned char>& buf)
{
	ifstream infile(inputfilename, ios::in | ios::binary);
	if (!infile)
	{
		throw std::runtime_error("Unable to read the selected file. Aborting.");
	}

	infile.seekg(offset, ios_base::beg);
	infile.read((char*)buf.data(), buf.size());
	if (!infile)
	{
		throw std::runtime_error("Unable to read the selected file. Aborting.");
	}
	infile.close();
}

void SetFileContent(std::string const& outputfilename, size_t offset,
	unsigned char const* buf, size_t len)
{
	fstream outfile(outputfilename, ios::in | ios::out | ios::binary);
	if (!outfile)
	{
		throw std::runtime_error("Unable to open the selected file for write. Aborting.");
	}

	outfile.seekg(offset, ios_base::beg);
	outfile.write((char*)buf, len);
	if (!outfile)
	{
		throw std::runtime_error("Unable to write the selected file. Aborting.");
	}
	outfile.close();
}

void CopyResources(std::string const& inputfilename)
{
	// Strip the last directory element
	std::string targetdir(inputfilename, 0, inputfilename.find_last_of("\\/"));
	char currdir[1024];
	
	GetModuleFileName(NULL, currdir, 1024);
	std::string sourcefile(currdir);
	std::string sourcedir(sourcefile, 0, sourcefile.find_last_of("\\/"));

	CopyFile((sourcedir+"\\FTSE_config.json").c_str(),
		(targetdir + "\\FTSE_config.json").c_str(), TRUE);
	CopyFile((sourcedir + "\\ftse.lua").c_str(),
		(targetdir + "\\ftse.lua").c_str(), TRUE);
	CopyFile((sourcedir + "\\FTSE.dll").c_str(),
		(targetdir + "\\FTSE.dll").c_str(), FALSE);
}

int main()
{
	try
	{
		char inputfilename[1024];
		ZeroMemory(inputfilename, 1024);
		MessageBoxA(NULL, "Please select the path to the Fallout Tactics BOS.EXE executable to patch.", "FTSEInstaller", MB_ICONINFORMATION);
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = inputfilename;
		ofn.nMaxFile = sizeof(inputfilename);
		ofn.lpstrFilter = "All\0*.*\0BOS Executable\0*.exe\0";
		ofn.nFilterIndex = 2;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == FALSE)
		{
			throw std::runtime_error("No file was selected.  Aborting.");
		}
		PatchType* patch = patches;
		vector<PatchType*> patches_to_apply;
		while (patch->source_data != NULL)
		{
			vector<unsigned char> buf(patch->len);
			GetFileContent(inputfilename, patch->offset, buf);
			if (memcmp(buf.data(), patch->target_data, patch->len) == 0)
			{
				// OK - patch is already applied
			}
			else if (memcmp(buf.data(), patch->source_data, patch->len) != 0)
			{
				throw std::runtime_error("This executable does not match the expected source.  Aborting.");
			}
			else
			{
				patches_to_apply.push_back(patch);
			}
			patch++;
		}
		
		if (patches_to_apply.size() == 0)
		{
			MessageBoxA(NULL, "The FTSE patch has already been applied to this executable. Exiting.", "FTSEInstaller", MB_ICONINFORMATION);
		}
		else
		{
			BackupFile(std::string(inputfilename));
			for (auto patch : patches_to_apply)
			{
				SetFileContent(inputfilename, patch->offset, patch->target_data, patch->len);
			}

			CopyResources(std::string(inputfilename));

			MessageBox(NULL, "Patch applied.  Backup of the executable was created with a .ftse.bak extension.", "FTSEInstaller", MB_ICONINFORMATION);
		}
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, e.what(), "FTSEInstaller", MB_ICONSTOP);
	}
	return 0;

}