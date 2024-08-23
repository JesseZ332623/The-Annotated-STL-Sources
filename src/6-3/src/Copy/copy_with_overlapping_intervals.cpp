#include <MyLib/myLogerDef.h>

#include <bits/stl_algobase.h>
#include <numeric>
#include <stdexcept>
#include <cstring>

#ifndef NEED_PTR
#define NEED_PTR false
#endif

/**
 * @brief STL 的 copy() 算法在拷贝区间是否有重叠的情况下，结果可能会出现问题，
 *        本用例便要说明上述情况。
*/

using namespace MyLib::MyLoger;

template <typename Type>
void showContain(const Type * __begin, const Type * __end, const std::string & __describe);

int main(int argc, char const *argv[])
{
    /**
     * @brief 只用数组简单的说明情况，
     *        但实际上 copy() 算法的结果和容器的迭代器实现有很大的关系。
    */
    int array[10]      = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int emptyArray[10] = {0};

    int * resultCopyEnd = nullptr;

    /**
     * @brief [array + 2, array + 5) 区间在数组中如下所示：
     * 
     *        {1, 2, [3, 4, 5, 6), 7, 8, 9, 10}
     * 
     *        将区间中的数据拷贝到另一个相同类型的容器，区间并无重叠，结果肯定没问题。
     * 
     *        copy() 算法返回一个迭代器，指向了 __result + (__begin - __end) 这个位置
    */
    resultCopyEnd = std::copy(array + 2, array + 5, emptyArray);

    system("cls");
    showContain(array, array + 10, "Original array:");
    showContain(emptyArray, emptyArray + 10, "emptyArray receive array's data:");


    showContain(array, array + 10, "Original array:");

    /**
     * @brief 输出区间的终点与输入区间的重叠，
     *        在连续内存上进行该操作，结果是符合预期的。
     *         
     *        result      first           last
     *        {<1, 2, 3, [4, 5>, 6, 7, 8, 9), 10}
     * 
     *        拷贝后结果为：
     *        
     *        {4, 5, 6, 7, 8, 6, 7, 8, 9, 10}
     * 
    */
    resultCopyEnd = std::copy(array + 3, array + 8, array);
    showContain(array, array + 10, "array after overlapping intervals copy:");

    std::iota(array, array + 10, 1);
    showContain(array, array + 10, "Original array:");

#if true
    /**
     * @brief 输出区间的起点和 copy 区间重叠，
     *        在连续内存上操作结果就有可能不符合预期了。
     *        当然，现代 STL 足够智能，在检查到上述情况后，
     *        就直接调用 std::memmove() 想将原内存拷贝一份在进行拷贝，而非 std::memcopy()
     *        
     *              result
     *          first           last
     *        {[1, <2, 3, 4, 5, 6), 7>, 8, 9, 10}
     * 
     *        拷贝后结果为：
     *        
     *        {1, 1, 2, 3, 4, 5, 7, 8, 9, 10}
    */
    resultCopyEnd = std::copy(array, array + 5, array + 1);
    showContain(array, array + 10, "array after overlapping intervals copy:");
#endif
    
    return EXIT_SUCCESS;
}

template <typename Type>
void showContain(const Type * __begin, const Type * __end, const std::string & __describe)
{
    if (!__begin || !__end) {
        throw std::invalid_argument("showContain(): Fanction argument is nullptr!\n");
    }

    NOTIFY_LOG(__describe + '\n');
    while (__begin != __end)
    {
        CORRECT_LOG(*__begin); putchar(' ');

#if NEED_PTR      
        putchar('\t');
        CORRECT_LOG(__begin);
        putchar('\n');
#endif

        ++__begin;
    }
    putchar('\n');
    printSplitLine(30, '-');
}