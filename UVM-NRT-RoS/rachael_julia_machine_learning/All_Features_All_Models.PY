'''
Rachael Chertok and Julia Sober
Audio Machine Learning for Rain on Snow Detection

In this file, the following features
Autocorrelation Coefficient (ACC), Zero Crossing Rate (ZCR), Temporal Entropy (Ht), Spectral Entropy (Hf),
Acoustic Complexity Index (ACI), Spectral Cover (SC)

TODO: Add Background Noise features (BgN)

The following models are used: Support Vector Machine (SVM), Random Forests (RF), XGBoost (XGB), and Logistic Regression (LR)

TODO: Try different hyperparams for RF etc.

Feature importance is used to determine which audio features add to the model
TODO: Implement feature importance
'''



import librosa
import librosa.display
import librosa.feature
import matplotlib.pyplot as plt
import numpy as np
import os
import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
from sklearn.model_selection import LeaveOneOut, train_test_split
from sklearn.svm import SVC
from sklearn.linear_model import LogisticRegression
from sklearn.inspection import permutation_importance
import statsmodels.api as sm
import maad
from maad.features import temporal_entropy
from maad.features import spectral_entropy
import scipy
from scipy import signal

import xgboost as xgb
from sklearn.ensemble import GradientBoostingClassifier

