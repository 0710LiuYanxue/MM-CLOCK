import csv
import random

# 读取trace.csv文件
data = []
with open('/opt/eval/simulate/testTrace/zipfi_0.6_1.2/zipfian_1_0_10000.csv', 'r') as file:
    csv_reader = csv.reader(file)
    next(csv_reader)  # 跳过第一行
    for row in csv_reader:
        data.append(row)

# 重新输出所有的False
output_data = []
for row in data:
    if row[1] == 'False':
        output_data.append(row)

# 计算修改后False和True的数量
total_count = len(output_data)
false_count = int(total_count * 0.9)
true_count = total_count - false_count

# 随机修改False和True的数量
random.shuffle(output_data)
output_data = output_data[:false_count] + [['', 'True']] * true_count

# 将结果写入新的csv文件
with open('modified_trace.csv', 'w', newline='') as file:
    csv_writer = csv.writer(file)
    csv_writer.writerow(['pages', 'is_write'])
    csv_writer.writerows(output_data)

print(f"修改后的数据已写入modified_trace.csv文件，其中包含{false_count}个False和{true_count}个True。")