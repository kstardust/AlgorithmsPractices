#include <stdio.h>
#include <stdlib.h>
#include "bstree.h"

int main(int argc, char **argv)
{
    T_tree tree = create_tree();
    int arra[10];
    for (int i = 0; i < 10; i++) {
        int k = arc4random() % 100;
        printf("%d\n", k);
        arra[i] = k;
        t_insert(tree, k);
    }

    for (int i = 0; i < 5; i++) {
        printf("--%d\n", arra[i]);
                T_node node = t_search(tree, arra[i]);
                t_left_rotate(tree, node);
        //        t_delete(tree, &node);
    }
    
    printf("-----\n");
    print_tree_preorder(tree);
    printf("-----\n");
    printf("max: %d\n", t_maximum(tree)->v);
    printf("2max: %d\n", t_predecessor(tree, t_maximum(tree))->v);
}
