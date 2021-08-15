#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar 23 11:39:05 2021

@author: mso257
"""
import numpy as np
import random
from pathlib import Path
from shutil import copyfile
import matplotlib.pyplot as plt
import numpy as np
from errno import EEXIST
from os import makedirs,path





def DataLoaderTrainTest(MainPath, SetSpk, NumVocab, SNR, mode):
    
    
    SpkLen=len(SetSpk)
    
    if mode == 'train':
        
        VocabIdx= random.sample(range(150,300),NumVocab*SpkLen)
        VocabIdx=[i+1 for i in VocabIdx]
        
    elif mode== 'test':
        
        VocabIdx= random.sample(range(0,150),NumVocab)
        VocabIdx=[i+1 for i in VocabIdx]
        
    else:
        
        print("A wrong mode was called. Please correct it...!")
        
    
    VocabIndxList= np.array_split(VocabIdx, SpkLen)
    
    ClnSpeechSavingPath= MainPath+'Ingegrated/'+mode+'/CleanSpeech'
    SNRSpeechSaningPath= MainPath+'Ingegrated/'+mode+'/SNR('+str(SNR)+')'
    
    Path(ClnSpeechSavingPath).mkdir(parents=True, exist_ok=True)  # create the path for clean data
    Path(SNRSpeechSaningPath).mkdir(parents=True, exist_ok=True)  # create the path for SNR


    
    for IdxSpk, Spk in enumerate(SetSpk):
    
        for vIdx  in VocabIndxList[IdxSpk]:
            
            
            copyfile(MainPath+Spk+'/clean speech_'+Spk+'/'+str(vIdx)+'.mat', ClnSpeechSavingPath+'/'+str(vIdx)+'.mat') # copy the selected cleen speech
            
            copyfile(MainPath+Spk+'/SNR('+str(SNR)+')_'+Spk+'/'+str(vIdx)+'.mat', SNRSpeechSaningPath+'/'+str(vIdx)+'.mat') # copy the speech with an particular SNR
        

    print("The "+mode+" data has been coppied")
    
    return VocabIdx



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
    plt.figure(figsize=(9, 6))
    plt.xlabel("Samples")
    plt.ylabel("Envelop")
    plt.title('Envelop of Word '+str(w)+' for Ch '+str(ch))
    plt.plot(x1,'--b' )
    plt.plot(x2,'-.r')
    plt.plot(x3, 'k')
    plt.legend([ "Noisy Speech Env","Clean Speech Env", "Predicted Clean Env"], loc='upper left')
    plt.savefig(Path+'/Words'+str(w)+'_Ch'+ str(ch)+'.png')
    plt.ioff()

        