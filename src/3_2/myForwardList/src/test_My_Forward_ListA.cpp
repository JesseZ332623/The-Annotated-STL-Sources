#include "./include/My_Forward_List.h"
#include "./My_Forward_List.cpp"

#include <cstdlib>
#include <ctime>

#include <forward_list>

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
    system("cls");

    std::srand((unsigned int)std::time(nullptr));

#if true    /*测试 初始化列表构建 和 循环头插尾插*/
    MyForwardList<int> newListA{};
    MyForwardList<int> newListB = {1, 2, 3, 4, 5, 34534, 2456, 2345, 245, 135, 6};

    puts("Test MyForwardList<int>::insertFront() and insertEnd() ");
    for (int insertTime = 0; insertTime < 5; ++insertTime)
    {
        newListA.insertFront(std::rand() % 100 + 1);
        newListA.insertEnd(std::rand() % 114514 + 1);
    }
    std::cout << newListA << '\n';
    std::cout << newListB << '\n';
#endif

#if true    /*测试 随机插入*/
    MyForwardList<std::vector<int>> vectorList;
    std::vector<int> testVector;

    puts("Test MyForwardList<std::vector<int>>::insert() ");
    for (int insertTime = 0; insertTime < 5; ++insertTime)
    {
        testVector.clear();
        for (int index = 0; index < 3; ++index) { testVector.insert(testVector.begin() + index, std::rand() % 100 + 1); }
        vectorList.insert(testVector, vectorList.begin() + insertTime);     
    }
    showVectorListContent(vectorList);
#endif 

#if true   /*测试 随机删除*/
    puts("Test MyForwardList<std::vector<int>>::erase() ");
    vectorList.erase(vectorList.begin() + 2);                               
    showVectorListContent(vectorList);
#endif

#if true   /*测试 末尾插入*/
    puts("Test MyForwardList<std::vector<int>>::insertEnd() ");
    std::reverse(testVector.begin(), testVector.end());
    vectorList.insertEnd(testVector);                                       
    showVectorListContent(vectorList);
#endif

#if true   /*测试 头部插入*/
    puts("Test MyForwardList<std::vector<int>>::insertFront() ");         
    vectorList.insertFront(testVector);
    showVectorListContent(vectorList);
#endif

#if true /*测试 拷贝构造函数*/
    puts("Test MyForwardList<std::vector<int>>::MyForwardList(const MyForwardList & __forwardList) ");
    MyForwardList<std::vector<int>> vectorList_B(vectorList);             
    showVectorListContent(vectorList_B);
#endif

#if true /*测试 移动构造函数*/
    puts("Test MyForwardList<std::vector<int>>::MyForwardList(MyForwardList && __forwardList) ");
    MyForwardList<std::vector<int>> vectorList_C(std::move(vectorList_B));  // 移动后 vectorList_B 失效
    showVectorListContent(vectorList_C);
#endif

#if true /*测试 拷贝构造运算符*/
    puts("Test MyForwardList<std::vector<int>>::operator=(const MyForwardList & __forwardList)");
    vectorList_B = vectorList;                                               
    showVectorListContent(vectorList_B);
#endif

#if true /*测试 移动构造运算符*/
    puts("Test MyForwardList<std::vector<int>>::operator=(MyForwardList && __forwardList)");
    vectorList_C = std::move(vectorList);                                    // 移动后 vectorList 失效
    showVectorListContent(vectorList_C);  
#endif      

#if true /*测试 swap*/
    MyForwardList<std::vector<int>> beSwapList = {{1, 2, 3}, {5, 6, 7}, {8, 8, 8}};
    MyForwardList<std::vector<int>> beSwapListB = {{2, 2, 2}};

    showVectorListContent(beSwapList);
    showVectorListContent(beSwapListB);

    beSwapListB.swap(beSwapList);                                       

    showVectorListContent(beSwapList);
    showVectorListContent(beSwapListB);
#endif

    return EXIT_SUCCESS;
}