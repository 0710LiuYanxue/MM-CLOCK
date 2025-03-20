import csv

# 定义算法列表和ramSize列表
algos = ["EACLOCK","EACLOCK-FDW","EACLOCK-FWA","ARC","WATT","2Q","S3-FIFO","LIRS","Hyperbolic","LeanEvict","LRU-2","Clock Sweep","lru","CLOCK","Random"]

# 创建字典用于存储每个algo在不同ramSize下的writes数据
writes_data = {algo: {} for algo in algos}

# 读取trace1.csv文件
with open('/opt/eval/simulate/build/outdir4/output.csv', 'r') as file:
    csv_reader = csv.reader(file)
    next(csv_reader)  # 跳过第一行
    for row in csv_reader:
        algo = row[0]
        ram_size = int(row[1])
        writes = int(row[4])
        if algo in algos:
            if ram_size not in writes_data[algo]:
                writes_data[algo][ram_size] = 0
            writes_data[algo][ram_size] += writes

# 获取所有ramSize的列表
ram_sizes = sorted(set(size for algo_data in writes_data.values() for size in algo_data))

# 将统计结果写入新的csv文件
output_data = [['ramSize'] + algos]

for ram_size in ram_sizes:
    row = [ram_size]
    for algo in algos:
        writes_sum = writes_data[algo].get(ram_size, 0)
        row.append(writes_sum)
    output_data.append(row)

# 将结果写入新的csv文件
with open('output_writes.csv', 'w', newline='') as file:
    csv_writer = csv.writer(file)
    csv_writer.writerows(output_data)

print("数据统计完成，并已写入output_writes.csv文件。")