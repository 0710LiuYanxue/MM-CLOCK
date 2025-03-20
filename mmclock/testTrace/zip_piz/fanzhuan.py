import csv

# 读取原始CSV文件，处理数据，并写入新的CSV文件
with open('/opt/eval/simulate/testTrace/zip_piz/zip99trace.csv', 'r') as file_in, open('new_zipfian.csv', 'w', newline='') as file_out:
    reader = csv.reader(file_in)
    writer = csv.writer(file_out)

    for row in reader:
        data = int(row[0])
        new_data = 10000 - data
        writer.writerow([new_data, row[1]])