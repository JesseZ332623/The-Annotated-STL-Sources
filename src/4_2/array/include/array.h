#ifndef __ARRAY_H_
#define __ARRAY_H_

#include <compare>                  // 三项比较运算符 <=> 以及针对该运算符相关算法的实现
#include <type_traits>              // 该头文件用于类型萃取
#include <initializer_list>         // 初始化列表类所在
#include <bits/stl_algobase.h>      // 一些基础的 STL 算法实现
#include <bits/range_access.h>      // 一些遍历算法的实现，如（std::begin()，std::end()）

/**
 * @brief `std::array` 容器的类型特质
 * 
 * @tparam Type 数组的元素类型
 * @tparam Size 数组大小
 */
template <typename Type, std::size_t Size>
struct array_traits
{
    using ArrayType = Type[Size];

    /**
     * @brief 使用 `std::is_swappable<Type>`
     *        来检测类型 Type 是否支持交换操作。
     */
    using Is_Swappable              = std::is_swappable<Type>;

    /**
     * @brief 使用 `std::is_nothrow_swappable<Type>`
     *        来检测类型 Type 是否支持不抛异常的交换操作。
    */
    using Is_NotThrow_Swappable     = std::is_nothrow_swappable<Type>;
};

/**
 * @brief 针对 Size 为 0 时的`std::array` 容器的类型特质的偏特化
 * 
 * @tparam Type 数组的元素类型
*/
template <typename Type>
struct array_traits<Type, 0>
{
    /**
     * @brief `ZeroLengthArray` 是一个内部嵌套结构体，它被定义为零长度数组的替代类型。
     *        因为零长数组本身没有意义，所以需要一个替代类型来模仿其行为。
     */
    struct ZeroLengthArray
    {
        /**
         * @brief 针对零长数组的访问操作，显然这是不现实的，
         *        所以会调用 `__builtin_trap()` 函数触发硬件陷阱，让程序崩溃。
         */
        __attribute__((__always_inline__, __noreturn__))
        Type & operator[](std::size_t) const noexcept 
        { 
            /**
             * 在 x86 架构下，该函数会被编译为 `int $3` 指令，
             * 这是一个软件中断指令，令 CPU 停止执行当前进程，
             * 跳转到事先定义好的中断处理程序 ISR 中去，这通常是调试器或操作系统的异常处理代码。
             */
            __builtin_trap(); 
        }

        /**
         * @brief 如果对零长数组作出显式类型转换，如：
         *        `int * arrPointer = static_cast<int *>(std::array<int, 0>());`
         *        就会调用该运算符重载，返回空指针，从而避免未定义行为。
        */
        __attribute__((__always_inline__))
        constexpr explicit operator Type *() const noexcept { return nullptr; }
    };
    
    /**
     * 零长数组内没有任何元素，所以两个对象的交换永远都是异常安全的。
    */
    using Is_Swappable              = std::true_type;
    using Is_NotThrow_Swappable     = std::true_type;
};  

/**
 * @brief 一个标准的容器，存储了固定长度且连续的元素。
 * 
 * @ingroup 属于序列式容器
 * 
 * @tparam Type 数组的元素类型，必须是一个明确的，完整的类型。
 * @tparam Size 数组的元素数量
 */
template <typename Type, std::size_t Size>
struct array
{
    typedef Type                    value_type;
    typedef value_type *            pointer;
    typedef const value_type *      const_pointer;
    typedef value_type &            reference;
    typedef const value_type &      const_reference;
    typedef std::size_t             size_type;
    typedef std::ptrdiff_t          difference_type;

    typedef value_type *                            iterator;
    typedef const value_type *                      const_iterator;
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    typename array_traits<Type, Size>::ArrayType elements;   // 数组被创建在此处

    /** 
     * @brief 用 __n 填充这个数组，
     *        constexpr 关键字表面这个函数会在编译期完成运行。
    */
    constexpr void fill(const value_type & __n)
    {
        std::fill_n(this->begin(), this->end(), __n);
    }

