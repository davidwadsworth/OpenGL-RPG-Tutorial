#include "logger.h"
#include "game.h"

bool Logger::flag = true;
std::string Logger::file_path = "debug.err";

std::ostream& Logger::get_stream() {
	static std::ofstream file(Logger::file_path);
	return Logger::flag ? std::cerr : file;
}

std::string Logger::get_severity(int severity)
{
	std::string sev;
	switch (severity)
	{
	case 0:
		sev = "Low";
		break;
	case 1:
		sev = "Medium";
		break;
	default:
		sev = "High";
	}
	return sev;
}

void Logger::to_file(std::string file_path)
{
	Logger::flag = false;
	Logger::file_path = file_path;
}

void Logger::message(std::string message)
{
	Logger::get_stream() << message << std::endl;
}

void Logger::warning(std::string message, int severity)
{
	Logger::get_stream() << "WARNING: " << message << std::endl << "SEVERITY: " << Logger::get_severity(severity) <<  std::endl;
}

void Logger::error(std::string message, int severity)
{
	Logger::get_stream() << "ERROR: " << message << std::endl << "SEVERITY: " << Logger::get_severity(severity) << std::endl;

	if (severity > 0)
		Game::exit = true;
	if (severity > 1)
		throw;
}
