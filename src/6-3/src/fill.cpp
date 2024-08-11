#include <MyLib/simpleContainerOperator.h>

#include <list>
#include <bits/stl_algobase.h>

/**
 * @brief std::fill() 和 std::fill_n() 没什么好说的，
 *        将容器指定迭代器范围内的值全部设成 value
 */

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;

    std::list<std::string> stringList(10);

    std::fill(stringList.begin(), stringList.end(), "Good Job!");

    showContainerToStream(std::cout, stringList, 10);

    std::fill_n(stringList.begin(), 10, "Do you like VAN you see~");

    showContainerToStream(std::cout, stringList, 2);

    std::vector<int> numbers(5);

    /**
     * 显然，像这样的越界覆写操作是很危险的。
    */
    //std::fill_n(numbers.begin(), 100, 1);

    /**
     * 正确的做法是为这个容器创建一个插入迭代器，
     * 然后再进行 “越界” 访问操作。
     * 
     * 像下面的调用，以 numbers 的 numbers.begin() 迭代器为基础，
     * 产生一个插入迭代器，每进行一次赋值操作，都会在 numbers.begin() 处进行一次插入。
    */
    std::fill_n(std::inserter(numbers, numbers.begin()), 15, 43);

    showContainerToStream(std::cout, numbers, 15 + numbers.size());

    return EXIT_SUCCESS;
}