    /**
     * @brief 与另一个 array 类交换数据。
     *        noexcept 关键字会在确定表达式 `array_traits<Type, Size>::Is_NotThrow_Swappable::value`
     *        的值后来决定抛不抛异常。
    */
    constexpr void 
    swap(array & __other) noexcept(array_traits<Type, Size>::Is_NotThrow_Swappable::value)
    {
        std::swap_ranges(this->begin(), this->end(), __other.begin());
    }

    constexpr iterator          
    begin(void) noexcept        { return iterator(this->data()); }

    constexpr const_iterator    
    begin(void) const noexcept  { return const_iterator(this->data()); }

    constexpr const_iterator    
    cbegin(void) const noexcept { return const_iterator(this->data()); }

    constexpr reverse_iterator  
    rbegin(void) noexcept       { return reverse_iterator(this->data() + Size); }

    constexpr const_reverse_iterator  
    rbegin(void) const noexcept { return const_reverse_iterator(this->data() + Size); }

    constexpr const_reverse_iterator  
    crbegin(void) const noexcept { return const_reverse_iterator(this->data() + Size); }

    constexpr iterator
    end(void) noexcept          { return iterator(this->data() + Size); }

    constexpr const_iterator 
    end(void) const noexcept    { return const_iterator(this->data() + Size); }

    constexpr const_iterator 
    cend(void) const noexcept    { return const_iterator(this->data() + Size); }

    constexpr reverse_iterator
    rend(void) noexcept         { return reverse_iterator(this->data()); }

    constexpr const_reverse_iterator
    rend(void) const noexcept   { return const_reverse_iterator(this->data()); }

    constexpr const_reverse_iterator
    crend(void) const noexcept   { return const_reverse_iterator(this->data()); }

    constexpr size_type size()      const noexcept { return Size; }
    constexpr size_type max_size()  const noexcept { return Size; }

    constexpr bool empty() const noexcept { return (this->size() == 0); }

    constexpr reference operator[](size_type __n) noexcept
    { 
        /**
         * __glibcxx_requires_subscript() 这个宏用于在编译器检查类型是否支持 [] 运算符，
         * 以及是否支持索引操作，它的一种可能的展开如下（C++ 17 版本及以上）：
         * 
         * static_assert(
         *                  std::requires { { expr[0] } -> std::same_as<decltype(expr) &>; }, 
         *                  "Expression does not support subscript operator"
         * )
         */
        __glibcxx_requires_subscript(__n);
        return this->elements[__n]; 
    }

    constexpr const_reference operator[](size_type __n) const noexcept
    { 
        __glibcxx_requires_subscript(__n);
        return this->elements[__n]; 
    }

    constexpr reference at(size_type __n)
    {
        if (__n >= Size)
        {
            /**
             * __throw_out_of_range_fmt() 是一个比较低级的抛异常函数，
             * 支持传入格式化字符串来说明异常。
             * 
             * __N() 宏可以标记包裹的字符串，在编译时这个被标记的字符串会被交给像 xgettext（属于 GNU 工具链）这样的工具去翻译，
             * 让该字符串在不同的语言环境下被翻译成不同的语言。
            */
            std::__throw_out_of_range_fmt(
                __N("array::at() __n (which is: %zu) >= Size (which is: %zu)"), __n, Size
            );
        }

        return this->elements[__n];
    }

    constexpr const_reference at(size_type __n) const
    {
        /**
         * 这里我感觉就是程序员搁那炫技，使用一个三元运算符合并所有的逻辑，如下所示：
         * 
         *  (expression) ? (left-value) : (throw-exception, left-value);
         * 
         * 可以看出为了保证在任何时候这条语句的返回值都为左值（因为函数的返回值通常是一个临时的右值），
         * 特地在异常函数后面补充一个 this->elements[__n]，
         * 但异常函数 __throw_out_of_range_fmt() 调用后执行流就已经不在函数内了，因此这个返回值永远都不会被使用，且完美符合语法。
         * 
         * 妙啊 ~
        */
        return (__n < Size) ? this->elements[__n] 
                            : (std::__throw_out_of_range_fmt(
                                    __N(
                                            "array::at() __n (which is: %zu)" ">= Size (which is: %zu)"
                                        ), __n, Size), this->elements[__n]
                                );
    }

