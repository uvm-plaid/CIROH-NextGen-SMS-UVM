# import modules
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# sklearn utilities
from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix, classification_report

# sklearn models
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.decomposition import PCA

# imports for feature extraction
import librosa
import librosa.display
import IPython.display as ipd
import os

# code adapted from https://www.hackersrealm.net/post/extract-features-from-audio-mfcc-python
'''
Need to:
    -Get the files (load them from the GitHub repository)
    -Extract the audio signal (in that in a function for the others? Do we do it first, separately?)
    -Get the features from each file
        *Zero crossing rate (Librosa)
        *Autocorrelation coefficient (Pandas) 
    (Note: We want to label each type as heavy rain, light rain, silent, etc.)
    -Train_test_split (
    -Run SVM
    -Leave one out cross validation of 10-fold cross validation?    
    
''''


def feature_extraction(file_path):
    # load the audio file
    x, sample_rate = librosa.load(file_path, res_type='kaiser_fast')
    # extract features from the audio
    mfcc = np.mean(librosa.feature.mfcc(y=x, sr=sample_rate, n_mfcc=50).T, axis=0)
    
    return mfcc

features = {}
directory = 'mircophone-sampling/TestingSamples/WateringCan/'
for audio in os.listdir(directory):
    audio_path = directory+audio
    features[audio_path] = feature_extraction(audio_path)


