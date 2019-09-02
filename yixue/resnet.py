import load
import random
import numpy as np
import matplotlib.pyplot as plt
import shutil
from torchvision import models
import torch
from torch.autograd import Variable
import torch.nn as nn
import torch.nn.functional as F
from torch.optim import lr_scheduler
from torch import optim
from torchvision.datasets import ImageFolder
from torchvision.utils import make_grid
from torch.utils.data import Dataset,DataLoader
import time
import resnet_data

'''
because the dataset is largely unbalanced in terms of student distribution
we don't pick test/train set from different students, but from different video samples
'''

# if __name__ == 'resnet':
if __name__ == '__main__':

    train_image_,train_label_=resnet_data.train_image_,resnet_data.train_label_
    validation_image_,validation_label_=resnet_data.validation_image_,resnet_data.validation_label_    
    # print(image_[0])
    # print(label_[0])
    # print(image_.shape)

    class BaselineDataset_train(Dataset):
      def __init__(self):
            pass

      def __len__(self):
            return len(train_label_)

      def __getitem__(self, index):
            '''
            Generates one sample of data
            '''
            X = train_image_[index]
            y = train_label_[index]
            return X, y

    class BaselineDataset_validation(Dataset):
      def __init__(self):
            pass

      def __len__(self):
            return len(validation_label_)

      def __getitem__(self, index):
            '''
            Generates one sample of data
            '''
            X = validation_image_[index]
            y = validation_label_[index]
            return X, y

    # class BaselineDataset(Dataset):
    #   def __init__(self, sample_IDs):
    #         self.sample_IDs = sample_IDs

    #   def __len__(self):
    #         return len(self.sample_IDs)

    #   def __getitem__(self, index):
    #         '''
    #         Generates one sample of data
    #         '''
    #         # Select sample
    #         ID = self.sample_IDs[index]

    #         X = image_[ID]
    #         y = label_[ID]

    #         return X, y

    params = {'batch_size': 64,
              'shuffle': True,
              'num_workers': 0}

    max_epochs = 20

    # total_length=len(label_)
    # temp=list(range(total_length))
    # random.shuffle(temp)
    # two_thirds=int(2*total_length/3)
    # partition={'train': temp[:two_thirds], 'validation':temp[two_thirds:]}

    training_set = BaselineDataset_train()
    train_data_gen = DataLoader(training_set, **params)

    validation_set = BaselineDataset_validation()
    valid_data_gen = DataLoader(validation_set, **params)

    dataset_sizes = {'train':len(training_set),'valid':len(validation_set)}
    dataloaders = {'train':train_data_gen,'valid':valid_data_gen}

    model_ft = models.resnet18(pretrained=True)
    # modify output layer
    num_ftrs = model_ft.fc.in_features
    model_ft.fc = nn.Linear(num_ftrs, 2)
    # modify channels of input layer
    # padding may have some problem
    # model_ft.conv1.in_channels=10

    if torch.cuda.is_available():
        model_ft = model_ft.cuda()

    learning_rate = 0.001
    criterion = nn.CrossEntropyLoss()
    optimizer_ft = optim.SGD(model_ft.parameters(), lr=0.001, weight_decay=1e-2, momentum=0.9)
    exp_lr_scheduler = lr_scheduler.StepLR(optimizer_ft, step_size=7, gamma=0.1)

    def train_model(model, criterion, optimizer, scheduler, num_epochs=5):
        since = time.time()

        best_model_wts = model.state_dict()
        best_acc = 0.0

        for epoch in range(num_epochs):
            print('Epoch {}/{}'.format(epoch, num_epochs - 1))
            print('-' * 10)

            # Each epoch has a training and validation phase
            for phase in ['train', 'valid']:
                if phase == 'train':
                    scheduler.step()
                    model.train(True)  # Set model to training mode
                else:
                    model.train(False)  # Set model to evaluate mode

                running_loss = 0.0
                running_corrects = 0

                # Iterate over data.
                for data in dataloaders[phase]:
                    # get the inputs
                    inputs, labels = data
                    # print(inputs.shape)

                    # wrap them in Variable
                    if torch.cuda.is_available():
                        inputs = Variable(inputs.cuda())
                        labels = Variable(labels.cuda())
                    else:
                        inputs, labels = Variable(inputs), Variable(labels)

                    # zero the parameter gradients
                    optimizer.zero_grad()
                    
                    # forward
                    outputs = model(inputs)
                    _, preds = torch.max(outputs.data, 1)
                    loss = criterion(outputs, labels)

                    # print(outputs)
                    # print(preds)
                    # print(labels.data)

                    # backward + optimize only if in training phase
                    if phase == 'train':
                        loss.backward()
                        optimizer.step()

                    # statistics
                    # running_loss += loss.data[0]
                    running_loss += loss.item()
                    running_corrects += torch.sum(preds == labels.data).item()
                    # print(running_corrects)
                    # print('whole length ',dataset_sizes[phase])

                epoch_loss = running_loss / dataset_sizes[phase]
                epoch_acc = running_corrects / dataset_sizes[phase]

                print('{} Loss: {:.4f} Acc: {:.4f}'.format(
                    phase, epoch_loss, epoch_acc))

                # deep copy the model
                if phase == 'valid' and epoch_acc > best_acc:
                    best_acc = epoch_acc
                    best_model_wts = model.state_dict()

            print()

        time_elapsed = time.time() - since
        print('Training complete in {:.0f}m {:.0f}s'.format(
            time_elapsed // 60, time_elapsed % 60))
        print('Best val Acc: {:4f}'.format(best_acc))

        # load best model weights
        model.load_state_dict(best_model_wts)
        return model

    model_ft = train_model(model_ft, criterion, optimizer_ft, exp_lr_scheduler,
                       num_epochs=max_epochs)


