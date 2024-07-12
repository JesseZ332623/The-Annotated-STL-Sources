#ifndef __STACK_H_
#define __STACK_H_

#include <deque>

/**
 * @brief `std::stack` 是一种先进后出（First In Last Out FILO）的数据结构，
 *        它只有一个出口，`stack` 允许从出口压入元素，移除元素，以及取得栈顶元素。
 *        但除了最顶端之外，没有任何其他方法可以存取 `stack` 的其他元素，
 *        所以 `stack` 不会有遍历容器的行为，也就不会为它设计迭代器。
 * 
 * @brief - 像栈这样以底层容器完成所有工作，
 *          具有 “修改某物接口，形成另一种风貌” 的容器，称为 `Adapter`（适配器），
 *          所以 `std::stack` 不算容器（Container），而算容器的适配器（Container Adapter）。
 * 
 * @tparam Type         栈元素的类型
 * @tparam Sequence     栈的底层容器，默认采用缺省的双端队列（`std::deque<Type>`）
 *                      或双向链表（`std::list<Type>`）
*/
template <typename Type, typename Sequence = std::deque<Type>>
class stack
{
    /**
     * @brief 比较两个 stack 内部的元素数是否相等。
    */
    friend bool operator==(const stack & __a, const stack & __b) { return __a.container == __b.container; }

    /**
     * @brief 比较 stack __a 内部的元素数是否小于 stack __b 内部的元素数。
    */
    friend bool operator<(const stack & __a, const stack & __b) { return __a.container < __b.container; }

    public:
        // 直接萃取栈底层容器的相关类型即可
        typedef typename Sequence::value_type       value_type;
        typedef typename Sequence::size_type        size_type;
        typedef typename Sequence::reference        reference;
        typedef typename Sequence::const_reference  const_reference;

    protected:
        Sequence container;     // 底层的容器实例
    
    public:
        /**
         * @brief 检查是否为空栈
        */
        bool empty() { return this->container.empty(); }

        /**
         * @brief 求栈当前的元素数
        */
        size_type size() { return this->container.size(); }

        /**
         * @brief 取当前的栈顶元素。
        */
        reference top() { return this->container.back(); }

        /**
         * @brief 取当前的栈顶元素。
        */
        const_reference top() const { return this->container.back(); }

        /**
         * @brief 压栈
        */
        void push(Type & __value) { this->container.push_back(__value); }

        /**
         * @brief 出栈
        */
        void pop() { this->container.pop_back(); }
};

#endif // __STACK_H_