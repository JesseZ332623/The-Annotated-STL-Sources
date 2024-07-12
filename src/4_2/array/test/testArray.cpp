#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>

#include "../include/array.h"


int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;

    array<int, 15> array_1 = {1, 23, 2345, 56, 25, -1, 455};
    array<int, 15> array_2 = {65, 5, 45, 445, 5667, 9};

    showContainerToStream(std::cout, array_1, array_1.size());
    showContainerToStream(std::cout, array_2, array_2.size());

    swap(array_1, array_2);

    showContainerToStream(std::cout, array_1, array_1.size());
    showContainerToStream(std::cout, array_2, array_2.size());

    return EXIT_SUCCESS;
}
