from tracemalloc import start
import numpy as np
import matplotlib.pyplot as plt


size = []
init = 65536
while init <= 1073741824:
    size.append(init)
    init = init * 2

#two-side
#intra-node communication (2 processes on the same node)
time1= [
        0.000003285,
        0.000006118,
        0.000010881,
        0.000017888,
        0.000030908,
        0.000057193,
        0.000113469,
        0.000445014,
        0.001402784,
        0.002694348,
        0.004671630,
        0.008221035,
        0.015080522,
        0.028904538,
        0.056330412
]
#inter-node communication (2 processes on different nodes)
time2= [
        0.000012540,
        0.000018651,
        0.000030021,
        0.000053666,
        0.000097390,
        0.000182540,
        0.000358961,
        0.000700542,
        0.001400862,
        0.002787589,
        0.005552566,
        0.011089445,
        0.022040248,
        0.067305338,
        0.130772883
]

#one-side
#intra-node communication
time3 = [
        0.000002278,
        0.000004324,
        0.000009480,
        0.000013213,
        0.000025619,
        0.000050261,
        0.000177173,
        0.000741319,
        0.001169403,
        0.002012430,
        0.002164884,
        0.004237024,
        0.008194074,
        0.015998625,
        0.032673790
        ]
#one-side
#inter-node communication
time4 = [
        0.000019854,
        0.000021887,
        0.000030309,
        0.000038778,
        0.000060104,
        0.000103338,
        0.000189727,
        0.000364258,
        0.000713638,
        0.001408075,
        0.002801543,
        0.005559405,
        0.014065400,
        0.030383871,
        0.062176394
]

# plt.plot(size, time1, '*')
# plt.xlabel('Size')
# plt.ylabel('Time')
# plt.legend(loc=4) 
# plt.title('intra-node communication')
# plt.show()


plt.plot(size, time3, '*')
plt.xlabel('Size')
plt.ylabel('Time')
plt.legend(loc=4) 
plt.title('one-side intra-node communication')
plt.show()

plt.plot(size, time4, '*')
plt.xlabel('Size')
plt.ylabel('Time')
plt.legend(loc=4) 
plt.title('one-side inter-node communication')
plt.show()


#intra-node communication (2 processes on the same node)
z1 = np.polyfit(size,time1,1)
p1 = np.poly1d(z1)
print(p1)

#inter-node communication (2 processes on different nodes)
z2 = np.polyfit(size,time2,1)
p2 = np.poly1d(z2)
print(p2)

#one-side
#intra-node communication
z3 = np.polyfit(size,time3,1)
p3 = np.poly1d(z3)
print(p3)

#one-side
#inter-node communication
z4 = np.polyfit(size,time4,1)
p4 = np.poly1d(z4)
print(p4)       