import matplotlib.pyplot as plt
import numpy as np

x = []
for i in range(1,267):
    x.append(i)
y = []

with open("rf.csv", "r") as f:
    junk_line = f.readline()
    for line in f:
        line = line.split(",")
        y.append(float(line[2]))

#plt.yscale('log')
plt.plot(y)
plt.title("Model Accuracy")
plt.xticks([20,40,60,80,100,120,140,160,180,200,220,240,260,280])
plt.show()
