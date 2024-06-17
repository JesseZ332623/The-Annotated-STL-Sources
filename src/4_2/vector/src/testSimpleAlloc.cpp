#include "../include/simpleAlloc.h"

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <memory>

int main(int argc, char const *argv[])
{
    std::srand(std::time(nullptr));

    int * memPtr = Simple_Alloc<int, std::allocator<int>>::allocate(1000);

    for (int index = 0; index < 1000; ++index)
    {
        memPtr[index] = std::rand() % 114 + 1;
    }

    system("cls");
    std::for_each(memPtr, memPtr + 1000, [](const int __n) { std::cout << __n << ' '; });

    Simple_Alloc<int, std::allocator<int>>::deallocate(memPtr, 1000);

    return EXIT_SUCCESS;
}
