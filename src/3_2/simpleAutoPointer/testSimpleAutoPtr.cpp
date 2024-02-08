#include "./include/simpleAutoPointer.h"
#include <iostream>

int main(int argc, char const *argv[])
{

    myAutoPointer<unsigned int> numberPointer(new unsigned int(114514));
    myAutoPointer<std::string> strPointer(new std::string("Test String."));

    std::cout << numberPointer.operator->() << ": " << *numberPointer << '\n';
    std::cout << strPointer.operator->() << ": " << *strPointer << '\n';

    return EXIT_SUCCESS;
}