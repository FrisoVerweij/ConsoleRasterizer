#include "utils.h"

void assert(bool condition, std::string message, const std::source_location location)
{
	if (!condition)
	{
		std::cout << "Assertion error hit in function '" << location.function_name() << "' at line " << location.line() << " in file " << location.file_name() << ":\nMessage: " << message << std::endl;
		std::terminate();
	}
}