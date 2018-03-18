// HelloWorld.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include "lib.h"
//#include "version.h"

int main()
{
	std::cout << "Hello, World! v." << version() << "\n";
    return 0;
}
