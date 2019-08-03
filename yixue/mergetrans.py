import os
import numpy as np
import json

all_pos = []
all_confidence= []
path="webcam_features"
filenames=os.listdir(path) 
filenames=[os.path.splitext(filename)[0] for filename in filenames if filename !='npy_key.md'] # list of all filenames
filenames=set(filenames)
test=len(filenames)
# test=20
i=0
for filename in filenames:
    if(i>test):
        break
    # try:
    #     with open(path+"\\"+filename+".json","r") as f: # open json file
    #         dict_t=json.load(f)
    #         key_t=sorted(list(dict_t.keys()))
    #         one_label=[dict_t[key] for key in key_t]    
    #         one_label+=[filename]   
    # except FileNotFoundError:
    #     # print(filename)
    #     continue

    try:
        f=np.load(path+"\\"+filename+".npy",'r') # open npy file
        # add filename at the beginning of each feature for matching

        one_pos=[filename]
        one_confidence=[filename]
        one_pos+=[np.asarray(
                [np.asarray(ff[2][1].tolist())] # rotation
                +[np.asarray(ff[2][2].tolist())] # translation
                ) for ff in f] # magic number from npy_key.md
        one_confidence+=[np.asarray(
                ff[1].tolist() # overall confidence
                ) for ff in f] # magic number from npy_key.md
        
        
    except FileNotFoundError:
        # print(filename)
        continue
    all_pos.append(np.asarray(one_pos))
    all_confidence.append(np.asarray(one_confidence))
    i+=1

all_pos=np.asarray(all_pos)
all_confidence=np.asarray(all_confidence)
assert(len(all_pos)==len(all_confidence))

with open("webcam_pos.npy", "wb") as f:
    np.save(f,all_pos)

with open("webcam_confidence.npy", "wb") as f:
    np.save(f,all_confidence)
