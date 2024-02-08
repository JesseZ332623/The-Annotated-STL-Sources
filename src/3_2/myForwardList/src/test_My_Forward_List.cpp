#include "./include/My_Forward_List.h"

template <typename InputIterator, typename Type>
InputIterator find(InputIterator first, InputIterator last, const Type & value)
{
    /*
        首先确保 first 和 last 是两个不同的迭代器，
        并且范围开始迭代器所指向的元素不是要搜索的值。
    */
    while (first != last && first->getValue() != value) 
    { 
        /*迭代器自增*/
        ++first; 
    }

    return first;
}

int main(int argc, char const *argv[])
{
    MyForwardList<int> newList;

#if true
    for (int index = 0; index < 5; ++index)
    { 
        newList.insertFront(index); 
        newList.insertEnd(index + 2);
    }
#endif

    std::cout << newList << '\n';

    newList.deleteEnd();

    std::cout << newList << '\n';

    auto iter = find(newList.begin(), newList.end(), 0);

    std::cout << iter->getValue();

    return EXIT_SUCCESS;
}
