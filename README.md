# 侯捷《STL 源码分析》学习笔记，目前已经学习至：3-2 迭代器是一种 Smart Pointer

## 学习到 该章节的时候手一热搓了个单项链表容器的实现，详见：`./src/3_2/myForwardList/`，目前完成的功能如下

- 默认构造函数
- 拷贝构造函数及运算符
- 初始化列表拷贝构造
- 移动构造函数及运算符
- 从链表头部插入
- 从链表尾部插入
- 从链表头部删除节点
- 从链表尾部删除节点
- 随机插入
- 随机删除
- 返回链表节点数
- 返回链表首/尾迭代器
- 重载 << 运算符输出链表内数据
- 析构函数

## 未来针对 `./src/3_2/myForwardList/` 需要补充的功能有

- 重载更多的构建函数，让他适配从其他容器拷贝数据进行构造
- 提供更多的算法，比如 `find`，`sort`，`merge` 等（目前 sort 已经初步实现，等待测试）

Author: [JesseZ332623](https://github.com/JesseZ332623)

LICENCE: [MIT LICENCE](https://opensource.org/license/mit/)

Latest Update: 2024.02.12
