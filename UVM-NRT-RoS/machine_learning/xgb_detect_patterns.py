import pandas as pd
from matplotlib import pyplot as plt
import numpy as np

df = pd.read_csv("xgb.csv")

accuracies_by_n_est = []
accuracies_by_lr = []

for n_e in range(1, 30):
    mean_accuracy = df[df["num_estimators"] == n_e]["accuracy"].mean()
    accuracies_by_n_est.append(mean_accuracy)

for lr in np.arange(0.1,0.6,0.1):
    #fixing big in numpy
    if lr == 0.30000000000000004:
        lr = 0.3
    mean_accuracy = df[df["learn_rate"] == lr]["accuracy"].mean()
    print(lr,mean_accuracy)
    accuracies_by_lr.append(mean_accuracy)


print("Variance in Average Accuracy by Num Estimators",np.var(np.array(accuracies_by_n_est)))
print("Variance in Average Accuracy by Learning Rate",np.var(np.array(accuracies_by_lr)))


fig, axs = plt.subplots(2, 1, layout='constrained')
plt.suptitle("XGBoost Model Evaluation")
axs[0].plot(accuracies_by_n_est)
axs[1].plot(accuracies_by_lr)

axs[0].set_title("Average Accuracy by Num Estimators")
axs[1].set_title("Average Accuracy by Learning Rate")

plt.savefig("xgb.png")

plt.show()