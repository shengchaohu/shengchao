import os
import numpy as np
import json
import load
import unionfind
import interval

'''
There are overlapping and duplicates in original datasets. This file does the data cleaning work.
'''

# def maxlen(data=load.data)->int:
# 	"""
# 	return the index of max length of cost time
# 	"""
# 	length=[len(d) for d in data]
# 	dd=enumerate(length)
# 	d={l:i for (i,l) in dd}
# 	big=max(list(d.keys()))
# 	return d[big]

# def question_set(label=load.label)->(list,list,list):
# 	"""
# 	return list of file index of duplicated question id (which means it is done by same student) and unduplicated ones
# 	"""
# 	question=[l[6] for l in label] # all question id
# 	# dd=enumerate(question)
# 	# d={l:i for (i,l) in dd} # dict doesn't allow duplicated keys, it's already a set
# 	# len(question)
# 	question_set=set(question) # find duplicated question id

# 	index_d=[i for i in range(len(question)) if question.count(question[i]) > 1]
# 	# index_und=[i for i in range(len(question)) if question.count(question[i]) == 1]
# 	index_und=None

# 	return index_d,index_und,question # put all the duplicated file index in a list for future use

# def file_to_delete()->(list,list):
# 	'''
# 	use overlapping start/end time to determine which file should be deleted.
# 	file index and a forest (in the form of a parent list of each file index) is returned
# 	'''
# 	index,_,question=question_set()
# 	print(index)
# 	dd=enumerate(index)
# 	d={l:i for (i,l) in dd}
# 	ufs=unionfind.UnionFindSet(len(index))
# 	for u in range(len(index)):
# 		question_id=load.label[index[u]][6]
# 		for i in range(len(question)): # find the (file) index of this question_id and then turn it into index
# 			if question[i]==question_id and i != index[u]:
# 				temp=d[i]
# 				break
# 		ufs.union(u,temp)
# 	return index,ufs._parents

def duplicate_file(user_id=-2, question_id=6, label=load.label, data=load.data)->unionfind.UnionFindSet:
	'''
	use same user_id and question_id to determine which file should be deleted.
	return a UnionFindSet object
	'''
	ufs=unionfind.UnionFindSet(len(label))
	for file_index in range(len(label)):
		for file_index_another in range(len(label)):
			if file_index_another != file_index \
				and label[file_index][question_id]==label[file_index_another][question_id] \
				and label[file_index][user_id]==label[file_index_another][user_id]:
				# if(data[file_index].shape!=data[file_index_another].shape):
				# 	print(file_index,file_index_another)
				# 	break
				ufs.union(file_index,file_index_another)
				break
	return ufs

def zero_file(data=load.data)->list: # notice need to modify load.data
	'''
	return a list of file index, each file is all zero
	'''
	zero=[i for i in range(len(data)) if sum(sum(data[i])).all()==0]
	return zero

def file_delete()->(list,list):
	'''
	return a list of file name to delete and a list of file name index, and save the file name to a file
	'''
	file_name=-1
	ufs=duplicate_file()
	# zero=zero_file()
	# 这是全删了，应该要保留一个
	file_delete_index=[i for i in range(len(ufs._parents)) if i != ufs.find(i) or ufs._ranks[i] >1]
	# file_delete_index=list(set([i for i in range(len(ufs._parents)) if i != ufs.find(i) or ufs._ranks[i] >1]) | set(zero))
	print(file_delete_index)
	file_delete_name=[load.label[i][file_name] for i in file_delete_index]
	with open("webcam_filename_delete.npy", "wb") as f:
		np.save(f,file_delete_name)
	return file_delete_name,file_delete_index


# [z for z in zero if aa[z]==z and bb[z]==1]

# student=[l[-1] for l in label]
# len(student)
# student_set=set(student)
# len(student_set)

