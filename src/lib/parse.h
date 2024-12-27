#pragma once

#include <string>
#include <utility>
#include <vector>

#include "lib/to.h"

// Emulates python's split() function.
std::vector<std::string> split(std::string&& str,
                               const std::string& delim = " ",
                               bool delimMultiChar = false,
                               bool trimOriginal = true,
                               bool trimSplit = true);

template <class To>
To splitTo(std::string&& str,
           const std::string& delim = " ",
           bool delimMultiChar = false,
           bool trimOriginal = true,
           bool trimSplit = true) {
  return to<To>(split(std::move(str), delim, delimMultiChar, trimOriginal, trimSplit));
}

template <class Lhs = std::string, class Rhs = std::string>
std::pair<Lhs, Rhs> splitToPair(std::string&& str,
                                const std::string& delim = " ",
                                bool delimMultiChar = false,
                                bool trimOriginal = true,
                                bool trimSplit = true) {
  return splitTo<std::pair<Lhs, Rhs>>(std::move(str), delim, delimMultiChar, trimOriginal,
                                      trimSplit);
}
