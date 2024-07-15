#include "../include/array.h"

#include <MyLib/myLib.h>

/**
 * @brief 创建一个  non-trival type
 */
class testContainer
{
    private:    
        int * data;
        std::size_t size;

    public:

        testContainer(void) : data(nullptr) { }

        testContainer(std::size_t __n, const int __val) : testContainer()
        {
            this->data = new int[__n];
            std::fill_n(this->data, __n, __val);
            this->size = __n;
        }

        testContainer(const testContainer & __contain) : testContainer()
        {
            this->data = new int[__contain.size];
            std::copy(__contain.data, __contain.data + __contain.size, this->data);

            this->size = __contain.size; 
        }

        testContainer(testContainer && __contain) : testContainer()
        {
            this->data = __contain.data;
            this->size = __contain.size;

            __contain.data = nullptr;
            __contain.size = 0ULL;

        }

        void show(void) const noexcept
        {
            using namespace MyLib::SimpleContainerOperator;

            std::for_each(
                            this->data, this->data + this->size, 
                            [](const int n) { std::cout << n << ' '; }
                        ); puts("");
        }

        ~testContainer() { delete[] this->data; }
};

//testContainer::callTime = 0;

int main(int argc, char const *argv[])
{
    using namespace MyLib::SimpleContainerOperator;

    testContainer cStyle[5] = 
    {
        testContainer(5, 1), testContainer(5, 2), testContainer(5, 3),
        testContainer(5, 4), testContainer(5,5)
    };

    array<testContainer, 5> nontrivalArray = toArray(cStyle);
    array<testContainer, 1> nontrivalArray_2 = toArray({testContainer(5, 10)});

    for (int index = 0; index < nontrivalArray.size(); ++index)
    {
        nontrivalArray[index].show(); 
    }

    nontrivalArray_2[0].show();

    return EXIT_SUCCESS;
}
