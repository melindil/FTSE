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

#include <stdint.h>

#pragma pack(push,1)
typedef struct
{
	char* perkShortName;
	uint32_t maxLevels;
	uint32_t reqLevel;
	char* perkBonusStat;
	uint32_t perkBonusAmt;
	char* perkReqStat1;
	uint32_t perkReqAmt1;
	uint32_t perkAndOrFlag;
	char* perkReqStat2;
	uint32_t perkReqAmt2;
	uint32_t perkReqST;
	uint32_t perkReqPE;
	uint32_t perkReqEN;
	uint32_t perkReqCH;
	uint32_t perkReqIN;
	uint32_t perkReqAG;
	uint32_t perkReqLK;
	char* perkRestrictedRace;
	char* perkRequiredRace;
} FOTPerkTableEntry;
#pragma pack(pop)