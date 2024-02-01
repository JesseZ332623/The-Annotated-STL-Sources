#ifndef _MY_ALLOCATOR_H_
#define _MY_ALLOCATOR_H_

#include "./simpleAlloc.h"
#include "./bits/stl_list.h"

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

/**
 * @brief 自己实现的 STL 分配器，底层使用 SGI 分配器来进行内存的管理，对象的构建和析构使用置位构造。
 * 
 * @tparam __Type       要通过该分配器分配内存的类型
*/
template <typename __Type>
class MyAllocator
{
public:

    /*使用 SGI 分配器进行内存管理*/
    using SGI_Allocator = SimpleAllocate::SimpleAllocator<__Type, sgiAlloc>;

    using value_type = __Type;             // 值
    using pointer = __Type *;              // 指针
    using constPointer = const __Type *;   // 常量指针
    using refernece = __Type &;            // 引用
    using constReference = const __Type &; // 常量引用
    using sizeType = std::size_t;          // std::size_t      类型的别名（该类型用于表示对象的大小）
    using differenceType = std::ptrdiff_t; // std::ptrdiff_t   类型的别名（该类型作为两个地址之间的差值使用）

    MyAllocator() = default;

    MyAllocator(const MyAllocator &) = default;

    /**
     * @brief 为类型 U 重新绑定分配器，
     * 通常在 STL 容器需要为内部存储不同类型的对象时，会使用该结构体进行类型适配（这个用法目前还不明确）。
     */
    template <typename U>
    struct rebind { using other = MyAllocator<U>; };

    /**
     * @brief           调用先前实现的 SGI_Allocator::allocate() 函数分配内存。
     *
     * @param __n       要分配的内存大小
     * @param __hint    在实现上可能用于增进区域性，或者完全忽略
     *
     * @return          返回已开辟内存的指针
     */
    pointer allocate(sizeType __n, const void *__hint = 0) { return SGI_Allocator::allocate(__n); }

    /**
     * @brief           调用先前实现的 SGI_Allocator::deallocate() 函数去归还 SGI_Allocator::allocate() 开辟的内存。
     *
     * @param __buffer  要释放的内存起始地址
     * @param __n       要释放内存的大小，根据该参数来确定使用哪一级分配器
     *
     * @return non-return
     */
    void deallocate(pointer __buffer, sizeType __n) { SGI_Allocator::deallocate(__buffer, __n); }

    /**
     * @brief           调用先前实现的 __construct 函数，通过置位构造的方式构建已分配的内存。
     *
     * @param __buffer  通过 allocate 函数分配后的内存块的起始地址
     * @param __value   要构建这片内存的值
     *
     * @return non-return
     */
    void construct(pointer __buffer, constReference __value) { __construct(__buffer, __value); }

    /**
     * @brief           调用先前实现的 __destroy 函数，析构由 construct 函数构建的内存。
     *
     * @param __buffer  要析构的起始内存地址
     */
    void destruct(pointer __buffer) { __destroy(__buffer); }

    /**
     * @brief       获得对象地址的辅助函数，在访问迭代器中的引用类型时需要获取原对象地址，F如 vector 的 at 方法。
     *
     * @param __x   传入一个变量的引用
     *
     * @return      返回传入引用类型的地址
     */
    pointer getAddress(refernece __x) const { return (pointer)&__x; }

    /**
     * @brief       获得对象地址的辅助函数，在访问迭代器中的引用类型时需要获取原对象地址，比如 vector 的 at 方法。
     *
     * @param __x   传入一个变量的常量引用
     *
     * @return      返回传入引用类型的常量地址
     */
    constPointer getConstAddress(constReference __x) const { return (constPointer)&__x; }

    /**
     * @brief   获取分配器可分配的当前类型的最大理论数量，通常在 STL 容器进行扩容的时候会查询。
     *
     * @return  一个临时的右值，是本机无符号整型的最大值除以当前单个类型的容量。
     */
    sizeType maxSize() const { return sizeType(UINT_MAX / sizeof(__Type)); }

    ~MyAllocator() = default;
};

//typedef MyAllocator<std::string> stringAlloc;

#endif // _MY_ALLOCATOR_H_