#include "../include/deque.h"

#include <algorithm>
#include <MyLib/myLogerDef.h>

//#include <deque>

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    system("cls");

    My_Deque<int> deque_1;


    for (int index = 0; index < 10; ++index)
    {
        deque_1.push_back(67);
        deque_1.push_front(817);
    }

#if true
    NOTIFY_LOG("size of deque_1 = [" + std::to_string(deque_1.size()) + "]\n");

    for (const int n : deque_1)
    {
        std::cout << n << " ";
    }
#endif

    DONE

    return EXIT_SUCCESS;
}
