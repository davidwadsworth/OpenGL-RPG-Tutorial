#pragma once
#include <iostream>
#include <fstream>

class Logger
{
	static std::unique_ptr<std::ostream> get_stream();
	static bool flag;
	static std::string file_path;
public:
	Logger() = delete;

	static void to_file(std::string file_path = "output.err");
	static void message(std::string message);
	static void warning(std::string message, int severity);
	static void error(std::string message, int severity);
};


