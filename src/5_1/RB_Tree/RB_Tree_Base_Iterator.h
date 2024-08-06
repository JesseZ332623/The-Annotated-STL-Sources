#ifndef __RB_TREE_BASE_ITERATOR_H_
#define __RB_TREE_BASE_ITERATOR_H_

#include <iterator>

//#include <set>

#include "./RB_Tree_Node.h"

struct RBTree_Base_Iterator
{
    // 指向一个红黑树节点的指针
    typedef RBTree_Node_Base::base_ptr          base_ptr;

    // 迭代器类型，为双向迭代器
    typedef std::bidirectional_iterator_tag     iterator_category;

    typedef std::ptrdiff_t                      differece_type;

    base_ptr node;  // 内部维护的一个红黑树节点指针

    /**
     * @brief 前进到下一个节点，使用中序遍历（左，根，右）
     *        作为子类 `operator++()` 的辅助函数。
     */
    void increment(void);
    
    /**
     * @brief 后退到上一个节点，使用中序遍历
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

void RBTree_Base_Iterator::decrement(void)
{
    /** 
     * 若为红节点，且节点的祖父节点就是它自己
    */
    if (
        this->node->color == RB_TREE_RED &&
        this->node->parent->parent == this->node
    )
    {
        // 往下来到当前节点的右节点即可
        this->node = this->node->right;
    }
    /**
     * 如果当前节点有左子节点
    */
    else if (this->node->left != nullptr)
    {
        base_ptr temp_left = this->node->left;

        while (temp_left->right != nullptr)
        {
            temp_left = temp_left->right;
        }
    }
    else
    {
        base_ptr temp_parent = this->node->parent;

        while (this->node == temp_parent->left)
        {
            this->node  = temp_parent;
            temp_parent = temp_parent->parent;
        }

        this->node = temp_parent;
    }
}

#endif // __RB_TREE_BASE_ITERATOR_H_