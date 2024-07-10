#include "../include/deque.h"

#include <algorithm>
#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>
#include <MyLib/myDelay.h>
#include <deque>

#ifndef DEQUE_BUFFER_SIZE
#define DEQUE_BUFFER_SIZE 0
#endif

using namespace MyLib::MyLoger;

#define SHOW_DEQUE(deque)     showContainerToStream(std::cout, deque, deque.size());
#define GET_DEQUE_SIZE(dequeName) CORRECT_LOG(std::string(dequeName) + " size = " + std::to_string(deque_1.size()) + '\n');

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;

    system("cls");

    
    My_Deque<int, DEQUE_BUFFER_SIZE> deque_1 = {0x1F, 0x2D, 0x3C};
    std::size_t deque_1_bufferSize = My_Deque<int, DEQUE_BUFFER_SIZE>::iterator::getBufferSize();

    WARNING_LOG(
                    "My_Deque<int> Buffer size = " + 
                    std::to_string(deque_1_bufferSize) + " elements each.\n\n"
                );

    SHOW_DEQUE(deque_1);

#if true

    NOTIFY_LOG("Call push_back() member method 12 times.\n");
    for (int index = 0; index < 12; ++index)
    {
        deque_1.push_back(index);
    }

    SHOW_DEQUE(deque_1);

    NOTIFY_LOG("Call erase(deque_1.begin() + 2, deque_1.end() - 3) member method.\n");
    deque_1.erase(deque_1.begin() + 2, deque_1.end() - 3);

    SHOW_DEQUE(deque_1);

    NOTIFY_LOG("Call insert(deque_1.end() - 4, 114514) member method.\n");
    deque_1.insert(deque_1.end() - 4, 114514);

    SHOW_DEQUE(deque_1);

    GET_DEQUE_SIZE("deque_1");
#endif

    DONE
    return EXIT_SUCCESS;
}
