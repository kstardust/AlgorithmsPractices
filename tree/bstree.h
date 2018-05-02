#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#define RBT_COLOR_BLACK 0x1
#define RBT_COLOR_RED   0x2

#define TR_LEFT  0x1
#define TR_RIGHT 0x2

struct t_node;
struct t_tree;

typedef struct t_node* T_node;
typedef struct t_tree* T_tree;

struct _rbt_data {
    int color;
    void* data;
};

struct t_node {
    T_node p;
    T_node left;
    T_node right;
    int v;
    struct _rbt_data _data;
#define data _data.data
#define rbt_color _data.color
};

struct t_tree {
    T_node root;
    T_node t_nil;
};

T_tree create_tree();
T_node t_search(T_tree tree, int k);
T_node t_insert(T_tree tree, int k);

T_node t_maximum(T_tree tree);
T_node t_minimum(T_tree tree);
T_node t_predecessor(T_tree tree, T_node node);
T_node t_successor(T_tree tree, T_node node);

void t_transplant(T_tree tree, T_node v, T_node u);
void t_left_rotate(T_tree tree, T_node x);
void t_right_rotate(T_tree tree, T_node x);

void t_delete(T_tree tree, T_node* node);
void t_tree_free(T_tree* tree);

T_node rbt_insert(T_tree tree, int k);

void print_tree_preorder(T_tree tree);
void print_tree_inorder(T_tree tree);

#endif
