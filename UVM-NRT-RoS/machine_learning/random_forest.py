"""
Machine Learning on .wav dataset using MFCC extraction and Random Forest
Spencer R. Karofsky
"""
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
from matplotlib import pyplot as plt
from MfccExtractor import MfccExtractor

most_accurate = 0
most_accurate_params = []
#with open("rf.txt","w") as f:
 #   f.write("Hyperparameter Optimization using Random Forest\n\n")
#with open("rf.csv", "w") as f:
  #  f.write("Hyperparameter Optimization using random forest\n")

mfcc_extractor = MfccExtractor(num_mfcc=1)
X_train,X_test,y_train,y_test = mfcc_extractor.get_data()


progress = 0
for num_trees in range(1,15):
    for m in range(1,20):
        progress += 1

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
       # with open("rf.csv","a") as f:
          #  f.write(f"{m},{num_trees},{accuracy}\n")

print("done")
#with open("rf.txt","a") as f:
   # f.write(f"\nMost accurate configuration is {most_accurate_params[0]} mfcc's, {most_accurate_params[1]} RF trees, which yielded {most_accurate_params[2]*100:.2f}% accuracy.")
