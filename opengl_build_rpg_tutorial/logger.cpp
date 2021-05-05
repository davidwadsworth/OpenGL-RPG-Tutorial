#include "logger.h"
#include "game.h"

bool Logger::flag = true;
std::string Logger::file_path = "";

std::unique_ptr<std::ostream> Logger::get_stream() {
	return (Logger::flag
		? std::make_unique<std::ostream>(std::cerr.rdbuf())
		: std::make_unique<std::ofstream>(Logger::file_path)
		);
}

void Logger::to_file(std::string file_path)
{
	Logger::flag = false;
	Logger::file_path = file_path;
}

void Logger::message(std::string message)
{
	*Logger::get_stream() << message << std::endl;
}

void Logger::warning(std::string message, int severity)
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

	*Logger::get_stream() << "WARNING: " << message << std::endl << "SEVERITY: " << sev <<  std::endl;
}

void Logger::error(std::string message, int severity)
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

	*Logger::get_stream() << "ERROR: " << message << std::endl << "SEVERITY: " << sev << std::endl;

	if (severity > 0)
		Game::exit = true;
	if (severity > 1)
		throw;
}
