#ifndef __PRIORITY_QUEUE_H_
#define __PRIORITY_QUEUE_H_

#include <vector>
#include <functional>
#include <algorithm>

/**
 * @brief priority_queue    优先队列容器的实现
 * 
 * @tparam Type             队列元素类型     
 * @tparam Sequence         维护队列的底层数据结构类型
 * @tparam Compare          用于比较队列元素所用的类型
 */
template <typename Type, 
            typename Sequence = std::vector<Type>, 
            typename Compare = std::less<typename Sequence::value_type>
        >
class priority_queue
{
    public:
        typedef typename Sequence::value_type           value_type;+
        typedef typename Sequence::size_type            size_type;
        typedef typename Sequence::reference            reference;
        typedef typename Sequence::const_reference      const_reference;
    
    protected:
        Sequence    container;          // 队列底层容器
        Compare     comp;               // 队列的比较算法

    public:
        /**
         * @brief 默认构造函数，构造默认的优先队列。
         */
        priority_queue() : container() {};

        /**
         * @brief 构造函数，构造优先队列。
         * 
         * @param __comp        队列的比较算法
         */
        explicit priority_queue(const Compare & __comp) : container(), comp(__comp) {}

        /**
         * @brief 构造函数，从一个外部的 std::vector<Type> 中拷贝数据，构造优先队列
         * 
         * @tparam InputInterator   输入迭代器
         */
        template <typename InputInterator>
        priority_queue(
                        InputInterator __first, InputInterator __last, 
                        const Compare & __comp
                        ) : container(__first, __last), comp(__comp)
        {
            std::make_heap(container.begin(), container.end(), this->comp);
        }

        /**
         * @brief  构造函数，从一个外部的 std::vector<Type> 中拷贝数据，
         *         但是使用默认的比较规则。
         * 
         * @tparam InputIterator 
         */
        template <typename InputIterator>
        priority_queue(InputIterator __first, InputIterator __last) : container(__first, __last)
        {
            std::make_heap(this->container.begin(), this->container.end(), this->comp);
        }

        /**
         * @brief 检查队列是否为空
         * 
         * @return true     空队列
         * @return false    不是空队列
         */
        bool empty(void) const noexcept { return this->container.empty(); }

        /**
         * @brief 返回队列的长度
         */
        size_type size(void) const noexcept { return this->container.size(); }

        /**
         * @brief 返回满二叉树中，根节点的值。
         */
        value_type top(void) const { return this->container.front(); }

        /**
         * @brief 往队列中插入元素。
         */
        void push(value_type & __value);

        /**
         * @brief 移除队列的一个元素。
         */
        void pop(void);
};

template <typename Type, typename Sequence, typename Compare>
void priority_queue<Type, Sequence, Compare>::push(priority_queue::value_type & __value)
{
    try
    {
        this->container.push_back(__value);
        std::push_heap(
                        this->container.begin(), this->container.end(),
                        this->comp
                    );
    }
    catch (...)
    {
        this->container.clear();
    }
}

template <typename Type, typename Sequence, typename Compare>
void priority_queue<Type, Sequence, Compare>::pop(void)
{
    try
    {
        std::pop_heap(
                        this->container.begin(), this->container.end(), 
                        this->comp
                    );
        this->container.pop_back();
    }
    catch(...)
    {
        this->container.clear();
    }
    
}

#endif // __PRIORITY_QUEUE_H_