#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#define TYPE_BS_TREE  0x01
#define TYPE_RB_TREE  0x02
#define TYPE_AVL_TREE 0x04

#define RBT_COLOR_BLACK 0x1
#define RBT_COLOR_RED   0x2

#define AVL_BALANCED     0
#define AVL_LEFT_HEAVY  -1
#define AVL_RIGHT_HEAVY  1

struct t_node;
struct t_tree;

typedef struct t_node* T_node;
typedef struct t_tree* T_tree;

struct _rbt_data {
    void* data;    
    int color;
};

struct t_node {
    T_node p;
    T_node left;
    T_node right;
    int v;
    struct _rbt_data _data;
#define t_data _data.data
#define rbt_color _data.color
#define avl_bf _data.color      /* balance factor */
};

struct t_tree {
    T_node root;
    T_node t_nil;
    unsigned char type;
};

T_tree create_tree(int type);
T_node t_insert(T_tree tree, int k);
T_node t_search(T_tree tree, int k);
void t_delete(T_tree tree, T_node* nodep);
void t_tree_free(T_tree* tree);

T_node t_maximum(T_tree tree);
T_node t_minimum(T_tree tree);
T_node t_predecessor(T_tree tree, T_node node);
T_node t_successor(T_tree tree, T_node node);

void   t_transplant(T_tree tree, T_node v, T_node u);
T_node t_left_rotate(T_tree tree, T_node x);
T_node t_right_rotate(T_tree tree, T_node x);

T_node bst_insert(T_tree tree, int k);
void   bst_delete(T_tree tree, T_node* nodep);

T_node rbt_insert(T_tree tree, int k);
void   rbt_delete(T_tree tree, T_node* nodep);

T_node avl_insert(T_tree tree, int k);
void   avl_delete(T_tree tree, T_node* nodep);

void print_tree_preorder(T_tree tree);
void print_tree_inorder(T_tree tree);

#endif
