'''
Rachael Chertok and Julia Sober
Audio Machine Learning for Rain on Snow Detection

In this file, the following features
Autocorrelation Coefficient (ACC), Zero Crossing Rate (ZCR), Temporal Entropy (Ht), Spectral Entropy (Hf),
Acoustic Complexity Index (ACI), Spectral Cover (SC)

This file is specifically used for running a MLP (ANN)
'''

import librosa
import librosa.display
import librosa.feature
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
from sklearn.model_selection import train_test_split
import statsmodels.api as sm
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras.layers import Dense, Flatten
from keras import Sequential
import maad
from maad.features import temporal_entropy
import scipy
from scipy import signal
from sklearn.preprocessing import StandardScaler, MinMaxScaler

def main():
    # --------------Extracting audio features for each category of audio samples-------------------------------
    # light_rain, medium_rain, etc. will contain all the audio features, concatenated, after the function returns
    light_rain_features = wav_to_audio_features("../microphone-sampling/TestingSamples/lightShower/")
    medium_rain_features = wav_to_audio_features("../microphone-sampling/TestingSamples/mediumShower/")
    heavy_couscous_features = wav_to_audio_features("../microphone-sampling/TestingSamples/heavyCouscousHail/")
    light_couscous_features = wav_to_audio_features("../microphone-sampling/TestingSamples/lightCouscousHail/")
    #mason_rain_features = wav_to_audio_features("../microphone-sampling/TestingSamples/MasonJarRain/")
    nothing_features = wav_to_audio_features("../microphone-sampling/TestingSamples/Nothing/")
    watering_can_features = wav_to_audio_features("../microphone-sampling/TestingSamples/WateringCan/")
    real_snow_features = wav_to_audio_features("../microphone-sampling/TestingSamples/RealSnow/")

    # --------------------Create Labels for Light, Medium, Heavy, etc.-------------------------------------
    light_rain_labels = np.full(len(light_rain_features), 0)
    med_rain_labels = np.full(len(medium_rain_features), 1)
    heavy_couscous_labels = np.full(len(heavy_couscous_features), 2)
    light_couscous_labels = np.full(len(light_couscous_features), 3)
    #mason_rain_labels = np.full(len(mason_rain_features), 4)
    nothing_labels = np.full(len(nothing_features), 5)
    watering_can_labels = np.full(len(watering_can_features), 6)
    real_snow_labels = np.full(len(real_snow_features), 7)

    # ----------------------Concatenating the features and labels into X (features) and Y (labels)----------------
    # Concatenate together the 8 NumPy arrays into one array, which will be the features array
    X = np.concatenate([light_rain_features, medium_rain_features, heavy_couscous_features,
                        light_couscous_features, nothing_features, watering_can_features,
                        real_snow_features], axis=0)
    # Concatenate together the 8 NumPy LABEL arrays into one array, which will be the target array
    Y = np.concatenate(
        [light_rain_labels, med_rain_labels, heavy_couscous_labels, light_couscous_labels,
         nothing_labels, watering_can_labels, real_snow_labels], axis=0)
    df = pd.DataFrame(X)
    df['target'] = Y

    # ------------------------------------Run the models-------------------------------------------------------
    history = run_models(X, Y)
    plot(history)

def plot(history):
    plt.figure(figsize=(20, 10))
    plt.title('Model Accuracy by Epoch Using Multi-Layer Perceptron')
    plt.plot(history.history['accuracy'], label="Accuracy")
    plt.plot(history.history['val_accuracy'], label="Validation Accuracy")
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.legend()
    plt.savefig('images/ANN_Graph.png')

