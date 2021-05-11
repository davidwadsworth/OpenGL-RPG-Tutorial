#pragma once
#include <iostream>
#include <fstream>

class Logger
{
	static std::ostream& get_stream();
	static bool flag;
	static std::string file_path;
public:
	Logger() = delete;

	static enum SEVERITY
	{
		LOW,
		MEDIUM,
		HIGH
	};

	static void to_file(std::string file_path = "debug.err"); // call before any message functions to switch program to 
	static void message(std::string message);
	static void warning(std::string message, SEVERITY severity);
	static void error(std::string message, SEVERITY severity);

private:
	static std::string get_severity(SEVERITY severity);
};


