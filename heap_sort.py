import numpy as np
import math

def parent(i):
    return math.floor((i+1)/2) - 1

def left(i):
    return i*2 + 1

def right(i):
    return i*2 + 2

def build_max_heap(heap):
    hsize = len(heap)
    for i in range(math.floor(hsize/2)-1, -1, -1):
        max_heapify(heap, i)

def max_heapify(heap, i):
    hsize = len(heap)
    while True:
        l = left(i)
        r = right(i)
        largest = i
        if l < hsize and heap[l] > heap[i]:
            largest = l
        if r < hsize and heap[r] > heap[largest]:
            largest = r
        if largest != i:
            heap[i], heap[largest] = heap[largest], heap[i]
            i = largest
        else: return
        
def heap_sort(heap):
    build_max_heap(heap)
    hsize = len(heap)
    for i in range(hsize-1, 0, -1):
          heap[i], heap[0] = heap[0], heap[i]
          max_heapify(heap[:i], 0)
    
random_array = np.random.randint(9999, size=(10,))
heap_sort(random_array)
print(random_array)