def wav_to_audio_features(folder_path):
    ACC_list = []
    ZCR_list = []
    Ht_list = []
    Hf_list = []
    ACI_list = []
    SC_list = []
    # os.listdir gets all files in the specified directory
    for filename in os.listdir(folder_path):
        # Librosa/File setup
        full_path = os.path.join(folder_path, filename)
        x, sr = librosa.load(full_path)

        # Get a spectrogram to use as the input to the spectral_entropy, ACI, and spectral cover functions
        # f is the array of sample frequencies
        # t is the array of segment times
        # sxx is a spectrogram of x (the audio signal, originally in the time domain
        f, t, sxx = scipy.signal.spectrogram(x, fs=sr)

        # Extracting Auto Correlation Coefficient
        acc = extract_ACC(x)
        ACC_list.append(acc)

        # Extracting Zero Crossing Rate
        zcr = extract_ZCR(x)
        ZCR_list.append(zcr)

        # Extracting Temporal Entropy (Ht)
        Ht = extract_Ht(x)
        Ht_list.append([Ht])

        # Extract Spectral Entropy (Hf)
        Hf = extract_Hf(sxx, f)
        Hf_list.append([Hf])

        # -------------Acoustic Complexity Index (ACI)--------------------
        # Use sxx from the spectrogram
        ACI = extract_ACI(sxx)
        ACI_list.append([ACI])

        # -------------Spectral Cover (SC)---------------------------------
        # Using the maad spectrogram for SC to get the ext variable,
        # to be able to get the spectrogram with no noise.
        SC = extract_SC(x, sr)
        SC_list.append(SC)

    # Convert all lists into arrays to return and be used by
    ACC_array = np.array(ACC_list)
    ZCR_array = np.array(ZCR_list)
    Ht_array = np.array(Ht_list)
    Hf_array = np.array(Hf_list)
    ACI_array = np.array(ACI_list)
    SC_array = np.array(SC_list)

    combined_array = np.concatenate((ACC_array, ZCR_array, Ht_array, Hf_array, ACI_array, SC_array), axis=1)
    return combined_array


def extract_ACC(audio_signal):
    return sm.tsa.acf(audio_signal, nlags=2000)


def extract_ZCR(audio_signal):
    zero_crossings = librosa.feature.zero_crossing_rate(audio_signal, pad=False)
    # pads zcr array with 0's so we can concatenate it with acc
    fixed_length_zcr = librosa.util.fix_length(zero_crossings[0], size=1000, mode='edge')
    return fixed_length_zcr


def extract_Ht(audio_signal):
    return maad.features.temporal_entropy(audio_signal)


def extract_Hf(spectrogram, frequency_samples):
    # It seems like all the parameters need to be listed to get the correct output
    # Use EAS because that is the Entropy of Average Spectrum
    EAS, ECU, ECV, EPS, EPS_KURT, EPS_SKEW = maad.features.spectral_entropy(spectrogram, frequency_samples)
    return EAS

def extract_ACI(spectrogram):
    _, _, ACI = maad.features.acoustic_complexity_index(spectrogram)
    return ACI

def extract_SC(audio_signal, sampling_rate):
    sxx_power, tn, fn, ext = maad.sound.spectrogram(audio_signal, sampling_rate)
    sxx_no_noise = maad.sound.median_equalizer(sxx_power, display=True, extent=ext)
    sxx_dB_no_noise = maad.util.power2dB(sxx_no_noise)
    LFC, MFC, HFC = maad.features.spectral_cover(sxx_dB_no_noise, fn)
    return [LFC, MFC, HFC]


def run_models(X, Y):
    # -------------------------Implementing Artificial Neural Network--------------------------------
    # Normalizing the data
    scaler = StandardScaler()
    X = scaler.fit_transform(X)

    xtrain, xtest, ytrain, ytest = train_test_split(X, Y, test_size=0.3, random_state=42)

    xtrain, xval, ytrain, yval = train_test_split(xtrain, ytrain, test_size=0.2, random_state=0)
    ytrain_1hot = tf.keras.utils.to_categorical(ytrain, num_classes=8)
    yval_1hot = tf.keras.utils.to_categorical(yval, num_classes=8)
    ytest_1hot = tf.keras.utils.to_categorical(ytest, num_classes=8)

    layers = [
        Flatten(),
        # Hidden layer with 100 neurons and relu activation
        Dense(100, activation=elu"),
        # Hidden layer with 100 neurons and relu activation
        Dense(100, activation="relu"),
        # Hidden layer with 100 neurons and relu activation
        Dense(100, activation="relu"),
        # Hidden layer with 100 neurons and relu activation
        Dense(100, activation="relu"),
        # Output layer with 8 neurons for 8 classes and softmax activation
        Dense(8, activation="softmax")
    ]

    # early stopping
    callback = keras.callbacks.EarlyStopping(monitor='loss', patience=3)

    model = Sequential(layers)

    model.compile(optimizer='SGD', loss="categorical_crossentropy", metrics=['accuracy'])

    # This will start the training and save each epoch output in the history list.
    history = model.fit(xtrain, ytrain_1hot, batch_size=16, epochs=50, validation_data=(xval, yval_1hot), callbacks=[callback])
    return history

if __name__ == "__main__":
    main()




