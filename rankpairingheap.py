class Node:
    def __init__(self, value):
        self.value = value
        self.child = None
        self.next_sibling = None
        self.rank = 0


class PairingHeap:
    def __init__(self):
        self.root = None
    def insert(self, value):
        nod_nou = Node(value)
        self.root = self._merge(self.root, nod_nou)

    def merge(self, other_heap):
        self.root = self._merge(self.root, other_heap.root)

    def find_max(self):
        if self.root is None:
            return None
        nod_maxim = self.root
        nod_curent = nod_maxim.next_sibling
        while nod_curent is not None:
            if nod_curent.value > nod_maxim.value:
                nod_maxim = nod_curent
            nod_curent = nod_curent.next_sibling
        return nod_maxim.value

    def delete_max(self):
        if self.root is None:
            return None
        max_value = self.root.value
        if self.root.child is None:
            self.root = None
        else:
            root_nou = self._combine_pairs(self.root.child)
            self.root = root_nou
        return max_value

    def _merge(self, nod1, nod2):
        if nod1 is None:
            return nod2
        if nod2 is None:
            return nod1
        if nod1.value > nod2.value:
            nod2.next_sibling = nod1.child
            nod1.child = nod2
            return nod1
        else:
            nod1.next_sibling = nod2.child
            nod2.child = nod1
            return nod2

    def _combine_pairs(self, nod_prim):
        if nod_prim is None or nod_prim.next_sibling is None:
            return nod_prim
        else:
            a = nod_prim
            b = nod_prim.next_sibling
            c = b.next_sibling
            a.next_sibling = None
            b.next_sibling = None
            c = self._combine_pairs(c)
            return self._merge(self._merge(a, b), c)

heap = {}
with open("mergeheap.in", "r") as f:
    n, q = map(int, f.readline().split())
    for i in range(q):
        line = f.readline().split()
        if line[0] == "1":
            if line[1] not in heap:
                heap[line[1]] = PairingHeap()
            heap[line[1]].insert(int(line[2]))
        elif line[0] == "2":
            with open("mergeheap.out", "a") as g:
                g.write(str(heap[line[1]].find_max()) + '\n')
                heap[line[1]].delete_max()
        elif line[0] == "3":
            if line[1] not in heap:
                heap[line[1]] = PairingHeap()
            heap[line[1]].merge(heap[line[2]])