def kmp_table(w):
    """
    https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm
    -1: idx_s+1
    0: idx_s+0    
    """
    ptable = [-1]
    i = 0     
    j = 1   # current position in w (first one is already set to -1)
    while j < len(w):        
        if w[i] == w[j]:
            ptable.append(ptable[i])
        else:
            ptable.append(i) # matches so far
            while i >= 0 and w[i] != w[j]:
                # find smaller prefix
                i = ptable[i]
        i += 1
        j += 1
    return ptable

def kmp(s, w):
    ptable = kmp_table(w)
    i = 0 # idx_s
    j = 0 # idx_w
    length_w = len(w)
    while i < len(s):        
        if s[i] == w[j]:            
            if j == length_w-1:
                return (i-j)
            i += 1
            j += 1
        else:            
            j = ptable[j]
            if j < 0:                
                i += 1 # -1 means we need to move to the next char in s
                j += 1 # set to zero
    return None


assert kmp_table("ABCDABD") == [-1,0,0,0,-1,0,2]
assert kmp_table("ABACABABC") == [-1,0,-1,1,-1,0,-1,3,2]
assert kmp_table("ABACABABA") == [-1,0,-1,1,-1,0,-1,3,-1]
print(kmp_table("ABABABCABABABD"))
print(kmp_table("ABABABCABABABCD"))
print(kmp("ABABABCABABABCD", "BAB"))

    