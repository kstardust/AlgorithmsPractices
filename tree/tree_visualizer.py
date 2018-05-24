import cv2
import numpy as np
import istree_py


class Config(object):
    node_size = 30
    interspace = 10


class Node(object):
    def __init__(self, v, x=0, y=0, color='b'):
        self.left = None
        self.right = None
        self.v = v
        self.p = None
        self.x = x
        self.y = y
        self.color = color

    @property
    def center(self):
        return (int(self.x), int(self.y))


class TreeVisualizer(object):
    def __init__(self, tree):
        self.tree = tree
        depth = self.tree_height(self.tree)
        nodes = 2**depth
        self.width = nodes*Config.node_size + Config.interspace*(nodes+1)
        self.height = depth*Config.node_size + Config.interspace*(depth+2)
        self.img = np.ones([self.height, self.width, 3], dtype='float32')

    def draw(self):
        self._draw(self.tree)

    def _draw(self, t):
        if t is not None:
            if t.p is None:
                t.x = self.width/2
                t.y = Config.interspace + Config.node_size

            if t.left is not None:
                if t.p is None:
                    px = self.width
                else:
                    px = t.p.x
                offset = abs(t.x - px)
                t.left.x = t.x - offset/2
                t.left.y = t.y + Config.interspace + Config.node_size
                cv2.line(self.img, t.center, t.left.center, (255, 0, 0), 2, cv2.LINE_AA)

            if t.right is not None:
                if t.p is None:
                    px = self.width
                else:
                    px = t.p.x
                offset = abs(t.x - px)
                t.right.x = t.x + offset/2
                t.right.y = t.y + Config.interspace + Config.node_size
                cv2.line(self.img, t.center, t.right.center, (255, 0, 0), 2, cv2.LINE_AA)

            thickness = 1
            fontFace = cv2.FONT_HERSHEY_SIMPLEX
            fontScale = 0.5
            if t.color == 'r':
                color = (0, 0, 255)
            else:
                color = (0, 0, 0)

            img = cv2.circle(self.img, t.center, int(Config.node_size/2),
                             color, -1, cv2.LINE_8, 0)
            box, baseLine = cv2.getTextSize(str(t.v), fontFace, fontScale, thickness)
            org = (int(t.x-box[0]/2), int(t.y+box[1]/2))
            img = cv2.putText(self.img, str(t.v), org,
                              fontFace, fontScale, (0, 255, 0), thickness, cv2.LINE_AA)
            self._draw(t.left)
            self._draw(t.right)

    def show(self):
        cv2.namedWindow("Tree", cv2.WINDOW_NORMAL)
        img = self.img
        cv2.imshow("Tree", img)
        img = cv2.cvtColor(self.img, cv2.COLOR_BGR2RGB)
#        cv2.imwrite("test.png", img)
        cv2.waitKey(0)

    @classmethod
    def tree_height(cls, node):
        if node is None:
            return 0
        lh = 1 + cls.tree_height(node.left)
        rh = 1 + cls.tree_height(node.right)
        return lh if lh > rh else rh


def _build_tree(preorder_it, pstart, pend,
                inorder_it, istart, iend, pnode, preorder_color):
    if pstart > pend or istart > iend:
        return None
    node_v = preorder_it[pstart]
    node = Node(node_v)

    if preorder_color is not None:
        node.color = preorder_color[pstart]
    node.p = pnode

    for index in range(istart, iend+1):
        if node_v == inorder_it[index]:
            break

    node.left = _build_tree(preorder_it, pstart+1, pstart+(index-istart),
                             inorder_it, istart, index-1, node, preorder_color)
    node.right = _build_tree(preorder_it, pstart+(index-istart)+1, pend,
                              inorder_it, index+1, iend, node, preorder_color)

    return node


def build_tree(preorder_it, inorder_it, preorder_color):
    return _build_tree(preorder_it, 0, len(preorder_it)-1,
                       inorder_it, 0, len(preorder_it)-1, None, preorder_color)


def _build_from_istree(tree, root, node):
    left = root.contents.left
    right = root.contents.right

    if not istree_py.is_nil_node(tree, left):
        left_node = Node(left.contents.v)
        color = left.contents._data.color
        left_node.color = 'r' if \
                          color == istree_py.RBT_COLOR_RED else 'b'
        left_node.p = root
        _build_from_istree(tree, left, left_node)
        node.left = left_node
    if not istree_py.is_nil_node(tree, right):
        right_node = Node(right.contents.v)
        color = right.contents._data.color
        right_node.color = 'r' if \
                           color == istree_py.RBT_COLOR_RED else 'b'
        right_node.p = root
        _build_from_istree(tree, right, right_node)
        node.right = right_node


def build_from_istree(tree):
    root = tree.contents.root
    if istree_py.is_nil_node(tree, root):
        return None
    node = Node(root.contents.v)
    color = root.contents._data.color
    node.color = 'r' if color == istree_py.RBT_COLOR_RED else 'b'
    node.p = None
    _build_from_istree(tree, root, node)
    return node


def pre_order(node):
    if node is None:
        return
    print(node.v, node.color)
    pre_order(node.left)
    pre_order(node.right)


def in_order(node):
    if node is None:
        return
    in_order(node.left)
    print(node.v, node.color)
    in_order(node.right)


def draw_tree(preorder_it, inorder_it, preorder_color=None):
    tree = build_tree(preorder_it, inorder_it, preorder_color)
    panel = TreeVisualizer(tree)
    panel.draw()
    panel.show()

    
# PREORDER_it = [47780, 43433, 45065, 60825, 67992, 99752]
# inorder_it = [43433, 45065, 47780, 52098, 60825, 67992, 99752]
# preorder_color = ['b', 'b', 'r', 'r', 'b', 'b', 'r']
# draw_tree(preorder_it, inorder_it, preorder_color)
tree = istree_py.create_tree(istree_py.TYPE_AVL_TREE)
for _ in range(10):
    k = np.random.randint(0, 100)
    istree_py.t_insert(tree, k)
