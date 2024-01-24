/*
    1.9.2 临时对象的产生和应用：
        本例测试 仿函数 在 std::for_each 算法中的应用。
*/

#include <iostream>
#include <vector>
#include <algorithm>

template <typename Type>
class Print
{
    public:
        /**
         * @brief 重载了 ( ) 符号，可以使用 object(value) 的语法输出 value 的值。
         * 
         * @param __elememnt    要传入的值
         * 
         * @return non-return
        */
        void operator()(const Type & __element) { std::cout << __element << ' '; }
};

int main(int argc, char const *argv[])
{
    std::vector<int> arrayList = {11, 21, 13, 354, 135, 3565};

    /*
        在 std::for_each 中使用 Print<int>() 的语法创建了一个匿名临时对象。
        这种语法也叫仿函数，且该对象在 for_each 函数运行结束后就销毁。
    */
    std::for_each(arrayList.begin(), arrayList.end(), Print<int>());
    
    return EXIT_SUCCESS;
}