def main():
    #--------------Extracting audio features for each category of audio samples-------------------------------
    # light_rain, medium_rain, etc. will contain all of the audio features, concatenated, after the function returns
    light_rain_features = wav_to_audio_features("../microphone-sampling/TestingSamples/lightShower/")
    # print(light_rain_features.shape)
    medium_rain_features = wav_to_audio_features("../microphone-sampling/TestingSamples/mediumShower/")
    # print(medium_rain_features.shape)
    heavy_rain_features = wav_to_audio_features("../microphone-sampling/TestingSamples/heavyShower/")
    # print(heavy_rain_features.shape)
    heavy_couscous_features = wav_to_audio_features("../microphone-sampling/TestingSamples/heavyCouscousHail/")
    # print(heavy_couscous_features.shape)
    light_couscous_features = wav_to_audio_features("../microphone-sampling/TestingSamples/lightCouscousHail/")
    # print(light_couscous_features.shape)
    mason_rain_features = wav_to_audio_features("../microphone-sampling/TestingSamples/MasonJarRain/")
    # print(mason_rain_features.shape)
    nothing_features = wav_to_audio_features("../microphone-sampling/TestingSamples/Nothing/")
    # print(nothing_features.shape)
    watering_can_features = wav_to_audio_features("../microphone-sampling/TestingSamples/WateringCan/")
    # print(watering_can_features.shape)

    
    #--------------------Create Labels for Light, Medium, Heavy, etc.-------------------------------------
    light_rain_labels = np.full(len(light_rain_features), "light rain")
    med_rain_labels = np.full(len(medium_rain_features), "medium rain")
    heavy_rain_labels = np.full(len(heavy_rain_features), "heavy rain")
    heavy_couscous_labels = np.full(len(heavy_couscous_features), "heavy couscous hail")
    light_couscous_labels = np.full(len(light_couscous_features), "light couscous hail")
    mason_rain_labels = np.full(len(mason_rain_features), "mason jar rain")
    nothing_labels = np.full(len(nothing_features), "nothing")
    watering_can_labels = np.full(len(watering_can_features), "watering can")

    #----------------------Concatenating the features and labels into X (features) and Y (labels)----------------
    # Concatenate together the 8 NumPy arrays into one array, which will be the features array
    X = np.concatenate([light_rain_features, medium_rain_features, heavy_rain_features, heavy_couscous_features,
                        light_couscous_features, mason_rain_features, nothing_features, watering_can_features], axis=0)
    # Concatenate together the 8 NumPy LABEL arrays into one array, which will be the target array
    Y = np.concatenate(
        [light_rain_labels, med_rain_labels, heavy_rain_labels, heavy_couscous_labels, light_couscous_labels,
         mason_rain_labels, nothing_labels, watering_can_labels], axis=0)
    df = pd.DataFrame(X)
    df['target'] = Y

    #------------------------------------Run the models-------------------------------------------------------
    model_accuracies = run_models(X, Y)

    #-----------------------Graph accuracies based on the accuracies of each model and validation form----------
    plot_accuracies(model_accuracies)


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

        #TODO: Try maad.sound.load for Ht, Hf, ACI and SC?? See if that makes a difference??


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


        #Extracting Temporal Entropy (Ht)
        Ht = extract_Ht(x)
        Ht_list.append([Ht])

        #Extract Spectral Entropy (Hf)
        #TODO: Maybe try power spectrum instead? See if you can get it working
        # Need to provide a spectrogram (we will use a PSD (power spectral density), as recommended by the spectral_entropy function)
        # pxx, frequencies = plt.psd(x, sr)

        Hf = extract_Hf(sxx, f)
        Hf_list.append([Hf])

        # -------------Acoustic Complexity Index (ACI)--------------------
        #Use sxx from the spectrogram
        ACI = extract_ACI(sxx)
        ACI_list.append([ACI])


        #-------------Spectral Cover (SC)---------------------------------
        #Using the maad spectrogram for SC to get the ext variable,
        #to be able to get the spectrogram with no noise.
        #TODO: Should spectral entropy also use maad.sound.spetrogram? Try interchanging theses

        SC = extract_SC(x, sr)
        SC_list.append(SC)

    #Convert all lists into arrays to return and be used by
    ACC_array = np.array(ACC_list)
    ZCR_array = np.array(ZCR_list)
    Ht_array = np.array(Ht_list)
    Hf_array = np.array(Hf_list)
    ACI_array = np.array(ACI_list)
    SC_array = np.array(SC_list)

    combined_array = np.concatenate((ACC_array, ZCR_array, Ht_array, Hf_array, ACI_array, SC_array), axis = 1)
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
    # It seems like all of the parameters need to be listed to get the correct output
    # Use EAS because that is the Entropy of Average SpectruM
    EAS, ECU, ECV, EPS, EPS_KURT, EPS_SKEW = maad.features.spectral_entropy(spectrogram, frequency_samples)
    #TODO: Should only EAS be used? Would it help if we also used the other outputs? Look into
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
    # Performing SVM, RF, and XGBoost algorithms with and without leave one out cross validation
    # and recording the accuracies
    all_accuracies = []

    #feature_names = ["ACC", "ZCR", "Ht", "Hf", "ACI", "SC"]
    feature_names = [f"feature{i}" for i in range(X.shape[1])]
    print(X.shape[1])

    #-------------------------IMPLEMENTING SVM--------------------------------------
    svc_object = SVC(kernel='linear')

    loo = LeaveOneOut()
    accuracies = []
    for train_index, test_index in loo.split(X):
        X_train, X_test = X[train_index], X[test_index]
        Y_train, Y_test = Y[train_index], Y[test_index]

        svc_object.fit(X_train, Y_train)
        accuracy = svc_object.score(X_test, Y_test)
        accuracies.append(accuracy)

    mean_accuracy_SVM_LOO = np.mean(accuracies)
    all_accuracies.append(mean_accuracy_SVM_LOO)
    print("Mean Accuracy (SVM): ", mean_accuracy_SVM_LOO)

    # SVM without LOO
    svc_object = SVC(kernel='linear')
    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=42)
    svc_object.fit(X_train, Y_train)
    accuracy_SVM = svc_object.score(X_test, Y_test)
    all_accuracies.append(accuracy_SVM)
    print("Accuracy (SVM no LOO):", accuracy_SVM)

    #-------------------------IMPLEMENTING RANDOM FOREST-------------------------------
    loo = LeaveOneOut()
    accuracies_rf = []

    #Random Forest Leave One Out
    for train_index, test_index in loo.split(X):
        X_train, X_test = X[train_index], X[test_index]
        Y_train, Y_test = Y[train_index], Y[test_index]

        clf = RandomForestClassifier(n_estimators=100, random_state=42)
        clf.fit(X_train, Y_train)
        Y_pred = clf.predict(X_test)
        accuracy = accuracy_score(Y_test, Y_pred)
        accuracies_rf.append(accuracy)

    mean_accuracy_RF_LOO = np.mean(accuracies_rf)
    all_accuracies.append(mean_accuracy_RF_LOO)
    print("Mean Accuracy (RF): ", mean_accuracy_RF_LOO)

    # Random Forest without leave one out
    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=42)
    clf = RandomForestClassifier(n_estimators=100, random_state=42)
    clf.fit(X_train, Y_train)
    Y_pred = clf.predict(X_test)
    accuracy_RF = accuracy_score(Y_test, Y_pred)
    all_accuracies.append(accuracy_RF)
    print("Accuracy (RF no LOO):", accuracy_RF)

    #Feature importance
    #TODO: Change n_repeats and n_jobs?

    '''
    result_rf = permutation_importance(
        clf, X_test, Y_test, n_repeats=10, random_state=42, n_jobs=2
    )
    rf_importances = pd.Series(result_rf.importances_mean, index=feature_names)

    fig, ax = plt.subplots()
    rf_importances.plot.bar(yerr=result_rf.importances_std, ax=ax)
    ax.set_title("Feature importances using permutation on full model: RF")
    ax.set_ylabel("Mean accuracy decrease")
    fig.tight_layout()
    plt.show()

    '''
    #-------------------------IMPLEMENTING XgBoost--------------------------------------
    '''
    print("In XGBOOST")
    loo = LeaveOneOut()
    accuracies_xgboost = []

    for train_index, test_index in loo.split(X):
        X_train, X_test = X[train_index], X[test_index]
        Y_train, Y_test = Y[train_index], Y[test_index]

        #clf = xgb.XGBClassifier(n_estimators=100, random_state=42)
        clf = GradientBoostingClassifier(n_estimators=100, random_state=42)
        clf.fit(X_train, Y_train)
        Y_pred = clf.predict(X_test)
        accuracy = accuracy_score(Y_test, Y_pred)
        accuracies_xgboost.append(accuracy)

    mean_accuracy_RF_LOO = np.mean(accuracies_xgboost)
    all_accuracies.append(mean_accuracy_RF_LOO)
    print("Mean Accuracy (XGBoost): ", mean_accuracy_RF_LOO)

    # XGBoost without leave one out
    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=42)
    clf = GradientBoostingClassifier(n_estimators=100, random_state=42)
    clf.fit(X_train, Y_train)
    Y_pred = clf.predict(X_test)
    accuracy_RF = accuracy_score(Y_test, Y_pred)
    all_accuracies.append(accuracy_RF)
    print("Accuracy (XGBoost no LOO):", accuracy_RF)

    #TODO: Try using this XGBoost instead?

    # Train a model using the scikit-learn API
    #xgb_classifier = GradientBoostingClassifier(n_estimators=100, objective='binary:logistic', tree_method='hist', eta=0.1, max_depth=3, enable_categorical=True)
    #xgb_classifier.fit(X_train, Y_train)

    # Convert the model to a native API model
    #model = xgb_classifier.get_booster()
    '''
    #-------------------------Implementing Logistic Regression--------------------------------------
    print("In Log Reg")
    loo = LeaveOneOut()
    accuracies_rf = []

    #Random Forest Leave One Out
    for train_index, test_index in loo.split(X):
        X_train, X_test = X[train_index], X[test_index]
        Y_train, Y_test = Y[train_index], Y[test_index]

        clf = LogisticRegression(random_state=0, max_iter = 150)
        clf.fit(X_train, Y_train)
        Y_pred = clf.predict(X_test)
        accuracy = accuracy_score(Y_test, Y_pred)
        accuracies_rf.append(accuracy)

    mean_accuracy_RF_LOO = np.mean(accuracies_rf)
    all_accuracies.append(mean_accuracy_RF_LOO)
    print("Mean Accuracy (LR): ", mean_accuracy_RF_LOO)

    # Random Forest without leave one out
    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.3, random_state=42)
    clf = LogisticRegression(random_state=0, max_iter = 150) #TODO: Change random_state?
    clf.fit(X_train, Y_train)
    Y_pred = clf.predict(X_test)
    accuracy_RF = accuracy_score(Y_test, Y_pred)
    all_accuracies.append(accuracy_RF)
    print("Accuracy (LR no LOO):", accuracy_RF)

    return all_accuracies

def plot_accuracies(all_accuracies):
    #-------------------------Plotting accuracies---------------------------------------------------
    # Plotting accuracies
    methods = ['SVM', 'SVM (w/o CV)', 'Random Forest', 'Random Forest (w/o CV)', 'Logistic Regression', 'Logistic Regression (w/o CV)']
    plt.figure(figsize=(30, 10))
    bars = plt.bar(methods, all_accuracies, color=['blue', 'orange', 'yellow', 'pink', 'purple', 'green'])
    plt.title('Accuracy Comparison All Audio Features')
    plt.xlabel('Method')
    plt.ylabel('Accuracy')
    plt.ylim(0, 1)
    for bar, accuracy in zip(bars, all_accuracies):
        plt.text(bar.get_x() + bar.get_width() / 2, bar.get_height(), f'{accuracy:.2f}', ha='center', va='bottom')
    plt.savefig('images/All_Audio_Features_SVM_RF_XGB_LR_Graph.png')
    plt.show()

if __name__=="__main__":
    main()




