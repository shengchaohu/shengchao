import os
import numpy as np
import json
import load
import unionfind
import interval

def equivalance_file_set(id, label=load.label, data=load.data)->unionfind.UnionFindSet:
    '''
    use xxx as equivalance condition.
    return a UnionFindSet object
    '''
    ufs=unionfind.UnionFindSet(len(label))
    for file_index in range(len(label)):
        for file_index_another in range(len(label)):
            if label[file_index][id]==label[file_index_another][id]:
                ufs.union(file_index,file_index_another)
                break
    return ufs

def student_file_set(user_id=-2)->unionfind.UnionFindSet:
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
    return equivalance_file_set(id=user_id)

def question_file_set(question_id=6)->unionfind.UnionFindSet:
    '''
    use same question_id as equivalance condition.
    return a UnionFindSet object
    '''
    # ufs=unionfind.UnionFindSet(len(label))
    # for file_index in range(len(label)):
    #     for file_index_another in range(len(label)):
    #         if label[file_index][question_id]==label[file_index_another][question_id]:
    #             ufs.union(file_index,file_index_another)
    #             break
    return equivalance_file_set(id=question_id)

def subject_file_set(subject_id=-5)->unionfind.UnionFindSet:
    '''
    use same subject as equivalance condition.
    return a UnionFindSet object
    '''
    # ufs=unionfind.UnionFindSet(len(label))
    # for file_index in range(len(label)):
    #     for file_index_another in range(len(label)):
    #         if label[file_index][subject_id]==label[file_index_another][subject_id]:
    #             ufs.union(file_index,file_index_another)
    #             break
    return equivalance_file_set(id=subject_id)

def difficulty_file_set(difficulty=2)->unionfind.UnionFindSet:
    '''
    use same difficulty as equivalance condition.
    return a UnionFindSet object
    '''
    # ufs=unionfind.UnionFindSet(len(label))
    # for file_index in range(len(label)):
    #     for file_index_another in range(len(label)):
    #         if label[file_index][difficulty]==label[file_index_another][difficulty]:
    #             ufs.union(file_index,file_index_another)
    #             break
    return equivalance_file_set(id=difficulty)

def correctness_file_set(is_correct=3)->unionfind.UnionFindSet:
    '''
    use is_correct as equivalance condition.
    return a UnionFindSet object
    '''
    # ufs=unionfind.UnionFindSet(len(label))
    # for file_index in range(len(label)):
    #     for file_index_another in range(len(label)):
    #         if label[file_index][is_correct]==label[file_index_another][is_correct]:
    #             ufs.union(file_index,file_index_another)
    #             break
    return equivalance_file_set(id=is_correct)

def school_file_set(school_id=-7)->unionfind.UnionFindSet:
    '''
    use same school as equivalance condition.
    return a UnionFindSet object
    '''
    return equivalance_file_set(id=school_id)

def section_file_set(section_id=-6)->unionfind.UnionFindSet:
    '''
    use same section as equivalance condition.
    return a UnionFindSet object
    '''
    return equivalance_file_set(id=section_id)

def topic_file_set(topic_id=-4)->unionfind.UnionFindSet:
    '''
    use same topic as equivalance condition.
    return a UnionFindSet object
    '''
    return equivalance_file_set(id=topic_id)

def course_file_set(course_id=1)->unionfind.UnionFindSet:
    '''
    use same topic as equivalance condition.
    return a UnionFindSet object
    '''
    return equivalance_file_set(id=course_id)

def return_cluster(ufs):
    return ufs.postorder()

