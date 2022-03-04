#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>

/*
splits a string into x substrings dividing at all delimiter positions, returned as a vector

eg. delimiter_split("jarvis.png", '.') -> {"jarvis", "png"}

@author David Wadsworth
*/

std::vector<std::string> delimiter_split(const GLchar* str, GLchar delim);


std::vector<std::string> delimiter_split(std::string str, GLchar delim);