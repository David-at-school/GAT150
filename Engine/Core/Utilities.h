#pragma once
#include <string>
#include <SDL.h>

namespace ds
{
	std::string string_tolower(const std::string& str);
	bool istring_compare(const std::string& str1, const std::string& str2);
}