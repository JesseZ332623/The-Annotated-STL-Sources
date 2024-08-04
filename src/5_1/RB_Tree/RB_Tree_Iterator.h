#ifndef __RB_TREE_ITERATOR_H_
#define __RB_TREE_ITERATOR_H_

#include <iterator>

#include "./RB_Tree_Node.h"

struct RBTree_Base_Iterator
{
    // 指向一个红黑树节点的指针
    typedef RBTree_Node_Base::base_ptr base_ptr;

    // 迭代器类型，为双向迭代器
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef ptrdiff_t differece_type;

    base_ptr node;  // 内部维护的一个红黑树节点指针

    /**
     * @brief 前进到下一个节点
     *        作为子类 `operator++()` 的辅助函数。
     */
    void increment(void);
    
    /**
     * @brief 后退到上一个节点
     *        作为子类 `operator--()` 的辅助函数。
     */
    void decrement(void);
};

void RBTree_Base_Iterator::increment(void)
{
    // 如果右节点存在
    if (this->node->right != nullptr)
    {
        node = node->right; // 去到当前节点的右节点

        // 然后不断的向左，到达树的底部
        while (node->left != nullptr)
        {
            node = node->left;
        }
    }
    else    // 若右节点不存在
    {
        // 拿到当前节点的父节点
        base_ptr temp_parent = this->node->parent;
        
        /**
         * 如果当前节点等于父节点 temp_parent 的右节点，
         * 就不断上溯，直到不为父节点的右节点。
        */
        while (this->node == temp_parent->right)
        {
            this->node  = temp_parent;
            temp_parent = temp_parent->parent;
        }

        /**
         * 若此时右子节点不等于此时的父节点，
         * 那么此时的父节点就是结果，否则此时的 node 为解答
        */
        if (this->node->right != temp_parent)
        {
            this->node = temp_parent;
        }
    }
}

#endif // __RB_TREE_ITERATOR_H_