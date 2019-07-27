import os
import numpy as np
import json
import load
import unionfind
import interval
import cluster
import timeseries
import matplotlib.pyplot as plt

def plot_scatter_timeordered(student_length=34, function_name=timeseries.student_difficulty_timeordered, \
        x_axis='question index (alrealdy ordered by start time)', y_axis=None):
    '''
    scatter correctness vs selected dependent variable for all 34 students in a figure, ordered by question start time
    '''
    sorted_student_correctness,_=timeseries.student_correctness_timeordered()
    sorted_student_dv,_=function_name()
    # plt.figure()
    for i in range(student_length):
        color=['red' if cc==1 else 'blue' for cc in sorted_student_correctness[i]]
        # plt.subplot(34,1,int(i)+1)
        plt.figure()
        plt.title('student No. '+str(i+1))
        plt.xlabel(x_axis)
        plt.ylabel(y_axis)
        plt.scatter(range(len(sorted_student_dv[i])), sorted_student_dv[i], c=color)
        plt.show()

def pie_plot(function_name=cluster.student_file_set, id=-2, figure_name='portion of samples'):
    ufs=function_name()
    cl=cluster.return_cluster(ufs)
    cl_len=[len(c) for c in cl]
    cl_label=[load.label[c[0]][id] for c in cl]
    explode=[0.1]*len(cl_len)
    plt.figure(figure_name,(9,7))
    plt.title(figure_name)
    plt.pie(x=cl_len, labels=cl_label, explode=explode, radius=1, autopct='%1.1f%%', startangle=67)
    plt.show()

def printout():
    print('These basic info about the question and student are provided:')
    print(load.labelname[:-2],'\n')
    print('Figures below shows relations between correctness and some of these info \n')

    timeseries_method_name=['student_course_timeordered', 'student_difficulty_timeordered', 'student_section_timeordered',\
         'student_subject_timeordered', 'student_timelength_timeordered', 'student_topic_timeordered']
    timeseries_method=[eval('timeseries'+'.'+t) for t in timeseries_method_name]
    timeseries_y_axis=[t.split('_')[1]+'_id (or name)' for t in timeseries_method_name]
    for i in range(len(timeseries_method_name)):
        print('In the below scatter plots, Red point means correctness, Blue point means incorrectness')
        print('For the next 34 figures, each shows the possible relation between correctness and '+\
            timeseries_method_name[i].split('_')[1]+' for a student (there are 34 students in total in the dataset)')
        plot_scatter_timeordered(34,timeseries_method[i],'question index (alrealdy ordered by start time)',timeseries_y_axis[i])

    print('Below are pie graphes \n')
    cluster_method_name=['correctness_file_set', 'course_file_set', 'difficulty_file_set','school_file_set', \
        'section_file_set', 'student_file_set', 'subject_file_set', 'topic_file_set']
    cluster_method=[eval('cluster'+'.'+t) for t in cluster_method_name]
    cluster_figure_name=['portion of different '+t.split('_')[0]+' in the dataset' for t in cluster_method_name]
    v_id=[3,1,2,-7,-6,-2,-5,-4]
    for i in range(len(cluster_method_name)):
        print('Below is a pie graph, total sample number is ', len(load.label))
        pie_plot(cluster_method[i],v_id[i],cluster_figure_name[i])

