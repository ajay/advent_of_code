#include <string>
#include <vector>

// Emulates python's split() function.
std::vector<std::string> split(std::string&& str,
                               const std::string& delim = " ",
                               bool trim = true);

std::vector<size_t> strVecToNumVec(const std::vector<std::string>& strVec);

std::pair<std::string, std::string> strVecToStrPair(
    const std::vector<std::string>& strVec);

std::pair<size_t, size_t> strVecToNumPair(
    const std::vector<std::string>& strVec);

std::vector<size_t> strToNumVec(const std::string& str);
