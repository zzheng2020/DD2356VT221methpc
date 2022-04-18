# Implementation of matplotlib function 
import numpy as np 
import matplotlib.pyplot as plt 
  
# example data 
# xval = [1, 2, 4, 8, 12, 16, 20, 24, 28, 32]
# yval = [7233.54, 13312.8, 20032.1, 23582.0, 20917.6,
#         21175.4, 24931.8, 28348.8, 30917.1, 32568.2]
# yer  = [958.164, 1489.61, 6485.26, 3510.57, 2157.7,
#         670.91, 1241.95, 719.638, 740.836, 796.183]

xval = ['static', 'dynamic', 'guided']
yval = [216591, 62.7, 32811.8]
yer  = [5557.55, 0.357771, 448.197]
  
plt.errorbar(xval, yval, yerr = yer, fmt='.k', elinewidth=2, capsize=10) 
  
plt.title('bandwidth and different types of schedules')
# plt.xlabel('')
plt.ylabel('bandwidth')


plt.show()