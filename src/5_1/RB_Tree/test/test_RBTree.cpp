#include "../RB_Tree.h"
#include <functional>

template <typename Key, typename Value>
struct KeyGetter
{
    Key operator() (const std::pair<Key, Value> & __pair) const
    { return __pair.first; }
};


int main(int argc, char const *argv[])
{
    RB_Tree<int, int, KeyGetter<int, int>, std::less<int>> testRB_Tree;

    return EXIT_SUCCESS;
}
