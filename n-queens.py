import numpy as np

def _n_queens(res, m, n):
    counter = 0
    if m == n:
        counter += 1
    for j in range(n):
        c = 0
        for coord in res[0:m]:
            if j == coord[1] or abs(m-coord[0]) == abs(j-coord[1]):
                continue
            else:
                c += 1
        if c == m:
            res[m] = [m, j]
            counter += _n_queens(res, m+1, n)
    return counter

def n_queens(n):
    res = [None for n in range(n)]
    return _n_queens(res, 0, n)

print(n_queens(9))

