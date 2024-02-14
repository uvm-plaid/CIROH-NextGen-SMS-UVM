import matplotlib.pyplot as plt


accuracies = []
with open("optimization.csv", "r") as f:
    line = f.readline()
    line = f.readline()
    line = line.split(",")
    mfccs = line[0]
    trees = line[1]
