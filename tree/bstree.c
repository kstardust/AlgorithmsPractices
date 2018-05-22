#include "bstree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LEFT  1
#define RIGHT 2

#define ERR_EXIT(x) \
    do { perror(x); exit(1); } while (0) 

T_node _t_maximum(T_node p, T_node tnil);
T_node _t_minimum(T_node p, T_node tnil);
int _node_height(T_node node, T_node nil);

void rbt_insert_fix(T_tree tree, T_node node);
void rbt_delete_fix(T_tree tree, T_node node);
void avl_insert_fix(T_tree tree, T_node node);
void avl_delete_fix(T_tree tree, T_node node, int position);


void*
t_malloc(size_t size)
{
    void *mem = malloc(size);
    if (!mem) {
        ERR_EXIT("cannot malloc");
    }
    return mem;
}

T_tree
create_tree(int type)
{
    T_tree tree = (T_tree)t_malloc(sizeof(struct t_tree));
    T_node nil_node = (T_node)t_malloc(sizeof(struct t_node));
    bzero(nil_node, sizeof(struct t_node));
    tree->t_nil = nil_node;
    tree->root = nil_node;
    tree->type = type;
    if (type == TYPE_RB_TREE)
        tree->t_nil->rbt_color = RBT_COLOR_BLACK;
    else if (type == TYPE_AVL_TREE)
        tree->t_nil->avl_bf = AVL_BALANCED;
    
    return tree;
}

T_node
t_insert(T_tree tree, int k)
{
    switch (tree->type) {
    case TYPE_RB_TREE:
        return rbt_insert(tree, k);
    case TYPE_AVL_TREE:
        return avl_insert(tree, k);
    default:
        return bst_insert(tree, k);
    }
}

void
t_delete(T_tree tree, T_node* nodep)
{
    switch (tree->type) {
    case TYPE_RB_TREE:
        rbt_delete(tree, nodep);
        break;
    case TYPE_AVL_TREE:
        avl_delete(tree, nodep);
        break;
    default:
        bst_delete(tree, nodep);
        break;
    }
}
    
void
t_tree_free(T_tree* tree)
{
    //
    *tree = NULL;
}

T_node
t_search(T_tree tree, int k)
{
    T_node p = tree->root;
    while ( p != tree->t_nil && p->v != k ) {
        if (p->v > k)
            p = p->left;
        else
            p = p->right;
    }
    return p;
}

T_node
bst_insert(T_tree tree, int k)
{
    T_node node = (T_node)t_malloc(sizeof(struct t_node));
    node->v = k;
    node->left = tree->t_nil;
    node->right = tree->t_nil;
    node->p = tree->t_nil;

    T_node *p = &tree->root;
    T_node parent = *p;
    while (*p != tree->t_nil) {
        parent = *p;
        if ((*p)->v > node->v) {
            p = &(*p)->left;
        } else {
            p = &(*p)->right;
        }
    }
    node->p = parent;
    *p = node;
    return node;
}

T_node
t_predecessor(T_tree tree, T_node node)
{
    if (node->left != tree->t_nil) {
        return _t_maximum(node->left, tree->t_nil);
    }
    T_node x = node;
    T_node y = node->p;
    while (y != tree->t_nil && x == y->left) {
        x = y;
        y = y->p;
    }
    return y;
}

