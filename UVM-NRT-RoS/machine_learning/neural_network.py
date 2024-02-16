"""
Deep Learning on .wav dataset using MFCC extraction and Neural Networks
"""
from sklearn.model_selection import train_test_split
import os
import librosa
import numpy as np
import tensorflow as tf
import tensorflow as tf
keras = tf.keras
from keras import Sequential, layers


def wav2MfccList(folder_path):
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
        mfccs = librosa.feature.mfcc(y=y, sr=sr, n_mfcc=1)

        # Append MFCC's to list
        mfcc_list.append(mfccs)

    # Convert the list of 2D arrays to a 3D NumPy array
    mfcc_array = np.array(mfcc_list)

    # Return the 3D NumPy array
    return mfcc_array

# Load corresponding files into lists
folder_paths = ["../microphone-sampling/TestingSamples/lightShower","../microphone-sampling/TestingSamples/mediumShower","../microphone-sampling/TestingSamples/heavyShower","../microphone-sampling/TestingSamples/heavyCouscousHail","../microphone-sampling/TestingSamples/lightCouscousHail","../microphone-sampling/TestingSamples/MasonJarRain","../microphone-sampling/TestingSamples/Nothing"]

light_rain = wav2MfccList(folder_paths[0])
medium_rain = wav2MfccList(folder_paths[1])
heavy_rain = wav2MfccList(folder_paths[2])
heavy_couscous = wav2MfccList(folder_paths[3])
light_couscous = wav2MfccList(folder_paths[4])
mason_rain = wav2MfccList(folder_paths[5])
nothing = wav2MfccList(folder_paths[6])

# Create Labels for Light, Medium, Heavy
light_rain_labels = np.full(len(light_rain),0)
med_rain_labels = np.full(len(medium_rain),1)
heavy_rain_labels = np.full(len(heavy_rain),2)
heavy_couscous_labels = np.full(len(heavy_couscous),3)
light_couscous_labels = np.full(len(light_couscous),4)
mason_rain_labels = np.full(len(mason_rain),5)
nothing_labels = np.full(len(nothing),6)

# Concatenate together the 10 NumPy arrays into one array
X = np.concatenate([light_rain,medium_rain,heavy_rain,heavy_couscous,light_couscous,mason_rain,nothing], axis=0)
y = np.concatenate([light_rain_labels,med_rain_labels,heavy_rain_labels,heavy_couscous_labels,light_couscous_labels,mason_rain_labels,nothing_labels], axis=0)

# Reshape the 3d array to 2d; sklearn can't process a 3d array
X = X.reshape(X.shape[0], -1)

# Convert to one-hot encoding
NUM_CLASSES = 7
y = np.eye(NUM_CLASSES)[y]

# Split the data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

model = Sequential()
model.add(layers.Dense(activation="relu",input_shape=(X_train[0].shape),units=128))
model.add(layers.Dropout(0.2))
model.add(layers.Dense(activation="relu",units=NUM_CLASSES))


model.compile(optimizer='adam',
              loss='categorical_crossentropy',
              metrics=['accuracy'])

model.fit(X_train, y_train, epochs=5)