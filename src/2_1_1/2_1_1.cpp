#include "./include/2_1_1.h"

#include <vector>
#include <list>
//#include <algorithm>

#if true
template <typename InputIterator, typename Function> _GLIBCXX20_CONSTEXPR
Function for_each(InputIterator first, InputIterator last, Function f)
{
    // concept requirements
    __glibcxx_function_requires(_InputIteratorConcept<_InputIterator>)
    __glibcxx_requires_valid_range(first, last);

    /* 使用迭代器遍历整个容器，对容器内的每一个元素都执行 f 操作。*/
    for (; first != last; ++first) { f(*first); }

    /*返回操作函数，这样可以支持链式调用*/
    return f;
}
#endif

int main(int argc, char const *argv[])
{
    /*
        vector在元素数量超过当前存储容量时，
        会调用成员函数 Allocator::allocate()，函数内部会调用 _allocate() 函数。

        std::vector 会取得 Allocator::allocate() 分配的内存的地址，
        然后调用 Allocator::construct()，去构建对象。

        当元素被清除（如调用）std::vector::clear() 或者对象作用域到期，
        会调用 Allocator::deallocate() 和 Allocator::destruct() 函数去析构和释放这片内存
    */
    std::vector<int, myStl::Allocator<int>> arrayList_A = {1, 2, 3};
    std::vector<int, myStl::Allocator<int>> arrayList_B = {1, 2, 3, 8, 9, 322};

    for_each(arrayList_A.begin(), arrayList_A.end(), [](const int __n){ std::printf("%d ", __n); }); puts("");

    arrayList_A.push_back(10);

    for_each(arrayList_A.begin(), arrayList_A.end(), [](const int __n){ std::printf("%d ", __n); }); puts("");

    std::printf("arrayList_A elements number = %zd, capacity = %zd, container size = %zd", 
                arrayList_A.size() - 1, arrayList_A.capacity(), sizeof(arrayList_A), arrayList_A.max_size());

    return EXIT_SUCCESS;
}
