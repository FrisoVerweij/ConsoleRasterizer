#include <iostream>
#include "utils.h"

void assert(bool condition, std::string message, const std::source_location location)
{
	if (!condition)
	{
		std::cout << "Assertion error hit in function '" << location.function_name() << "' at line " << location.line() << " in file " << location.file_name() << ":\nMessage: " << message << std::endl;
		std::terminate();
	}
}

std::vector<std::string> splitString(std::string line, std::string delimiter)
{
	std::vector<std::string> output;

	size_t idx;
	while ((idx = line.find_first_of(delimiter)) != std::string::npos)
	{
		output.push_back(line.substr(0, idx));
		line.erase(0, idx + 1);
	}
	output.push_back(line);
	return output;
}

std::vector<float> vectorStof(const std::vector<std::string>& input)
{
	std::vector<float> output;
	output.reserve(input.size());
	for (std::string s : input)
		output.push_back(std::stof(s));

	return output;
}

std::vector<int> vectorStoi(const std::vector<std::string>& input)
{
	std::vector<int> output;
	output.reserve(input.size());
	for (std::string s : input)
		output.push_back(std::stoi(s));

	return output;
}