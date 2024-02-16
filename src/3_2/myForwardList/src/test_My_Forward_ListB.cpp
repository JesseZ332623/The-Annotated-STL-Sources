#include "./include/My_Forward_List.h"
#include "./My_Forward_List.cpp"

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
    MyForwardList<std::vector<int>> newList = {{1, 2, 3}, {3, 2, 1}, {3431, 25, 2456, 2}};
    MyForwardList<std::vector<std::string>> stringVecList = {{"245134513"}, {"9dfagae"}, {"gfvbadhadb"}};
    MyForwardList<std::string> stringList = {{{"111"}, {"hdtjwsrt"}, {"dtfgaEF"}}};

    showVectorListContent(stringVecList);
    showVectorListContent(newList);

    std::cout << stringList << '\n';
    
    return EXIT_SUCCESS;
}
