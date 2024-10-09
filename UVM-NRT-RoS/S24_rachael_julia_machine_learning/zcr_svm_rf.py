# This file is the same as zcr_acc_svm_rf.py but without acc

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

def wav_to_ZCR(folder_path):
    zcr_list = []
    # os.listdir gets all files in the specified directory
    for filename in os.listdir(folder_path):
        # Librosa/File setup
        full_path = os.path.join(folder_path, filename)
        x, sr = librosa.load(full_path)

        # Zero Crossing Rate
        zero_crossings = librosa.feature.zero_crossing_rate(x, pad=False)
        fixed_length_zcr = librosa.util.fix_length(zero_crossings[0], size=1000, mode='edge')
        zcr_list.append(fixed_length_zcr)

    zcr_array = np.array(zcr_list)
    return zcr_array


# light_rain, medium_rain, etc. contains the acc_array and zcr_array, concatenated, after the function returns
# So, these will contain the features
light_rain_features = wav_to_ZCR("../microphone-sampling/TestingSamples/lightShower/")
print(light_rain_features.shape)
medium_rain_features = wav_to_ZCR("../microphone-sampling/TestingSamples/mediumShower/")
#print(medium_rain_features.shape)
heavy_rain_features = wav_to_ZCR("../microphone-sampling/TestingSamples/heavyShower/")
#print(heavy_rain_features.shape)
heavy_couscous_features = wav_to_ZCR("../microphone-sampling/TestingSamples/heavyCouscousHail/")
#print(heavy_couscous_features.shape)
light_couscous_features = wav_to_ZCR("../microphone-sampling/TestingSamples/lightCouscousHail/")
#print(light_couscous_features.shape)
mason_rain_features = wav_to_ZCR("../microphone-sampling/TestingSamples/MasonJarRain/")
#print(mason_rain_features.shape)
nothing_features = wav_to_ZCR("../microphone-sampling/TestingSamples/Nothing/")
#print(nothing_features.shape)
watering_can_features = wav_to_ZCR("../microphone-sampling/TestingSamples/WateringCan/")
#print(watering_can_features.shape)

# Create Labels for Light, Medium, Heavy, etc.
light_rain_labels = np.full(len(light_rain_features),"light rain")
med_rain_labels = np.full(len(medium_rain_features),"medium rain")
heavy_rain_labels = np.full(len(heavy_rain_features),"heavy rain")
heavy_couscous_labels = np.full(len(heavy_couscous_features),"heavy couscous hail")
light_couscous_labels = np.full(len(light_couscous_features),"light couscous hail")
mason_rain_labels = np.full(len(mason_rain_features),"mason jar rain")
nothing_labels = np.full(len(nothing_features),"nothing")
watering_can_labels = np.full(len(watering_can_features),"watering can")

# Concatenate together the 8 NumPy arrays into one array, which will be the features array
X = np.concatenate([light_rain_features,medium_rain_features,heavy_rain_features,heavy_couscous_features,light_couscous_features,mason_rain_features,nothing_features,watering_can_features], axis=0)
# Concatenate together the 8 NumPy LABEL arrays into one array, which will be the target array
Y = np.concatenate([light_rain_labels, med_rain_labels, heavy_rain_labels, heavy_couscous_labels, light_couscous_labels, mason_rain_labels, nothing_labels, watering_can_labels], axis=0)
df = pd.DataFrame(X)
df['target'] = Y

# Performing SVM and RF algorithms with and without leave one out cross validation
# and recording the accuracies
all_accuracies = []

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


# IMPLEMENTING RANDOM FOREST
loo = LeaveOneOut()
accuracies_rf = []

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

# Plotting accuracies
methods = ['SVM', 'SVM (w/o CV)', 'Random Forest', 'Random Forest (w/o CV)']
plt.figure(figsize=(10, 6))
bars = plt.bar(methods, all_accuracies, color=['blue', 'orange', 'yellow', 'pink'])
plt.title('Accuracy Comparison ZCR Only')
plt.xlabel('Method')
plt.ylabel('Accuracy')
plt.ylim(0, 1)
for bar, accuracy in zip(bars, all_accuracies):
    plt.text(bar.get_x() + bar.get_width() / 2, bar.get_height(), f'{accuracy:.2f}', ha='center', va='bottom')
plt.savefig('images/ZCR_SVM_RF_Graph.png')
plt.show()
