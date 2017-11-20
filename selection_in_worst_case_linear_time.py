import numpy as np
from insection_sort import insection_sort
import heapq

_GROUP_SIZE = 5

def lower_mid(n):
    if n < 0: return 0
    return (n+1)//2

def _select(A, n):
    global _GROUP_SIZE
    length = len(A)
    
    if length < 1:
        return A
    
    groups = length // _GROUP_SIZE
    last_group_size = length % _GROUP_SIZE
    medians = []
    m_counter = 0
    
    for i in range(groups):
        r = insection_sort(A[i*_GROUP_SIZE: (i+1)*_GROUP_SIZE])
        medians.append(r[lower_mid(_GROUP_SIZE) - 1])
        m_counter += 1
        
    if last_group_size != 0:
        begin = length-last_group_size
        r = insection_sort(A[begin:])
        medians.append(r[lower_mid(last_group_size) - 1])
        m_counter += 1

    _select(medians, lower_mid(m_counter))
    
def select(A, n):
    return _select(A, n)


arra = np.random.randint(0, 1000, (13))

arra = list(set(arra))
n = np.random.randint(0, len(arra))
#print(heapq.nlargest(n, arra)[-1])
print(arra)
print(select(arra, 0))
