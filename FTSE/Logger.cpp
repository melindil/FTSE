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

#include "Logger.h"

#include <Windows.h>
#include <iostream>
#include <iomanip>

#include "lua.hpp"

#include <errno.h>

Logger::Logger(std::string const& logname)
	: logfile_(logname, std::ios::out), dotimestamp_(true)
{
	if (!logfile_)
	{
		std::cout << errno << std::endl;
	}
}

Logger::~Logger()
{
}

void Logger::timestamp()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	logfile_ << std::setw(4) << std::setfill('0') << st.wYear << "/"
		<< std::setw(2) << std::setfill('0') << st.wMonth << "/"
		<< std::setw(2) << std::setfill('0') << st.wDay << " "
		<< std::setw(2) << std::setfill('0') << st.wHour << ":"
		<< std::setw(2) << std::setfill('0') << st.wMinute << ":"
		<< std::setw(2) << std::setfill('0') << st.wSecond << "."
		<< std::setw(3) << std::setfill('0') << st.wMilliseconds << " ";
	dotimestamp_ = false;
}

void Logger::Log(std::string const& msg)
{
	timestamp();
	*this << msg << std::endl;
}

static int l_log(lua_State* l)
{
	Logger** mylogger = (Logger**)luaL_checkudata(l, 1, "Logger");
	(*mylogger)->Log(std::string(luaL_checkstring(l, 2)));
	return 0;

}

void Logger::RegisterLUA(lua_State* l)
{
	Logger** logptrptr = (Logger**)lua_newuserdata(l, sizeof(Logger*));
	*logptrptr = this;
	luaL_newmetatable(l, "Logger");
	lua_pushcfunction(l, l_log);
	lua_setfield(l, -2, "log");
	lua_pushvalue(l, -1);
	lua_setfield(l, -2, "__index");
	lua_setmetatable(l, -2);
	lua_setglobal(l, "logger");
}