#include "delimiter_split.h"

std::vector<std::string> delimiter_split(const GLchar* str, GLchar delim)
{
	std::vector<std::string> result;
	std::string temp;

	while (*str)
	{
		// if delimiter is found pushback temp string into result list and start on the next string
		if (*str == delim)
		{
			result.push_back(std::string(temp));
			temp.clear();
			str++;
		}
		temp.push_back(*str++);
	}
	result.push_back(std::string(temp));

	return result;
}

std::vector<std::string> delimiter_split(std::string str, GLchar delim)
{
	return delimiter_split(str.c_str(), delim);
}
