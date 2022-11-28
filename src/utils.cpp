#include "utils.h"

void assert(bool condition, std::string message)
{
	if (!condition)
	{
		std::cout << message << std::endl;
		std::terminate();
	}
}