import math

class Node:
    def __init__(self, value):
        self.value = value
        self.parent = None
        self.children = []
        self.degree = 0
        self.marked = False

class BinomialHeap:
    def __init__(self):
        self.trees = []
        self.min_node = None
        self.count = 0

    def is_empty(self):
        return self.min_node is None

    def insert(self, value):
        node = Node(value)
        new_heap = BinomialHeap()
        new_heap.trees.append(node)
        self.merge(new_heap)

    def get_min(self):
        return self.min_node.value

    def extract_min(self):
        min_node = self.min_node
        self.trees.remove(min_node)
        self.merge(BinomialHeap(*min_node.children))
        self._find_min()
        self.count -= 1
        return min_node.value

    def merge(self, other_heap):
        self.trees.extend(other_heap.trees)
        self.count += other_heap.count
        self._find_min()

    def _find_min(self):
        self.min_node = None
        for tree in self.trees:
            if self.min_node is None or tree.value < self.min_node.value:
                self.min_node = tree

    def decrease_key(self, node, new_value):
        if new_value > node.value:
            raise ValueError
        node.value = new_value
        self._bubble_up(node)

    def delete(self, node):
        self.decrease_key(node, float('-inf'))
        self.extract_min()

    def _bubble_up(self, node):
        parent = node.parent
        while parent is not None and node.value < parent.value:
            node.value, parent.value = parent.value, node.value
            node, parent = parent, node

    def _link(self, tree1, tree2):
        if tree1.value > tree2.value:
            tree1, tree2 = tree2, tree1
        tree2.parent = tree1
        tree1.children.append(tree2)
        tree1.degree += 1
        
    def find_max(self):
        if not self.trees:
            raise ValueError("Heap-ul este gol")
        max_value = float('-inf')
        max_node = None
        for tree in self.trees:
            if tree.value > max_value:
                max_value = tree.value
                max_node = tree
        return max_node.value

    def extract_max(self):
        if not self.trees:
            raise ValueError("Heap-ul este gol")
        max_value = float('-inf')
        max_node = None
        for tree in self.trees:
            if tree.value > max_value:
                max_value = tree.value
                max_node = tree
        self.trees.remove(max_node)
        self.merge(BinomialHeap(*max_node.children))
        self._find_min()
        self.count -= 1
        return max_node.value

    def delete_max(self):
        max_value = self.extract_max()
        return max_value
    
    def _consolidate(self):
        max_degree = int(math.log(self.count, 2))
        degree_to_tree = [None] * (max_degree + 1)

        while self.trees:
            current = self.trees.pop(0)
            degree = current.degree
            while degree_to_tree[degree] is not None:
                other = degree_to_tree[degree]
                degree_to_tree[degree] = None
                if current.value < other.value:
                    self._link(current, other)
                else:
                    self._link(other, current)
                degree += 1
            degree_to_tree[degree] = current

        self.min_node = None
        self.trees = [tree for tree in degree_to_tree if tree is not None]

    def _len_(self):
        return self.count

heap = {}
with open("mergeheap.in", "r") as f:
    n, q = map(int, f.readline().split())
    for i in range(q):
        line = f.readline().split()
        if line[0] == "1":
            if line[1] not in heap:
                heap[line[1]] = BinomialHeap()
            heap[line[1]].insert(int(line[2]))
        elif line[0] == "2":
            with open("mergeheap.out", "a") as g:
                g.write(str(heap[line[1]].find_max()) + '\n')
                heap[line[1]].delete_max()
        elif line[0] == "3":
            if line[1] not in heap:
                heap[line[1]] = BinomialHeap()
            heap[line[1]].merge(heap[line[2]])