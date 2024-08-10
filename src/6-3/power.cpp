#include <MyLib/myLogerDef.h>
#include <MyLib/simpleContainerOperator.h>

#include <numeric>      // 一些和数值有关的 STL 算法所在
#include <vector>
#include <functional>

/**
 * @brief std::power() 一个 SGI 专属的数值类型算法，
 *        不在 STL 标准之列，它计算某个数字的 n 幂次方，
 *        注意：此处 n 幂次方不一定是乘幂，也可以由用户指定。
*/

/**
 * @brief “证同元素” 之：任何数 + 0 都等于它自己
*/
template <typename Type>
inline Type identity_element(std::plus<Type>) { return Type(0); }

/**
 * @brief “证同元素” 之：任何数 * 1 都等于它自己
*/
template <typename Type>
inline Type identity_element(std::multiplies<Type>) { return Type(1); }

/**
 * @brief 计算某个数字的 n 幂次方（幂运算规则由用户指定）
 * 
 * @tparam Type                 某个数字的类型
 * @tparam Intenger             表示 n 次方的哪个数字类型（一般是 int 类型）
 * @tparam MonoidOperator       进行幂运算操作的类型，可以是函数指针，函数符，Lamba 表达式
 * @param x                     底数
 * @param n                     指数
 * @param operation             幂运算操作
 * 
 * @return Type                 运算结果
*/
template <typename Type, typename Intenger, typename MonoidOperator>
Type power(Type x, Intenger n, MonoidOperator operation)
{
    // 处理指数为 0 的情况
    if (!n) { return identity_element(operation); }
    else
    {
        // 如果指数 n 为偶数，如 2，4，6，8 ....
        while (!(n & 1))
        {
            n >>= 1;                // 不断除以 2，直到 n 被除到 1 为止
            x = operation(x, x);    // 进行一次幂运算
        }

        /**
         * 若 x 为 1 的情况下，就不进入循环，直接返回即可。
         * 其余的基数底数要进入下一个循环运算。
        */
        Type result = x;            
        n >>= 1;
        while (n != 0)
        {
            x = operation(x, x);
            if ((n & 1) != 0)
            {
                result = operation(result, x);
            }
            n >>= 1;
        }
        return result;
    }
}

/**
 * @brief 计算某个数字的 n 幂次方
 *       （幂运算规则为默认的 `std::multiplies<Type>`）
*/
template <typename Type, typename Intenger>
inline Type power(Type x, Intenger n)
{
    return power(x, n, std::multiplies<Type>());
}

int main(int argc, char const *argv[])
{
    using namespace MyLib::MyLoger;

    float result = power(2.5, 8);

    NOTIFY_LOG(result);

    return EXIT_SUCCESS;
}