T_node
t_successor(T_tree tree, T_node node)
{
    if (node->right != tree->t_nil) {
        return _t_minimum(node->right, tree->t_nil);
    }
    T_node x = node;
    T_node y = node->p;
    while (y != tree->t_nil && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

void
t_transplant(T_tree tree, T_node u, T_node v)
{
    if (u->p == tree->t_nil) {
        tree->root = v;
    } else if (u == u->p->left) {
        u->p->left = v;
    } else {
        u->p->right = v;
    }
    if (v) {
        v->p = u->p;
    }
}

void
bst_delete(T_tree tree, T_node* nodep)
{
    T_node node = *nodep;
    if (node) {    
        if (node->left == tree->t_nil) {
            t_transplant(tree, node, node->right);
        } else if (node->right == tree->t_nil) {
            t_transplant(tree, node, node->left);
        } else {
            T_node suc = _t_minimum(node->right, tree->t_nil);
            if (suc->p != node) {
                t_transplant(tree, suc, suc->right);
                suc->right = node->right;
                suc->right->p = suc;
            }
            t_transplant(tree, node, suc);
            suc->left = node->left;
            node->left->p = suc;
        }
        free(node);
        *nodep = NULL;
    }
}

T_node
_t_maximum(T_node p, T_node tnil)
{
    while (p->right != tnil)
        p = p->right;
    return p;
}

T_node
t_maximum(T_tree tree)
{
    T_node p = tree->root;
    return _t_maximum(p, tree->t_nil);
}

T_node
_t_minimum(T_node p, T_node tnil)
{
    while (p->left != tnil)
        p = p->left;
    return p;
}

T_node
t_minimum(T_tree tree)
{
    T_node p = tree->root;
    return _t_minimum(p, tree->t_nil);
}

T_node
t_left_rotate(T_tree tree, T_node x)
{
    T_node y = x->right;
    if ( y == tree->t_nil)
        return x;
    
    x->right = y->left;
    if (y->left != tree->t_nil) {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == tree->t_nil) {
        tree->root = y;
    } else if (x == x->p->left) {
        x->p->left = y;
    } else {
        x->p->right = y;
    }
    y->left = x;
    x->p = y;

    return y;
}

T_node
t_right_rotate(T_tree tree, T_node x)
{
    T_node y = x->left;
    if ( y == tree->t_nil)
        return x;
    
    x->left = y->right;
    if (y->right != tree->t_nil) {
        y->right->p = x;
    }
    y->p = x->p;
    if (x->p == tree->t_nil) {
        tree->root = y;
    } else if (x == x->p->left) {
        x->p->left = y;
    } else {
        x->p->right = y;
    }
    y->right = x;
    x->p = y;

    return y;
}    

T_node
rbt_insert(T_tree tree, int k)
{
    T_node node = bst_insert(tree, k);
    node->rbt_color = RBT_COLOR_RED;
    rbt_insert_fix(tree, node);
    return node;
}

void
rbt_insert_fix(T_tree tree, T_node node)
{
    while (node->p->rbt_color == RBT_COLOR_RED) {
        if (node->p == node->p->p->left) {
            T_node sibling = node->p->p->right;
            if (sibling->rbt_color == RBT_COLOR_RED) {
                node->p->rbt_color = RBT_COLOR_BLACK;
                sibling->rbt_color = RBT_COLOR_BLACK;
                node->p->p->rbt_color = RBT_COLOR_RED;
                node = node->p->p;
            } else {
                if (node == node->p->right) {
                    node = node->p;
                    t_left_rotate(tree, node);
                }
                node->p->rbt_color = RBT_COLOR_BLACK;
                node->p->p->rbt_color = RBT_COLOR_RED;
                t_right_rotate(tree, node->p->p);
            }
        } else {
            if (node->p == node->p->p->right) {
                T_node sibling = node->p->p->left;
                if (sibling->rbt_color == RBT_COLOR_RED) {
                    node->p->rbt_color = RBT_COLOR_BLACK;
                    sibling->rbt_color = RBT_COLOR_BLACK;
                    node->p->p->rbt_color = RBT_COLOR_RED;
                    node = node->p->p;
                } else {
                    if (node == node->p->left) {
                        node = node->p;
                        t_right_rotate(tree, node);
                    }
                    node->p->rbt_color    = RBT_COLOR_BLACK;
                    node->p->p->rbt_color = RBT_COLOR_RED;
                    t_left_rotate(tree, node->p->p);
                }
            }
        }
    }
    tree->root->rbt_color = RBT_COLOR_BLACK;
}

void
rbt_delete(T_tree tree, T_node* nodep)
{
    T_node node = *nodep;
    T_node x, y;
    y = node;
    int y_original_color = y->rbt_color;
    
    if (node) {
        if (node->left == tree->t_nil) {
            x = node->right;
            t_transplant(tree, node, node->right);
        } else if (node->right == tree->t_nil) {
            x = node->left;
            t_transplant(tree, node, node->left);
        } else {
            y = _t_minimum(node->right, tree->t_nil);
            y_original_color = y->rbt_color;
            x = y->right;
            if (y->p == node) {
                x->p = y;
            } else {
                t_transplant(tree, y, y->right);
                y->right = node->right;
                y->right->p = y;
            }
            t_transplant(tree, node, y);
            y->left = node->left;
            y->left->p = y;
            y->rbt_color = node->rbt_color;
        }
        if (y_original_color == RBT_COLOR_BLACK) {
            rbt_delete_fix(tree, x);
        }
        free(node);
        *nodep = NULL;
    }
}

void
rbt_delete_fix(T_tree tree, T_node node)
{
    while (node != tree->root && node->rbt_color == RBT_COLOR_BLACK) {
        if (node == node->p->left) {
            T_node sibling = node->p->right;
            if (sibling->rbt_color == RBT_COLOR_RED) {
                sibling->rbt_color = RBT_COLOR_BLACK;
                node->p->rbt_color = RBT_COLOR_RED;
                t_left_rotate(tree, node->p);
                sibling = node->p->right;
            }
            if (sibling->right->rbt_color == RBT_COLOR_BLACK &&
                sibling->left->rbt_color == RBT_COLOR_BLACK) {
                sibling->rbt_color = RBT_COLOR_RED;
                node = node->p;
            } else {
                if (sibling->right->rbt_color == RBT_COLOR_BLACK) {
                    sibling->rbt_color       = RBT_COLOR_RED;
                    sibling->left->rbt_color = RBT_COLOR_BLACK;
                    t_right_rotate(tree, sibling);
                    sibling = node->p->right;
                }
                sibling->rbt_color        = node->p->rbt_color;
                sibling->right->rbt_color = RBT_COLOR_BLACK;
                node->p->rbt_color        = RBT_COLOR_BLACK;
                t_left_rotate(tree, node->p);
                node = tree->root;
            }
        } else {
            T_node sibling = node->p->left;
            if (sibling->rbt_color == RBT_COLOR_RED) {
                sibling->rbt_color = RBT_COLOR_BLACK;
                node->p->rbt_color = RBT_COLOR_RED;
                t_right_rotate(tree, node->p);
                sibling = node->p->left;
            }
            if (sibling->right->rbt_color == RBT_COLOR_BLACK &&
                sibling->left->rbt_color == RBT_COLOR_BLACK) {
                sibling->rbt_color = RBT_COLOR_RED;
                node = node->p;
            } else {
                if (sibling->left->rbt_color == RBT_COLOR_BLACK) {
                    sibling->rbt_color        = RBT_COLOR_RED;
                    sibling->right->rbt_color = RBT_COLOR_BLACK;
                    t_left_rotate(tree, sibling);
                    sibling = node->p->left;
                }
                sibling->rbt_color       = node->p->rbt_color;
                sibling->left->rbt_color = RBT_COLOR_BLACK;
                node->p->rbt_color       = RBT_COLOR_BLACK;
                t_right_rotate(tree, node->p);
                node = tree->root;
            }
        }
    }
    node->rbt_color = RBT_COLOR_BLACK;
}

T_node
avl_insert(T_tree tree, int k)
{
    T_node node = bst_insert(tree, k);
    node->avl_bf = AVL_BALANCED;
    avl_insert_fix(tree, node);        
    return node;
}

void
avl_delete(T_tree tree, T_node* nodep)
{
    T_node node = *nodep;
    T_node action_node = node->p;

    /* The action position is a reference to the parent node from 
       which a node has been physically removed

       The action position indicate the first node whose height has 
       been affected (possibly changed) by the deletion 
    */
    
    int position = 0;
    if (node == node->p->right) {
        position = RIGHT;
    } else if (node == node->p->left) {
        position = LEFT;
    }
    if (node) {
        if (node->left == tree->t_nil) {
            t_transplant(tree, node, node->right);
        } else if (node->right == tree->t_nil) {
            t_transplant(tree, node, node->left);
        } else {
            T_node suc = _t_minimum(node->right, tree->t_nil);
            action_node = suc;
            if (suc == suc->p->right) {
                position = RIGHT;
            } else if (suc == suc->p->left) {
                position = LEFT;
            }
            if (suc->p != node) {
                action_node = suc->p;
                t_transplant(tree, suc, suc->right);
                suc->right = node->right;
                suc->right->p = suc;
            }
            t_transplant(tree, node, suc);
            suc->avl_bf = node->avl_bf;
            suc->left = node->left;
            node->left->p = suc;
        }
        free(node);
        *nodep = NULL;
        avl_delete_fix(tree, action_node, position);
    }
}

void
avl_insert_fix(T_tree tree, T_node node)
{
    for (T_node x = node->p; x != tree->t_nil; x = node->p) {
        if (node == x->right) {
            if (x->avl_bf == AVL_RIGHT_HEAVY) {
                if (node->avl_bf == AVL_LEFT_HEAVY) {
                    T_node y = node->left;
                    t_right_rotate(tree, node);
                    t_left_rotate(tree, x);
                    
                    if (y->avl_bf == AVL_LEFT_HEAVY) {
                        x->avl_bf = AVL_BALANCED;
                        node->avl_bf = AVL_RIGHT_HEAVY;
                    } else if (y->avl_bf == AVL_RIGHT_HEAVY) {
                        x->avl_bf = AVL_LEFT_HEAVY;
                        node->avl_bf = AVL_BALANCED;
                    } else {
                        x->avl_bf = AVL_BALANCED;
                        node->avl_bf = AVL_BALANCED;
                    }
                    
                    y->avl_bf = AVL_BALANCED;
                } else {
                    t_left_rotate(tree, x);
                    x->avl_bf = AVL_BALANCED;
                    node->avl_bf = AVL_BALANCED;
                }
                break;
            } else {
                if (x->avl_bf == AVL_LEFT_HEAVY) {
                    x->avl_bf = AVL_BALANCED;
                    break;
                }
                x->avl_bf = AVL_RIGHT_HEAVY;
                node = x;
            }
        } else {
            if (x->avl_bf == AVL_LEFT_HEAVY) {
                if (node->avl_bf == AVL_RIGHT_HEAVY) {
                    T_node y = node->right;
                    t_left_rotate(tree, node);
                    t_right_rotate(tree, x);
                    
                    if (y->avl_bf == AVL_RIGHT_HEAVY) {
                        x->avl_bf = AVL_BALANCED;
                        node->avl_bf = AVL_LEFT_HEAVY;
                    } else if (y->avl_bf == AVL_LEFT_HEAVY) {
                        x->avl_bf = AVL_RIGHT_HEAVY;
                        node->avl_bf = AVL_BALANCED;
                    } else {
                        x->avl_bf = AVL_BALANCED;
                        node->avl_bf = AVL_BALANCED;
                    }
                    
                    y->avl_bf = AVL_BALANCED;
                    
                } else {
                    t_right_rotate(tree, x);
                    x->avl_bf = AVL_BALANCED;
                    node->avl_bf = AVL_BALANCED;
                }
                break;
            } else {
                if (x->avl_bf == AVL_RIGHT_HEAVY) {
                    x->avl_bf = AVL_BALANCED;
                    break;
                }
                x->avl_bf = AVL_LEFT_HEAVY;
                node = x;
            }
        }
    }
}

void
avl_delete_fix(T_tree tree, T_node node, int position)
{
    T_node x = node;
    if (position == RIGHT) {
        node = x->right;
    } else if (position == LEFT) {
        node = x->left;
    }
    T_node p;
    for (; x != tree->t_nil; x = p) {
        p = x->p;
        /* 
           The right and left child of a leaf node(nodes have no child) are
           the same(both equal to tree->t_nil). so we use position for the 
           first navigation.
        */
        if (position == LEFT || (node == x->left && position == 0)) {
            if (x->avl_bf == AVL_RIGHT_HEAVY) {
                T_node sibling = x->right;
                
                if (sibling->avl_bf == AVL_LEFT_HEAVY) {
                    T_node y = sibling->left;
                    t_right_rotate(tree, sibling);
                    node = t_left_rotate(tree, x);
                    
                    if (y->avl_bf == AVL_BALANCED) {
                        x->avl_bf = AVL_BALANCED;
                        sibling->avl_bf = AVL_BALANCED;
                    } else if (y->avl_bf == AVL_LEFT_HEAVY) {
                        x->avl_bf = AVL_BALANCED;
                        sibling->avl_bf = AVL_RIGHT_HEAVY;
                    } else if (y->avl_bf == AVL_RIGHT_HEAVY) {
                        x->avl_bf = AVL_LEFT_HEAVY;
                        sibling->avl_bf = AVL_BALANCED;
                    }
                    y->avl_bf = AVL_BALANCED;
                } else {
                    node = t_left_rotate(tree, x);
                    if (sibling->avl_bf == AVL_BALANCED) {
                        sibling->avl_bf = AVL_LEFT_HEAVY;
                        x->avl_bf = AVL_RIGHT_HEAVY;
                        break;
                    } else {
                        sibling->avl_bf = AVL_BALANCED;
                        x->avl_bf = AVL_BALANCED;
                    }
                }
            } else {
                if (x->avl_bf == AVL_BALANCED) {
                    x->avl_bf = AVL_RIGHT_HEAVY;
                    break;
                }
                x->avl_bf = AVL_BALANCED;
                node = x;
            }
        } else {
            if (x->avl_bf == AVL_LEFT_HEAVY) {
                T_node sibling = x->left;                
                if (sibling->avl_bf == AVL_RIGHT_HEAVY) {
                    T_node y = sibling->right;
                    t_left_rotate(tree, sibling);
                    node = t_right_rotate(tree, x);
                    
                    if (y->avl_bf == AVL_BALANCED) {
                        x->avl_bf = AVL_BALANCED;
                        sibling->avl_bf = AVL_BALANCED;
                    } else if (y->avl_bf == AVL_LEFT_HEAVY) {
                        x->avl_bf = AVL_RIGHT_HEAVY;
                        sibling->avl_bf = AVL_BALANCED;
                    } else if (y->avl_bf == AVL_RIGHT_HEAVY) {
                        x->avl_bf = AVL_BALANCED;
                        sibling->avl_bf = AVL_LEFT_HEAVY;
                    }
                    y->avl_bf = AVL_BALANCED;
                } else {
                    node = t_right_rotate(tree, x);
                    if (sibling->avl_bf == AVL_BALANCED) {
                        sibling->avl_bf = AVL_RIGHT_HEAVY;
                        x->avl_bf = AVL_LEFT_HEAVY;
                        break;
                    } else {
                        sibling->avl_bf = AVL_BALANCED;
                        x->avl_bf = AVL_BALANCED;
                    }
                }
            } else {
                if (x->avl_bf == AVL_BALANCED) {
                    x->avl_bf = AVL_LEFT_HEAVY;
                    break;
                }
                x->avl_bf = AVL_BALANCED;
                node = x;
            }
        }
        position = 0;
    }
}

char* color_helper(int color)
{
    switch(color) {
    case RBT_COLOR_BLACK:
        return "black";
    case RBT_COLOR_RED:
        return "red";
    case AVL_BALANCED:
        return "balanced";
    case AVL_LEFT_HEAVY:
        return "left heavy";
    case AVL_RIGHT_HEAVY:
        return "right heavy";
    default:
        return "undefined";
    }
}

void
_print_tree_preorder(T_node node, T_node tnil)
{
    if (node != tnil) {
        printf("%d color: %s\n",
               node->v, color_helper(node->rbt_color));
        _print_tree_preorder(node->left, tnil);
        _print_tree_preorder(node->right, tnil);
    }
}

void
print_tree_preorder(T_tree tree)
{
    T_node node = tree->root;
    _print_tree_preorder(node, tree->t_nil);
}

void
_print_tree_inorder(T_node node, T_node tnil)
{
    if (node != tnil) {
        _print_tree_inorder(node->left, tnil);
        printf("%d color: %s\n",
               node->v, color_helper(node->rbt_color));
        _print_tree_inorder(node->right, tnil);
    }
}

void
print_tree_inorder(T_tree tree)
{
    T_node node = tree->root;
    _print_tree_inorder(node, tree->t_nil);
}

int
_node_height(T_node node, T_node nil)
{
    int depth = 0;
    if (node != nil) {
        int ld = 1 + _node_height(node->left, nil);
        int rd = 1 + _node_height(node->right, nil);
        depth = (ld > rd ? ld : rd);
    }
    return depth;
}

int
tree_height(T_tree tree)
{
    return _node_height(tree->root, tree->t_nil);
}
