import pandas
from everywhereml.sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split

X, y = load_iris(return_X_y=True)
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42)


clf = RandomForestClassifier(n_estimators=5).fit(X_train, y_train)

forest_port = clf.to_arduino(instance_name="irisClassifier")
with open("IrisClassifier.h", "w") as outfile:
    outfile.write(forest_port)