    constexpr reference front() noexcept 
    { 
        // 基于 SFINAE 特性设计的宏，用于保证该操作是在非空容器上使用的。
        __glibcxx_requires_nonempty();

        return this->elements[(size_type)0]; 
    }

    constexpr const_reference front() const noexcept 
    { 
        __glibcxx_requires_nonempty();

        return this->elements[(size_type)0]; 
    }

    constexpr reference back() noexcept
    {
        __glibcxx_requires_nonempty();

        return this->elements[Size - 1];
    }

    constexpr const_reference back() const noexcept
    {
        __glibcxx_requires_nonempty();

        return this->elements[Size - 1];
    }

    /**
     * @brief 转换为 C 风格数组。
    */
    constexpr pointer data(void) noexcept
    {
        return static_cast<pointer>(this->elements);
    }

    /**
     * @brief 转换为 C 风格只读数组。
    */
    constexpr const_pointer data(void) const noexcept
    {
        return static_cast<const_pointer>(this->elements);
    }
};

/**
 * @brief 比较两个 array<Type, Size> 对象是否完全相等。
*/
template <typename Type, std::size_t Size>
constexpr inline bool
operator==(const array<Type, Size> & __a, const array<Type, Size> & __b)
{
    return std::equal(__a.begin(), __a.end(), __b.begin());
}

/**
 * @brief `std::__detail::__synth3way_t<Type>`是 C++20 中引入的用于三向比较运算符`<=>`的辅助类型。
 *        这个类型主要用于解决三向比较运算符返回值的类型问题，确保比较操作的一致性和可预测性。
 *        在 C++20 之前，比较运算符如`<`, `<=`, `==`, `!=`, `>`, `>=`分别返回 `bool` 类型的值，这在大多数情况下工作得很好。
 *        但是，这些操作符的分散使用有时会导致代码的重复和不一致。
 *        为了解决这些问题，C++20 引入了三向比较运算符` <=> `，它能够在一个操作符中实现`<`, `==`, 和`>`的比较逻辑。
 */
template <typename Type, std::size_t Size>
constexpr std::__detail::__synth3way_t<Type>
operator<=>(const array<Type, Size> & __a, const array<Type, Size> & __b)
{
    /**
     * 如果数组的长度不为 0，
     * 且 std::__is_memcmp_ordered<Type>::value 返回真（意为 Type 类型可以被 memcmp() 安全的调用）
    */
    if constexpr (Size && std::__is_memcmp_ordered<Type>::value)
    {
        /**
         * __is_constant_evaluated() 函数用于检查函数是在编译期还是在运行期间调用的。
         * 此处 if 语句表明：函数必须在运行期被调用，才执行下面的语句块。
        */
        if (!std::__is_constant_evaluated())
        {
            constexpr std::size_t n = Size * sizeof(Type);      // 拿到数组的字节数

            /** 
             * __builtin_memcmp() 函数是 GCC 或 Clang 编译器内置的函数，和 C 标准库的 memcmp() 类似，
             * 但是能直接调用 CPU 指令进行优化比较，因此它会比 memcmp() 更快。
            */
            return __builtin_memcmp(__a.data(), __b.data(), n) <=> 0;
        }

        for (std::size_t index = 0; index < Size; ++index)
        {
            /**
             * std::__detail::__synth3way() 是一个内部函数，它会返回两个值的三向结果。
            */
            auto ordering = std::__detail::__synth3way(__a[index], __b[index]);

            // 若比较过程中出现任何一次不相等，直接返回就行了。
            if (ordering != 0) { return ordering; }
        }

        return std::strong_ordering::equal;
    }
}

template <typename Type, std::size_t Size>
constexpr inline std::enable_if_t<array_traits<Type,Size>::Is_Swappable::value> 
swap(array<Type, Size> & __a, array<Type, Size> & __b) noexcept(noexcept(__a.swap(__b)))
{
    __a.swap(__b);
}

#endif // __ARRAY_H_