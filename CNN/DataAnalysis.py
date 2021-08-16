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
from keras.layers.convolutional import MaxPooling1D

from window_slider import Slider
import scipy.io
import matplotlib.pyplot as plt
import numpy as np
from errno import EEXIST
from os import makedirs,path
import math
import random

# Initial Parameters

SNR=-6

# Data prepration

def DataPrepration(NoisySig,CleanSig,WindSize, NumShift):
    
    
    NoisyFeatSet=[]
    CleanFeatSet=[]

    TargetIndex = WindSize
    slider = Slider(WindSize,NumShift)
    slider.fit(NoisySig) 
    while True:
        window_data = slider.slide()
        # do your stuff
        NoisyFeatSet.append(window_data)
        CleanFeatSet.append(CleanSig[:,math.floor(TargetIndex/2)])
        TargetIndex= TargetIndex+NumShift
        if slider.reached_end_of_list(): break
    
    return  NoisyFeatSet, CleanFeatSet


def DataPrepration2(NoisySig,CleanSig,WindSize):
    
    
#    NoisySig=  [0] * (math.ceil(WindSize/2)) + NoisySig + [0] * (math.ceil(WindSize/2))
#    CleanSig=  [0] * (math.ceil(WindSize/2)) + CleanSig + [0] * (math.ceil(WindSize/2))
   
    
    NoisyFeatSet=[]
    CleanFeatSet=[]

    Index= list(np.sort(np.random.randint(WindSize,np.shape(NoisySig)[1] -WindSize, np.shape(NoisySig)[1])))
   


    for idx in Index:
        # do your stuff
        NoisyFeatSet.append(NoisySig[:, idx - math.floor(WindSize/2) : idx + math.ceil(WindSize/2)  ])
        CleanFeatSet.append(CleanSig[:,idx])

    
    return  NoisyFeatSet, CleanFeatSet




# Creaate the Dir
def mkdir_p(mypath):
    '''Creates a directory. equivalent to using mkdir -p on the command line'''



    try:
        makedirs(mypath)
    except OSError as exc: # Python >2.5
        if exc.errno == EEXIST and path.isdir(mypath):
            pass
        else: raise
        
# plot the data

def PlotingFeature(x1, x2, x3, w, ch): # plot the spectrogram
    
    Path='./ResultPlot/Word_'+str(w)
    mkdir_p(Path)
    plt.figure(figsize=(15, 8))
    plt.xlabel("Samples")
    plt.ylabel("Envelop")
    plt.title('Envelop of Word '+str(w)+' for Ch '+str(ch))
    plt.plot(x1,'--b' )
    plt.plot(x2,'-.r')
    plt.plot(x3, 'k')
    plt.legend([ "Noisy Speech Env","Clean Speech Env", "Predicted Clean Env"], loc='upper left')
    plt.savefig(Path+'/Words'+str(w)+'_Ch'+ str(ch)+'.png')
    plt.ioff()

def smooth(x,window_len=11,window='hanning'):
   
    if x.ndim != 1:
        raise ValueError ("smooth only accepts 1 dimension arrays.")

    if x.size < window_len:
        raise ValueError ("Input vector needs to be bigger than window size.")


    if window_len<3:
        return x


    if not window in ['flat', 'hanning', 'hamming', 'bartlett', 'blackman']:
        raise ValueError ("Window is on of 'flat', 'hanning', 'hamming', 'bartlett', 'blackman'")


    s=np.r_[x[window_len-1:0:-1],x,x[-2:-window_len-1:-1]]
    #print(len(s))
    if window == 'flat': #moving average
        w=np.ones(window_len,'d')
    else:
        w=eval('np.'+window+'(window_len)')

    y=np.convolve(w/w.sum(),s,mode='valid')
    return y



        
TrainCleanDataEnvLPF1=np.empty([1, 16])
TrainNoisyDataEnvLPF1=np.empty([1, 16])
TrainWordNum=5
n_steps=128
n_features=16
# overlap_step=0

