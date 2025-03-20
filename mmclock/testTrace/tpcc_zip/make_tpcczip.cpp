#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main() {
    // 打开输入文件
    ifstream inputFile1("/opt/eval/simulate/WATT-traces/WATT_competition_traces/TPC_C/trace.csv");
    if (!inputFile1.is_open()) {
        cerr << "Error opening input file trace1.csv" << endl;
        return 1;
    }

    ifstream inputFile2("/opt/eval/simulate/WATT-traces/WATT_competition_traces/ZipfRO/trace.csv");
    if (!inputFile2.is_open()) {
        cerr << "Error opening input file trace2.csv" << endl;
        return 1;
    }

    // 打开输出文件
    ofstream outputFile("tpcc_zip_4.csv");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file trace3.csv" << endl;
        return 1;
    }

    // 写入输出文件头
    outputFile << "pages,is_write" << endl;

    // 读取输入文件并写入输出文件
    int count = 0;
    string line;
    while (getline(inputFile1, line)) {
        // 跳过第一行
        if (count == 0) {
            count++;
            continue;
        }

        // 写入数据
        outputFile << line << endl;

        count++;

        // 读取 500000 条数据
        if (count == 250000) {
            break;
        }
    }

    count = 0;
    while (getline(inputFile2, line)) {
        // 跳过第一行
        if (count == 0) {
            count++;
            continue;
        }

        // 写入数据
        outputFile << line << endl;

        count++;

        // 读取 500000 条数据
        if (count == 250000) {
            break;
        }
    }

    count = 0;
    while (getline(inputFile1, line)) {
        // 跳过第一行
        if (count == 0) {
            count++;
            continue;
        }

        // 写入数据
        outputFile << line << endl;

        count++;

        // 读取 500000 条数据
        if (count == 250000) {
            break;
        }
    }

    count = 0;
    while (getline(inputFile2, line)) {
        // 跳过第一行
        if (count == 0) {
            count++;
            continue;
        }

        // 写入数据
        outputFile << line << endl;

        count++;

        // 读取 500000 条数据
        if (count == 250000) {
            break;
        }
    }



    // 关闭文件
    inputFile1.close();
    inputFile2.close();
    outputFile.close();

    return 0;
}