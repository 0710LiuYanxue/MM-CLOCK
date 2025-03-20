#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main() {
    // 打开输入文件
    ifstream inputFile("trace.csv");
    if (!inputFile.is_open()) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    // 打开输出文件
    ofstream outputFile("Zipfian_scan.csv");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file" << endl;
        return 1;
    }

    // 写入输出文件头
    outputFile << "pages,is_write" << endl;

    // 读取输入文件并插入新数据
    int count = 0;
    int new_data_index = 1;
    string line;
    while (getline(inputFile, line)) {
        // 跳过第一行
        if (count == 0) {
            count++;
            continue;
        }

        // 解析数据
        stringstream ss(line);
        string pages, is_write;
        getline(ss, pages, ',');
        getline(ss, is_write, ',');

        // 写入数据
        outputFile << pages << "," << "False" << endl;

        // 插入新数据
        if (count % 10000 == 0) {
            for (int i = 0; i < 500; i++) {
                outputFile << new_data_index << "," << "False" << endl;
                new_data_index++;
                if(new_data_index>10000)
                    new_data_index=1;
            }
        }

        count++;
    }

    // 关闭文件
    inputFile.close();
    outputFile.close();

    return 0;
}