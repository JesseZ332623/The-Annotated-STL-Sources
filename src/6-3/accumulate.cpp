#include <MyLib/myLogerDef.h>

//#include <iterator>     // std::ostream_iterator 所在
#include <numeric>      // 一些和数值有关的 STL 算法所在

#include <functional>
#include <vector>

/**
 * @brief std::accumulate() 负责对容器的指定范围内进行累计（求和，求差或其他操作）
 *        第三个参数 __init 表示计算的初始值，
 *        第四个参数可以是一个仿函数，用于确定累计的规则（比如 std::plus，std::minus）
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    std::vector<int> numbers = {1, 2, 3, 4, 6, 7, 8, 9, 10};

    int partTotal   = std::accumulate(numbers.begin(), numbers.begin() + 5, 0);
    
    int totalNumber = std::accumulate(numbers.begin(), numbers.end(), 0, std::plus<int>());

    CORRECT_LOG(
        "Total of vector numbers = " + std::to_string(totalNumber) + 
        "\nnumbers[0] + ... + numbers[5] = " + std::to_string(partTotal) + '\n'
    );

    return EXIT_SUCCESS;
}
