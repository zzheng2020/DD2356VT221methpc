# Implementation of matplotlib function 
import numpy as np 
import matplotlib.pyplot as plt 
  
# example data 
xval = [1, 2, 4, 8, 12, 16, 20, 24, 28, 32]
yval = [11.057484, 5.490295, 3.240177, 2.076436, 1.433599,
        1.107265, 0.885072, 0.755384, 0.636473, 0.565894]
yer  = [549.951, 854.872, 3306.66, 1660.04, 1211.94,
        353.745, 696.446, 339.937, 403.758, 418.944]

# xval = ['static', 'dynamic', 'guided']
# yval = [216591, 62.7, 32811.8]
# yer  = [4395.82, 0.282843, 375.559]
  
plt.errorbar(xval, yval, fmt='.k', elinewidth=2, capsize=10) 
  
plt.title('time and the different number of threads')
# plt.xlabel('')
plt.ylabel('time (s)')


plt.show()