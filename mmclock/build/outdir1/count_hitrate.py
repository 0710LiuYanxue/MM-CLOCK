import csv

def process_csv():
    input_file_path = "output.csv"
    output_file_path = "output_missrate.csv"
    algos = ["EACLOCK","EACLOCK-FDW","EACLOCK-FWA","ARC","WATT","2Q","S3-FIFO","LIRS","Hyperbolic","LeanEvict","LRU-2","Clock Sweep","lru","CLOCK","Random"]

    #algos = ["EACLOCK","EACLOCK-FDW","EACLOCK-FWA","ARC","WATT","2Q","S3fifo","LIRS","W-TinyLFU","TinyLFU_3","LRU-2","lru","CLOCK", "Clock Sweep","Hyperbolic","LeanEvict","Random"]

    ram_size_to_algo_miss_rate_map = {}

    with open(input_file_path, 'r', newline='') as input_file:
        reader = csv.reader(input_file)
        header = next(reader)
        if "algo" not in header:
            raise ValueError("Invalid input file format. Expected 'algo' in header.")

        for row in reader:
            algo, ram_size, elements, reads, writes = row
            if algo in algos:
                miss_rate = 100.0 * int(reads) / int(elements)
                if int(ram_size) not in ram_size_to_algo_miss_rate_map:
                    ram_size_to_algo_miss_rate_map[int(ram_size)] = {}
                ram_size_to_algo_miss_rate_map[int(ram_size)][algo] = miss_rate

    with open(output_file_path, 'w', newline='') as output_file:
        writer = csv.writer(output_file)
        writer.writerow(["ramSize"] + algos)
        for ram_size, algo_miss_rate_map in ram_size_to_algo_miss_rate_map.items():
            row = [ram_size]
            for algo in algos:
                if algo in algo_miss_rate_map:
                    row.append(f'{algo_miss_rate_map[algo]:.4f}')
                else:
                    row.append('')
            writer.writerow(row)

    print("miss rate 统计完成，并已写入 output_missrate.csv 文件。")

process_csv()