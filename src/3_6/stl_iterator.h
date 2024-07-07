#ifndef __STL_ITERATOR_H_
#define __STL_ITERATOR_H_

/**
 * Date: 2024.7.7
 * STL 的迭代器设计详解
*/

#include <cctype>

/**
 * 首先，确定 STL 使用的 5 个迭代器类型，分别为：
 * 
 * 1. 只写迭代器
 * 2. 只读迭代器
 * 3. 单向迭代器（继承自 input_iterator_tag）
 * 4. 双向迭代器（继承自单项迭代器）
 * 5. 随机访问迭代器（继承自双向迭代器）
*/
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iteratora_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iteratora_tag {};


/**
 * @brief 一个标准的 STL 迭代器所需要的内置类型别名，如果要开发兼容 STL 的迭代器，
 *        就需要继承这个类。
 * 
 * @tparam Category     迭代器类型
 * @tparam Type         迭代器所指数据类型
 * @tparam Distance     表示两个迭代器距离的类型（通常是 `std::ptrdiff_t`）
 * @tparam Pointer      迭代器所指数据的指针类型
 * @tparam Reference    迭代器所指数据的引用类型
*/
template <
            typename Category, typename Type, typename Distance = std::ptrdiff_t,
            typename Pointer = Type *, typename Reference = Type &
        >
struct iterator
{
    typedef Category        iterator_category;
    typedef Type            value_type;
    typedef Distance        difference_type;
    typedef Pointer         pointer;
    typedef Reference       reference;
};


/**
 * @brief 类型萃取（type_traits）提取迭代器内部的相应类型别名。
 * 
 * @tparam 迭代器的类型
*/
template <typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::iterator_category            iterator_category;
    typedef typename Iterator::value_type                   value_type;
    typedef typename Iterator::difference_type              difference_type;
    typedef typename Iterator::pointer                      pointer;
    typedef typename Iterator::reference                    reference;
};

/**
 * @brief 针对裸指针（如 `int *` `double *`）等类型的 `iterator_traits` 偏特化
 *        这样即便传入裸指针也能正常萃取出相应的类型。
 * 
 * @tparam Type 迭代器所指数据的类型
*/
template <typename Type>
struct iterator_traits<Type *>
{
    /**
     * 裸指针可以支持随机访问，
     * 所以使用 `random_access_iterator_tag` 标签。
    */
    typedef random_access_iterator_tag      iterator_category;
    typedef Type                            value_type;
    typedef std::ptrdiff_t                  difference_type;
    typedef Type *                          pointer;
    typedef Type &                          reference;
};

/**
 * @brief 针对只读裸指针（如 `const int *` `const char *`）
 *        等类型`iterator_traits` 偏特化
*/
template <typename Type>
struct iterator_traits<const Type *>
{
    /**
     * 裸指针可以支持随机访问，
     * 所以使用 `random_access_iterator_tag` 标签。
    */
    typedef random_access_iterator_tag      iterator_category;
    typedef Type                            value_type;
    typedef std::ptrdiff_t                  difference_type;
    typedef Type *                          pointer;
    typedef Type &                          reference;
};

/**
 * @brief 全局函数，用于分辨迭代器的类型
 * 
 * @tparam Iterator     迭代器类型
 * @param __iterator    任意一个符合 STL 标准迭代器
 *                     （但这个类型仅仅是为了出发编译器的重载决议，有没有名字都一样）
 * 
 * @return 该迭代器的 Tag
*/
template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator & __iterator)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;

    return category();
}

/**
 * @brief 全局函数，确定迭代器的 `difference_type` 类型。
 * 
 * @tparam Iterator     迭代器类型
*/
template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type *
distance_type(const Iterator &)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
}

/**
 * @brief 确定迭代器的 `value_type` 类型。
 * 
 * @tparam Iterator     迭代器类型
*/
template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type * 
value_type(const Iterator &)
{
    return static_cast<typename iterator_traits<Iterator>::value_type *>(0);
}

/**
 * @brief 针对只写迭代器，
 *        以及继承了前者的单向迭代器所设计的，计算两个迭代器距离的函数。
*/
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator __first, InputIterator __last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type distance = 0;

    while (__first != __last) { ++__first; ++distance; }

    return distance;
}

/**
 * @brief 针对随机访问迭代器设计的，计算两个迭代器距离的函数。
*/
template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator __first, RandomAccessIterator __last, random_access_iterator_tag)
{
    /**
     * 只有连续的容器才会使用 RandomAccessIterator，直接用尾减去首即可。
    */
    return (__last - __first);
}

/**
 * @brief 供客端调用的 `distance` 函数，求两个迭代器之间的距离。
*/
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator __first, InputIterator __last, input_iterator_tag)
{
    /**
     * 萃取出这个迭代器的标签。 
    */
    typedef typename iterator_traits<InputIterator>::iterator_category catagory;

    /**
     * 根据 catagory 确定该调用哪个版本的 __distance 函数。
    */
    return __distance(__first, __last, catagory());
}

/**
 * @brief 针对只写迭代器，单向迭代器设计的，用于把迭代器向后偏移 __n 个元素的函数。
*/
template <typename InputIterator, typename Distance>
inline void __advance(InputIterator & __iter, Distance __n, input_iterator_tag)
{
    while (__n--) { ++__iter; }
}

/**
 * @brief 针对双向迭代器设计的，用于把迭代器向后偏移 __n 个元素的函数。
*/
template <typename BidirectionalIterator, typename Distance>
inline void __advance(BidirectionalIterator & __iter, Distance __n, bidirectional_iteratora_tag)
{
    if (__n >= 0)
    {
        while (__n--) { ++__iter; }
    }
    else
    {
        while (__n++) { --__iter; }
    }
}

template <typename RandomAccessIterator, typename Distance>
inline void __advance(RandomAccessIterator & __iter, Distance __n, random_access_iterator_tag)
{
    __iter += __n;
}

template <typename InputIterator, typename Distance>
inline void my_advance(InputIterator & __iter, Distance __n)
{
    __advance(__iter, __n, iterator_category(__iter));
}

#endif // __STL_ITERATOR_H_