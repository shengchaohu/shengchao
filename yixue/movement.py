import numpy as np
import load

def calculate_movement(three_axis=1, sample_start=0, sample_end=len(load.label), 
    pos=load.pos, confidence=load.confidence, confidence_threadshold=0.4):
    '''
    calcaluate the movement using x, y and z. default setting calculates movements in x,y and z axis
    given confidence_threadshold
    '''
    move=[]
    for i in range(sample_start, sample_end): # for each question
        s=0
        count=0
        for j in range(1,len(confidence[i])):
            if float(confidence[i][j])>=confidence_threadshold:
                if three_axis==1:
                    temp=sum(list(map(abs,pos[i][j][1])))
                else:
                    temp=sum(list(map(abs,pos[i][j][1][0:2])))
                s+=temp
                count+=1
        if s>0:
            s=s/count
        move.append(s)
    return move