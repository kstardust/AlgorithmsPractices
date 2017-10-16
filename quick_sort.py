import numpy as np
import math
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time

size = 100
index = [i for i in range(size)]
random_array = index[:]
np.random.shuffle(random_array)
data = []

def save(array, i, j):
    data.append([array[:], i, j])

def update_plot(hl, y):
    hl.set_ydata(y)
    plt.pause(0.01)
    
def quick_sort(array, begin, end):
    if begin < end:
        p = partition(array, begin, end)
        quick_sort(array, begin, p-1)
        quick_sort(array, p+1, end)

def partition(array, begin, end):
    pivot = array[end]
    i = begin - 1
    for j in range(begin, end):
        if array[j] < pivot:
            i += 1
            array[j], array[i] = array[i], array[j]
            save(random_array, i, j)
    array[i+1], array[end] = array[end], array[i+1]
    save(random_array, i+1, end)
    return i + 1

def update(frame_number):
    rain_drops['color'] = [0, 0, 1, 1]
    rain_drops['size'] = 10
    if frame_number > len(data):
        return 
    else:
        if frame_number != len(data):
            current_index = frame_number
            rain_drops['position'] = list(zip(index, data[current_index][0]))
            
            for i in data[current_index][1:]:
                rain_drops['color'][i] = [1, 0, 0, 1]
                rain_drops['size'][i] = 30

    scat.set_sizes(rain_drops['size'])
    scat.set_facecolors(rain_drops['color'])
    scat.set_offsets(rain_drops['position'])

quick_sort(random_array, 0, len(random_array)-1)
print(len(data))
fig = plt.figure()
fig.canvas.set_window_title('QS')
ax = fig.add_axes([0.1, 0.1, 0.8, 0.8], frameon=True)
ax.set_xlim(0, 100) #, ax.set_xticks([])
ax.set_ylim(0, 100) # , ax.set_yticks([])

rain_drops = np.zeros(size, dtype=[('position', int, 2),
                                   ('size', int, 1),
                                   ('color',  float, 4)])

rain_drops['position'] = [0, 0]
rain_drops['color'] = [0, 0, 1, 1]
rain_drops['size'] = 10

scat = ax.scatter(rain_drops['position'][:, 0], rain_drops['position'][:, 1],
                  s=rain_drops['size'], lw=0.5, facecolors=rain_drops['color'])

animation = FuncAnimation(fig, update, interval=100)
plt.show()

fig = plt.figure(figsize=(7,7))
ax = fig.add_axes([0, 0, 100, 100])
scat = ax.scatter(index, random_array, color=['blue']*size)
plt.show()

# def update(f):
#     scat.set_offsets([[x, y] for x, y in zip(index, random_array)])

# animation = FuncAnimation(fig, update, interval=100)
# plt.show()

# plt.ion()
# plt.figure(1)
# hl, = plt.plot(index, random_array, 'o', markersize=2)
# plt.axis([0, size, min(random_array), max(random_array)])



