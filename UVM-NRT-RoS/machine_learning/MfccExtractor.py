import librosa
import numpy as np
from sklearn.model_selection import train_test_split
import os
import tensorflow as tf


class MfccExtractor:
    # Constructor
    def __init__(self,num_mfcc=1,flatten=True,one_hot_encode=False):
        self.num_mfcc = num_mfcc
        self.flatten = flatten
        self.one_hot_encode = one_hot_encode
        self.num_classes = None

    # Functions
    def wav2MfccList(self,folder_path):
        mfcc_list = []
        # shortest = 100000000
        for filename in os.listdir(folder_path):
            # Load the audio file
            y, sr = librosa.load(folder_path + '/' + filename)
            # if shortest > y.shape[0]:
            #   shortest = y.shape[0]
            #  print(shortest)
            # Trim the audio to the shortest audio clip
            y = y[:63681]

            # Extract MFCC features
            mfccs = librosa.feature.mfcc(y=y, sr=sr, n_mfcc=self.num_mfcc)

            # Append MFCC's to list
            mfcc_list.append(mfccs)

        # Convert the list of 2D arrays to a 3D NumPy array
        mfcc_array = np.array(mfcc_list)

        # Return the 3D NumPy array
        return mfcc_array

    def get_data(self):
        X = None
        y = []

        folder_paths = []
        with open("folder_paths.txt", "r") as f:
            self.num_classes = 0
            for line in f:
                line = line.rstrip("\n")
                mfcc_list = self.wav2MfccList(line)
                label_name = line.split("/")[-1]
                if self.one_hot_encode:
                    labels = np.full(len(mfcc_list), self.num_classes)
                else:
                    labels = np.full(len(mfcc_list),label_name)

                self.num_classes += 1

                if X is None:
                    X = mfcc_list
                    y = labels
                else:
                    X = np.concatenate([X, mfcc_list], axis=0)
                    y = np.concatenate([y, labels], axis=0)
        if self.flatten:
            # Reshape the 3d array to 2d; sklearn can't process a 3d array
            X = X.reshape(X.shape[0], -1)

        if self.one_hot_encode:
            # Convert to one-hot encoding
            y = tf.one_hot(y, depth=self.num_classes, dtype=tf.int32)
            X = np.array(X)
            y = np.array(y)

        X = np.squeeze(X, axis=1)
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

        return X_train, X_test, y_train, y_test

