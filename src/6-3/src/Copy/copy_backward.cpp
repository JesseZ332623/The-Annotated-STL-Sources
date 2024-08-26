#include "./showContainer.h"

#include <bits/stl_algobase.h>
#include <numeric>
#include <deque>

/**
 * @brief std::copy_backward() 算法和 copy() 类似，
 *        只是执行 “逆向拷贝”，即从区间 [first, last) 的 last 迭代器开始，
 *        逆向拷贝到 result 迭代器，result 也是逆向移动的。
 * 
 *        需要注意的是，std::copy_backward() 仅接受被标记为：BidirectionalIterator（双向迭代器），
 *        来执行它那 “倒行逆施” 的拷贝操作。
*/

int main(int argc, char const *argv[])
{
    system("cls");

    int array[]           = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    std::size_t arraySize = sizeof(array) / sizeof(int);

    int arrayCopy[arraySize] = {0};

    showContain(array, array + arraySize, "Original array:");

    /**
     * @brief std::copy_backward() 的基本用法（不重叠的情况）
     * 
     *                                   result
     *             first           last
     *      {0, 1, [2, 3, 4, 5, 6, 7), 8}  ↑
     *              <------------
     *                         <------------
     * 
     *      结果为：
     * 
     *      {0, 1, 2, 3, 2, 3, 4, 5, 6}
    */
    std::copy_backward(array + 2, array + 7, array + arraySize);

    showContain(
        array, array + arraySize, 
        "After copy_backward(array + 2, array + 7, array + arraySize), array = "
    );

    std::iota(array, array + arraySize, 0);

    showContain(array, array + arraySize, "Original array:");

    /**
     * @brief std::copy_backward() 的基本用法（重叠的情况）
     *                  
     *                          result
     *                first            last
     *        {<0, 1, [2, 3, 4, 5>, 6, 7), 8}
     *                 <-------------
     *          <----------------
     * 
     *      结果为：
     * 
     *        {2, 3, 4, 5, 6, 5, 6, 7, 8}
     * 
     *      和 copy 一样，再对物理上连续连续的内存进行 copy_backward 操作，
     *      且输出的起点再拷贝区间内时，会被检查到从而调用 memmove() 避免错误的拷贝。
    */
    std::copy_backward(array + 2, array + 7, array + 5);

    showContain(array, array + arraySize, 
                "After copy_backward(array + 2, array + 7, array + 5), array = "
    );

    std::deque<int> numberDeque(15);
    std::iota(numberDeque.begin(), numberDeque.end(), 1);

    NOTIFY_LOG("Original numberDeque:\n");
    showContainerToStream(std::cout, numberDeque, numberDeque.size());
    printSplitLine(40, '-');

    std::deque<int>::iterator copyStart  = numberDeque.begin();
    std::deque<int>::iterator copyEnd    = numberDeque.begin();
    std::deque<int>::iterator copyResult = numberDeque.begin();

    std::advance(copyStart, 3);
    std::advance(copyEnd, 12);
    std::advance(copyResult, 10);

    /**
     * @brief 和 copy() 算法一样，如果传入迭代器，哪怕是随机访问迭代器，
     *        出现 “输出的起点再拷贝区间内” 这样的传参时，就不会去匹配调用
     *        memmove() 的版本，而是忠诚的完成拷贝操作，这样产生的结果是不符合预期的。
     *                          
     *                                result
     *                 first                 last
     *        {1 <2 3 [4 5 6 7 8 9 10 11> 12 13) 14 15}
     * 
     *        结果是：
     *        
     *        {1 4 5 6 7 8 9 10 11 12 11 12 13 14 15}
    */
    std::copy_backward(copyStart, copyEnd, copyResult);

    NOTIFY_LOG("After copy_backward(copyStart, copyEnd, copyResult), numberDeque = \n");
    showContainerToStream(std::cout, numberDeque, numberDeque.size());
    printSplitLine(40, '-');

    return EXIT_SUCCESS;
}
