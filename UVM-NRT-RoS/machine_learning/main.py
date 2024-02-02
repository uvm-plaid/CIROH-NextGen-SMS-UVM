"""
Machine Learning on .wav dataset using MFCC extraction and Random Forest
Spencer R. Karofsky
"""
import librosa
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
print("test")

'''
Parameters: number, the digit being spoken
Returns: mfcc_array, an array of MFCC's for the 10 audio samples for 
wav2MfccList reads in all 50 .wav samples for a number, extracts the MFCC's, and returns a NumPy array of length 50, where each element contains the MFCC's for that audio sample.
'''
def wav2MfccList(rain_type):
    mfcc_list = []
    for i in range(10):
        filepath = "./rain/UVM-NRT-RoS_microphone-sampling_TestingSamples_" + rain_type + str(i) + ".wav"
        # Load the audio file
        y, sr = librosa.load(filepath)
        print(y.shape)

        # Extract MFCC features
        mfccs = librosa.feature.mfcc(y=y, sr=sr, n_mfcc=20)

        print(mfccs.shape)

        # Pad or trim the MFCCs to a fixed length (e.g., 400)
        padded_mfccs = librosa.util.pad_center(mfccs, size=86, axis=1)
        print(padded_mfccs.shape)

        # Append MFCC's to list
        mfcc_list.append(padded_mfccs)

    # Convert the list of 2D arrays to a 3D NumPy array
    mfcc_array = np.array(mfcc_list)

    # Return the 3D NumPy array
    return mfcc_array

# Load corresponding files into lists
light = wav2MfccList("lightRain")
medium = wav2MfccList("mediumRain")
heavy = wav2MfccList("heavyRain")


# Concatenate together the 10 NumPy arrays into one array
mfcc = np.concatenate([light,medium,heavy], axis=0)

# Create y labels
y = []
for i in range(3):
    for j in range(10):
        y.append(i)

# Reshape the 3d array to 2d; sklearn can't process a 3d array
mfcc = mfcc.reshape(mfcc.shape[0], -1)

# Split the data
X_train, X_test, y_train, y_test = train_test_split(mfcc, y, test_size=0.6, random_state=42)

# Create a Random Forest classifier
rf_classifier = RandomForestClassifier(n_estimators=5, random_state=42)

# Fit the model to the training data
rf_classifier.fit(X_train, y_train)

# Make predictions on the test data
predictions = rf_classifier.predict(X_test)

# Evaluate the accuracy
accuracy = accuracy_score(y_test, predictions)
print(f"Model Accuracy: {accuracy*100:.2f}%")
