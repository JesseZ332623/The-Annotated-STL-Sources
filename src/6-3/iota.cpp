#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>

#include <vector>
#include <functional>
#include <numeric>

/**
 * @brief std::iota() 的功能非常简单，
 *        就是在容器的给定范围内制造一个由 __value 开始的递增序列。
 * 
 *        至于 iota 到底是什么的缩写，就不得而知了。。。。
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;

    std::vector<int> numberList(10);

    std::iota(numberList.begin(), numberList.end(), 0);

    showContainerToStream(std::cout, numberList, 10);

    return EXIT_SUCCESS;
}
