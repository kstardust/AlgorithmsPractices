#include "bstree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ERR_EXIT(x) \
    do { perror(x); exit(1); } while (0) 

T_node _t_maximum(T_node p, T_node tnil);
T_node _t_minimum(T_node p, T_node tnil);
void rbt_insert_fix(T_tree tree, T_node node);
void rbt_delete_fix(T_tree tree, T_node node);

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
create_tree()
{
    T_tree tree = (T_tree)t_malloc(sizeof(struct t_tree));
    T_node nil_node = (T_node)t_malloc(sizeof(struct t_node));
    bzero(nil_node, sizeof(struct t_node));
    tree->t_nil = nil_node;
    tree->root = nil_node;
    tree->t_nil->rbt_color = RBT_COLOR_BLACK;
    return tree;
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
t_insert(T_tree tree, int k)
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
t_delete(T_tree tree, T_node* nodep)
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

void
t_left_rotate(T_tree tree, T_node x)
{
    T_node y = x->right;
    if ( y == tree->t_nil)
        return;
    
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
}

void
t_right_rotate(T_tree tree, T_node x)
{
    T_node y = x->left;
    if ( y == tree->t_nil)
        return;
    
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
}    

T_node
rbt_insert(T_tree tree, int k)
{
    T_node node = t_insert(tree, k);
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

char* color_helper(int color)
{
    switch(color) {
    case RBT_COLOR_BLACK:
        return "black";
    case RBT_COLOR_RED:
        return "red";
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

