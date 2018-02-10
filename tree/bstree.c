#include "bstree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ERR_EXIT(x) \
    do { perror(x); exit(1); } while (0) 

T_node _t_maximum(T_node p, T_node tnil);
T_node _t_minimum(T_node p, T_node tnil);

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
    return tree;
}

void
t_tree_free(T_tree* t_tree_p)
{
    T_tree tree = *t_tree_p;
    if (tree) {
    //
        free(tree);
        *t_tree_p = NULL;
    }
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
    if (node && node != tree->t_nil) {    
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
t_maximum(T_tree tree)
{
    T_node p = tree->root;
    return _t_maximum(p, tree->t_nil);
}

T_node
t_minimum(T_tree tree)
{
    T_node p = tree->root;
    return _t_minimum(p, tree->t_nil);
}

void
t_left_rotate(T_tree tree, T_node node)
{
    if (node && node != tree->t_nil) {
        T_node right = node->right;
        
        if (right == tree->t_nil)
            return;
        
        node->right = right->left;
        if (right->left != tree->t_nil)
            right->left->p = node;
        
        right->p = node->p;
        if (node->p == tree->t_nil) {
            tree->root = right;
        } else if (node == node->p->right) {
            node->p->right = right;
        } else {
            node->p->left = right;
        }
        node->p = right;
        right->left = node;
    }
}

void
t_right_rotate(T_tree tree, T_node node)
{
    if (node && node != tree->t_nil) {
        T_node left = node->left;
        if (left == tree->t_nil)
            return;

        node->left = left->right;
        if(left->right != tree->t_nil)
            left->right->p = node;

        left->p = node->p;
        if (node->p == tree->t_nil) {
            tree->root = left;
        } else if (node == node->p->right) {
            node->p->right = left;
        } else {
            node->p->left = left;
        }
        node->p = left;
        left->right = node;
    }
}


void
_print_tree_preorder(T_node node, T_node tnil)
{
    if (node != tnil) {
        printf("%d\n", node->v);
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

T_node
_t_maximum(T_node p, T_node tnil)
{
    while (p->right != tnil)
        p = p->right;
    return p;
}

T_node
_t_minimum(T_node p, T_node tnil)
{
    while (p->left != tnil)
        p = p->left;
    return p;
}

