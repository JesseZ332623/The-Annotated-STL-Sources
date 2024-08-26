#include "./showContainer.h"

#include <bits/stl_algobase.h>
#include <numeric>
#include <stdexcept>
#include <cstring>
#include <deque>

/**
 * @brief STL 的 copy() 算法在拷贝区间是否有重叠的情况下，结果可能会出现问题，
 *        本用例便要说明上述情况。
*/

using namespace MyLib::MyLoger;

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

    /**
     * @brief 不过，有些情况下的区间重叠的拷贝会出现错误，
     *        如下面的 deque 所示：
    */
    std::deque<int> numberDeque = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::deque<int> numberDequeCopy(numberDeque.size());

    /**
     * @brief 声明两个 deuque 的迭代器，划定拷贝范围，和输出范围，如下所示：
     * 
     *                first  result     last
     *        {10, 9, [8, 7, <6, 5, 4, 3), 2, 1>}
     * 
     *        拷贝后结果为：
     *      
     *        {10, 9, 8, 7, 8, 7, 6, 5, 4, 1}
     * 
     *         显然，这是不符合预期结果的，原因很简单，deque 所用的迭代器是 RandomAccessIterator，
     *         这样的迭代器传入 copy() 算法中不可以执行非常底层的内存拷贝操作，所以就不会调用 memmove()，
     *         而是非常忠实的更具传入的区间进行拷贝操作。
    */
    std::deque<int>::iterator copyStart     = numberDeque.begin();
    std::deque<int>::iterator copyEnd       = numberDeque.begin();
    std::deque<int>::iterator copyResult    = numberDeque.begin();
    std::advance(copyStart, 2);
    std::advance(copyEnd, 7);
    std::advance(copyResult, 4);

    NOTIFY_LOG("Original numberDeque, size = " + std::to_string(numberDeque.size()) + '\n');
    showContainerToStream(std::cout, numberDeque, numberDeque.size());
    printSplitLine(30, '-');

    // 拷贝区间不重叠的话，自然是没有问题的
    std::copy(copyStart, copyEnd, numberDequeCopy.begin());
    NOTIFY_LOG("Original numberDequeCopy, size = " + std::to_string(numberDeque.size()) + '\n');
    showContainerToStream(std::cout, numberDequeCopy, numberDequeCopy.size());
    printSplitLine(30, '-');

    // 带有重复区间的拷贝，结果会有问题。
    std::copy(copyStart, copyEnd, copyResult);

    NOTIFY_LOG("After std::copy(copyStart, copyEnd, numberDequeCopy.begin()), numberDequeCopy = \n");
    showContainerToStream(std::cout, numberDeque, numberDeque.size());
    printSplitLine(30, '-');

    return EXIT_SUCCESS;
}