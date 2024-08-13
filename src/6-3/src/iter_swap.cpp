#include <MyLib/simpleContainerOperator.h>

#include <bits/stl_algobase.h>
#include <vector>

/**
 * @brief iter_swap() 没什么好说的，
 *        传入两个有效的迭代器，交换两个迭代器所指向的值。
 * 
 * 值得注意的是：在旧版本由于语言特性的不完全，
 * 需要通过类型萃取得到传入迭代器的 value_type，然后再进行交换，
 * 但在 C++11 标准以及后续的版本，C++ 标准库中自带 std::swap()，
 * 也就不需要那么复杂的准备工作了。 
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;
    using namespace MyLib::MyLoger;

    std::vector<int> numbers    = {1, 2, 3, 4, 5, 6, 114514};
    std::vector<int> numbers_B  = {1, 3, 5, 7};

    std::vector<int>::iterator iterA = numbers.end() - 1;
    std::vector<int>::iterator iterB = numbers_B.begin();

    NOTIFY_LOG("Original vector:\n");
    showContainerToStream(std::cout, numbers, numbers.size());
    showContainerToStream(std::cout, numbers_B, numbers_B.size());

    // 即使两个迭代器来自两个同类型的不同容器，也是可以执行的。
    std::iter_swap(iterA, iterB);

    printSplitLine(35, '-');

    NOTIFY_LOG("After std::iter_swap(iterA, iterB):\n");
    showContainerToStream(std::cout, numbers, numbers.size());
    showContainerToStream(std::cout, numbers_B, numbers_B.size());

    return EXIT_SUCCESS;
}
