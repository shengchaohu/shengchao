import os
import numpy as np
import json
import load
import unionfind
import interval
import cluster

def student_variable_timeordered(variable_id, student_cluster=cluster.return_cluster(cluster.student_file_set()), 
    question_start_time_delta=8):
    '''
    return a list of list of certain attribute (variable) of question done by each student, ordered by question start time
    and a list of list of sorted student's file index
    '''
    sorted_student=[sorted(student,key=lambda x:load.label[x][question_start_time_delta]) for student in student_cluster]

    if variable_id in [2,3]:
        sorted_student_v=[list(map(int,[load.label[s][variable_id] for s in student])) for student in sorted_student]
    elif variable_id in [5]:
        sorted_student_v=[[(float(load.label[s][variable_id])-float(load.label[s][question_start_time_delta])) \
            for s in student] for student in sorted_student]
    elif variable_id in [1,-4,-5,-6]:
        sorted_student_v=[[load.label[s][variable_id] for s in student] for student in sorted_student]

    return sorted_student_v, sorted_student


def student_correctness_timeordered(is_correct=3):
    '''
    return a list of list of correctness of question done by each student, ordered by question start time
    and a list of list of sorted student's file index
    '''
    # sorted_student=[sorted(student,key=lambda x:load.label[x][question_start_time_delta]) for student in student_cluster]
    # sorted_student_correctness=[list(map(int,[load.label[s][is_correct] for s in student])) for student in sorted_student]
    return student_variable_timeordered(variable_id=is_correct)

def student_difficulty_timeordered(difficulty=2):
    '''
    return a list of list of difficulty of question done by each student, ordered by question start time
    and a list of list of sorted student's file index
    '''
    # sorted_student=[sorted(student,key=lambda x:load.label[x][question_start_time_delta]) for student in student_cluster]
    # sorted_student_difficulty=[list(map(int,[load.label[s][difficulty] for s in student])) for student in sorted_student]
    return student_variable_timeordered(variable_id=difficulty)

def student_timelength_timeordered(question_end_time_delta=5):
    '''
    return a list of list of time length of question done by each student, ordered by question start time
    and a list of list of sorted student's file index
    '''
    # sorted_student=[sorted(student,key=lambda x:load.label[x][question_start_time_delta]) for student in student_cluster]
    # sorted_student_timelength=[[(float(load.label[s][question_end_time_delta])-float(load.label[s][question_start_time_delta])) \
    #     for s in student] for student in sorted_student]
    return student_variable_timeordered(variable_id=question_end_time_delta)

def student_subject_timeordered(subject_id=-5):
    '''
    return a list of list of subject of question done by each student, ordered by question start time
    and a list of list of sorted student's file index
    '''
    # sorted_student=[sorted(student,key=lambda x:load.label[x][question_start_time_delta]) for student in student_cluster]
    # sorted_student_subject=[[load.label[s][subject_id] for s in student] for student in sorted_student]
    return student_variable_timeordered(variable_id=subject_id)

def student_topic_timeordered(topic_id=-4):
    '''
    return a list of list of topic of question done by each student, ordered by question start time
    and a list of list of sorted student's file index
    '''
    # sorted_student=[sorted(student,key=lambda x:load.label[x][question_start_time_delta]) for student in student_cluster]
    # sorted_student_topic=[[load.label[s][topic_id] for s in student] for student in sorted_student]
    return student_variable_timeordered(variable_id=topic_id)

def student_section_timeordered(section_id=-6):
    '''
    return a list of list of section of question done by each student, ordered by question start time
    and a list of list of sorted student's file index
    '''
    return student_variable_timeordered(variable_id=section_id)

def student_course_timeordered(course_id=1):
    '''
    return a list of list of course of question done by each student, ordered by question start time
    and a list of list of sorted student's file index
    '''
    return student_variable_timeordered(variable_id=course_id)





