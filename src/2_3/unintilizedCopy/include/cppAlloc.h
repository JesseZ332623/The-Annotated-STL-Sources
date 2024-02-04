#ifndef _CPP_ALLOC_H_
#define _CPP_ALLOC_H_

#include <new>      // 用于 placement new（置位构造）
#include <cstddef>  // ptrdiff_t 和 size_t 位于此处
#include <cstdlib>  // exit 和 EXIT_FAILURE 位于此处
#include <climits>  // UINT_MAX 位于此处
#include <iostream> // std::cerr 和 std::endl 位于此处

namespace CPP_ALLOC
{
    /**
     * @brief           调用 ::operator new 动态的分配指定数量和类型的内存。
     *
     * @tparam          __Type 要分配内存的类型
     *
     * @param __size    要分配内存的数量，是有符号类型，可以表示负的内存大小，这为指针运算提供了便利
     * @param __ptr     指定要分配内存类型的指针
     *
     * @return          分配成功返回这片内存的地址，如果失败就直接退出了
     */
    template <typename __Type>
    inline __Type *__allocate(std::ptrdiff_t __size, __Type *__ptr)
    {
        /*
            :: 意味着调用全局命名空间，避免命名空间冲突。

            operator new 负责分配内存，通过 (__size * sizeof(__Type)) 计算出要分配的具体内存数量，
            然后再强制转换成 size_t 类型，最后将返回的这片内存的地址强制转换成函数传入的类型，最后返回给指针 temp。

            使用 ::operator new 能够在内存分配失败时抛出异常。这可以使函数获得异常安全的特性，如果分配失败，调用栈会被正确释放和回滚。
            此外 ::operator new 是 C++ 中的一个全局函数，它利用当前平台的内存分配机制来分配内存，使用它可以使代码在不同平台上都能正常工作。
        */
        __Type *temp = (__Type *)(::operator new((std::size_t)(__size * sizeof(__Type))));

        /*若 temp 为空指针，意味着当前的内存已经分无可分，就需要越过缓冲区往标准错误输出内存耗尽的消息，而后退出程序。*/
        if (temp == nullptr)
        {
            std::cerr << "Out of Memory!" << std::endl;
            exit(EXIT_FAILURE);
        }

        /*若成功分配就返回这个指针*/
        return temp;
    }

    /**
     * @brief               释放指定的内存
     *
     * @tparam __Type       要释放的内存类型
     *
     * @param  __buffer     要释放的内存起始地址
     *
     * @return non-return
     */
    template <typename __Type>
    inline void __deallocate(__Type *__buffer) { ::operator delete(__buffer); }

    /**
     * @brief               利用 placement new 实现初始化（构建）已分配内存的功能
     *
     * @tparam __Type_1     已分配内存的类型
     * @tparam __Type_2     构建这片内存的类型（初始化，构建分离）
     *
     * @param __ptr         指向已分配内存的指针
     * @param __value       要构建这片内存的值
     *
     * @return non-return
     */
    template <typename __Type_1, typename __Type_2>
    inline void __construct(__Type_1 *__ptr, const __Type_2 &__value)
    {
        /*
            这条语句的意思是：
            在已经分配好的内存地址（__ptr）中构造了一个 __Type_1 类型的对象（使用拷贝构造），
            使用 __value 进行初始化。

            这种分配方式叫做 置位构造（Placement new），它可以在一块预先分配的内存中构造对象，而不需要先申请内存。
        */
        new (__ptr) __Type_1(__value);
    }

    /**
     * @brief           调用析构函数销毁指定地址的数据
     *
     * @tparam __Type   要析构的内存类型
     *
     * @param __ptr     要析构的内存的地址
     *
     * @return non-return
     */
    template <typename __Type>
    inline void __destroy(__Type *__ptr) { __ptr->~__Type(); }
}

#endif // _CPP_ALLOC_H_