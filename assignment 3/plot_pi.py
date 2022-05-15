import matplotlib.pyplot as plt
import numpy as np

#4.1
number1 = [
    4,
    8,
    16,
    32,
    64,
    128]
time1 =[
    6.945,
    3.500,
    2.371,
    1.103,
    0.603,
    0.478
    ]


plt.plot(number1, time1, '-*')
plt.xlabel('number of processes')
plt.ylabel('execution time(s)')
plt.legend(loc=4) 
plt.title('Strong Scaling Test-Linear Reduction Algorithm')
plt.show()

#4.2
number2 = [
    4,
    8,
    16,
    32,
    64,
    128]
time2 =[
    6.893,
    3.912,
    2.898,
    1.017,
    0.625,
    0.481
    ]


plt.plot(number2, time2, '-*')
plt.xlabel('number of processes')
plt.ylabel('execution time(s)')
plt.legend(loc=4) 
plt.title('Strong Scaling Test-Binary Tree Reduction')
plt.show()