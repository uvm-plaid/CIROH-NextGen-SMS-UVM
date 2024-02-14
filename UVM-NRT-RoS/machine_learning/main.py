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
from matplotlib import pyplot as plt

most_accurate = 0
most_accurate_params = []
#with open("optimization.txt","w") as f:
 #   f.write("Hyperparameter Optimization using Random Forest\n\n")
#with open("optimization.csv", "w") as f:
  #  f.write("Hyperparameter Optimization using random forest\n")

progress = 0
for num_trees in range(1,15):
    for m in range(1,20):
        progress += 1
        def wav2MfccList(folder_path):
            mfcc_list = []
           # shortest = 100000000
            for filename in os.listdir(folder_path):
                # Load the audio file
                y, sr = librosa.load(folder_path + '/'+ filename)
                #if shortest > y.shape[0]:
                 #   shortest = y.shape[0]
                  #  print(shortest)
                # Trim the audio to the shortest audio clip
                y = y[:63681]

                # Extract MFCC features
                mfccs = librosa.feature.mfcc(y=y, sr=sr,n_mfcc=m)

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
        light_rain_labels = np.full(len(light_rain),"light rain")
        med_rain_labels = np.full(len(medium_rain),"medium rain")
        heavy_rain_labels = np.full(len(heavy_rain),"heavy rain")
        heavy_couscous_labels = np.full(len(heavy_couscous),"heavy couscous hail")
        light_couscous_labels = np.full(len(light_couscous),"light couscous hail")
        mason_rain_labels = np.full(len(mason_rain),"mason jar rain")
        nothing_labels = np.full(len(nothing),"nothing")

        # Concatenate together the 10 NumPy arrays into one array
        X = np.concatenate([light_rain,medium_rain,heavy_rain,heavy_couscous,light_couscous,mason_rain,nothing], axis=0)
        y = np.concatenate([light_rain_labels,med_rain_labels,heavy_rain_labels,heavy_couscous_labels,light_couscous_labels,mason_rain_labels,nothing_labels], axis=0)

        # Reshape the 3d array to 2d; sklearn can't process a 3d array
        X = X.reshape(X.shape[0], -1)

        # Split the data
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

        # Create a Random Forest classifier
        rf_classifier = RandomForestClassifier(n_estimators=num_trees, random_state=42)

        # Fit the model to the training data
        rf_classifier.fit(X_train, y_train)

        # Make predictions on the test data
        predictions = rf_classifier.predict(X_test)

        # Evaluate the accuracy
        accuracy = accuracy_score(y_test, predictions)
        if accuracy > most_accurate:
            most_accurate = accuracy
            most_accurate_params = []
            most_accurate_params.append(m)
            most_accurate_params.append(num_trees)
            most_accurate_params.append(most_accurate)
        print(f"{progress}/{20*15}")
       # with open("optimization.csv","a") as f:
          #  f.write(f"{m},{num_trees},{accuracy}\n")

print("done")
#with open("optimization.txt","a") as f:
   # f.write(f"\nMost accurate configuration is {most_accurate_params[0]} mfcc's, {most_accurate_params[1]} RF trees, which yielded {most_accurate_params[2]*100:.2f}% accuracy.")

feature_names = [f"feature {i}" for i in range(X.shape[1])]

from sklearn.inspection import permutation_importance
import pandas as pd

result = permutation_importance(
    rf_classifier, X_test, y_test, n_repeats=10, random_state=42, n_jobs=2
)

forest_importances = pd.Series(result.importances_mean, index=feature_names)
for i, sample in enumerate(forest_importances):
    if sample != 0:
        print(i)


print(len(forest_importances))

from matplotlib import pyplot as plt
fig, ax = plt.subplots()
forest_importances.plot.bar(yerr=result.importances_std, ax=ax)
ax.set_title("Feature importances using permutation on full model")
ax.set_ylabel("Mean accuracy decrease")
fig.tight_layout()
plt.show()
