import librosa
import numpy as np
from sklearn.model_selection import train_test_split
import os
import tensorflow as tf


class AmplitudeEnvelopeExtractor:
    # Constructor
    def __init__(self,flatten=True,one_hot_encode=False):
        self.flatten = flatten
        self.one_hot_encode = one_hot_encode
        self.num_classes = None

    # Functions

    # Source for calculating AE: https://www.analyticsvidhya.com/blog/2022/05/comparison-of-the-rms-energy-and-the-amplitude-envelope/
    def wav2AmplitudeEnvelope(self,folder_path):
        ae_list = []
        # shortest = 100000000
        for filename in os.listdir(folder_path):
            # Load the audio file
            y, sr = librosa.load(folder_path + '/' + filename)
            # if shortest > y.shape[0]:
            #   shortest = y.shape[0]
            #  print(shortest)
            # Trim the audio to the shortest audio clip
            y = y[:63681]

            FRAME_SIZE = 1024
            HOP_LENGTH = 512

            ae = np.array([max(y[i:i + FRAME_SIZE]) for i in range(0, y.size, HOP_LENGTH)])

            # Append MFCC's to list
            ae_list.append(ae)

        # Convert the list of 2D arrays to a 3D NumPy array
        mfcc_array = np.array(ae_list)

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
                mfcc_list = self.wav2AmplitudeEnvelope(line)
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

#        X = np.squeeze(X, axis=1)
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

        return X_train, X_test, y_train, y_test

