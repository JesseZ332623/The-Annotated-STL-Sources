# 侯捷《STL 源码分析》学习笔记，目前已经学习至：4-2 序列式容器的 vector，且已经搓完了 vector

![cover](https://i0.hdslb.com/bfs/archive/83d95f897ae4d9e52f25d4076bd7daec3946d9d6.jpg@600w_600h_1c.png)

## 2024.02.17

## 学习到 该章节的时候手一热搓了个单项链表容器的实现，详见：`./src/3_2/myForwardList/`，目前完成的功能如下

- 默认构造函数
- 拷贝构造函数及运算符
- 从 `std::vector<Type>` 拷贝数据进行构造
- 初始化列表拷贝构造
- 移动构造函数及运算符
- 从链表头部插入
- 从链表尾部插入
- 从链表头部删除节点
- 从链表尾部删除节点
- 随机插入
- 随机删除
- 返回链表节点数
- 返回链表首 / 尾迭代器
- 检查是否为空链表
- 交换两个不同链表的数据
- 按值查找链表节点 / 按条件查找链表节点
- 重载 << 运算符输出链表内数据
- 重载 [] 符号用下标去访问链表节点
- 析构函数

### 未来针对 `./src/3_2/myForwardList/` 需要补充的功能有

- 重载更多的构建函数，让他适配从其他容器拷贝数据进行构造（如从 `std::vector` `std::array` `C 风格数组等`）
- 提供更多的算法，比如 `sort`，`merge` 等（目前 sort 已经初步实现，等待测试）

## 2024.06.18

## 学习到第四章 序列式容器的 vector，且已经搓完了 vector

- 详见 [一个基本的 vector 容器实现](https://github.com/JesseZ332623/The-Annotated-STL-Sources/blob/master/src/4_2/vector/include/myVector.h)

## 2024.06.20

## 仔细学习了 C++ STL 的内存操作函数

- 详见 [补充：C++ 的内存操作函数](https://github.com/JesseZ332623/The-Annotated-STL-Sources/blob/master/src/4_2/%E8%A1%A5%E5%85%85%EF%BC%9AC%2B%2B%20%E7%9A%84%E5%86%85%E5%AD%98%E6%93%8D%E4%BD%9C%E5%87%BD%E6%95%B0.md)

Author: [JesseZ332623](https://github.com/JesseZ332623)

LICENCE: [MIT LICENCE](https://opensource.org/license/mit/)

Latest Update: 2024.06.20
