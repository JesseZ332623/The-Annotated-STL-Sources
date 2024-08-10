#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>

#include <numeric>      // 一些和数值有关的 STL 算法所在
#include <vector>
#include <functional>

/**
 * @brief std::partial_sum() 对容器指定范围内的数据计算 “局部总和”
 *        例：
 *        numbers = {1, 2, 3, 4, 5};
 *        results = {};
 *         
 *        std::partial_sum(numbers.begin(), numbers.end(), result.begin());
 * 
 *        results = {1, 3, 6, 10, 15};
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;
    using namespace MyLib::MyLoger;

    system("cls");

    std::vector<std::size_t> numbers;
    std::vector<std::size_t> result(10);

    pushRandomValue(numbers, 64);

    NOTIFY_LOG("numbers = ");
    showContainerToStream(std::cout, numbers, 10);

    std::partial_sum(numbers.begin(), numbers.end(), result.begin());

    NOTIFY_LOG("\nAfter std::partial_sum(numbers.begin(), numbers.end(), result.begin())\n");
    NOTIFY_LOG("result  = ");
    showContainerToStream(std::cout, result, 10);

    /**
     * 和其他算法一样，也可以传入仿函数等，改变计算的规则。
     */
    std::partial_sum(
        numbers.begin(), numbers.end(), result.begin(),
        std::multiplies<int>() 
    );

    NOTIFY_LOG("\nAfter std::partial_sum(numbers.begin(), numbers.end(), result.begin(), std::multiplies<int>())\n");
    NOTIFY_LOG("result  = ");
    showContainerToStream(std::cout, result, 10);

    DONE;
    return EXIT_SUCCESS;
}
