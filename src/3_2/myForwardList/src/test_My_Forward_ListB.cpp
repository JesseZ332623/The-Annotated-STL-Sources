#include "./include/My_Forward_List.h"
#include "./My_Forward_List.cpp"

#define IF_FIND(originalContent, returnIter) (returnIter != originalContent.end()) \
        ? std::cout << "Find! Value = " << returnIter->getValue() << " Address = " << returnIter.get() << '\n' \
        : std::cout << "Not Found!\n"; 

#define IF_VECTOR_FIND(originalContent, returnIter) returnIter != originalContent.end() \
        ? std::cout << "Find Value = " << *returnIter << "Address = " << returnIter.get() << '\n' \
        : std::cout << "Not Found!\n"; 

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
#if true    /*测试 嵌套一些 STL 类型的兼容性*/

    /*单向链表内每一个节点都是由 int 类型构成的动态数组*/
    MyForwardList<std::vector<int>> newList = {{1, 2, 3}, {3, 2, 1}, {3431, 25, 2456, 2}};

    /*单向链表内每一个节点都是由 std::string 类型构成的动态数组*/
    MyForwardList<std::vector<std::string>> stringVecList = {{"245134513"}, {"9dfagae"}, {"gfvbadhadb"}};

    /*单向链表内每一个节点都是由 std::string 类型*/
    MyForwardList<std::string> stringList = {{{"111"}, {"hdtjwsrt"}, {"dtfgaEF"}}};

    showVectorListContent(stringVecList);
    showVectorListContent(newList);

    std::cout << stringList << '\n';
#endif

#if true /*测试 operator[] 的使用*/

    puts("Test MyForwardList<Type>::ItemReference MyForwardList<Type>::operator[](long int __index)");
    stringList[0].getValue() = "114514";                // 修改 stringList 中第一个节点的值

    
    std::cout << stringList[2].getValue() << '\n';      // 获取 stringList 第 3 个节点的节点值

    std::cout << newList[0] << '\n';                    // 获取 newList 第 1 个 节点的节点值

    MyForwardList<double> scoreList(5);

    for (int index = 0; index < scoreList.size(); ++index)
    {
        scoreList[index].getValue() = (80 + std::rand() % 21 / 100.0);
    }

    std::cout << scoreList << '\n';


#if false   /*测试越界访问*/
    newList[4];
    newList[-1];
#endif

#if true        /*测试 find(const Type & __value) 的使用*/

    puts("Test MyForwardList<Type>::ListIter MyForwardList<Type>::find(const Type & __value)");
    MyForwardList<int> integerList = {8, 6, 98, 2345, 114514, 1919810};
    auto itA = integerList.find(2345);
    auto itB = integerList.find(0);
    IF_FIND(integerList, itA);
    IF_FIND(integerList, itB);

    auto itC = stringList.find("dtfgaEF");
    IF_FIND(stringList, itC);

    auto itD = newList.find({1, 2, 3});
    IF_VECTOR_FIND(newList, itD);

    auto itE = newList.find({1, 2, 4});
    IF_VECTOR_FIND(newList, itE);

    puts("Test MyForwardList<Type>::ListIter MyForwardList<Type>::find_if(FindRule __findRule)");
    auto itF = integerList.find_if([](const int __target) -> bool { return (__target > 114514); });
    IF_FIND(integerList, itF);

#endif

#endif 
    
    return EXIT_SUCCESS;
}
