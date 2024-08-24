#include "./showContainer.h"

#include <algorithm>
#include <stdexcept>
#include <type_traits>

#include <vector>
#include <list>
#include <deque>

/**
 * @brief TrivalData 虽然有用户自定的构造函数，
 *        但是按照平凡类型去操作没有任何问题，
 *        只是不设置的话，编译器是检测不出来的。
*/
class TrivalData
{
    private:
        int data;
    
    public:
        TrivalData() = default;
        TrivalData(int n) : data(n) {}

};

// 定义一个自定义的类型特征模板
template<typename T>
struct __type_traits
{
    static constexpr bool _is_trivial = false;
};

// 特化 __type_traits 以标记 TrivalData 为平凡类型
template<>
struct __type_traits<TrivalData>
{
    static constexpr bool _is_trivial = true;
};

int main(int argc, char const *argv[])
{
    const char charArray[5]     = {'a', 'b', 'c', 'd', 'e'};
    char       charArrayCopy[5] = {0};

    /**
     * @brief 调用的版本是：
     * 
     *          std::copy(const char *, const char *, char *);
    */
    std::copy(charArray, charArray + 5, charArrayCopy);
    showContain(charArray, charArray + 5, "charArray[5]:");
    showContain(charArrayCopy, charArrayCopy + 5, "charArrayCopy[5]:");

    const wchar_t wcharArray[5]     = {'a', 'b', 'c', 'd', 'e'};
    wchar_t       wcharArrayCopy[5] = {0};
    
    /**
     * @brief 调用的版本是：
     * 
     *          std::copy(const wchar_t *, const wchar_t *, wchar_t *);
    */
    std::copy(wcharArray, wcharArray + 5, wcharArrayCopy);
    showContain(wcharArray, wcharArray + 5, "wcharArray[5]:");
    showContain(wcharArrayCopy, wcharArrayCopy + 5, "wcharArrayCopy[5]:");

    int numbers[5] = {1, 2, 3, 4, 5};

    /**
     * @brief 函数的调用链如下：
     *          copy(int *, int *, int *);
     *              __copy_dispatch(T *, T *);
     *                  __copy_t(__true_type);
     * 
     * numbers 是一个平凡类型，所以 __copy_t() 的参数是 __true_type。
     * 
     * 值得注意的是，这里的 copy() 是自赋值，虽然浪费但是合法。
    */
    std::copy(numbers, numbers + 5, numbers);
    showContain(numbers, numbers + 5, "numbers[5]:");

    std::list<short> numberList = {0, 1, 2, 3, 4, 5, 6};
    std::list<short> numberListCopy(numberList.size());

    NOTIFY_LOG("std::list<short> numberList:\n");
    showContainerToStream(std::cout, numberList, numberList.size());
    printSplitLine(30, '-');

    /**
     * @brief 函数的调用链如下：
     *                  copy();
     *                      __copy_dispatch();
     *                          __copy(__input_iterator);
     * 
     * std::list 作为 STL 容器，拷贝操作离不开迭代器。
    */
    std::copy(numberList.begin(), numberList.end(), numberListCopy.begin());

    NOTIFY_LOG("std::list<short> numberListCopy:\n");
    showContainerToStream(std::cout, numberListCopy, numberListCopy.size());
    printSplitLine(30, '-');

    std::vector<short> numberVector = {5, 6, 7, 8, 9, 10};
    std::vector<short> numberVectorCopy(numberVector.size());

    NOTIFY_LOG("std::vector<short> numberVector:\n");
    showContainerToStream(std::cout, numberVector, numberVector.size());
    printSplitLine(30, '-');

    /**
     * @brief 函数的调用链如下：
     *          copy();
     *              __copy_dispatch(T *, T *);
     *                  __copy_t(__true_type);
     * 
     * 这可能有些出乎意料，但是搓过 vector 后就知道，
     * vector 的迭代器实际上就是这个类型的裸指针，所以这样的重载匹配是正确的。
    */
    std::copy(numberVector.begin(), numberVector.end(), numberVectorCopy.begin());

    NOTIFY_LOG("std::vector<short> numberVectorCopy:\n");
    showContainerToStream(std::cout, numberVectorCopy, numberVectorCopy.size());
    printSplitLine(30, '-');


    std::deque<TrivalData> numberDeque = {7, 6, 5, 4, 3, 2, 1};
    std::deque<TrivalData> numberDequeCopy(numberDeque.size());

    /**
     * @brief 函数的调用链如下：
     *                  copy();
     *                      __copy_dispatch();
     *                         __copy_t(random_access_iterator); 
     *                              __copy_d();
     * 
     * std::deque 内部的迭代器是 random_access_iterator，这样的重载匹配没问题。
    */
    std::copy(numberDeque.begin(), numberDeque.end(), numberDequeCopy.begin());

    std::vector<TrivalData> dataVector = {8, 9, 7, 5, 4};
    std::vector<TrivalData> dataVectorCopy(dataVector.size());

    /**
     * @brief 函数的调用链如下：
     *          copy();
     *              __copy_dispatch(T *, T *);
     *                  __copy_t(__false_type);
     * 
     * 这里需要注意的是：虽然 TrivalData 是一个平凡类型，但是没有经过设置，
     * 编译器仍然会认为它是一个不平凡类型。
    */
    std::copy(dataVector.begin(), dataVector.end(), dataVectorCopy.begin());

    return EXIT_SUCCESS;
}
