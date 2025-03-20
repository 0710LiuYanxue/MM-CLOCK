#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>

using namespace std;

int main() {
    // 打开输入 CSV 文件
    ifstream inputFile("output.csv");
    if (!inputFile.is_open()) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    // 打开输出 CSV 文件
    ofstream outputFile("output_missrate.csv");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file" << endl;
        return 1;
    }

    // 创建一个映射，其中键是 ramSize，值是算法和 miss rate 的映射
    map<int, map<string, double>> ramSizeToAlgoMissRateMap;

    // 创建一个向量，其中包含要输出的算法
    //vector<string> algos = {"amclock","amclock1","amclock2","amclock3","amclock4","amclock5","amclock6","amclock7","amclock8","amclock9","amclock10","amclock11","amclock_Final","amclock_DW","arc", "lru_k2_z-1","lru","clock", "clocksweep","WATT","hyperbolic20"};
    //vector<string> algos ={"lru","clock","mclock0101","mclock0201","mclock0301","mclock0401","mclock0501","mclock0601","mclock0701","mclock0801","mclock01001","mclock01401","mclock01601","mclock02001","mclock02401"};
    //vector<string> algos = {"amclock","arc", "lru_k2_z-1","lru","clock", "clocksweep","amclock_Final","WATT","hyperbolic20"};
    //vector<string> algos = {"amclock_DW","amclock_Final","amclock_WA15","amclock_WA","amclock_WA25","amclock_WA3","amclock_WA35","amclock_WA4","arc","WATT","lru_k2_z-1","lru","clock", "clocksweep","hyperbolic20"};
    //vector<string> algos = {"EACLOCK","EACLOCK-FDW","EACLOCK-FWA","ARC","WATT","LRU-2","lru","CLOCK", "Clock Sweep","Hyperbolic","LeanEvict","Random"};
    //vector<string> algos = {"amclock_epoch_4_","amclock_epoch_2_","amclock_epoch_1","amclock_epoch_2","amclock_epoch_3","amclock_epoch_4","amclock_epoch_5","amclock_epoch_6", "amclock_epoch_7","amclock_epoch_8","amclock_epoch_16","amclock_epoch_24"};
    //vector<string> algos = {"amclock_Weight_1","amclock_Weight_2","amclock_Weight_3","amclock_Weight_4","amclock_Weight_5","amclock_Weight_6","amclock_Weight_7","amclock_Weight_8", "amclock_Weight_9","amclock_Weight_10","amclock_Weight_11","amclock_Weight_12","amclock_Weight_14","amclock_Weight_18","amclock_Weight_24"};
    //vector<string> algos = {"amclock_action_1","amclock_action_2","amclock_action_3","amclock_action_4","amclock_action_5","amclock_action_6","amclock_action_7","amclock_action_8", "amclock_action_9"};
    //vector<string> algos = {"amclock_upLimit_1","amclock_upLimit_2","amclock_upLimit_4","amclock_upLimit_6","amclock_upLimit_8","amclock_upLimit_12","amclock_upLimit_16","amclock_upLimit_20", "amclock_upLimit_24", "amclock_upLimit_28", "amclock_upLimit_32"};
    //vector<string> algos = {"amclock_DW_upLimit_1","amclock_DW_upLimit_2","amclock_DW_upLimit_4","amclock_DW_upLimit_6","amclock_DW_upLimit_8","amclock_DW_upLimit_12","amclock_DW_upLimit_16","amclock_DW_upLimit_20", "amclock_DW_upLimit_24", "amclock_DW_upLimit_28", "amclock_DW_upLimit_32"};
    //vector<string> algos = {"amclock_DW_HR_1","amclock_DW_HR_2","amclock_DW_HR_3","amclock_DW_HR_4","amclock_DW_HR_5","amclock_DW_HR_6","amclock_DW_HR_7","amclock_DW_HR_8", "amclock_DW_HR_9","amclock_DW_HR_10","EACLOCK","ARC","WATT","LRU-2"};
    vector<string> algos = {"EACLOCK","EACLOCK-FDW","EACLOCK-FWA","ARC","WATT","LRU-2","lru","CFLRU_0","CLOCK", "Clock Sweep","Hyperbolic","LeanEvict","Random"};

    // 读取输入 CSV 文件的每一行
    string line;
    while (getline(inputFile, line)) {
        // 跳过表头行
        if (line.find("algo") != string::npos) {
            continue;
        }

        // 解析每一行
        stringstream ss(line);
        string algo, ramSize, elements, reads, writes;
        getline(ss, algo, ',');
        getline(ss, ramSize, ',');
        getline(ss, elements, ',');
        getline(ss, reads, ',');
        getline(ss, writes, ',');

        // 计算 miss rate
        double missRate = 100.0 * stoi(reads) / stoi(elements);

        // 将算法和 miss rate 添加到映射中
        ramSizeToAlgoMissRateMap[stoi(ramSize)][algo] = missRate;
    }

    // 写入输出 CSV 文件头
    outputFile << "ramSize";
    for (const auto& algo : algos) {
        outputFile << ',' << algo;
    }
    outputFile << endl;

    // 写入输出 CSV 文件数据
    for (const auto& [ramSize, algoMissRateMap] : ramSizeToAlgoMissRateMap) {
        outputFile << ramSize;
        for (const auto& algo : algos) {
            outputFile << ',' << fixed << setprecision(4) << const_cast<std::map<std::string, double>&>(algoMissRateMap)[algo];
        }
        outputFile << endl;
    }

    // 关闭输入和输出 CSV 文件
    inputFile.close();
    outputFile.close();

    cout<<"miss rate统计完成，并已写入output_missrate.csv文件。"<<endl;
    return 0;
}