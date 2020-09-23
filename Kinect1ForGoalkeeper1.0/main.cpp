#include "System.h"

int main()
{
	System system;

	if (!system.Init()){
		std::cout << "init failed£¡" << std::endl;
		return -1;
	}

	if (!system.Run()){
		std::cout << "run time error£¡" << std::endl;
		return -1;
	}

	return 0;
}