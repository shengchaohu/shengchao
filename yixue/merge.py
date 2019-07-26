import os
import numpy as np
import json

all_features = []
all_eye_features = []
all_labels = []
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
    try:
        with open(path+"/"+filename+".json","r") as f: # open json file
            dict_t=json.load(f)
            key_t=sorted(list(dict_t.keys()))
            one_label=[dict_t[key] for key in key_t]    
            one_label+=[filename]   
    except FileNotFoundError:
        # print(filename)
        continue

    try:
        f=np.load(path+"/"+filename+".npy",'r') # open npy file
        # add filename at the beginning of each feature for matching
        one_features=[filename]
        one_features+=[np.asarray([np.asarray(fff.tolist()) for fff in ff[3][1][1]]) for ff in f] # magic number from npy_key.md
        # one_features=np.array(one_features)

        eye_features=[filename]
        #left eye
        eye_features+=[np.asarray(
                [np.asarray(ff[4][1].tolist())]+[np.asarray(fff.tolist()) for fff in ff[4][2]] # left eye
                +[np.asarray(ff[5][1].tolist())]+[np.asarray(fff.tolist()) for fff in ff[5][2]] # right eye
                ) for ff in f] # magic number from npy_key.md
        # eye_features=np.array(eye_features)
        
        
    except FileNotFoundError:
        # print(filename)
        continue
    all_labels.append(np.asarray(one_label))
    all_features.append(np.asarray(one_features))
    all_eye_features.append(np.asarray(eye_features))
    i+=1

all_features=np.asarray(all_features)
all_labels=np.asarray(all_labels)
all_eye_features=np.asarray(all_eye_features)
assert(len(all_features)==len(all_labels))
assert(len(all_features)==len(all_eye_features))

with open("webcam_feature.npy", "wb") as f:
    np.save(f,all_features)

with open("webcam_eye_feature.npy", "wb") as f:
    np.save(f,all_eye_features)

with open("webcam_label.npy", "wb") as f:
    np.save(f,all_labels)

with open("webcam_filename.npy", "wb") as f:
    np.save(f,filenames)

with open("webcam_labelname.npy", "wb") as f:
    np.save(f,key_t)



