import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


fig = plt.figure()
ax = fig.add_axes([0.1, 0.1, 0.8, 0.8], frameon=True)
ax.set_xlim(0, 100) #, ax.set_xticks([])
ax.set_ylim(0, 100) # , ax.set_yticks([])

n_drops = 50
rain_drops = np.zeros(n_drops, dtype=[('position', float, 2),
                                      ('color',    float, 4)])

rain_drops['position'] = np.random.uniform(0, 100, (n_drops, 2))
rain_drops['color'] = [0, 0, 1, 1]

scat = ax.scatter(rain_drops['position'][:, 0], rain_drops['position'][:, 1],
                  s=10, lw=0.5, facecolors=rain_drops['color'])

def update(frame_number):
    current_index = frame_number
    rain_drops['position'][current_index] = np.random.uniform(0, 100, 2)
    rain_drops['color'] = [0, 0, 1, 1]
    rain_drops['color'][current_index] = (1, 0, 0, 1)
    scat.set_facecolors(rain_drops['color'])
    scat.set_offsets(rain_drops['position'])

animation = FuncAnimation(fig, update, interval=100)
plt.show()
