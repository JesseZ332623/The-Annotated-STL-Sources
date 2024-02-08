#include "./simpleAutoPointer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

int main(int argc, char const *argv[])
{
    std::srand((unsigned int)std::time(nullptr));

    myAutoPointer<unsigned int> numberPointer(new unsigned int(114514));
    myAutoPointer<std::string> strPointer(new std::string("Test String."));

    std::cout << *numberPointer << '\n';
    std::cout << *strPointer << '\n';

    return EXIT_SUCCESS;
}
