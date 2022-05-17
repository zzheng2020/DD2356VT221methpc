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
    5.787,
    2.906,
    1.661,
    1.128,
    0.567,
    0.371
    ]


# plt.plot(number1, time1, '-*')
# plt.xlabel('number of processes')
# plt.ylabel('execution time(s)')
# plt.legend(loc=4) 
# plt.title('Strong Scaling Test-Linear Reduction Algorithm')
# plt.show()

#4.2
number2 = [
    4,
    8,
    16,
    32,
    64,
    128]
time2 =[
    5.760,
    2.900,
    1.756,
    1.129,
    0.564,
    0.367
    ]


# plt.plot(number2, time2, '-*')
# plt.xlabel('number of processes')
# plt.ylabel('execution time(s)')
# plt.legend(loc=4) 
# plt.title('Strong Scaling Test-Binary Tree Reduction')
# plt.show()

#4.3
number3 = [
    4,
    8,
    16,
    32,
    64,
    128]
time3 =[
    5.778,
    2.880,
    1.447,
    0.941,
    0.499,
    0.215
    ]

# plt.plot(number3, time3, '-*')
# plt.xlabel('number of processes')
# plt.ylabel('execution time(s)')
# plt.legend(loc=4) 
# plt.title('Strong Scaling Test-Non-Blocking Linear Reduction Algorithm')
# plt.show()


#4.4
number4 = [
    4,
    8,
    16,
    32,
    64,
    128]
time4 =[
    5.789,
    2.892,
    2.218,
    1.251,
    0.588,
    0.310
    ]

# plt.plot(number4, time4, '-*')
# plt.xlabel('number of processes')
# plt.ylabel('execution time(s)')
# plt.legend(loc=4) 
# plt.title('Strong Scaling Test-MPI_Reduce')
# plt.show()


#bonus
number5 =[
    9,
    16,
    25,
    64,
    100]
time5 =[
    0.929280,
    0.634950,
    0.426979,
    0.184074,
    0.089162
    ]

plt.plot(number5, time5, '-*')
plt.xlabel('number of processes')
plt.ylabel('execution time(s)')
plt.legend(loc=4) 
plt.title('Fox 1200x1200 Measure')
plt.show()