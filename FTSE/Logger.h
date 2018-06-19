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

#include <fstream>
#include <string>

struct lua_State;

class Logger
{
public:
	Logger(std::string const& name);
	~Logger();

	void RegisterLUA(lua_State* l);

	void Log(std::string const& msg);

	template<typename T>
	Logger& operator<<(T& t) 
	{ 
		if (dotimestamp_)
		{
			timestamp();
		}
		logfile_ << t; 
		return *this;
	}
	template<typename T>
	Logger& operator<<(T const& t)
	{ 
		if (dotimestamp_)
		{
			timestamp();
		}
		logfile_ << t; 
		return *this; 
	}

	// this is the type of std::cout
	typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
	// this is the function signature of std::endl
	typedef CoutType& (*StandardEndLine)(CoutType&);
	// define an operator<< to take in std::endl
	Logger& operator<<(StandardEndLine manip)
	{
		manip(logfile_);
		dotimestamp_ = true;
		return *this;
	}
private:
	void timestamp();
	bool dotimestamp_;
	std::ofstream logfile_;
};



