#include<iostream>
#include<cstdlib>
#include "Error.h"

void fatalError(std::string errorMessage)
{
	std::cout << "ERROR:: " + errorMessage + "\n";
	std::cin.get();
	exit(EXIT_FAILURE);
}
