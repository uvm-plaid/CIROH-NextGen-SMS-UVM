import pandas as pd
from matplotlib import pyplot as plt

df = pd.read_csv("optimization.csv")
print(df.head())

accuracies_by_trees = []
accuracies_by_n_mfccs = []

accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 1].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 2].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 3].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 4].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 5].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 6].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 7].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 8].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 9].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 10].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 11].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 12].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 13].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 14].mean())
accuracies_by_trees.append(df["accuracy"][df["n_trees"] == 15].mean())

plt.plot(accuracies_by_trees)
plt.title("Average Accuracy by Number of Trees")
plt.show()


accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 1].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 2].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 3].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 4].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 5].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 6].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 7].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 8].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 9].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 10].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 11].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 12].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 13].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 14].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 15].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 16].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 17].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 18].mean())
accuracies_by_n_mfccs.append(df["accuracy"][df["n_mfcc"] == 19].mean())

plt.plot(accuracies_by_n_mfccs)
plt.title("Average Accuracy by Number of MFCC's")
plt.show()