for  i in range(TrainWordNum):

    TrainCleanSig = scipy.io.loadmat('/storage/mso257/SpeechIntelligibility/Data/CleanSpeech/'+str(i+1)+'.mat')
    TrainNoisySig=scipy.io.loadmat('/storage/mso257/SpeechIntelligibility/Data/SNR('+str(SNR)+')/'+str(i+1)+'.mat')    
        
    EachTrainCleanDataEnvLPF1=TrainCleanSig["Data_env_lpf1"]
    #TrainCleanDataDataToSubCh= TrainCleanSig["data_data2subCh"]
    #TrainCleanProcessedOutputMFile= TrainCleanSig["processed_output_m_file"]

    EachTrainNoisyDataEnvLPF1=TrainNoisySig["Data_env_lpf1"]
    #TrainNoisyDataDataToSubCh= TrainNoisySig["data_data2subCh"]
    #TrainNoisyProcessedOutputMFile= TrainNoisySig["processed_output_m_file"]

    TrainCleanDataEnvLPF1= np.concatenate((TrainCleanDataEnvLPF1,EachTrainCleanDataEnvLPF1), axis=0)
    TrainNoisyDataEnvLPF1= np.concatenate((TrainNoisyDataEnvLPF1,EachTrainNoisyDataEnvLPF1), axis=0)
    
TrainCleanDataEnvLPF1=np.delete(TrainCleanDataEnvLPF1,0,0)
# TrainCleanDataEnvLPF1=TrainCleanDataEnvLPF1.reshape((int((24000/n_steps)*TrainWordNum),n_steps * n_features))
TrainNoisyDataEnvLPF1=np.delete(TrainNoisyDataEnvLPF1,0,0)
# TrainNoisyDataEnvLPF1 = TrainNoisyDataEnvLPF1.reshape((int((24000/n_steps)*TrainWordNum), n_steps, n_features))  
    
TrainNoisyDataEnv,TrainCleanDataEnv = DataPrepration2(TrainNoisyDataEnvLPF1.T, TrainCleanDataEnvLPF1.T, n_steps)
TrainCleanDataEnv=np.array(TrainCleanDataEnv)

TrainNoisyDataEnv=np.array([TrainNoisyDataEnv[i].T for i in range(len(TrainNoisyDataEnv))])
# CNN Model

model = Sequential()
model.add(Conv1D(filters=64, kernel_size=3, activation='relu', input_shape=(n_steps, n_features), padding="same"))
model.add(MaxPooling1D(pool_size=2,padding="same"))
# model.add(Dropout(0.2))

# model.add(Conv1D(filters=32, kernel_size=3, activation='relu', padding="same"))
# model.add(MaxPooling1D(pool_size=2,padding="same"))
# model.add(Dropout(0.2))


model.add(Flatten())
model.add(Dense(500, activation='relu'))
model.add(Dense(n_features))
model.compile(optimizer='adam', loss='mean_absolute_error')

# fit model
model.fit(TrainNoisyDataEnv, TrainCleanDataEnv, epochs=10, verbose=1)


WindowSmoothSize=500
# Test Phase
# Words=[5,25,75,100,150,200,250, 290]
# Channels=[0, 1, 3, 5, 7, 10, 13, 15]

Words=[ 290]
Channels=[ 1, 7,  15]
for w in Words:
    
    
    SmoothedPred=[]
    
    TestNoisySig=scipy.io.loadmat('/storage/mso257/SpeechIntelligibility/Data/SNR('+str(SNR)+')/'+str(w)+'.mat')
    TestCleanSig=scipy.io.loadmat('/storage/mso257/SpeechIntelligibility/Data/CleanSpeech/'+str(w)+'.mat')
    TestNoisyDataEnv,TestCleanDataEnv= DataPrepration2(TestNoisySig["Data_env_lpf1"].T, TestCleanSig["Data_env_lpf1"].T, n_steps)
    TestNoisyDataEnv=[each.T for each in TestNoisyDataEnv]
    TestNoisyDataEnv= np.array(TestNoisyDataEnv)
    TestCleanDataEnv= np.array(TestCleanDataEnv)
    Prediction=model.predict(TestNoisyDataEnv)
    Prediction= Prediction.T
    
    for ch in range(16):
        
        EachChSmoothedPred=smooth(Prediction[ch,:],WindowSmoothSize)
        SmoothedPred.append(EachChSmoothedPred[0:24000-WindowSmoothSize-1])
        
    SmoothedPred=np.array( SmoothedPred)
    
     #Save file mat
    scipy.io.savemat('./ResultMatData/Test(SNR'+str(SNR)+')/'+str(w)+'.mat', mdict={'Prediction': SmoothedPred})
    
    for ch in Channels:
        
        
        PlotingFeature(TestNoisySig["Data_env_lpf1"].T[ch,:],TestCleanSig["Data_env_lpf1"].T[ch,:], Prediction[ch,:], w, ch+1)
        PlotingFeature(TestNoisySig["Data_env_lpf1"].T[ch,:],TestCleanSig["Data_env_lpf1"].T[ch,:], SmoothedPred[ch,:], w, ch+1)


































