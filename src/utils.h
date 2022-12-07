#pragma once
#include <string>
#include <vector>
#include <source_location>

void assert(bool condition, std::string message, const std::source_location location = std::source_location::current());

std::vector<std::string> splitString(std::string line, std::string delimiter);

std::vector<float> vectorStof(const std::vector<std::string>& input);
std::vector<int> vectorStoi(const std::vector<std::string>& input);