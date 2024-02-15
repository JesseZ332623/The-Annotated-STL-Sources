#include "./include/My_Forward_List.h"
#include "./My_Forward_List.cpp"

#include <cstdlib>
#include <ctime>

template <typename InputIterator, typename Type>
InputIterator find(InputIterator first, InputIterator last, const Type &value)
{
    /*
        首先确保 first 和 last 是两个不同的迭代器，
        并且范围开始迭代器所指向的元素不是要搜索的值。
    */
    while (first != last && first->getValue() != value) { ++first; }

    return first;
}

template <typename Type>
void showVectorListContent(const MyForwardList<std::vector<Type>> & __vecList)
{
    std::cout << "List Node Count = " << __vecList.size() << '\n';

    for (auto vecItem : __vecList)
    {
        std::cout << vecItem << '\n';
    }
}

int main(int argc, char const *argv[])
{
#if true
    std::srand((unsigned int)std::time(nullptr));
    MyForwardList<std::vector<int>> vectorList;
    std::vector<int> testVector;

    /*测试初始化列表构建和循环头插尾插*/
    MyForwardList<int> newListA{};
    MyForwardList<int> newListB = {1, 2, 3, 4, 5, 34534, 2456, 2345, 245, 135, 6};

    puts("Test MyForwardList<int>::insertFront() and insertEnd() ");
    for (int insertTime = 0; insertTime < 5; ++insertTime)
    {
        newListA.insertFront(std::rand() % 100 + 1);
        newListA.insertEnd(std::rand() % 100 + 1);
    }
    std::cout << newListA << '\n';
    std::cout << newListB << '\n';

    puts("Test MyForwardList<std::vector<int>>::insert() ");
    for (int insertTime = 0; insertTime < 5; ++insertTime)
    {
        testVector.clear();
        for (int index = 0; index < 3; ++index) { testVector.insert(testVector.begin() + index, std::rand() % 100 + 1); }
        vectorList.insert(testVector, vectorList.begin() + insertTime);     // 随机插入
    }
    showVectorListContent(vectorList);

    puts("Test MyForwardList<std::vector<int>>::erase() ");
    vectorList.erase(vectorList.begin() + 2);                               // 随机删除
    showVectorListContent(vectorList);

    puts("Test MyForwardList<std::vector<int>>::insertEnd() ");
    std::reverse(testVector.begin(), testVector.end());
    vectorList.insertEnd(testVector);                                       // 末尾插入
    showVectorListContent(vectorList);

    puts("Test MyForwardList<std::vector<int>>::insertFront() ");           // 头部插入
    vectorList.insertFront(testVector);
    showVectorListContent(vectorList);

    puts("Test MyForwardList<std::vector<int>>::MyForwardList(const MyForwardList & __forwardList) ");
    MyForwardList<std::vector<int>> vectorList_B(vectorList);               // 拷贝构造函数
    showVectorListContent(vectorList_B);

    puts("Test MyForwardList<std::vector<int>>::MyForwardList(MyForwardList && __forwardList) ");
    MyForwardList<std::vector<int>> vectorList_C(std::move(vectorList_B));  // 移动构造函数，移动后 vectorList_B 失效
    showVectorListContent(vectorList_C);

    puts("Test MyForwardList<std::vector<int>>::operator=(const MyForwardList & __forwardList)");
    vectorList_B = vectorList;                                               // 拷贝构造运算符
    showVectorListContent(vectorList_B);

    puts("Test MyForwardList<std::vector<int>>::operator=(MyForwardList && __forwardList)");
    vectorList_C = std::move(vectorList);                                    // 移动构造运算符，移动后 vectorList 失效
    showVectorListContent(vectorList_C);                                     
#endif

    return EXIT_SUCCESS;
}