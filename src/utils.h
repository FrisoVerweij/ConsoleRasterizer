#pragma once
#include <string>
#include <source_location>

void assert(bool condition, std::string message, const std::source_location location = std::source_location::current());