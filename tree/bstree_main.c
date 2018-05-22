#include <stdio.h>
#include <stdlib.h>
#include "bstree.h"
int main(int argc, char **argv)
{
    /* struct t_node node; */
    /* node.rbt_color = RBT_COLOR_BLACK; */
    /* node.data = (void*)1; */

    /* T_node node_p = (T_node)malloc(sizeof(struct t_node)); */
    /* node_p->rbt_color = RBT_COLOR_RED; */
    /* node_p->data = (void*)2; */
        
    /* printf("%d %d\n", node.rbt_color, (int)node.data); */
    /* printf("%d %d\n", node_p->rbt_color, (int)node_p->data); */
    
    /* exit(0); */
    
    T_tree tree = create_tree(TYPE_BS_TREE);
    T_node n, m;
    int arra[] = {
55,
21,
82,
7,
5,
76,
69,
74,
70,
96,
93,
43,
63,
14,
24,
53,
93,
14,
85,
15,
0,
    };
    const int max = 1000;
    T_node nodes[max];
    for (int i = 0; i < max; i++) {
        int k = arc4random() % 100000;
        //        printf("%d\n",k);
        //        arra[i] = k;
        /* if (arra[i] == 12) */
        /*     n = rbt_insert(tree, arra[i]); */
        /* else if (arra[i] == 30) */
        /*     m = t_insert(tree, arra[i]); */
        /* else */
        nodes[i] = t_insert(tree, k);
        /* printf("-----pre\n"); */
        /* print_tree_preorder(tree); */
        /* printf("-----in\n"); */
        /* print_tree_inorder(tree); */
    }

    //    t_right_rotate(tree, n);
    /* for (int i = 0; i < 5; i++) { */
    /*     printf("--%d\n", arra[i]); */
    /*     T_node node = t_search(tree, arra[i]); */
    /*     t_delete(tree, &node); */
    /* } */
    //    return 0;
    /* printf("-----pre\n"); */
    /* print_tree_preorder(tree); */
    /* printf("-----in\n"); */
    /* print_tree_inorder(tree); */
    
    for (int i = 0; i < max; i++ ) {
        //        printf("%d\n", i);
        int k = arc4random() % 10000;
        if (k > 100) {
            //            printf("-%d\n", nodes[i]->v);
            t_delete(tree, nodes+i);
        }
    /* printf("-----pre\n"); */
    /* print_tree_preorder(tree); */
    /* printf("-----in\n"); */
    /* print_tree_inorder(tree); */
    }

    printf("-----pre\n");
    print_tree_preorder(tree);
    printf("-----in\n");
    print_tree_inorder(tree);
    return 0;
    printf("depth: %d\n", tree_height(tree));
    printf("max: %d\n", t_maximum(tree)->v);
    printf("2max: %d\n", t_predecessor(tree, t_maximum(tree))->v);
}
