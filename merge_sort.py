import numpy as np

def merge(array, b, m, e):
    left_size = m - b + 1
    right_size = e - m
    left = array[b:b+left_size]
    right = array[m+1:e+1]
    left_index = 0
    right_index = 0
    for i in range(b, e+1):
        if right_index >= right_size or left_index >= left_size:
            if left_index < left_size:
                array[i] = left[left_index]
                left_index += 1
            elif right_index < right_size:
                array[i] = right[right_index]
                right_index += 1
        elif left[left_index] < right[right_index]:
            array[i] = right[right_index]
            right_index += 1
        else:
            array[i] = left[left_index]
            left_index += 1
        
def merge_sort(array, b, e):
    if b < e:
        m = (e+b)//2
        merge_sort(array, b, m)
        print
        merge_sort(array, m+1, e)
        merge(array, b, m, e)

counter = 0
for i in range(1000):            
    random_array = np.random.randint(0, 100, size=(20,))
    random_array = random_array.tolist()
    correct = sorted(random_array, reverse=True)
    merge_sort(random_array, 0, len(random_array)-1)
    if correct != random_array:
        counter += 1

print(counter)

