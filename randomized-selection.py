import numpy as np

def randomized_partition(A, p, r):
    random_index = np.random.randint(p, r+1)
    A[r], A[random_index] = A[random_index], A[r]
    pivot = A[r]
    j = p - 1
    for i in range(p, r):
        if A[i] <= pivot:
            j += 1
            A[i], A[j] = A[j], A[i]
    j += 1
    A[j], A[r] = A[r], A[j]
    return j

def randomized_selection(A, p, r, i):
    q = randomized_partition(A, p, r)
    k = q - p + 1
    if k == i:
        return A[q]
    elif k > i:
        return randomized_selection(A, p, q-1, i)
    else:
        return randomized_selection(A, q+1, r, i-k)

n = 20
arra = np.random.randint(0, 1000, (n,))
arra = arra.tolist()
i = np.random.randint(0, n) + 1
print(i, arra)
print(randomized_selection(arra, 0, n-1, i))  
print(sorted(arra)[i-1])
