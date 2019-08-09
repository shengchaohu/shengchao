import numpy as np
import load
import movement
import os
import pandas as pd
import cluster

overlapping_studentid_list=[6840100809417043, 6880100813533684, 56342, 75962, 75799, 6840100811617840, 79276, \
        71824, 75783, 6860100811868538, 68868, 6840100809224972, 6840100809576695, 76253, \
        6840100809224840, 6860100811867827, 50369, 6880100813784710]

# move_xy=movement.calculate_movement(three_axis=0)
# move_xyz=movement.calculate_movement()

def factory(aClass, *args, **kargs):
    return aClass(*args, **kargs)

class User_records():
    def __init__(self, path="user records"):
        '''
        suppose all files are csv file
        store a list of filenames
        '''
        self.filenames=os.listdir(path)
        self.path=path

    def open(self, filename):
        '''
        open one csv file
        return a pandas dataframe
        '''
        data=pd.read_csv(self.path+'\\'+filename)
        return data

class User_records_convert(User_records):
    def __init__(self, path="user records"):
        User_records.__init__(self, path)

    def convert(self, filename):
        '''
        convert pandas dataframe to the form of numpy array, like load.label
        return a numpy array of array, and a list of label name
        '''
        df=self.open(filename)
        column=df.columns.tolist()
        df=df.to_numpy()
        return df, column

class User_records_update(User_records):
    def __init__(self, path="user records"):
        User_records.__init__(self, path)

    def update_user_records(self,*new_columns):
        '''
        update arbitrary number of columns of new data to existing user records
        You should give the name of new_column and data of new_column (in this order)
        It should match the existing data by using user_id and question_id
        '''
        for new_column in new_columns:
            if(type(new_column)==str):
                temp=new_column
            else:
                update_user_records_addone(temp, new_column)

    def update_user_records_addone(self, new_column_name, new_column):
        '''
        update one new column to the existing data in all files
        It should match the existing data by using user_id and question_id
        in the end it will write to new csv file
        '''
        # still need to modify....
        user_id=1
        question_id=10
        new_data=-1
        filenames=self.filenames
        new_df=pd.DataFrame()
        no_search=set() # should be in parent function
        end=-1
        for file in filenames: 
            data=self.open(file)
            temp=[new_data]*len(data)
            data[new_column_name]=temp
            for i in range(len(data)):
                # match user_id and question_id in one of the overlapping file_id or user_id
                if data.iloc[i][user_id] in no_search:
                    continue
                index=search(no_search, i, user_id, question_id, data)
                if index>=0:
                    data.iloc[i][-1]=new_column[index]
                    new_df=new_df.append(data.iloc[i][:])
            print('Done with one file! '+file)
        new_df.to_csv("overlapping_records.csv")


    def search(self, no_search, i, user_column_index, question_column_index, data, label=load.label)->int:
        '''
        return -1 if not found, else return the file index
        '''
        target_user_id=data.iloc[i][user_column_index]
        target_question_id=data.iloc[i][question_column_index]
        cll_file_index=return_overlapping_file_index()
        for c in cll_file_index:
            # print('student id in my record', label[c[0]][-2])
            # assert(type(int(label[c[0]][-2]))==type(int(target_user_id)))
            if int(label[c[0]][-2])==int(target_user_id):
                for cc in c:
                    # 6 is question_id index in my label
                    # print('user and question is ',target_user_id,target_question_id)
                    # print('question id in my record', label[cc][6])
                    # assert(type(str(label[cc][6]))==type(str(target_question_id)))
                    if str(label[cc][6])==str(target_question_id):
                        # print('user and question is ',target_user_id,target_question_id)
                        # print('question id in my record', label[cc][6])
                        # print('found one')
                        return cc
            else:
                no_search.add(target_user_id)
        return -1

def return_overlapping_file_index(given_list=overlapping_studentid_list):
    '''
    return the file_index of all samples of overlapping students
    '''
    ufs=cluster.student_file_set_specific(given_list)
    cl=cluster.return_cluster(ufs)
    # -2 is user_id index in my label
    cll_file_index=[c for c in cl if int(load.label[c[0]][-2]) in given_list]
    return cll_file_index


#need to save to file