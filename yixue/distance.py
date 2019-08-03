import os
import numpy as np
import json
import load
import unionfind
import interval
import cluster
import timeseries
import visual
import matplotlib.pyplot as plt

# count number of times

def eye_data(sample_id,frame)->((list,list),(list,list)):
    '''
    return left and right eye data
    '''
    x,y=visual.return_one_face(sample_id,frame,51,9)
    return (x[19:25], y[19:25]),(x[25:31],y[25:31])

def iris_data(sample_id,frame)->((list,list),(list,list)):
    '''
    return left and right iris data
    '''
    x,y=visual.return_one_face(sample_id,frame,51,9)
    return (x[52:61], y[52:61]),(x[62:71],y[52:61])

def pupil_data(sample_id,frame)->((list,list),(list,list)):
    '''
    return left and right pupil data
    '''
    x,y=visual.return_one_face(sample_id,frame,51,9)
    return (x[51:52], y[51:52]),(x[61:62],y[61:61])

def nose_data(sample_id,frame)->(list,list):
    '''
    return nose data
    '''
    x,y=visual.return_one_face(sample_id,frame,51,9)
    return (x[10:19], y[10:19])

def mouth_data(sample_id,frame)->(list,list):
    '''
    return mouse data
    '''
    x,y=visual.return_one_face(sample_id,frame,51,9)
    return (x[31:51], y[31:51])

def blinking():
    # outside eye or not
    pass

def distance_pupil_eyecenter():
    pass

def ratio():
    pass

def tilt():
    # if nose is vertical
    pass

def talk():
    pass











