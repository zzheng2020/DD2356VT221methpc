from cProfile import label
import matplotlib.pyplot as plt

import numpy as np

plt.rcParams['font.sans-serif'] = ['SimHei']

plt.rcParams['axes.unicode_minus'] = False

x = [500, 1000, 2000]

dys = [0.00261005, 0.0, 0.0]
# dy是误差
yr = [0.63491910, 2.54151549, 10.14023039]
ym = [0.14034531, 0.56194062, 2.24333527]
ymm = [0.20665907, 0.82745850, 3.30331857]

plt.errorbar(x, yr, yerr=dys, fmt='.k',ecolor='r', elinewidth=5, capsize=10 , label='reduced')
plt.errorbar(x, ym, yerr=0, fmt='.k',ecolor='b', elinewidth=5, capsize=10, label='model')
plt.errorbar(x, ymm, yerr=0,fmt='.k',ecolor='y', elinewidth=5, capsize=10, label='model with memory')

x = np.arange(10) / 10
y = (x + 0.1)**2


# 误差条

plt.xlabel("Number of Particles")

plt.ylabel("Execution time [s]")

plt.title("Error Bar")
plt.legend()
plt.show()