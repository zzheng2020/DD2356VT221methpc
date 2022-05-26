import matplotlib.pyplot as plt
import numpy as np

number = np.arange(1,17,1)

#N = M = 1000
time1 =[
    21.3247,
    17.0126,
    14.453,
    12.0731,
    11.8387,
    9.56103,
    9.15157,
    8.03189,
    7.46062,
    7.22373,
    7.31202,
    6.8363,
    7.08493,
    6.69515,
    6.66711,
    6.31601
    ]
speedup1 = [[time1[0]/i] for i in time1]

plt.plot(number, speedup1, '-*')
plt.xlabel('number of threads')
plt.ylabel('speedup')
plt.legend(loc=4) 
plt.title('Speedup with N = M = 1000')
plt.show()

#N = M = 2000
time2 =[
    213.358,
    131.595,
    101.787,
    78.1937,
    74.7028,
    64.5697,
    61.7464,
    50.7205,
    51.5862,
    47.772,
    44.7294,
    41.6831,
    39.9855,
    36.3467,
    35.752,
    34.7618
    ]

speedup2 = [[time2[0]/i] for i in time2]

plt.plot(number, speedup2, '-*')
plt.xlabel('number of threads')
plt.ylabel('speedup')
plt.legend(loc=4) 
plt.title('Speedup with N = M = 2000')
plt.show()