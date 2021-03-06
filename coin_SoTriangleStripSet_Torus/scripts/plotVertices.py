import sys
import csv
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d

# Read CSV
csvFileName = sys.argv[1]
csvData = []
with open(csvFileName, 'r') as csvFile:
    # csvReader = csv.reader(csvFile, delimiter=' ')
    csvReader = csv.reader(csvFile, delimiter=',')
    for csvRow in csvReader:
        csvData.append(csvRow)

# Get X, Y, Z
csvData = np.array(csvData)
csvData = csvData.astype(np.float)
X, Y, Z = csvData[:,0], csvData[:,1], csvData[:,2]

# # Plot X,Y,Z
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# ax.plot_trisurf(X, Y, Z, color='white', edgecolors='grey', alpha=0.5)
# ax.scatter(X, Y, Z, c='red')
# plt.show()



fig = plt.figure()
axes = plt.gca()
# axes.set_xlim([xmin,xmax])
# axes.set_ylim([ymin,ymax])
ax = fig.add_subplot(111, projection='3d')
numPoints = len(X)
for i in range(numPoints):
    ax.scatter(X[i], Y[i], Z[i], c='red')
    plt.pause(0.05)

plt.show()
