import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# data=pd.read_csv('test_ninggu_en_stat_nomissing_attention_that_worked.csv',sep='\t')
data=pd.read_csv('test_ninggu_en_stat.csv')

a=data.columns.tolist()

def set_element_number(column_name):
    temp=data[column_name]
    temp=temp.tolist()
    print(len(temp))
    return len(set(temp))

for aa in a:
    temp=set_element_number(aa)
    print('the # of different '+aa+' is '+str(temp))

def mean_per_student(column_name):
    _sum={}
    _count={}
    _mean={}
    for i in range(len(data)):
        if data['student_index'][i] not in _sum.keys():
            _sum[data['student_index'][i]]=data[column_name][i]
            _count[data['student_index'][i]]=1
        else:
            _sum[data['student_index'][i]]+=data[column_name][i]
            _count[data['student_index'][i]]+=1
    # print(_sum)
    # print(_count)
    for i in _sum.items():
        # print(_count[i[0]],i[1])
        _mean[i[0]]=i[1]/_count[i[0]]
    return _mean

def keep_two(column_name):
    l=list(mean_per_student(column_name).values())
    output=[float("{0:.2f}".format(ll)) for ll in l]
    return output

