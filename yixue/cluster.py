import os
import numpy as np
import json
import load
import unionfind
from functools import reduce
# import interval

def factory(aClass, *args, **kargs):
    return aClass(*args, **kargs)

class Equivalance(object):
    def __init__(self, log):
        '''
        log is of type array like object of array like object
        '''
        self.label=log

    def equivalance_file_set(self, label=None, id=float('inf'))->unionfind.UnionFindSet:
        '''
        use xxx as equivalance condition.
        return a UnionFindSet object
        '''
        if label is None: label=self.label
        ufs=unionfind.UnionFindSet(len(label))
        for file_index in range(len(label)):
            for file_index_another in range(len(label)):
                if label[file_index][id]==label[file_index_another][id]:
                    ufs.union(file_index,file_index_another)
                    break
        return ufs

    def return_cluster(self):
        '''
        return a list of list
        '''
        ufs=self.equivalance_file_set()
        return ufs.postorder()

class xxx_file_set(Equivalance):
    def __init__(self, feature_index=-2, log_file=load.label, ax_id=0):
        '''
        ax_id won't be used unless double_equivalance_file_set() is called
        '''
        Equivalance.__init__(self, log_file)
        self.log_file=log_file
        self.id=feature_index
        self.ax_id=ax_id

    def equivalance_file_set(self):
        '''
        use same user_id as equivalance condition.
        return a UnionFindSet object
        '''
        # ufs=unionfind.UnionFindSet(len(label))
        # for file_index in range(len(label)):
        #     for file_index_another in range(len(label)):
        #         if label[file_index][user_id]==label[file_index_another][user_id]:
        #             ufs.union(file_index,file_index_another)
        #             break
        return Equivalance.equivalance_file_set(self,id=self.id)

    def double_equivalance_file_set(self):
        '''
        after one clustering, for each cluster do another clustering according to ax_id
        return the number of second cluster in each first cluster as a dict
        '''
        first_cluster=self.return_cluster()
        res={}
        for s in first_cluster:
            temp=self.log_file[s]
            second_cluster=factory(xxx_file_set, self.ax_id, temp).return_cluster()
            total_second_cluster=len(second_cluster)
            res[self.log_file[s[0]][-5]]=total_second_cluster
        return res

class xxx_time_file_set(xxx_file_set):
    '''
    has specific method to calculate time length
    '''
    def __init__(self, feature_index=-2, log_file=load.label, ax_id=0, start_id=8,
                end_id=5):
        xxx_file_set.__init__(self, feature_index, log_file, ax_id)
        self.start_id=8
        self.end_id=5

    def time_length(self, log=None):
        if log is None:
            log=self.log_file
        res=[int(float(x[5]))-int(float(x[8])) for x in log]
        # res=reduce((lambda x,y:int(float(x[5]))-int(float(x[8]))
        #                     +int(float(y[5]))-int(float(y[8]))),log)
        return sum(res)

    def double_time_equivalance_file_set(self):
        '''
        after one clustering, calculate time cost for each cluster
        return a dict
        '''
        first_cluster=self.return_cluster()
        res={self.log_file[f[0]][-5]:(str(self.time_length(self.log_file[f]))+'s')
                for f in first_cluster}
        return res


# if __name__=='__main__':
student=factory(xxx_file_set, -2).return_cluster()
total_student=len(student)
print('total student #: '+str(total_student))

subject=factory(xxx_file_set, -5).return_cluster()
sample_number_by_subject={load.label[s[0]][-5]:len(s) for s in subject}
print("sample_number_by_subject: ",sample_number_by_subject)

test=factory(xxx_file_set, feature_index=-5, ax_id=-2).double_equivalance_file_set()
print("student_number_by_subject: ",test)

topic=len(factory(xxx_file_set, -4).return_cluster())
print('topic covered: ', topic)

test=factory(xxx_file_set, feature_index=-5, ax_id=-4).double_equivalance_file_set()
print(test)

