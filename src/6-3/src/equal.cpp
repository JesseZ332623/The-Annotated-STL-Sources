#include <MyLib/simpleContainerOperator.h>

#include <vector>
#include <bits/stl_algobase.h>

/**
 * @brief std::equal() 算法
 *        用于判断两个容器的指定迭代器范围内数据是否完全相等
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;
    using namespace MyLib::MyLoger;

    /**
     * 显然，数列 A 和 B 的内容不完全相等
    */
    std::vector<int> numberList_A = {1, 2, 3, 5, 7};
    std::vector<int> numberList_B = {1, 2, 3, 4, 5};

    NOTIFY_LOG("numberList_A = ");
    showContainerToStream(std::cout, numberList_A);

    NOTIFY_LOG("numberList_B = ");
    showContainerToStream(std::cout, numberList_B);

    if (std::equal(numberList_A.begin(), numberList_A.end(), numberList_B.begin()))
    {
        NOTIFY_LOG("numberList_A == numberList_B\n");
    }
    else { WARNING_LOG("numberList_A != numberList_B\n"); }

    return EXIT_SUCCESS;
}
