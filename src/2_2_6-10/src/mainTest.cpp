#include "./include/myAllocator.h"
#include <cstdio>
#include <bitset>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

/**
 * @brief   利用 std::bitset 将一个十进制整数，转换成二进制字符串
 * 
 * @tparam  dataType 传入数据的类型
 * 
 * @param   __number    传入的十进制整数
 * 
 * @return 转换后的二进制字符串
*/
template <typename dataType>
std::string decToBinary(const dataType __number);

template <typename dataType>
std::string decToBinary(const dataType __number)
{
    /*通过静态断言在编译器检查传入的类型是不是整数*/
    static_assert(std::is_integral<dataType>::value, "dataType must be an integer type!");

    /*将数据 __number 交给 std::bitset 去构建*/
    std::bitset<sizeof(dataType)> bitSet(__number);

    /*返回字符串*/
    return bitSet.to_string();
}

int main(int argc, char const *argv[])
{
    using SimpleAllocate::SimpleAllocator;
    std::srand((unsigned int)time(nullptr));

    std::vector<int, MyAllocator<int>> mydata = {1, 2, 3, 4, 5};

    for (const int number : mydata) { printf("%d ", number); } puts("");

    return EXIT_SUCCESS;
}