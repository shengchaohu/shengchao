import os
import numpy as np
import json
import load
import unionfind
import user_records
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

class Multiple_file_set():
    def __init__(self, pathname, feature_name='', ax_name=''):
        # xxx_file_set.__init__(self, feature_index, log_file, ax_id)
        self.user_r_convert=factory(user_records.User_records_convert, pathname)
        self.feature_name=feature_name
        self.ax_name=ax_name

    def get_index(self, numpy_column, index_name):
        '''
        return the two index of feature_name in the given numpy_column
        '''
        try:
            i=numpy_column.index(index_name)
        except ValueError:
            print('not found column name')
            i=999
        return i

    def get_filename(self):
        return self.user_r_convert.filenames

    def get_length(self):
        '''
        return a list of length of each file
        '''
        results=[]
        for file in self.user_r_convert.filenames:
            numpy_data, numpy_column=self.user_r_convert.convert(file)
            results.append(len(numpy_data))
        return results

    def equivalance_multiple_file_set(self):
        '''
        do the same thing as xxx_file_set does, but return everything in a list
        '''
        results=[]
        for file in self.user_r_convert.filenames:
            numpy_data, numpy_column=self.user_r_convert.convert(file)
            i1=self.get_index(numpy_column, self.feature_name)
            if i1==999:
                results.append([])
                continue
            one_object=factory(xxx_file_set, i1, numpy_data)
            results.append(one_object.return_cluster())
        return results

    def double_equivalance_multiple_file_set(self):
        '''
        do the same thing as xxx_file_set does, but return everything in a list
        '''
        results=[]
        for file in self.user_r_convert.filenames:
            numpy_data, numpy_column=self.user_r_convert.convert(file)
            i1=self.get_index(numpy_column, self.feature_name)
            i2=self.get_index(numpy_column, self.ax_name)
            one_object=factory(xxx_file_set, i1, numpy_data, i2)
            results.append(one_object.double_equivalance_file_set())
        return results

class Multiple_file_time_set(Multiple_file_set):
    def __init__(self, pathname, time_name='cost_time'):
        Multiple_file_set.__init__(self,pathname)
        self.time_name=time_name

    def multiple_time_length(self):
        results=[]
        for file in self.user_r_convert.filenames:
            numpy_data, numpy_column=self.user_r_convert.convert(file)
            i=self.get_index(numpy_column, self.time_name)
            cost_time=[int(n[i]) for n in numpy_data if len(str(n[i]))<=7]
            results.append(sum(cost_time))
        return results

class xxx_file_set(Equivalance):
    def __init__(self, feature_index=-2, log_file=load.label, ax_id=0):
        '''
        ax_id won't be used unless double_equivalance_file_set() is called
        '''
        Equivalance.__init__(self, log_file)
        # self.log_file=log_file
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
            temp=self.label[s]
            second_cluster=factory(xxx_file_set, self.ax_id, temp).return_cluster()
            total_second_cluster=len(second_cluster)
            res[self.label[s[0]][-5]]=total_second_cluster
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
            log=self.label
        res=[int(float(x[self.end_id]))-int(float(x[self.start_id])) for x in log]
        # res=reduce((lambda x,y:int(float(x[5]))-int(float(x[8]))
        #                     +int(float(y[5]))-int(float(y[8]))),log)
        return sum(res)

    def double_time_equivalance_file_set(self):
        '''
        after one clustering, calculate time cost for each cluster
        return a dict
        '''
        first_cluster=self.return_cluster()
        res={self.label[f[0]][-5]:(str(self.time_length(self.label[f]))+'s')
                for f in first_cluster}
        return res

def dict_match(lst, filename):
    '''
    match a dict of {filename(subject):list data}
    '''
    return {filename[i]:lst[i] for i in range(len(lst))}

if __name__=='__main__':
    # student=factory(xxx_file_set, -2).return_cluster()
    # total_student=len(student)
    # print('total student #: '+str(total_student))

    # subject=factory(xxx_file_set, -5).return_cluster()
    # sample_number_by_subject={load.label[s[0]][-5]:len(s) for s in subject}
    # print("sample_number_by_subject: ",sample_number_by_subject)

    # test=factory(xxx_file_set, feature_index=-5, ax_id=-2).double_equivalance_file_set()
    # print("student_number_by_subject: ",test)

    # topic=len(factory(xxx_file_set, -4).return_cluster())
    # print('topic covered: ', topic)

    # test=factory(xxx_file_set, feature_index=-5, ax_id=-4).double_equivalance_file_set()
    # print(test)

    # time=factory(xxx_time_file_set, -5).time_length()
    # print(time)

    # time=factory(xxx_time_file_set, -5).double_time_equivalance_file_set()
    # print(time)

    # print('...the following are for user records')

    aClass=factory(Multiple_file_set, 'user records', 'student_index')
    student=aClass.equivalance_multiple_file_set()
    filename=aClass.get_filename()
    filename=[f.split('_')[0] for f in filename]
    # number of students in each file
    st=[len(s) for s in student]
    sst=dict_match(st,filename)
    print('number of students in each file',sst)
    print('total student',sum(st))

    # total_case=aClass.get_length()
    # sst=dict_match(total_case,filename)
    # print('number of cases in each file',sst)
    # print('total cases',sum(total_case))

    # total_time=factory(Multiple_file_time_set,'user records').multiple_time_length()
    # sst=dict_match(total_time,filename)
    # print('number of time in each file',sst)
    # print('total time',sum(total_time))

    # aClass=factory(Multiple_file_set, 'user records', 'topic_id')
    # student=aClass.equivalance_multiple_file_set()
    # # number of students in each file
    # st=[len(s) for s in student]
    # sst=dict_match(st,filename)
    # print('number of topics in each file',sst)
    # print('total topics',sum(st))

    # aClass=factory(Multiple_file_set, 'user records', 'tag_code')
    # student=aClass.equivalance_multiple_file_set()
    # # number of students in each file
    # st=[len(s) for s in student]
    # sst=dict_match(st,filename)
    # print('number of tag code in each file',sst)
    # print('total tag code',sum(st))

    print('...the following are for attention')

    aClass=factory(Multiple_file_set, 'attention', 'student_index')
    student=aClass.equivalance_multiple_file_set()
    filename=aClass.get_filename()
    filename=[f.split('_')[0] for f in filename]
    # number of students in each file
    st=[len(s) for s in student]
    sst=dict_match(st,filename)
    print('number of students in each file',sst)
    print('total student',sum(st))

    total_case=aClass.get_length()
    sst=dict_match(total_case,filename)
    print('number of cases in each file',sst)
    print('total cases',sum(total_case))

    total_time=factory(Multiple_file_time_set,'attention').multiple_time_length()
    sst=dict_match(total_time,filename)
    print('number of time in each file',sst)
    print('total time',sum(total_time))

    aClass=factory(Multiple_file_set, 'attention', 'topic_id')
    student=aClass.equivalance_multiple_file_set()
    # number of students in each file
    st=[len(s) for s in student]
    sst=dict_match(st,filename)
    print('number of topics in each file',sst)
    print('total topics',sum(st))

    aClass=factory(Multiple_file_set, 'attention', 'tag_code')
    student=aClass.equivalance_multiple_file_set()
    # number of students in each file
    st=[len(s) for s in student]
    sst=dict_match(st,filename)
    print('number of tag code in each file',sst)
    print('total tag code',sum(st))



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



