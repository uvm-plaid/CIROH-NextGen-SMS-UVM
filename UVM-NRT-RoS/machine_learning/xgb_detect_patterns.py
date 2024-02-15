import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

df = pd.read_csv("xgb.csv")

accuracies_by_n_est = []
accuracies_by_lr = []

for n_mfcc_value in range(1, 30):
    mean_accuracy = df[df["num_estimators"] == n_mfcc_value]["accuracy"].mean()
    accuracies_by_n_est.append(mean_accuracy)

for n_mfcc_value in np.arange(0.1,0.6,0.1):
    mean_accuracy = df[df["learn_rate"] == n_mfcc_value]["accuracy"].mean()
    accuracies_by_lr.append(mean_accuracy)

print("Variance in Average Accuracy by Num Estimators",np.var(np.array(accuracies_by_n_est)))
print("Variance in Average Accuracy by Learning Rate",np.var(np.array(accuracies_by_lr)))

x_mfcc = [x for x in range(1,20)]
x_tree = [x for x in range(1,15)]


fig, axs = plt.subplots(2, 1, layout='constrained')
axs[0].plot(accuracies_by_n_est)
axs[1].plot(accuracies_by_lr)

axs[0].set_title("Average Accuracy by Num Estimators")
axs[1].set_title("Average Accuracy by Learning Rate")

plt.show()