#ifndef __RB_TREE_NODE_H_
#define __RB_TREE_NODE_H_

/**
 * 用布尔类型来表示树节点的颜色
*/
typedef bool RB_TREE_COLOR_TYPE;

const RB_TREE_COLOR_TYPE RB_TREE_RED   = false; // 树节点为红
const RB_TREE_COLOR_TYPE RB_TREE_BLACK = true;  // 树节点为黑

/**
 * @brief 红黑树的节点设计
 */
struct  RBTree_Node_Base
{
    typedef RB_TREE_COLOR_TYPE color_type;
    typedef RBTree_Node_Base * base_ptr;

    color_type color;   // 节点的颜色

    base_ptr parent;    // 该节点的父节点
    base_ptr left;      // 该节点的左子节点
    base_ptr right;     // 该节点的右子节点

    /**
     * @brief 找到树中值最小的节点，
     *        这很简单，不断寻找左节点，直到底部即可。
     * 
     * @param node          树中的某一个节点
     * @return base_ptr     树中值最小的节点
    */
    static base_ptr min_value(base_ptr node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    /**
     * @brief 找到树中值最大的节点，
     *        这很简单，不断寻找右节点，直到底部即可。
     * 
     * @param node          树中的某一个节点
     * @return base_ptr     树中值最大的节点
    */
    static base_ptr max_value(base_ptr node) {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }   
};

/**
 * @brief 一个完整的红黑树节点
 * 
 * @tparam Type 节点值类型
*/
template <typename Type>
class RBTree_Node : public RBTree_Node_Base
{
    typedef RBTree_Node<Type> * link_type;

    Type value_field;
};

#endif // __RB_TREE_NODE_H_