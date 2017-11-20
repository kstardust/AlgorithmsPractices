import numpy as np

def insection_sort(A):
    length = len(A)
    if length:
        for i in range(1, length):
            k = A[i]
            j = i - 1
            while j >= 0 and k < A[j]:
                A[j+1] = A[j]
                j -= 1
            A[j+1] = k
    return A

if __name__ == '__main__':
    arra = np.random.randint(0, 1000, 100)
    insection_sort(arra)
    print(arra)

