#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>

#include <numeric>      // 一些和数值有关的 STL 算法所在
#include <vector>

/**
 * @brief std::adjacent_difference() 求一个容器指定范围内相邻元素的差值，
 *        和 std::accumulate() 一样，第四个参数可以是一个仿函数，
 *        用于确定累计的规则（比如 std::plus，std::minus）。
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::SimpleContainerOperator;

    std::vector<int> numbers = {11, 22, 33, 44, 55, 66, 77, 88, 99, 110};
    std::vector<int> resultNumbers(10);

    std::adjacent_difference(numbers.begin(), numbers.end(), resultNumbers.begin());

    showContainerToStream(std::cout, resultNumbers, 10);

    // 也可以将计算结果覆盖到它自己
    std::adjacent_difference(numbers.begin(), numbers.end(), numbers.begin());

    showContainerToStream(std::cout, numbers, 10);
    
    return EXIT_SUCCESS;
}
