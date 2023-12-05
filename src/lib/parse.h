#pragma once

#include <string>
#include <vector>

// Emulates python's split() function.
std::vector<std::string> split(std::string&& str, const std::string& delim = " ", bool trim = true);
