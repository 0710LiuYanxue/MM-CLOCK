import numpy as np
from scipy.stats import zipfian
import matplotlib.pyplot as plt
import time

time_start = time.time()  # 记录开始时间

a = 1  # a 即为倾斜程度，越大越倾斜
n = 100000  # n 为生成数的最大范围，最大值

# 生成概率质量函数 (PMF)
pmf = zipfian.pmf(np.arange(1, n + 1), a, n)

time_end = time.time()  # 记录结束时间
time_sum = time_end - time_start  # 计算的时间差为程序的执行时间，单位为秒/s
print(time_sum)

# 归一化 PMF
pmf = pmf / np.sum(pmf)

#x = np.arange(1, 100001)
#plt.plot(x, pmf)
#plt.xlabel('Value')
#plt.ylabel('Probability')
#plt.title('Zipfian Distribution PMF')
#plt.savefig('zipfian_10w.png')

time_end = time.time()  # 记录结束时间
time_sum = time_end - time_start  # 计算的时间差为程序的执行时间，单位为秒/s
print(time_sum)

# 使用 numpy 的 random.choice 函数生成随机数，并显示进度
total_num = 1000000
step = 10000
data = []
for i in range(0, total_num, step):
    batch = np.random.choice(np.arange(n), size=step, p=pmf)
    data.extend(list(zip(batch, [False] * len(batch))))
    print(f'Generated {i + len(batch)}/{total_num} random numbers.')

time_end = time.time()  # 记录结束时间
time_sum = time_end - time_start  # 计算的时间差为程序的执行时间，单位为秒/s
print(time_sum)

# 将列表保存到 CSV 文件
with open('zipfian_1_0_10w.csv', 'w') as f:
    for item in data:
        f.write(f'{item[0]},{item[1]}\n')

time_end1 = time.time()  # 记录结束时间
time_sum = time_end1 - time_end  # 计算的时间差为程序的执行时间，单位为秒/s
print(time_sum)