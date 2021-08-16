#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May 14 22:03:21 2020

@author: Rahim_soleymanpour
"""


from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM, Dropout, Flatten
from keras.layers.convolutional import Conv1D
from keras.layers.convolutional import MaxPooling1D, AveragePooling1D
from keras.regularizers import l2
from keras import optimizers


import scipy.io
import matplotlib.pyplot as plt
import numpy as np
from errno import EEXIST
from os import makedirs,path
import os
from Utils import *
from pathlib import Path

os.environ["CUDA_VISIBLE_DEVICES"]="0"

# Initial Parameters

MainPath="/storage/mso257/SpeechIntelligibility/Data/DataWithoutKeywords/AllSpk/"



SNR=-8
TrainCleanDataEnvLPF1=np.empty([1, 16])
TrainNoisyDataEnvLPF1=np.empty([1, 16])
TrainWordNum=5
TrainSpkSet=["F4","M1","M2","M3","M5"] # ["F1","F3","F4","M1","M2","M3","M5"]
n_steps= 1
n_features=16

TestWordNum=150
TestSpkSet=["M4"]  # F2 and M4

TrainVocab=DataLoaderTrainTest(MainPath, TrainSpkSet, TrainWordNum, SNR, "train")
TestVocab=DataLoaderTrainTest(MainPath, TestSpkSet, TestWordNum, SNR, "test")
for  i in TrainVocab:

    TrainCleanSig = scipy.io.loadmat(MainPath+'Ingegrated/train/CleanSpeech/'+str(i)+'.mat')
    TrainNoisySig=scipy.io.loadmat(MainPath+'Ingegrated/train/SNR('+str(SNR)+')/'+str(i)+'.mat')    
        
    EachTrainCleanDataEnvLPF1=TrainCleanSig["Data_env_lpf1"]
    #TrainCleanDataDataToSubCh= TrainCleanSig["data_data2subCh"]
    #TrainCleanProcessedOutputMFile= TrainCleanSig["processed_output_m_file"]

    EachTrainNoisyDataEnvLPF1=TrainNoisySig["Data_env_lpf1"]
    #TrainNoisyDataDataToSubCh= TrainNoisySig["data_data2subCh"]
    #TrainNoisyProcessedOutputMFile= TrainNoisySig["processed_output_m_file"]

    TrainCleanDataEnvLPF1= np.concatenate((TrainCleanDataEnvLPF1,EachTrainCleanDataEnvLPF1), axis=0)
    TrainNoisyDataEnvLPF1= np.concatenate((TrainNoisyDataEnvLPF1,EachTrainNoisyDataEnvLPF1), axis=0)
    
TrainCleanDataEnvLPF1=np.delete(TrainCleanDataEnvLPF1,0,0)
TrainCleanDataEnvLPF1=TrainCleanDataEnvLPF1.reshape(len(TrainCleanDataEnvLPF1), n_features)
TrainNoisyDataEnvLPF1=np.delete(TrainNoisyDataEnvLPF1,0,0)
TrainNoisyDataEnvLPF1 = TrainNoisyDataEnvLPF1.reshape(len(TrainNoisyDataEnvLPF1), n_steps, n_features)  


# CNN Model

model = Sequential()
model.add(Conv1D(filters=32,strides=1, kernel_size=3, activation='relu',padding='same'))
model.add(MaxPooling1D(pool_size=2,padding='same'))
model.add(Dropout(0.2))

model.add(Conv1D(filters=64, strides=1, kernel_size=3, activation='relu',  padding="same"))
model.add(MaxPooling1D(pool_size=2,padding="same"))
model.add(Dropout(0.2))

model.add(Conv1D(filters=128, strides=1, kernel_size=3, activation='relu', padding="same"))
model.add(MaxPooling1D(pool_size=2,padding="same"))
model.add(Dropout(0.2))

model.add(Flatten())
model.add(Dense(256, activation='relu'))
model.add(Dense(n_features))
opt = optimizers.Adam(lr=0.0001)

model.compile(optimizer=opt ,loss='mean_absolute_error')
# model.compile(optimizer='adam', loss='mse')

# fit model
model.fit(TrainNoisyDataEnvLPF1,TrainCleanDataEnvLPF1, epochs=25, batch_size=128,verbose=1)


#%% Test Phase
# Words=[25,75,100,150,200,250, 290]
# Channels=[0, 1, 3, 5, 7, 10, 13, 15]

Words=  TestVocab  #TestVocab   #[ 105]
# Channels=[ 1, 4, 7, 13, 15]
Path('./ResultMatData/Integrated/AllSpk/Test(SNR'+str(SNR)+')_'+TestSpkSet[0]).mkdir(parents=True, exist_ok=True)  # create the path for clean data

for w in Words:
    
    TestNoisySig=scipy.io.loadmat(MainPath+'Ingegrated/test/SNR('+str(SNR)+')/'+str(w)+'.mat')
    TestCleanSig=scipy.io.loadmat(MainPath+'Ingegrated/test/CleanSpeech/'+str(w)+'.mat')
    TestNoisyDataEnvLPF1= TestNoisySig["Data_env_lpf1"].reshape((len(TestCleanSig["Data_env_lpf1"])), n_steps, n_features)
    TestCleanDataEnvLPF1= TestCleanSig["Data_env_lpf1"].T
    Prediction=model.predict(TestNoisyDataEnvLPF1)
    Prediction= Prediction.reshape((len(TestCleanSig["Data_env_lpf1"]), n_features)).T
    TestNoisyDataEnvLPF1= TestNoisySig["Data_env_lpf1"].T
    
    #Save file mat
    scipy.io.savemat('./ResultMatData/Integrated/AllSpk/Test(SNR'+str(SNR)+')_'+TestSpkSet[0]+'/'+str(w)+'.mat', mdict={'Prediction': Prediction})
    
 









