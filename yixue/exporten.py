import os
import numpy as np
import json
import load
import unionfind
import interval
import cluster
import movement

all=[]
ufs=cluster.subject_file_set()
cl=cluster.return_cluster(ufs)
for c in cl:
    if load.label[c[0]][-5]=='en':
        output=c

move_xy=movement.calculate_movement(three_axis=0)
move_xyz=movement.calculate_movement()

for ot in output:
    temp=load.label[ot].copy()
    temp[-1]=move_xy[ot]
    temp=np.append(temp,move_xyz[ot])
    all.append(temp)

all=np.asarray(all)

with open("with_movement.npy", "wb") as f:
    np.save(f,all)
