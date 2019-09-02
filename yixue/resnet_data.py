import load
import numpy as np
import torch
import cluster
import random

class Image():
    def __init__(self):
        self.data=load.data
        self.pos=load.pos
        self.confidence=load.confidence
        self.eye=load.eye
        self.label=load.label
        self.channel=144
        self.length=0

    def shuffle(self,part=4/5):
        '''
        return a dict of index shuffled dataset (partition of train and validation set)
        '''
        total_length=len(self.label)
        temp=list(range(total_length))
        random.shuffle(temp)
        two_thirds=int(part*total_length)
        partition={'train': temp[:two_thirds], 'validation':temp[two_thirds:]}
        return partition

    def delete_confidence(self, with_confidence_list):
        '''
        change [[x,y,confidence],....] to [[x,y],....]
        return a list
        '''
        without_confidence_list=[t[0:2] for t in with_confidence_list]
        return without_confidence_list

    def stack_one_image(self, file_index, temp_index, image_index, image_size):
        '''
        stack one image and convert it to tensor of shape (channel, height, width)
        return a tensor
        '''
        one_image=[]
        for i in range(self.channel):
            # build one channel
            frame_index=temp_index[i*image_size+image_index]
            # add facial landmarks
            one_channel=self.delete_confidence(self.data[file_index][frame_index].tolist())
            # add left pupil
            one_channel.append(self.eye[file_index][frame_index][0].tolist())
            # add left iris
            one_channel+=self.delete_confidence(self.eye[file_index][frame_index][1].tolist())
            # add right pupil
            one_channel.append(self.eye[file_index][frame_index][2].tolist())
            # add right iris
            one_channel+=self.delete_confidence(self.eye[file_index][frame_index][3].tolist())
            # add pos
            one_channel+=self.pos[file_index][frame_index].reshape(3,2).tolist()
            # convert to tensor
            one_channel=[torch.tensor(s) for s in one_channel]
            one_image.append(torch.stack(one_channel))
        # print(torch.stack(one_image).shape)
        return torch.stack(one_image)

    def stack_one_image_continuous_eye(self, file_index, temp_index, image_index, image_size):
        '''
        stack one image and convert it to tensor of shape (channel, height, width)
        return a tensor
        '''
        one_image=[]
        for i in range(self.channel):
            # build one channel
            frame_index=temp_index[i*image_size+image_index]
            # add left iris
            one_channel=self.delete_confidence(self.eye[file_index][frame_index][1].tolist())
            # add left pupil
            one_channel.append(self.eye[file_index][frame_index][0].tolist())
            # add right pupil
            one_channel.append(self.eye[file_index][frame_index][2].tolist())
            # add right iris
            one_channel+=self.delete_confidence(self.eye[file_index][frame_index][3].tolist())
            # add pos
            one_channel+=self.pos[file_index][frame_index].reshape(3,2).tolist()
            # convert to tensor
            one_channel=[torch.tensor(s) for s in one_channel]
            one_image.append(torch.stack(one_channel))
        # print(torch.stack(one_image).shape)
        return torch.stack(one_image)

    def stack_one_image_continuous(self, file_index, temp_index, image_index, image_size):
        '''
        stack one image and convert it to tensor of shape (channel, height, width)
        return a tensor
        '''
        one_image=[]
        for i in range(self.channel):
            # build one channel
            frame_index=temp_index[i+image_index*self.channel]
            # add facial landmarks
            one_channel=self.delete_confidence(self.data[file_index][frame_index].tolist())
            # add left pupil
            one_channel.append(self.eye[file_index][frame_index][0].tolist())
            # add left iris
            one_channel+=self.delete_confidence(self.eye[file_index][frame_index][1].tolist())
            # add right pupil
            one_channel.append(self.eye[file_index][frame_index][2].tolist())
            # add right iris
            one_channel+=self.delete_confidence(self.eye[file_index][frame_index][3].tolist())
            # add pos
            one_channel+=self.pos[file_index][frame_index].reshape(3,2).tolist()
            # convert to tensor
            one_channel=[torch.tensor(s) for s in one_channel]
            one_image.append(torch.stack(one_channel))
        return torch.stack(one_image)

    def return_data(self):
        '''
        return label and data as a 4d tensor (batch size, channel, height, width)
        '''
        images=[]
        labels=[]
        for i in range(len(self.label)):
            # process one 'file' at a time
            temp_confidence=[float(t) for t in self.confidence[i][1:]]
            # pick out the index for those with confidence larger than 0.4. i+1 because 0th is file name
            temp_index=[i+1 for i in range(len(temp_confidence)) if temp_confidence[i]>=0.4]
            temp_length=len(temp_index)
            # calculate the valid number of image (each is a stack of self.channel frames)
            temp_image_size=int(temp_length/self.channel)
            # add is_correct labels
            labels+=[int(self.label[i][3])]*temp_image_size
            # cut out the last few frames
            temp_index=temp_index[:(temp_image_size*self.channel)]
            for j in range(temp_image_size):
                # stach one image
                temp_image=self.stack_one_image(i,temp_index,j,temp_image_size)
                # resize to have channel of 3
                multiplier=temp_image.shape[0]/3
                temp_image=temp_image.view(3,temp_image.shape[2]*multiplier, temp_image.shape[2])
                images.append(temp_image)
        return torch.stack(images),labels

    def return_data_partition(self, type=0, partition=None):
        '''
        return label and data as a 4d tensor (batch size, channel, height, width)
        return (a training dataset, label) and (a validation data, label).
        '''
        if partition==None:
            partition=self.shuffle()
        train_images=[]
        train_labels=[]
        validation_images=[]
        validation_labels=[]
        for i in partition['train']:
            # process one 'file' at a time
            temp_confidence=[float(t) for t in self.confidence[i][1:]]
            # pick out the index for those with confidence larger than 0.4. i+1 because 0th is file name
            temp_index=[i+1 for i in range(len(temp_confidence)) if temp_confidence[i]>=0.4]
            temp_length=len(temp_index)
            # calculate the valid number of image (each is a stack of self.channel frames)
            temp_image_size=int(temp_length/self.channel)
            # add is_correct labels
            train_labels+=[int(self.label[i][3])]*temp_image_size
            # cut out the last few frames
            temp_index=temp_index[:(temp_image_size*self.channel)]
            for j in range(temp_image_size):
                # stach one image
                if type==0: 
                    temp_image=self.stack_one_image(i,temp_index,j,temp_image_size)
                elif type==1: # continuous:
                    temp_image=self.stack_one_image_continuous(i,temp_index,j,temp_image_size)
                elif type==2: # continuous and only eye data
                    temp_image=self.stack_one_image_continuous_eye(i,temp_index,j,temp_image_size)
                # resize to have channel of 3
                multiplier=int(temp_image.shape[0]/3)
                temp_image=temp_image.view(3,temp_image.shape[1]*multiplier, temp_image.shape[2])
                train_images.append(temp_image)

        for i in partition['validation']:
            # process one 'file' at a time
            temp_confidence=[float(t) for t in self.confidence[i][1:]]
            # pick out the index for those with confidence larger than 0.4. i+1 because 0th is file name
            temp_index=[i+1 for i in range(len(temp_confidence)) if temp_confidence[i]>=0.4]
            temp_length=len(temp_index)
            # calculate the valid number of image (each is a stack of self.channel frames)
            temp_image_size=int(temp_length/self.channel)
            # add is_correct labels
            validation_labels+=[int(self.label[i][3])]*temp_image_size
            # cut out the last few frames
            temp_index=temp_index[:(temp_image_size*self.channel)]
            for j in range(temp_image_size):
                # stach one image
                if type==0: 
                    temp_image=self.stack_one_image(i,temp_index,j,temp_image_size)
                elif type==1: # continuous:
                    temp_image=self.stack_one_image_continuous(i,temp_index,j,temp_image_size)
                elif type==2: # continuous and only eye data
                    temp_image=self.stack_one_image_continuous_eye(i,temp_index,j,temp_image_size)                # resize to have channel of 3
                multiplier=int(temp_image.shape[0]/3)
                temp_image=temp_image.view(3,temp_image.shape[1]*multiplier, temp_image.shape[2])
                validation_images.append(temp_image)

        return (torch.stack(train_images),train_labels),(torch.stack(validation_images),validation_labels)

    # def return_data_partition_continuous(self, partition=None):
    #     '''
    #     return label and data as a 4d tensor (batch size, channel, height, width)
    #     return (a training dataset, label) and (a validation data, label).
    #     '''
    #     if partition==None:
    #         partition=self.shuffle()
    #     train_images=[]
    #     train_labels=[]
    #     validation_images=[]
    #     validation_labels=[]
    #     for i in partition['train']:
    #         # process one 'file' at a time
    #         temp_confidence=[float(t) for t in self.confidence[i][1:]]
    #         # pick out the index for those with confidence larger than 0.4. i+1 because 0th is file name
    #         temp_index=[i+1 for i in range(len(temp_confidence)) if temp_confidence[i]>=0.4]
    #         temp_length=len(temp_index)
    #         # calculate the valid number of image (each is a stack of self.channel frames)
    #         temp_image_size=int(temp_length/self.channel)
    #         # add is_correct labels
    #         train_labels+=[int(self.label[i][3])]*temp_image_size
    #         # cut out the last few frames
    #         temp_index=temp_index[:(temp_image_size*self.channel)]
    #         for j in range(temp_image_size):
    #             # stach one image
    #             temp_image=self.stack_one_image_continuous(i,temp_index,j,temp_image_size)
    #             # resize to have channel of 3
    #             multiplier=int(temp_image.shape[0]/3)
    #             temp_image=temp_image.view(3,temp_image.shape[1]*multiplier, temp_image.shape[2])
    #             train_images.append(temp_image)

    #     for i in partition['validation']:
    #         # process one 'file' at a time
    #         temp_confidence=[float(t) for t in self.confidence[i][1:]]
    #         # pick out the index for those with confidence larger than 0.4. i+1 because 0th is file name
    #         temp_index=[i+1 for i in range(len(temp_confidence)) if temp_confidence[i]>=0.4]
    #         temp_length=len(temp_index)
    #         # calculate the valid number of image (each is a stack of self.channel frames)
    #         temp_image_size=int(temp_length/self.channel)
    #         # add is_correct labels
    #         validation_labels+=[int(self.label[i][3])]*temp_image_size
    #         # cut out the last few frames
    #         temp_index=temp_index[:(temp_image_size*self.channel)]
    #         for j in range(temp_image_size):
    #             # stach one image
    #             temp_image=self.stack_one_image_continuous(i,temp_index,j,temp_image_size)
    #             # resize to have channel of 3
    #             multiplier=int(temp_image.shape[0]/3)
    #             temp_image=temp_image.view(3,temp_image.shape[1]*multiplier, temp_image.shape[2])
    #             validation_images.append(temp_image)

    #     return (torch.stack(train_images),train_labels),(torch.stack(validation_images),validation_labels)



image=Image()
(train_image_,train_label_),(validation_image_,validation_label_)=image.return_data_partition(type=1)
# assert(train_image_[0].shape==(3,71*10,3))