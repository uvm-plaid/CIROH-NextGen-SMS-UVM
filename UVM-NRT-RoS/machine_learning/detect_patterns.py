import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

df = pd.read_csv("rf.csv")
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

print("Variance in Average Accuracy by Num RF Tree's",np.var(np.array(accuracies_by_trees)))
print("Variance in Average Accuracy by Num MFCC's",np.var(np.array(accuracies_by_n_mfccs)))


fig, axs = plt.subplots(2, 1, layout='constrained')
axs[0].plot(accuracies_by_n_mfccs)
axs[1].plot(accuracies_by_trees)
axs[0].set_title("Average Accuracy by Number of MFCC's")
axs[1].set_title("Average Acccuracy by Number of RF Trees")
plt.show()