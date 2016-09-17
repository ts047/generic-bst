#include "bst.h"

int main()
{
    AVLtree<int> t;
    t.insert(4);
    t.insert(14);
    t.insert(23);
    t.insert(9);
    t.insert(17);
    t.insert(21);

    t.printTest();

    return 0;
}

