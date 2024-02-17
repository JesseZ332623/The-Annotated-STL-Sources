#include "./include/My_Forward_List.h"
#include "./My_Forward_List.cpp"

int main(int argc, char const *argv[])
{
#if true    /*测试 从 std::vector<Type> 进行拷贝的拷贝构建函数*/
    puts("Test MyForwardList<Type>::MyForwardList(std::vector<Type> & __vector)");

    std::vector<int> vecArray = {1, 6, 356, 3562345, 465};

    MyForwardList<int> beCopyIntengerList(vecArray);

    std::cout << beCopyIntengerList << '\n';
#endif

#if true /* 测试  void MyForwardList<Type>::sort(void)*/
    MyForwardList<int> beSortedList = {65, 325345, 14};
    beSortedList.sort();

    std::cout << beSortedList;

#endif

    return 0;
}
