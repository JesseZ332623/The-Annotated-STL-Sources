#include <MyLib/simpleContainerOperator.h>

#include <bits/stl_algobase.h>
#include <vector>

/**
 * @brief iter_swap() 没什么好说的，
 *        传入两个有效的迭代器，交换两个迭代器所指向的值。
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;
    using namespace MyLib::MyLoger;

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    std::vector<int>::iterator iterA = numbers.begin() + 2;
    std::vector<int>::iterator iterB = numbers.end() - 2;

    NOTIFY_LOG("Original vector:\n");
    showContainerToStream(std::cout, numbers, numbers.size());

    std::iter_swap(iterA, iterB);

    printSplitLine(12, '-');

    NOTIFY_LOG("After std::iter_swap(iterA, iterB):\n");
    showContainerToStream(std::cout, numbers, numbers.size());

    return EXIT_SUCCESS;
}
