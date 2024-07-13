#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>

#include "../include/array.h"

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;

#if true
    array<int, 15> array_1 = {1, 23, 2345, 56, 25, -1, 455};
    array<int, 15> array_2 = {65, 5, 45, 445, 5667, 9};

    showContainerToStream(std::cout, array_1, array_1.size());
    showContainerToStream(std::cout, array_2, array_2.size());

    swap(array_1, array_2);

    showContainerToStream(std::cout, array_1, array_1.size());
    showContainerToStream(std::cout, array_2, array_2.size());
#endif

    int cStyleArray[] = {16, 3564, 56, 13, 6, 134, 546, -52};
    std::string someStrings[] = {"wdgfhyrg", "565", "cvfds", "dgfeehjty"};

    array<int, 8> array_3 = toArray(cStyleArray);
    array<int, 8> array_4 = toArray({16, 3564, 56, 13, 6, 134, 546, -52});
    array<std::string, 4> stringArray = toArray(someStrings);

    constexpr int constexprArray[] = {1, 2, 3, 4};
    array<int, 4> array_5 = {};
    array_5 = toArray(constexprArray);

    showContainerToStream(std::cout, array_4, array_4.size());
    showContainerToStream(std::cout, array_5, array_5.size());
    showContainerToStream(std::cout, stringArray, stringArray.size());

    return EXIT_SUCCESS;
}
