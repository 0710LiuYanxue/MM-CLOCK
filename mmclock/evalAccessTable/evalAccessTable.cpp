#include <string>
#include <utility>
#include <fstream>
#include <filesystem>
#include "evalAccessTable.hpp"

using namespace std;

//
// Created by dev on 05.10.21.
//
static void getOrDefaultAndSet(map<int, int> &history, int new_value, int pageRef,
                               int default_value, int* value);

EvalAccessTable::EvalAccessTable(std::string filename, std::string out_dir, bool do_run, bool test, bool benchmark): filename(std::move(filename)), output_dir(std::move(out_dir)){
        if(!do_run){
            return;
        }
        runFromFilename(test, benchmark);
    };

void EvalAccessTable::init(bool ignore_last_run, int max_ram){
    getDataFile();       // 解析输入的数据文件
    if(ignore_last_run){
        max_ram = -1;    // 这个-1表示忽略上次运行的结果，表示不限制RAM的使用，“不限制内存大小” / “使用默认值”等。
    }
    createLists(ignore_last_run, max_ram); // this runs "lru" (lru_stack_trace)
}

// 在我的main定义中test为false，benchmark为true✅
void EvalAccessTable::runFromFilename(bool test, bool benchmark) {
    // in case of full run (not test or benchmark)
    bool run_slow = false;
    bool full_run = false;
    // Ignore last if test run, else not!
    init(test, 100000000);               // 解析数据文件，并初始化ramSize

    bool do_it = true;
    if(do_it) {
        advanced_with_variations_algos();     // 运行默认算法，benchmark模式下，会循环测试kr, kw, e, rsi, rsa, wc组合的不同参数。

        uint KR = 1;                  // length of first list (read or access), 0 = infty
        uint epoch_size = 0;          // How many evictions are grouped to one epoch, 0 = infty, else RAMSIZE/epoch_size
        uint randSize = 8;            // How big is the random sample
        uint randSelector = 1;        // How many Elements should be evicted per sample

        uint KW = 1;                  // length of second list (write list), 0 = infty (not counted, becaus write_cost=0)
        bool write_as_read = true;  // use first lists for accesses (reads and writes) or only reads
        float write_cost = 0;         // how expensive are writes compared to reads

        float first_value = 1.0;    // dampening factor of first access
        modus mod = mod_max;        // what modus should be taken to aggregate 
        int Z = 0;                   // size of out of memory history -1 = infty

        randSize = 8;
        KR = 8;   
        epoch_size = 4;
        first_value = 0.1; 
        Z = 0;        
        mod = mod_max;
        // read and writes
        write_cost = 1; // To enable writes;              
        KW = 4;       
        write_cost = 1;

        // modified
        runAlgorithm("WATT", LFU_2K_E_real_Generator(KR, KW, epoch_size, randSize, randSelector, write_as_read, write_cost,
                                                       first_value, mod, Z));

    }
    else {
        if (benchmark) {
            advanced_with_variations_algos();

            for (int kr: {16, 8, 4, 2, 0}) for (int kw: {16, 8, 4, 2, 0}) for (int e: {0, 20, 10, 5, 1})
                for (int rsi: {10}) for (int rsa: {1, 5, 10}) for (int wc: {0, 1, 2, 4, 8, 2048}) {
                    if ((kw == 0 && wc != 1) || (kr == 0 && kw == 0))
                        continue;
                    string name = "kr" + to_string(kr)
                                  + "_kw" + to_string(kw)
                                  + "_e" + to_string(e)
                                  + "_rsi" + to_string(rsi)
                                  + "_rsa" + to_string(rsa)
                                  + "_wc" + to_string(wc);
                    if (kw != 0) {
                        runAlgorithm("lfu_vers2_" + name,
                                     LFU_Generator(kr, kw, e, rsi, rsa, false, wc));
                        runAlgorithm("lfu_vers3_" + name,
                                     LFU_2K_E_real2_Generator(kr, kw, e, rsi, rsa, false, wc));
                    } else {
                        runAlgorithm("lfu_vers2_" + name + "_war",
                                     LFU_Generator(kr, kw, e, rsi, rsa, true, wc));
                        runAlgorithm("lfu_vers3_" + name + "_war",
                                     LFU_2K_E_real2_Generator(kr, kw, e, rsi, rsa, true, wc));
                    }
                }
            for (int k: {32, 16, 8, 4, 2, 0})
                for (int e: {0, 20, 10, 5, 1})
                    for (int rsi: {10})
                        for (int rsa: {1, 5, 10})
                            for (int wc: {0, 1, 2, 4, 8, 2048}) {
                                string name = to_string(k)
                                              + "_e" + to_string(e)
                                              + "_rsi" + to_string(rsi)
                                              + "_rsa" + to_string(rsa)
                                              + "_wc" + to_string(wc);
                                runAlgorithm("lfu_k" + name, LFU_1K_E_real_Generator(k, e, rsi, rsa, wc, 0));
                                runAlgorithm("lfu_bla_k" + name, LFU_1K_E_Generator(k, e, rsi, rsa, wc, 0));
                            }


        }
        if (!test && !benchmark) {
            advanced_with_variations_algos();
            if (full_run) {
                runAlgorithm("lru_alt", LRU_Generator());
                runAlgorithm("lru_alt1", LRU1_Generator());
                runAlgorithm("lru_alt2a", LRU2a_Generator());
                runAlgorithm("lru_alt2b", LRU2b_Generator());
                runAlgorithm("lru_alt3", LRU3_Generator());
                runAlgorithm("cf_lru10", CfLRUGenerator(10));
                runAlgorithm("cf_lru20", CfLRUGenerator(20));
                runAlgorithm("cf_lru70", CfLRUGenerator(70));
                runAlgorithm("cf_lru80", CfLRUGenerator(80));
                runAlgorithm("cf_lru90", CfLRUGenerator(90));
                runAlgorithm("lfu_k_alt01", LFUalt_K_Generator(1));
                runAlgorithm("lfu_k_alt02", LFUalt_K_Generator(2));
                runAlgorithm("lfu_k_alt10", LFUalt_K_Generator(10));
                runAlgorithm("lfu_k_alt20", LFUalt_K_Generator(20));
                runAlgorithm("lru_k_alt01", LRUalt_K_Generator(1));
                runAlgorithm("lru_k_alt02", LRUalt_K_Generator(2));
                runAlgorithm("lru_k_alt10", LRUalt_K_Generator(10));
                runAlgorithm("lru_k_alt20", LRUalt_K_Generator(20));
                // runAlgorithm("opt3", Opt3_Generator());// broken
                if (run_slow) {
                    runAlgorithm("opt2", Opt2_Generator());
                    runAlgorithm("lru_alt2", LRU2_Generator());
                    runAlgorithm("cf_lru100", CfLRUGenerator(100));
                }
                for (int k: {16, 8, 4, 2}) {
                    for (int z: {100, 10, 1, -1}) {
                        runAlgorithm("lfu_k" + std::to_string(k) + "_z" + std::to_string(z), LFU_K_Z_Generator(k, z));
                        // runAlgorithm("lfu2_k" + std::to_string(k) + "_z" + std::to_string(z), LFU2_K_Z_Generator(k, z));
                        runAlgorithm("lru_k" + std::to_string(k) + "_z" + std::to_string(z), LRU_K_Z_Generator(k, z));
                        runAlgorithm("lfu_2k" + std::to_string(k) + "_z" + std::to_string(z) + "_T",
                                     LFU_2K_Z_Generator(k, k, z, true));
                        runAlgorithm("lfu_2k" + std::to_string(k) + "_z" + std::to_string(z) + "_F",
                                     LFU_2K_Z_Generator(k, k, z, false));
                        runAlgorithm("lfu_2k" + std::to_string(k) + "_z" + std::to_string(z) + "_TR",
                                     LFU_2K_Z_rand_Generator(k, k, z, 5, true));
                        runAlgorithm("lfu_2k" + std::to_string(k) + "_z" + std::to_string(z) + "_FR",
                                     LFU_2K_Z_rand_Generator(k, k, z, 5, false));
                    }
                    runAlgorithm("lfu_k_real_F_e" + std::to_string(k), LFU_2K_E_real_Generator(8, 4, k, 5, 5, false));
                    runAlgorithm("lfu_k_real2_F_e" + std::to_string(k), LFU_2K_E_real_Generator(4, 8, k, 5, 5, false));
                    runAlgorithm("lfu_k_real_F_e" + std::to_string(k) + "_wc" + std::to_string(8),
                                 LFU_Generator(8, 4, k, 5, 5, false, 8));
                    runAlgorithm("lfu_k_" + std::to_string(k), LFU_K_Generator(k));
                }
            }
        }
    }
    printToFile();
}

