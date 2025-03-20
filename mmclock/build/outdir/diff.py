import pandas as pd

# 读取trace.csv文件
data = pd.read_csv('/opt/eval/simulate/build/outdir/output_missrate_final_zip099.csv')


# 提取所有算法列的名称
algorithms = [col for col in data.columns[1:-1] if col != 'lrfu0.8']  # 排除第一列（ramSize）和最后一列（random）

# 创建一个空的DataFrame用于存储所有算法的差值
diff_data = pd.DataFrame()

# 将ramSize列拼接到diff_data中
diff_data['ramSize'] = data['ramSize']

# 遍历每个算法，计算差值并添加到新的DataFrame中
# 遍历每个算法，计算差值并添加到新的DataFrame中
for algorithm in algorithms:
    diff_values = round(data['random'] - data[algorithm], 4)
    diff_data[algorithm + '_diff'] = diff_values



# 将结果保存为一个新的CSV文件
diff_data.to_csv('all_algorithms_diff.csv', index=False)