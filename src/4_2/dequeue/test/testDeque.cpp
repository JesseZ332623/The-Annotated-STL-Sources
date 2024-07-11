#include "../include/deque.h"

#include <algorithm>
#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>
#include <MyLib/myDelay.h>
#include <random>
#include <deque>

#ifndef DEQUE_BUFFER_SIZE
#define DEQUE_BUFFER_SIZE 0
#endif

using namespace MyLib::MyLoger;

#define SHOW_DEQUE(deque)     showContainerToStream(std::cout, deque, deque.size());
#define GET_DEQUE_SIZE(deque, dequeName) CORRECT_LOG(std::string(dequeName) + " size = " + std::to_string(deque.size()) + '\n');

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;

    system("cls");

    std::random_device defaultRandDevice;
    std::mt19937_64 randEngine(defaultRandDevice());
    std::uniform_int_distribution dist(0, 114514);

    My_Deque<int, DEQUE_BUFFER_SIZE> deque_1 = {1, 2, 3, 0x7FFFFFFF};
    std::size_t deque_1_bufferSize = My_Deque<int, DEQUE_BUFFER_SIZE>::iterator::getBufferSize();

    WARNING_LOG(
                    "My_Deque<int> Buffer size = " + 
                    std::to_string(deque_1_bufferSize) + " elements each.\n\n"
                );

    SHOW_DEQUE(deque_1);

#if false
    for (int index = 0; index < 100000000; ++index) 
    { 
        deque_1.push_front(dist(randEngine));
        deque_1.push_back(dist(randEngine)); 
        //deque_1.insert(deque_1.begin() + 5, dist(randEngine));
        deque_1.push_front(dist(randEngine));
        deque_1.push_back(dist(randEngine)); 
        //deque_1.erase(deque_1.begin(), deque_1.begin() + 2);
       
        if (index % 1000000 == 0 && index >= 1000000) 
        {
            GET_DEQUE_SIZE(deque_1, "deque_1");
        }
    }
#endif

#if true
    NOTIFY_LOG("Call clear()\n");
    deque_1.clear();
    GET_DEQUE_SIZE(deque_1, "deque_1");
    NOTIFY_LOG("Call push_back(index) and push_front(index) member method 12 times.\n");
    for (int index = 0; index < 12; ++index)
    {
        deque_1.push_front(index);
        deque_1.push_back(index);
    }

    SHOW_DEQUE(deque_1);

    NOTIFY_LOG("Call erase(deque_1.begin() + 2, deque_1.end() - 3) member method.\n");
    deque_1.erase(deque_1.begin() + 2, deque_1.end() - 3);

    SHOW_DEQUE(deque_1);

    NOTIFY_LOG("Call insert(deque_1.end() - 4, 114514) member method.\n");
    deque_1.insert(deque_1.end() - 4, 114514);

    SHOW_DEQUE(deque_1);

    NOTIFY_LOG(
                "deque_1.at(1) = " + std::to_string(deque_1.at(1)) +
                "\ndeque_1[5] = " + std::to_string(deque_1[5]) + "\n\n"
            );

    My_Deque<int, DEQUE_BUFFER_SIZE> deque_2; deque_2 = deque_1;
    SHOW_DEQUE(deque_2);

    My_Deque<int, DEQUE_BUFFER_SIZE> deque_3; deque_3 = std::move(deque_2);
    SHOW_DEQUE(deque_2);
    SHOW_DEQUE(deque_3);
#endif

    DONE
    return EXIT_SUCCESS;
}
