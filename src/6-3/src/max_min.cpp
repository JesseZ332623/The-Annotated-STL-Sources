#include <MyLib/myLogerDef.h>

#include <bits/stl_algobase.h>

/**
 * @brief std::max() 和 std::min() 用法不用多说，
 *        用于比较两个值的大小。当然
*/

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    int array[] = {45, 95};

    NOTIFY_LOG(
        "Max value = " + 
        std::to_string(std::max(*array, *(array + 1))) + '\n'
    );

    NOTIFY_LOG(
        "Min value = " + 
        std::to_string(std::min(*array, *(array + 1))) + '\n'
    );

    return EXIT_SUCCESS;
}