void EvalAccessTable::advanced_with_variations_algos() {
    advanced_compare_algos();

    runAlgorithm("LeanEvict", Lean_Generator2(30));
}


void EvalAccessTable::advanced_compare_algos() {
    default_compare_algos();
}

void EvalAccessTable::default_compare_algos() {
    // read dist write
    runAlgorithm("opt", Opt_Generator());
    runAlgorithm("EACLOCK", amclock_Final_Generator());
    runAlgorithm("EACLOCK-FDW", amclock_DW_HR_generator(0.3));
    runAlgorithm("EACLOCK-FWA", amclock_WA_Generator());

    runAlgorithm("2Q", TwoQ_Generator());
    runAlgorithm("S3-FIFO", S3fifo_Generator());
    runAlgorithm("LIRS", LIRS_Generator());
    runAlgorithm("W-TinyLFU", W_TinyLFU_Generator());

    runAlgorithm("mmclock", mmclock_Generator());

    runAlgorithm("CLOCK", CLOCK_Generator());
    runAlgorithm("Clock Sweep", CLOCK_AC_Generator());
    runAlgorithm("Hyperbolic", Hyperbolic_generator(20));
 
    runAlgorithm("Random", Random_Generator());
    runAlgorithm("ARC", ARC_Generator());
    runAlgorithm("LRU-2", LRU_K_Z_Generator(2, -1));
}

