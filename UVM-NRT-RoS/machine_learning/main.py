"""
Machine Learning on .wav dataset using MFCC extraction and Random Forest
Spencer R. Karofsky
"""
import librosa
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
import os

'''
Parameters: number, the digit being spoken
Returns: mfcc_array, an array of MFCC's for the 10 audio samples for 
wav2MfccList reads in all 50 .wav samples for a number, extracts the MFCC's, and returns a NumPy array of length 50, where each element contains the MFCC's for that audio sample.
'''
def wav2MfccList(folder_path):
    mfcc_list = []
    for filename in os.listdir(folder_path):
        # Load the audio file
        y, sr = librosa.load(folder_path + '/'+ filename)

        # Trim the audio to the shortest audio clip
        y = y[:129302]

        # Extract MFCC features
        mfccs = librosa.feature.mfcc(y=y, sr=sr, n_mfcc=13)

        #print(mfccs.shape)

        # Append MFCC's to list
        mfcc_list.append(mfccs)

    # Convert the list of 2D arrays to a 3D NumPy array
    mfcc_array = np.array(mfcc_list)

    # Return the 3D NumPy array
    return mfcc_array

# Load corresponding files into lists
folder_paths = ["../microphone-sampling/TestingSamples/lightShower","../microphone-sampling/TestingSamples/mediumShower","../microphone-sampling/TestingSamples/heavyShower"]

light = wav2MfccList(folder_paths[0])
medium = wav2MfccList(folder_paths[1])
heavy = wav2MfccList(folder_paths[2])

# Create Labels for Light, Medium, Heavy
light_labels = np.full(len(light),"light rain")
med_labels = np.full(len(medium),"medium rain")
heavy_labels = np.full(len(heavy),"heavy rain")



# Concatenate together the 10 NumPy arrays into one array
X = np.concatenate([light,medium,heavy], axis=0)
y = np.concatenate([light_labels,med_labels,heavy_labels], axis=0)


# Reshape the 3d array to 2d; sklearn can't process a 3d array
X = X.reshape(X.shape[0], -1)


# Split the data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# Create a Random Forest classifier
rf_classifier = RandomForestClassifier(n_estimators=100, random_state=42)

# Fit the model to the training data
rf_classifier.fit(X_train, y_train)

# Make predictions on the test data
predictions = rf_classifier.predict(X_test)

# Evaluate the accuracy
accuracy = accuracy_score(y_test, predictions)
print(f"Model Accuracy: {accuracy*100:.2f}%")
