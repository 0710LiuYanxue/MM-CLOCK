import csv

# 读取trace1.csv文件
trace1_data = []
with open('/opt/eval/simulate/testTrace/zipfi_0.6_1.2/zipfian_1_0_10000.csv', 'r') as file:
    csv_reader = csv.reader(file)
    next(csv_reader)  # 跳过第一行
    for row in csv_reader:
        trace1_data.append(row)

# 读取trace2.csv文件
trace2_data = []
with open('false2true91.csv', 'r') as file:
    csv_reader = csv.reader(file)
    next(csv_reader)  # 跳过第一行
    for row in csv_reader:
        trace2_data.append(row)

# 校验数据行数是否相等
if len(trace1_data) != len(trace2_data):
    print("错误：两个文件数据行数不相等。")
    exit()

# 将trace1的pages和trace2的is_write进行一一对应的组合
combined_data = []
for i in range(len(trace1_data)):
    combined_row = [trace1_data[i][0], trace2_data[i][0]]
    combined_data.append(combined_row)

# 将结果写入新的csv文件
with open('combined_data.csv', 'w', newline='') as file:
    csv_writer = csv.writer(file)
    csv_writer.writerow(['pages', 'is_write'])
    csv_writer.writerows(combined_data)

print("数据组合完成，并已写入combined_data.csv文件。")