#ifndef _1_9_5_H
#define _1_9_5_H

#include <iostream>
#include <vector>

/*
    1_9_5 前闭后开区间表示法。

    任何一个 STL 算法，都需要获得由一对迭代器（泛型指针）所标示的区间，用来表示操作范围。
    这对迭代器所表示的是一个所谓的前闭后开区间，用 [frist,  last) 来表示。
    这种 off by one（偏移一格）的标示法，带来了许多方便，如果想设计对 STL 容器进行操作的算法，就会显得非常干净利落。
*/

/**
 * @brief 在一个 STL 容器内寻找一个值，并返回这个值的迭代器。
 * 
 * @tparam InputIterator STL 迭代器类型
 * @tparam Type          需要查找的元素类型
 * 
 * @param first          范围开始
 * @param last           范围结束
 * @param value          要搜索的值
 * 
 * @return               返回目标值的迭代器，如果找不到就返回容器最后一个元素的迭代器
*/
template <typename InputIterator, typename Type>
InputIterator find(InputIterator first, InputIterator last, const Type & value);

/**
 * @brief 循环输出 STL 容器中的值，即对容器内每一个元素都执行 f 操作
 * 
 * @tparam InputIterator STL 迭代器类型
 * @tparam Function          输出操作的函数指针，函数对象，或 Lamba 表达式
 * 
 * @param first          范围开始
 * @param last           范围结束
 * @param value          输出操作
 * 
 * @return               返回负责输出操作的函数
*/
template <typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function f);

#endif //_1_9_5_H
