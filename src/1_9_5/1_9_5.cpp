#include "./include/1_9_5.h"

template <typename InputIterator, typename Type>
InputIterator find(InputIterator first, InputIterator last, const Type & value)
{
    /*
        首先确保 first 和 last 是两个不同的迭代器，
        并且范围开始迭代器所指向的元素不是要搜索的值。
    */
    while (first != last && *first != value) 
    { 
        /*迭代器自增*/
        ++first; 
    }

    return first;
}

template <typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function f)
{
    /*
        使用迭代器遍历整个容器，对容器内的每一个元素都执行 f 操作。
    */
    for (; first != last; ++first) { f(*first); }

    /*返回操作函数，这样可以支持链式调用*/
    return f;
}

/*测试用例*/
int main(int argc, char const *argv[])
{
    std::vector<int> dataList = {18, 245, 254, 134, 2, 3};

    auto showContence = [](const int elememnt) { std::cout << elememnt << ' '; };

    for_each(dataList.begin(), dataList.end(), showContence);

    std::cout << find(dataList.begin(), dataList.end(), 2) - dataList.begin();

    return EXIT_SUCCESS;
}