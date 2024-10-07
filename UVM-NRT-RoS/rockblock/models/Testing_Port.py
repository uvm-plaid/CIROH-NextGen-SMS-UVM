import pandas as pd
from micromlgen import port
from sklearn import datasets
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC

# Loading test data from sci-kit learn and saving as csv
test_data = datasets.load_iris()
iris_df = pd.DataFrame(data=test_data.data, columns=test_data.feature_names)

# Prepping data for modeling
X = iris_df.drop(columns='target')
y = iris_df['target']
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.3, random_state=42)

# Creating, training, and exporting svm model
svm = SVC(kernel='rbf', gamma=0.01)
svm = svm.fit(X_train, y_train)
svm_c_code = port(svm)

file_path = "SVC.h"
with open(file_path, "w") as file:
    file.write(svm_c_code)


# Creating, training, and exporting random forest model
rf = RandomForestClassifier()
rf = rf.fit(X_train, y_train)
rf_c_code = port(rf)

file_path = "RFC.h"
with open(file_path, "w") as file:
    file.write(rf_c_code)
