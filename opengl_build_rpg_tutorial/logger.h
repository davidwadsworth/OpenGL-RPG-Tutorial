#pragma once
#include <iostream>
#include <fstream>

class Logger
{
	static std::ostream& get_stream();
	static bool flag;
	static std::string file_path;
	static std::string get_severity(int severity);
public:
	Logger() = delete;

	static void to_file(std::string file_path = "debug.err"); // call before any message functions to switch program to 
	static void message(std::string message);
	static void warning(std::string message, int severity);
	static void error(std::string message, int severity);
};


