ne#!/usr/bin/env python3
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
import scipy.io
import matplotlib.pyplot as plt
import numpy as np
from errno import EEXIST
from os import makedirs,path
import os

from Utils import * 

os.environ["CUDA_VISIBLE_DEVICES"]="0"

# Initial Parameters

SNR=0

TrainCleanDataEnvLPF1=np.empty([1, 16])
TrainNoisyDataEnvLPF1=np.empty([1, 16])
TrainWordNum=15
n_steps=1
n_features=16

for  i in range(TrainWordNum):

    TrainCleanSig = scipy.io.loadmat('/storage/mso257/SpeechIntelligibility/Data/DataWithoutKeywords/clean speech_M1/'+str(i+1)+'.mat')
    TrainNoisySig=scipy.io.loadmat('/storage/mso257/SpeechIntelligibility/Data/DataWithoutKeywords/SNR('+str(SNR)+')_M1/'+str(i+1)+'.mat')    
        
    EachTrainCleanDataEnvLPF1=TrainCleanSig["Data_env_lpf1"]
    #TrainCleanDataDataToSubCh= TrainCleanSig["data_data2subCh"]
    #TrainCleanProcessedOutputMFile= TrainCleanSig["processed_output_m_file"]

    EachTrainNoisyDataEnvLPF1=TrainNoisySig["Data_env_lpf1"]
    #TrainNoisyDataDataToSubCh= TrainNoisySig["data_data2subCh"]
    #TrainNoisyProcessedOutputMFile= TrainNoisySig["processed_output_m_file"]

    TrainCleanDataEnvLPF1= np.concatenate((TrainCleanDataEnvLPF1,EachTrainCleanDataEnvLPF1), axis=0)
    TrainNoisyDataEnvLPF1= np.concatenate((TrainNoisyDataEnvLPF1,EachTrainNoisyDataEnvLPF1), axis=0)
    
TrainCleanDataEnvLPF1=np.delete(TrainCleanDataEnvLPF1,0,0)
TrainCleanDataEnvLPF1=TrainCleanDataEnvLPF1.reshape((int((len(EachTrainCleanDataEnvLPF1)/n_steps)*TrainWordNum),n_steps * n_features))
TrainNoisyDataEnvLPF1=np.delete(TrainNoisyDataEnvLPF1,0,0)
TrainNoisyDataEnvLPF1 = TrainNoisyDataEnvLPF1.reshape((int((len(EachTrainNoisyDataEnvLPF1)/n_steps)*TrainWordNum), n_steps, n_features))  





# CNN Model

model = Sequential()
model.add(Conv1D(filters=32, kernel_size=3, activation='relu', input_shape=(n_steps, n_features), padding='same'))
model.add(MaxPooling1D(pool_size=2,padding='same'))
model.add(Dropout(0.2))

model.add(Conv1D(filters=64, kernel_size=3, activation='relu', padding="same"))
model.add(MaxPooling1D(pool_size=2,padding="same"))
model.add(Dropout(0.2))

# model.add(Conv1D(filters=16, kernel_size=3, activation='relu',  padding="same"))
# model.add(MaxPooling1D(pool_size=2,padding="same"))
# model.add(Dropout(0.2))

model.add(Flatten())
model.add(Dense(256, activation='relu'))
model.add(Dense(n_features*n_steps))
model.compile(optimizer='adam', loss='mean_absolute_error')
# model.compile(optimizer='adam', loss='mse')

# fit model
model.fit(TrainNoisyDataEnvLPF1, TrainCleanDataEnvLPF1, epochs=15, verbose=1)



# Test Phase
Words=[25,75,100,150,200,250, 290]
Channels=[0, 1, 3, 5, 7, 10, 13, 15]

# Words=[ 290]
Channels=[ 1, 4, 7, 13, 15]
for w in Words:
    
    TestNoisySig=scipy.io.loadmat('/storage/mso257/SpeechIntelligibility/Data/DataWithoutKeywords/SNR('+str(SNR)+')_M2/'+str(w)+'.mat')
    TestCleanSig=scipy.io.loadmat('/storage/mso257/SpeechIntelligibility/Data/DataWithoutKeywords/clean speech_M2/'+str(w)+'.mat')
    TestNoisyDataEnvLPF1= TestNoisySig["Data_env_lpf1"].reshape((int(len(TestCleanSig["Data_env_lpf1"])/n_steps), n_steps, n_features))
    TestCleanDataEnvLPF1= TestCleanSig["Data_env_lpf1"].T
    Prediction=model.predict(TestNoisyDataEnvLPF1)
    Prediction= Prediction.reshape((len(TestCleanSig["Data_env_lpf1"]), n_features)).T
    TestNoisyDataEnvLPF1= TestNoisySig["Data_env_lpf1"].T
    
    #Save file mat
    scipy.io.savemat('./ResultMatData/Test(SNR'+str(SNR)+')/'+str(w)+'.mat', mdict={'Prediction': Prediction})
    
    for ch in Channels:
        
        PlotingFeature(TestNoisyDataEnvLPF1[ch,:],TestCleanDataEnvLPF1[ch,:], Prediction[ch,:], w, ch+1)


def PlotingFeatureTemp(x): # plot the spectrogram
    
    
    
    fig, ax = plt.subplots(nrows=16 ,ncols=1)
    plt.figure(figsize=(9, 6))


    for row in ax:
        for col in row:
            
            col.plot(x[row,:])
            plt.xlabel("Samples")
            plt.ylabel("Envelop")
            plt.title('Envelop of Word '+str(w)+' for Ch '+str(row))
            plt.show()


PlotingFeatureTemp(TestNoisyDataEnvLPF1)
PlotingFeatureTemp(Prediction)
PlotingFeatureTemp(TestCleanDataEnvLPF1)














