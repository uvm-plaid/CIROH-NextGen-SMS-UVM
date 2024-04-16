"""
Machine Learning on .wav dataset using MFCC extraction and Random Forest
Spencer R. Karofsky
"""
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
from matplotlib import pyplot as plt
from MfccExtractor import MfccExtractor
from AmplitudeEnvelopeExtractor import AmplitudeEnvelopeExtractor
from RMS_Extractor import RMS_Extractor as RMS

most_accurate = 0
most_accurate_params = []
#with open("rf_rms.txt","w") as f:
 #   f.write("Hyperparameter Optimization using Random Forest\n\n")
#with open("rf_rms.csv", "w") as f:
#    f.write("Hyperparameter Optimization using random forest\n")

mfcc_extractor = MfccExtractor(num_mfcc=13)
ae_extractor = AmplitudeEnvelopeExtractor()
rms_extractor = RMS()
X_train,X_test,y_train,y_test = mfcc_extractor.get_data()
print(X_train.shape,X_test.shape,y_train.shape,y_test.shape)

#rf = RandomForestClassifier(n_estimators=6,random_state=42)
#rf.fit(X_train,y_train)
#predictions = rf.predict(X_test)
# Evaluate the accuracy
#accuracy = accuracy_score(y_test, predictions)
#print(accuracy)

for num_trees in range(1,15):
    # Create a Random Forest classifier
    rf_classifier = RandomForestClassifier(n_estimators=num_trees, random_state=42)

    # Fit the model to the training data
    rf_classifier.fit(X_train, y_train)

    # Make predictions on the test data
    predictions = rf_classifier.predict(X_test)

    # Evaluate the accuracy
    accuracy = accuracy_score(y_test, predictions)
    print(accuracy)
    if accuracy > most_accurate:
        most_accurate = accuracy
        most_accurate_params = []
        most_accurate_params.append(num_trees)
        most_accurate_params.append(most_accurate)
   # with open("rf_rms.csv","a") as f:
       # f.write(f"{num_trees},{accuracy}\n")

print("done")
#with open("rf_rms.txt","a") as f:
  #  f.write(f"\nMost accurate configuration is {most_accurate_params[0]} RF trees, which yielded {most_accurate_params[1]*100:.2f}% accuracy.")
