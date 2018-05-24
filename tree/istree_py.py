import ctypes
import os
import random


_file = 'libistree.so'
# _path = os.path.join(*(os.path.split(__file__)[:-1] + (_file,)))
_path = os.path.join('.', _file)
_mod = ctypes.cdll.LoadLibrary(_path)

TYPE_BS_TREE = 0x01
TYPE_RB_TREE = 0x02
TYPE_AVL_TREE = 0x04

RBT_COLOR_BLACK = 0x08
RBT_COLOR_RED = 0x10

AVL_BALANCED = 0
AVL_LEFT_HEAVY = -1
AVL_RIGHT_HEAVY = 1


class rbt_data(ctypes.Structure):
    _fields_ = [('data', ctypes.c_void_p),
                ('color', ctypes.c_int)]


class t_node(ctypes.Structure):
    pass


t_node._fields_ = [('p', ctypes.POINTER(t_node)),
                   ('left', ctypes.POINTER(t_node)),
                   ('right', ctypes.POINTER(t_node)),
                   ('v', ctypes.c_int),
                   ('_data', rbt_data)]


class t_tree(ctypes.Structure):
    _fields_ = [('root', ctypes.POINTER(t_node)),
                ('t_nil', ctypes.POINTER(t_node)),
                ('type', ctypes.c_uint8)]


_create_tree = _mod.create_tree
_create_tree.argtypes = (ctypes.c_int,)
_create_tree.restype = ctypes.POINTER(t_tree)


def create_tree(type):
    return _create_tree(type)


_t_insert = _mod.t_insert
_t_insert.argtypes = (ctypes.POINTER(t_tree), ctypes.c_int)
_t_insert.restype = ctypes.POINTER(t_node)


def t_insert(tree, value):
    return _t_insert(tree, value)


_t_search = _mod.t_search
_t_search.argtypes = (ctypes.POINTER(t_tree), ctypes.c_int)
_t_search.restype = ctypes.POINTER(t_node)


def t_search(tree, value):
    return _t_search(tree, value)


_t_delete = _mod.t_delete
_t_delete.argtypes = (ctypes.POINTER(t_tree), ctypes.POINTER(ctypes.POINTER(t_node)))


def t_delete(tree, node):
    _t_delete(tree, node)


_is_nil_node = _mod.is_nil_node
_is_nil_node.argtypes = (ctypes.POINTER(t_tree), ctypes.POINTER(t_node))
_is_nil_node.restype = ctypes.c_int


def is_nil_node(tree, node):
    return _is_nil_node(tree, node)


_print_tree_preorder = _mod.print_tree_preorder
_print_tree_preorder.argtypes = (ctypes.POINTER(t_tree),)


def print_tree_preorder(tree):
    _print_tree_preorder(tree)


_print_tree_inorder = _mod.print_tree_inorder
_print_tree_inorder.argtypes = (ctypes.POINTER(t_tree),)


def print_tree_inorder(tree):
    _print_tree_inorder(tree)
