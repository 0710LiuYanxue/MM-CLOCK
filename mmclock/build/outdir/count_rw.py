import csv

# 定义算法列表和ramSize列表
algos = ["amclock_DW", "amclock_Final", "amclock_WA15","amclock_WA","amclock_WA25","amclock_WA3","amclock_WA35","amclock_WA4", "arc", "WATT", "lru_k2_z-1", "lru", "clock", "clocksweep", "hyperbolic20"]

# 创建字典用于存储每个algo在不同ramSize下的reads和writes数据
reads_data = {algo: {} for algo in algos}
writes_data = {algo: {} for algo in algos}

# 读取trace1.csv文件
with open('/opt/eval/simulate/build/outdir/output.csv', 'r') as file:
    csv_reader = csv.reader(file)
    next(csv_reader)  # 跳过第一行
    for row in csv_reader:
        algo = row[0]
        ram_size = int(row[1])
        reads = int(row[3])
        writes = int(row[4])
        if algo in algos:
            if ram_size not in reads_data[algo]:
                reads_data[algo][ram_size] = 0
                writes_data[algo][ram_size] = 0
            reads_data[algo][ram_size] += reads
            writes_data[algo][ram_size] += writes

# 获取所有ramSize的列表
ram_sizes = sorted(set(size for algo_data in reads_data.values() for size in algo_data))

# 将reads统计结果写入新的csv文件
reads_output_data = [['ramSize'] + algos]

for ram_size in ram_sizes:
    row = [ram_size]
    for algo in algos:
        reads_sum = reads_data[algo].get(ram_size, 0)
        row.append(reads_sum)
    reads_output_data.append(row)

# 将writes统计结果写入新的csv文件
writes_output_data = [['ramSize'] + algos]

for ram_size in ram_sizes:
    row = [ram_size]
    for algo in algos:
        writes_sum = writes_data[algo].get(ram_size, 0)
        row.append(writes_sum)
    writes_output_data.append(row)

# 将结果写入新的csv文件
with open('reads_data.csv', 'w', newline='') as file:
    csv_writer = csv.writer(file)
    csv_writer.writerows(reads_output_data)

with open('writes_data.csv', 'w', newline='') as file:
    csv_writer = csv.writer(file)
    csv_writer.writerows(writes_output_data)

print("数据统计完成，并已写入reads_data.csv和writes_data.csv文件。")