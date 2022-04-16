from cProfile import label
import matplotlib.pyplot as plt

import numpy as np

plt.rcParams['font.sans-serif'] = ['SimHei']

plt.rcParams['axes.unicode_minus'] = False

x = [500, 1000, 2000]

dys = [0.00301272, 0.00575359, 0.05340875]
dyr = [0.00261005, 0.00747626, 0.01850089]

# dy是误差

ys = [1.08588130, 4.34608560, 17.43978970]
yr = [0.63491910, 2.54151549, 10.14023039]

plt.errorbar(x, ys, yerr=dys, fmt='co--',ecolor='r', elinewidth=5, capsize=10 , label='simple')
plt.errorbar(x, yr, yerr=dyr, fmt='co--',ecolor='b', elinewidth=5, capsize=10, label='reduced')

x = np.arange(10) / 10
y = (x + 0.1)**2


# 误差条

plt.xlabel("Number of Particles")

plt.ylabel("Execution time [s]")

plt.title("Error Bar")
plt.legend()
plt.show()