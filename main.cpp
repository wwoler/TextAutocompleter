#include "Autocompleater.h"
#include <iostream>

auto main() -> int
{
	Autocompleater autocomp;
	std::string message;

	std::cout << "Enter message[? - show | < - back | > - forward]: ";
	autocomp.input(message);

	std::cout << "You entered: "
		<< message << std::endl;

	return EXIT_SUCCESS;
}