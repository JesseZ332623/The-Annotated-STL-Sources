#include "../include/myVector.h"

#include <iostream>
#include <algorithm>

int main(int argc, char const *argv[])
{
    My_Vector<int> newVec(10, 125);

    std::cout << newVec.size() << ' ';

    std::for_each(newVec.begin(), newVec.end(), [](const int __n) { std::cout << __n << ' '; });

    return EXIT_SUCCESS;
}