def plot_one_face(sample_id=5, frame=1, location_face=51, location_iris=9):
    '''
    plot one frame. Note: frame starts from 1, the 0th element in one_sample is filename
    '''
    one_sample=load.data[sample_id]
    one_eye=load.eye[sample_id]
    one_frame=one_sample[frame]
    (one_eye_frame_left_pupil, one_eye_frame_left_iris, one_eye_frame_right_pupil, one_eye_frame_right_iris)= \
        (one_eye[frame][i] for i in range(4))
    assert(one_frame.shape==(51,3))
    assert(one_eye_frame_left_pupil.shape==(2,))
    assert(one_eye_frame_left_iris.shape==(9,3))
    x_axis=[t[0] for t in one_frame[0:location_face]]
    y_axis=[1-t[1] for t in one_frame[0:location_face]]
    x_axis_eye=[one_eye_frame_left_pupil[0]] \
                +[t[0] for t in one_eye_frame_left_iris[0:location_iris]] \
                +[one_eye_frame_right_pupil[0]] \
                +[t[0] for t in one_eye_frame_right_iris[0:location_iris]]
    y_axis_eye=[1-one_eye_frame_left_pupil[1]] \
                +[1-t[1] for t in one_eye_frame_left_iris[0:location_iris]] \
                +[1-one_eye_frame_right_pupil[1]] \
                +[1-t[1] for t in one_eye_frame_right_iris[0:location_iris]]
    plt.figure('frame No.'+str(frame)+' for sample '+str(sample_id)) # set fixed scale
    plt.plot(x_axis,y_axis,'xb')
    plt.plot(x_axis_eye,y_axis_eye,'.y')
    plt.show()

def return_one_face(sample_id=5, frame=1, location_face=51, location_iris=9):
    '''
    plot one frame. Note: frame starts from 1, the 0th element in one_sample is filename
    '''
    one_sample=load.data[sample_id]
    one_eye=load.eye[sample_id]
    one_frame=one_sample[frame]
    (one_eye_frame_left_pupil, one_eye_frame_left_iris, one_eye_frame_right_pupil, one_eye_frame_right_iris)= \
        (one_eye[frame][i] for i in range(4))
    assert(one_frame.shape==(51,3))
    assert(one_eye_frame_left_pupil.shape==(2,))
    assert(one_eye_frame_left_iris.shape==(9,3))
    x_axis=[t[0] for t in one_frame[0:location_face]]
    y_axis=[1-t[1] for t in one_frame[0:location_face]]
    x_axis_eye=[one_eye_frame_left_pupil[0]] \
                +[t[0] for t in one_eye_frame_left_iris[0:location_iris]] \
                +[one_eye_frame_right_pupil[0]] \
                +[t[0] for t in one_eye_frame_right_iris[0:location_iris]]
    y_axis_eye=[1-one_eye_frame_left_pupil[1]] \
                +[1-t[1] for t in one_eye_frame_left_iris[0:location_iris]] \
                +[1-one_eye_frame_right_pupil[1]] \
                +[1-t[1] for t in one_eye_frame_right_iris[0:location_iris]]
    return (x_axis+x_axis_eye, y_axis+y_axis_eye)

def plot_animation(length=40):
    i=1
    while(i<20):
        plot_one_face(frame=i)
        i+=1


# def plot_difficulty_correctness_timeordered(student=0):
#     '''
#     scatter correctness vs difficulty for one student, ordered by question start time
#     '''
#     sorted_student_correctness,_=timeseries.student_correctness_timeordered()
#     sorted_student_difficulty,_=timeseries.student_difficulty_timeordered()
#     color=['red' if cc==1 else 'blue' for cc in sorted_student_correctness[student]]
#     plt.title('plot_difficulty_correctness_timeordered')
#     plt.scatter(range(len(sorted_student_difficulty[student])), sorted_student_difficulty[student], c=color)
#     plt.show()

# def plot_timelength_correctness_timeordered(student=0):
#     '''
#     scatter correctness vs timelength for one student, ordered by question start time
#     '''
#     sorted_student_correctness,_=timeseries.student_correctness_timeordered()
#     sorted_student_timelength,_=timeseries.student_timelength_timeordered()
#     color=['red' if cc==1 else 'blue' for cc in sorted_student_correctness[student]]
#     plt.title('plot_timelength_correctness_timeordered')
#     plt.scatter(range(len(sorted_student_timelength[student])), sorted_student_timelength[student], c=color, label=color)
#     plt.show()









