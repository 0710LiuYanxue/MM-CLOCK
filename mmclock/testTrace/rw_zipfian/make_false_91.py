import csv
import random

# 定义生成的False和True的数量
false_count = 899999
true_count = 100000

# 生成随机布尔值列表
data = ['False'] * false_count + ['True'] * true_count
random.shuffle(data)

# 将数据写入csv文件
with open('false2true91.csv', 'w', newline='') as file:
    csv_writer = csv.writer(file)
    csv_writer.writerow(['is_write'])
    csv_writer.writerows([[value] for value in data])

print("数据生成完成，并已写入generated_data.csv文件。")