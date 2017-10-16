import numpy as np

def counting_sort(A):
    min_A = min(A)
    max_A = max(A)
    C = [0] * (max_A - min_A + 2)
    # C[0] is reserved for convenience.
    B = [0]*len(A)

    for i in range(len(A)):
        C[A[i]-min_A+1] += 1

    for i in range(1, len(C)):
        C[i] += C[i-1]
        
    for i in range(len(A)-1, -1, -1):
        # -1 because Python indices start from 0
        B[C[A[i]-min_A+1]-1] = A[i]
        C[A[i]-min_A+1] -= 1
        
    return B

random_array = np.random.randint(0, 30, size=(20,))
print(random_array)
print(counting_sort(random_array))
    
