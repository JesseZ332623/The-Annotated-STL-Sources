#include <MyLib/myLogerDef.h>

#include <numeric>      // 一些和数值有关的 STL 算法所在
#include <vector>

/**
 * std::inner_product() 求两个容器相同范围内的一般内积。
 * 一般内积的概念见：
 * https://github.com/JesseZ332623/The-Annotated-STL-Sources/blob/master/src/6-3/document/inner_product.png
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    std::vector<int> vector_v = {1, 2, 3, 4, 5};
    std::vector<int> vector_u = {12, 23, 34, 45, 50};

    /**
     * 0 + 1 * 12 + 2 * 23 + ... + 5 * 50
     */
    int innerProductResult = std::inner_product(
        vector_u.begin(), vector_u.end(), 
        vector_v.begin(), 0
    );

    /**
     * 当然也有特殊用法：这里传入仿函数，计算过程就变为：
     * 
     * 1000 + (1 - 12) + (2 - 23) - ... + (5 - 50)
    */
    int speialProductResult = std::inner_product(
        vector_u.begin(), vector_u.end(),
        vector_v.begin(), 1000,
        std::plus<int>(), std::minus<int>()
    );

    NOTIFY_LOG("Vector v = { ");
    for (const int num : vector_v) { CORRECT_LOG(num); std::putchar(' '); }
    NOTIFY_LOG("}\n");

    NOTIFY_LOG("Vector u = { ");
    for (const int num : vector_u) { CORRECT_LOG(num); std::putchar(' '); }
    NOTIFY_LOG("}\n");

    NOTIFY_LOG("<v, u> = " + std::to_string(innerProductResult) + '\n');
    NOTIFY_LOG("Special Product Result = " + std::to_string(speialProductResult) + '\n');

    return EXIT_SUCCESS;
}
