#include <bits/stl_algobase.h>
#include <cstdio>
#include <string>

/**
 * @brief std::lexicographical_compare()，以 “字典排列方式”
 *        对两个序列 [first_1, last_1)，[first_1，last_2) 进行比较，
 *        持续到下面 3 种情况为止：
 * 
 *        （1）某一组对应元素彼此不相等
 *        （2）同时到达 last_1 和 last_2，（两个序列大小相同）
 *        （3）到达 last_1 或 last_2 两个序列的大小不同
 * 
 * 反直觉的是：如果两个序列完全相等，返回 false，反之才返回 true
 * 
 * 和其他 STL 算法类似，lexicographical_compare 也准备了
 * 
 * inline bool
 * lexicographical_compare(_II1 __first1, _II1 __last1, 
 *                         _II2 __first2, _II2 __last2, _Compare __comp);
 * 
 * 这样的重载，用户可以指定排序的规则 __comp。
*/

int main(int argc, char const *argv[])
{
    /**
     * 准备两个看起来相似的字符串数组。
    */
    std::string stringsA[] = {"Jesse", "Mike", "Jeb"};
    std::string stringsB[] = {"Jesse", "Mike", "Jeb"};

    printf("strings A and strings B is %sidentical.\n", 
        (!std::lexicographical_compare(stringsA, stringsA + 3, stringsB, stringsB + 3)) ? "" : "not "
    );

    return EXIT_SUCCESS;
}
