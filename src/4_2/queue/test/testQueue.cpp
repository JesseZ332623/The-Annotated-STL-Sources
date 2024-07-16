#include "../include/queue.h"

#include <MyLib/myLogerDef.h>
#include <MyLib/myDelay.h>
#include <random>

struct front_and_back {};
struct all_info {};

template <typename Type>
void showQueueInfo(const queue<Type> & __queue, front_and_back);

template <typename Type>
void showQueueInfo(queue<Type> & __queue, all_info);

int main(int argc, char const *argv[])
{
    std::system("cls");

    queue<int>          queue_1;
    std::random_device  randomDevice;
    std::mt19937        randEngine(randomDevice());
    std::uniform_int_distribution<int> distribute(1, 15);

    for (int index = 0; index <= 5; ++index)
    {
        queue_1.push(distribute(randEngine));
        showQueueInfo(queue_1, front_and_back());
    }

    showQueueInfo(queue_1, all_info());
    
    return EXIT_SUCCESS;
}

template <typename Type>
void showQueueInfo(const queue<Type> & __queue, front_and_back)
{
    using namespace MyLib::MyLoger;
    using namespace MyLib::MyDelay;

    NOTIFY_LOG(
                "Queue size() = [" + std::to_string(__queue.size()) +
                "]\nQueue front() = " + 
                std::to_string(__queue.front()) + ' '
            );
    NOTIFY_LOG(
                "Queue back() = " + 
                std::to_string(__queue.back()) + '\n'
        );
    
    printSplitLine(40, '-');

    delay(75);
}

template <typename Type>
void showQueueInfo(queue<Type> & __queue, all_info)
{
    using namespace MyLib::MyLoger;

    std::size_t queueSize = __queue.size();

    NOTIFY_LOG(
                "Queue size() = [" + 
                std::to_string(__queue.size()) + "]\n"
            );

    NOTIFY_LOG("Queue back() = { ");

    for (std::size_t index = 0; index < queueSize; ++index)
    {
        CORRECT_LOG(std::to_string(__queue.back()) + ' ');
        __queue.pop();
    }

    NOTIFY_LOG("}\n");
}
