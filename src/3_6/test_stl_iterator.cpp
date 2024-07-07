#include <MyLib/myLogerDef.h>
#include "./stl_iterator.h"

#include <vector>
#include <list>
#include <unordered_map>

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    std::vector<int> vecArray = {7, 54, 3, 34, 76, 114514};
    std::list<int>   numList  = {8, 65, 2345, 4, 554, 114514, 7};
    std::unordered_map<std::string, int> dataMap = {
                                            {"Mike", 177}, {"Lucy", 1600}, 
                                            {"A Secrit Man", 114514}, {"Tom", 1811}
                                        };
    
    std::vector<int>::iterator vecIter  = vecArray.begin();
    std::list<int>::iterator   listIter = numList.begin();
    std::unordered_map<std::string, int>::iterator dataMapIter = dataMap.begin();

    std::vector<int>::difference_type vecSize  = distance(vecArray.begin(), vecArray.end());
    std::list<int>::difference_type   listSize = distance(numList.begin(), numList.end());
    std::unordered_map<std::string, int>::difference_type mapSize = distance(dataMap.begin(), dataMap.end());

    my_advance(vecIter, vecSize - 1);
    my_advance(listIter, listSize - 2);
    my_advance(dataMapIter, 1);

    system("cls");

    for (auto mapPair : dataMap) 
    { 
        CORRECT_LOG('{' + mapPair.first + ' ' + std::to_string(mapPair.second) + "}\n"); 
    }

    loger(
        std::cout, NOTIFY, 
        "vector size = ", vecSize, '\n',
        "list size = ", listSize, '\n',
        "map size = ", mapSize, '\n'
    );

    loger(
        std::cout, NOTIFY, 
        "(*vecIter) = ", *vecIter, '\n',
        "(*listIter) = ", *listIter, '\n',
        "(*dataMapIter) = {", (*dataMapIter).first, ' ', (*dataMapIter).second, "}\n"
    );

    return EXIT_SUCCESS;
}
