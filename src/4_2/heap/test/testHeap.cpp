#include "../include/heap_algorithm.h"
//#include "../../vector/include/myVector.h"

#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>
#include <vector>

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::SimpleContainerOperator;

    std::vector<int> container = {0, 1, 2, 3, 4, 8, 9, 3, 5, 4351, 764, 3324};

    showContainerToStream(std::cout, container, container.size());

    std::make_heap(container.begin(), container.end());

    showContainerToStream(std::cout, container, container.size());

    std::pop_heap(container.begin(), container.end());

    showContainerToStream(std::cout, container, container.size());

    std::sort_heap(container.begin(), container.end());

    showContainerToStream(std::cout, container, container.size());

    return EXIT_SUCCESS;
}
