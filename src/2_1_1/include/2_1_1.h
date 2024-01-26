#ifndef _EASY_ALLOCATOR_H_
#define _EASY_ALLOCATOR_H_

#include <new>              // 用于 placement new（置位构造）
#include <cstddef>          // ptrdiff_t 和 size_t 位于此处
#include <cstdlib>          // exit 和 EXIT_FAILURE 位于此处
#include <climits>          // UINT_MAX 位于此处
#include <iostream>         // std::cerr 和 std::endl 位于此处

/*编写一个简单的空间配置器*/

/*
    根据 STL 的规范，分配器（allocator）有以下的必要接口：

    using value_type        = __Type;                   // 值
    using pointer           = __Type*;                  // 指针
    using constPointer      = const __Type*;            // 常量指针
    using refernece         = __Type&;                  // 引用
    using constReference    = const __Type&;            // 常量引用
    using sizeType          = std::size_t;              // std::size_t      类型的别名（该类型用于表示对象的大小）
    using differenceType    = std::ptrdiff_t;           // std::ptrdiff_t   类型的别名（该类型作为两个地址之间的差值使用）

    allocator::rebind                          一个嵌套的模板类，用于唯一成员 othor，那是一个 typedef，代表 Allocator<U>

    allocator::allocator();                                                         默认构造函数
    allocator::allocator(const allocator &);                                        拷贝构造函数

    template <typename U> allocator::allocator(const allocator<U> &);               泛化的拷贝构造函数

    pointer allocator::getAddress(refernece __x) const;                             返回某个对象的地址

    constPointer allocator::getConstAddress(constReference __x) const;              返回某个 const 对象的地址

    配置空间，用于存储 __n 个 value_type 类型的对象， 第二个参数 __hint 可能用于增进区域性，或者完全忽略
    pointer allocator::allocate(sizeType __n, const void * __hint = 0);

    归还先前配置的空间
    void allocator::deallocate(pointer __buffer, sizeType __n);

    返回可成功配置的最大值
    sizeType allocator::maxSize() const;

    等同于 new (__buffer) __Type_1(__value); 只是多做了一层封装
    void allocator::construct(pointer __buffer, constReference __value);

    等同于 __buffer->~__Type(); 只是多做了一层封装
    void allocator::destruct(pointer __buffer);

    析构函数
    allocator::~allocator()
*/

namespace myStl
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
    inline __Type * _allocate(std::ptrdiff_t __size, __Type * __ptr)
    {
        /*
            :: 意味着调用全局命名空间，避免命名空间冲突。

            operator new 负责分配内存，通过 (__size * sizeof(__Type)) 计算出要分配的具体内存数量，
            然后再强制转换成 size_t 类型，最后将返回的这片内存的地址强制转换成函数传入的类型，最后返回给指针 temp。

            使用 ::operator new 能够在内存分配失败时抛出异常。这可以使函数获得异常安全的特性，如果分配失败，调用栈会被正确释放和回滚。
            此外 ::operator new 是 C++ 中的一个全局函数，它利用当前平台的内存分配机制来分配内存，使用它可以使代码在不同平台上都能正常工作。
        */
        __Type * temp = (__Type *)(::operator new((std::size_t)(__size * sizeof(__Type))));

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
    inline void _deallocate(__Type * __buffer) { ::operator delete(__buffer); }

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
    inline void _construct(__Type_1 * __ptr, const __Type_2 & __value) 
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
    inline void _destroy(__Type * __ptr) { __ptr->~__Type(); }

    /**
     * @brief 有了上述分配，构建，析构，释放操作的函数，就可以开始着手设计分配器了
     * 
     * @tparam __Type   要分配的内存类型
    */
    template <typename __Type>
    class Allocator
    {
        public:
            using value_type        = __Type;                   // 值
            using pointer           = __Type*;                  // 指针
            using constPointer      = const __Type*;            // 常量指针
            using refernece         = __Type&;                  // 引用
            using constReference    = const __Type&;            // 常量引用
            using sizeType          = std::size_t;              // std::size_t      类型的别名（该类型用于表示对象的大小）
            using differenceType    = std::ptrdiff_t;           // std::ptrdiff_t   类型的别名（该类型作为两个地址之间的差值使用）

            Allocator() = default;

            Allocator(const Allocator &) = default;

            /**
             * @brief 为类型 U 重新绑定分配器，
             * 通常在 STL 容器需要为内部存储不同类型的对象时，会使用该结构体进行类型适配（这个用法目前还不明确）。
            */
            template <typename U>
            struct rebind { using other = Allocator<U>; };

            /**
             * @brief           调用先前实现的 _allocate 函数分配内存。
             * 
             * @param __n       要分配的内存大小
             * @param __hint    在实现上可能用于增进区域性，或者完全忽略
             * 
             * @return          返回已开辟内存的指针
            */
            pointer allocate(sizeType __n, const void * __hint = 0) { return _allocate((differenceType)__n, (pointer)0); }

            /**
             *  @brief          调用先前实现的 _deallocate 函数去释放 allocate 开辟的内存。
             * 
             * @param __buffer  要释放的内存起始地址
             * @param __n       要释放内存的大小? 目前没有使用，可能是为了兼容标准
             * 
             * @return non-return
            */
            void deallocate(pointer __buffer, sizeType __n) { _deallocate(__buffer); }

            /**
             * @brief           调用先前实现的 _construct 函数，通过置位构造的方式构建已分配的内存。
             * 
             * @param __buffer  通过 allocate 函数分配后的内存块的起始地址
             * @param __value   要构建这片内存的值
             * 
             * @return non-return
            */
            void construct(pointer __buffer, constReference __value) { _construct(__buffer, __value); }

            /**
             * @brief           调用先前实现的 _destroy 函数，析构由 construct 函数构建的内存。
             * 
             * @param __buffer  要析构的起始内存地址
            */
            void destruct(pointer __buffer) { _destroy(__buffer); }

            /**
             * @brief       获得对象地址的辅助函数，在访问迭代器中的引用类型时需要获取原对象地址，比如 vector 的 at 方法。
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

            ~Allocator() = default;
    };

} // end of namespace myStl

#endif // _EASY_ALLOCATOR_H_