void EvalAccessTable::printToFile() {
    vector<string> names;

    ofstream out_stream;
    out_stream.open(output_file);
    // print algo names
    out_stream << first_csv_line <<endl;
    // print entries
    uint elements = data.size();
    for (auto &algo: read_write_list) {
        for(auto& entry: algo.second){
            out_stream <<
                algo.first << "," <<
                entry.first << "," <<
                elements << "," <<
                entry.second.first << "," <<
                entry.second.second << endl;
            
            // change,lx,throughput

            if(algo.first == "no")
            {
                for(uint i=0;i<(entry.second.first);i++)
                {
                    sleepRead();
                }

                for(uint i=0;i<(entry.second.second);i++)
                {
                    sleepWrite();
                }

                std::cout<<"totalSeconds:";
            }
        }
    }
}

// 解析用于test的数据文件，包括两个部分，访问的页面编号和is_write是否是写操作。
void EvalAccessTable::getDataFile() {
    {
        ifstream reader;
        reader.open(filename);
        assert(reader.good());
        string line;
        bool firstLine = true;
        map<int, int> last_access;
        int i = 0;
        while (getline(reader, line)) {
            if (firstLine) {
                firstLine = false;
                assert("pages,is_write" == line);
                continue;
            }
            Access &access = data.emplace_back();
            stringstream ss(line);
            string value;
            getline(ss, value, ',');
            access.pid = stoi(value);
            getline(ss, value);
            access.write = (value.find("rue") != std::string::npos);
            access.pos = i;

            getOrDefaultAndSet(last_access, i, access.pid, -1, &access.lastRef);

            i++;
        }
    }
    {
        map<int, int> next_access;
        int data_size = data.size();
        for (uint i = 0; i < data.size(); i++) {
            int pos = data_size - (i + 1);
            getOrDefaultAndSet(next_access, pos, data[pos].pid, data_size, &data[pos].nextRef);
        }
    }
}

// 如果当前的结果文件output.csv存在，则通过handleCsv读取旧数据，也就是说接着当前执行过的算法继续执行；否则执行lru算法。
void EvalAccessTable::createLists(bool ignore_last_run, int max_ram) {
    ifstream reader;
    reader.open(output_file);
    if (reader.good() && !ignore_last_run) {
        handleCsv(reader);
    } else {
        cout << "No old files found" << endl;
        filesystem::create_directory(output_dir);
    }
    runAlgorithmNonParallel("lru", LruStackDist(max_ram));
}

const rwListSubType& EvalAccessTable::getValues(std::string name) {
    assert(hasValues(name));
    return read_write_list[name];
}
bool EvalAccessTable::hasValues(std::string algo){
    return read_write_list.contains(algo);
}

bool EvalAccessTable::hasValue(std::string algo, RamSize ramSize){
     return hasValues(algo) &&
        read_write_list[algo].contains(ramSize);
}

bool EvalAccessTable::hasAllValues(std::string algo){
    if(ramSizes.size() == 0){
        return false;
    }
    for(RamSize size: ramSizes){
        if(!hasValue(algo, size)){
            return false;
        }
    }
    return true;
}

ramListType EvalAccessTable::missingValues(std::string algo){
    ramListType missing;
    for(RamSize size: ramSizes){
        if(!hasValue(algo, size)){
            missing.emplace(size);
        }
    }
    return missing;
}

void EvalAccessTable::handleCsv(ifstream &filestream){
    string line;
    bool first_line = true;
    while (getline(filestream, line)) {
        if (first_line) {
            assert(line == first_csv_line);
            first_line = false;
            continue;
        }
        stringstream ss(line);
        string algo, ram_size, elements, reads, writes;
        getline(ss, algo, ',');
        getline(ss, ram_size, ',');
        getline(ss, elements, ',');
        getline(ss, reads, ',');
        getline(ss, writes, ',');
        assert((uint) stoi(elements) == data.size());
        read_write_list[algo][stoi(ram_size)] = std::make_pair(stoi(reads), stoi(writes));
        if(!ramSizes.contains(stoi(ram_size))){
            ramSizes.emplace(stoi(ram_size));
        }
    }
}


static void getOrDefaultAndSet(map<int, int> &history, int new_value, int pageRef,
                        int default_value, int* value) {
    auto element = history.find(pageRef);
    if (element == history.end()) {
        *value = default_value;
    } else {
        *value = element->second;
    }
    history[pageRef] = new_value;
}
