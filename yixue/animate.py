import os
import load
import numpy as np
import matplotlib.pyplot as plt
import visual
from matplotlib import animation

def animate(frame_range, sample, id):
    f=[]
    fig = plt.figure()
    ax = plt.axes(xlim=(0, 1), ylim=(0, 1))
    # connect different parts of face

    for i in range(6):
        temp,=ax.plot([], [], '-b', lw=2)
        f.append(temp)
    for i in range(6,8):
        temp,=ax.plot([], [], '-c', lw=2)
        f.append(temp)
    for i in range(8,10):
        temp,=ax.plot([], [], '.r', lw=2)
        f.append(temp)

    # for i in face[:-2]:
    #     i,=ax.plot([], [], '-b', lw=2)
    # for i in face[-2:]:
    #     i,=ax.plot([], [], '.r', lw=2)
    # lefteyebrow, = ax.plot([], [], '-b', lw=2)

    # righteyebrow, = ax.plot([], [], '-b', lw=2)
    # nose, = ax.plot([], [], '-b', lw=2)
    # mouth, = ax.plot([], [], '-b', lw=2)
    # lefteye, = ax.plot([], [], '-b', lw=2)
    # righteye, = ax.plot([], [], '-b', lw=2)
    # leftiris, = ax.plot([], [], '-b', lw=2)
    # rightiris, = ax.plot([], [], '-b', lw=2)
    # leftpupil, = ax.plot([], [], '.r', lw=2)
    # rightpupil, = ax.plot([], [], '.r', lw=2)
    # face=[lefteyebrow, righteyebrow, nose, mouth, lefteye, righteye, leftiris, rightiris,\
    # leftpupil, rightpupil]

    # nested function
    def init():
        """Clears current frame."""
        for i in f:
            i.set_data([], [])
        # eyebrow.set_data([], [])
        # mouth.set_data([], [])
        # return (eye+mouth),

    def _animate(i, sample):
        """Draw figure.
        @param i: Frame counter
        @param sample: sample number
        @type i: int
        """
        x,y=visual.return_one_face(sample,i,51,9)
        # if score is not good:
        #     return
        # But need to modify the API in animation.FuncAnimation. Mission abandoned
        # 51+1+9+1+9=71
        assert(len(x)==71) 
        assert(len(y)==71)
        f[0].set_data(x[0:5], y[0:5])  #lefteyebrow
        f[1].set_data(x[5:10], y[5:10])  #righteyebrow
        f[2].set_data(x[10:19], y[10:19]) #nose
        f[3].set_data(x[31:51], y[31:51]) #mouth
        f[4].set_data(x[19:25], y[19:25]) #lefteye
        f[5].set_data(x[25:31], y[25:31]) #righteye
        f[6].set_data(x[52:61], y[52:61]) #leftiris
        f[7].set_data(x[62:71], y[62:71]) #rightiris
        f[8].set_data(x[51:52], y[51:52]) #leftpupil
        f[9].set_data(x[61:62], y[61:62]) #rightpupil
        # return (eye+mouth),

# face=[lefteyebrow, righteyebrow, nose, mouth, lefteye, righteye, leftiris, rightiris,\
#     leftpupil, rightpupil]

    # This call puts the work in motion
    # connecting init and animate functions and figure we want to draw
    animator = animation.FuncAnimation(fig, _animate, init_func=init,
    frames=range(1,frame_range), fargs=(sample,), interval=20, blit=False)
    # This call creates the video file.
    # Temporary, every frame is saved as PNG file
    # and later processed by ffmpeg encoder into MPEG4 file
    # we can pass various arguments to ffmpeg via extra_args
    animator.save('animations\\basic_animation'+str(id)+'.mp4', fps=30,
    extra_args=['-vcodec', 'libx264'],
    writer='ffmpeg_file')
    # plt.show()

def batch_animate(start=0,end=len(load.data)):
    '''
    generate a batch of animations
    '''
    for i in range(start,end):  
        animate(len(load.data[i]),i,i)
        print(str(i)+' has finished!')
        plt.close()

