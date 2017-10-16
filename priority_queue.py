import math
import numpy as np

def parent(i):
    return (i+1)//2 - 1

def left(i):
    return 2*(i+1) - 1

def right(i):
    return 2*(i+1)

def max_heapify(heap, i):
    hsize = len(heap)
    while True:
        r = right(i)
        l = left(i)    
        largest = i
        if r < hsize and heap[r] >= heap[largest]:
            largest = r
        if l < hsize and heap[l] >= heap[largest]:
            largest = l
        if largest != i:
            heap[largest], heap[i] = heap[i], heap[largest]
            i = largest
        else: return 

def build_max_heap(heap):
    hsize = len(heap)
    for i in range(hsize//2 - 1, -1, -1):
        max_heapify(heap, i)

def heap_maximum(heap):
    return heap[0]

def heap_extract_max(heap):
    hsize = len(heap)
    if hsize < 1:
        raise IndexError("heap underflow")
    max_ = heap_maximum(heap)
    heap[0] = heap[-1]
    heap.pop()
    max_heapify(heap, 0)
    return max_

def heap_increase_key(heap, i, key):
    if key < heap[i]:
        raise ValueError("new key must greater than current key")
    heap[i] = key
    while i > 0 and key > heap[parent(i)]:
        heap[i], heap[parent(i)] = heap[parent(i)], heap[i]
        i = parent(i)

def max_heap_insert(heap, n):
    heap += [-math.inf,]
    heap_increase_key(heap, len(heap)-1, n)

random_array = np.random.randint(9999, size=(10,)).tolist()
build_max_heap(random_array)
print(random_array)
max_heap_insert(random_array, 8888)
print(random_array)


