#include "../include/deque.h"

#include <algorithm>
#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>

//#include <deque>

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::SimpleContainerOperator;

    system("cls");

    My_Deque<int> deque_1;
    std::size_t deque_1_bufferSize = My_Deque<int>::iterator::getBufferSize();

    WARNING_LOG(
                    "My_Deque<int> Buffer size = " + 
                    std::to_string(deque_1_bufferSize) + " elements each.\n\n"
                );

    NOTIFY_LOG("Call push_back() member method 128 times.\n");
    for (int index = 0; index < 128; ++index)
    {
        deque_1.push_back(index);
    }

    showContainerToStream(std::cout, deque_1, deque_1.size() / 2);

    NOTIFY_LOG("Call pop_back() and pop_front() member method 12 times.\n");
    for (int index = 0; index < 12; ++index)
    {
        deque_1.pop_back();
        deque_1.pop_front();
    }

    showContainerToStream(std::cout, deque_1, deque_1.size() / 2);

    DONE

    return EXIT_SUCCESS;
}