time=factory(xxx_time_file_set, -5).time_length()
print(time)

time=factory(xxx_time_file_set, -5).double_time_equivalance_file_set()
print(time)

# class Question_file_set(Equivalance):
#     def question_file_set(self, question_id=6)->unionfind.UnionFindSet:
#         '''
#         use same question_id as equivalance condition.
#         return a UnionFindSet object
#         '''
#         # ufs=unionfind.UnionFindSet(len(label))
#         # for file_index in range(len(label)):
#         #     for file_index_another in range(len(label)):
#         #         if label[file_index][question_id]==label[file_index_another][question_id]:
#         #             ufs.union(file_index,file_index_another)
#         #             break
#         return Equivalance.equivalance_file_set(id=question_id)

# class Subject_file_set(Equivalance):
#     def subject_file_set(self, subject_id=-5)->unionfind.UnionFindSet:
#         '''
#         use same subject as equivalance condition.
#         return a UnionFindSet object
#         '''
#         # ufs=unionfind.UnionFindSet(len(label))
#         # for file_index in range(len(label)):
#         #     for file_index_another in range(len(label)):
#         #         if label[file_index][subject_id]==label[file_index_another][subject_id]:
#         #             ufs.union(file_index,file_index_another)
#         #             break
#         return Equivalance.equivalance_file_set(id=subject_id)

# def difficulty_file_set(difficulty=2)->unionfind.UnionFindSet:
#     '''
#     use same difficulty as equivalance condition.
#     return a UnionFindSet object
#     '''
#     # ufs=unionfind.UnionFindSet(len(label))
#     # for file_index in range(len(label)):
#     #     for file_index_another in range(len(label)):
#     #         if label[file_index][difficulty]==label[file_index_another][difficulty]:
#     #             ufs.union(file_index,file_index_another)
#     #             break
#     return equivalance_file_set(id=difficulty)

# def correctness_file_set(is_correct=3)->unionfind.UnionFindSet:
#     '''
#     use is_correct as equivalance condition.
#     return a UnionFindSet object
#     '''
#     # ufs=unionfind.UnionFindSet(len(label))
#     # for file_index in range(len(label)):
#     #     for file_index_another in range(len(label)):
#     #         if label[file_index][is_correct]==label[file_index_another][is_correct]:
#     #             ufs.union(file_index,file_index_another)
#     #             break
#     return equivalance_file_set(id=is_correct)

# def school_file_set(school_id=-7)->unionfind.UnionFindSet:
#     '''
#     use same school as equivalance condition.
#     return a UnionFindSet object
#     '''
#     return equivalance_file_set(id=school_id)

# def section_file_set(section_id=-6)->unionfind.UnionFindSet:
#     '''
#     use same section as equivalance condition.
#     return a UnionFindSet object
#     '''
#     return equivalance_file_set(id=section_id)

# def topic_file_set(topic_id=-4)->unionfind.UnionFindSet:
#     '''
#     use same topic as equivalance condition.
#     return a UnionFindSet object
#     '''
#     return equivalance_file_set(id=topic_id)

# def course_file_set(course_id=1)->unionfind.UnionFindSet:
#     '''
#     use same topic as equivalance condition.
#     return a UnionFindSet object
#     '''
#     return equivalance_file_set(id=course_id)

def student_file_set_specific(given_student_list, user_id=-2, label=load.label)->unionfind.UnionFindSet:
    '''
    use same user_id as equivalance condition. Also limit search in specific range (given_student_list)
    return a UnionFindSet object
    '''
    ufs=unionfind.UnionFindSet(len(label))
    for file_index in range(len(label)):
        for file_index_another in range(len(label)):
            if int(label[file_index][user_id]) in given_student_list \
                and label[file_index][user_id]==label[file_index_another][user_id]:
                ufs.union(file_index,file_index_another)
                break
    return ufs



