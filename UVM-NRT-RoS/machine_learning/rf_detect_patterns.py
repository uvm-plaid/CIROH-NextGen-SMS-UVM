import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

df = pd.read_csv("rf.csv")

accuracies_by_trees = []
accuracies_by_n_mfccs = []

for n_mfcc_value in range(1, 15):
    mean_accuracy = df[df["n_trees"] == n_mfcc_value]["accuracy"].mean()
    accuracies_by_trees.append(mean_accuracy)

for n_mfcc_value in range(1, 20):
    mean_accuracy = df[df["n_mfcc"] == n_mfcc_value]["accuracy"].mean()
    accuracies_by_n_mfccs.append(mean_accuracy)

print("Variance in Average Accuracy by Num RF Tree's",np.var(np.array(accuracies_by_trees)))
print("Variance in Average Accuracy by Num MFCC's",np.var(np.array(accuracies_by_n_mfccs)))

x_mfcc = [x for x in range(1,20)]
x_tree = [x for x in range(1,15)]

# find log regression
mfcc_fit = np.polyfit(np.log(x_mfcc), accuracies_by_n_mfccs, 1)
print(mfcc_fit)

fig, axs = plt.subplots(2, 1, layout='constrained')
axs[0].plot(accuracies_by_n_mfccs)
axs[1].plot(accuracies_by_trees)

axs[0].set_title("Average Accuracy by Number of MFCC's")
axs[1].set_title("Average Accuracy by Number of RF Trees")

plt.suptitle("Random Forest Model Evaluation")
plt.savefig("rf.png")

plt.show()