import os
import numpy as np
import matplotlib.pyplot as plt
import visual
from matplotlib import animation

fig = plt.figure()
ax = plt.axes(xlim=(0, 1), ylim=(0, 1))
line, = ax.plot([], [], lw=2)
_count=1 #  Note: frame starts from 1, the 0th element in one_sample is filename

def init():
    """Clears current frame."""
    line.set_data([], [])
    return line,

def animate(i):
    """Draw figure.
    @param i: Frame counter
    @type i: int
    """
    # x = np.linspace(0, 2, 1000)
    # y = np.sin(2 * np.pi * (x - 0.01 * i)) * np.cos(22 * np.pi * (x -
    # 0.01 * i))
    global _count
    x,y=visual.return_one_face(5,_count,51,9)
    line.set_data(x, y)
    _count+=1
    return line,

# This call puts the work in motion
# connecting init and animate functions and figure we want to draw
animator = animation.FuncAnimation(fig, animate, init_func=init,
frames=1200, interval=20, blit=True)
# This call creates the video file.
# Temporary, every frame is saved as PNG file
# and later processed by ffmpeg encoder into MPEG4 file
# we can pass various arguments to ffmpeg via extra_args
animator.save('basic_animation.mp4', fps=30,
extra_args=['-vcodec', 'libx264'],
writer='ffmpeg_file')
plt.show()