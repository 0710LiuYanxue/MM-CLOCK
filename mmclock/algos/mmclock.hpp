#include "EvictStrategy.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using mmclock_type = std::pair<PID, AccessCount>;
using mmclock_upper = EvictStrategyHashVector<mmclock_type>;

struct mmclock : public mmclock_upper {
    // 1. 主动驱逐算法的参数
    double alpha;       // 页面命中率影响因子
    double beta;        // 初始驱逐率影响因子
    double e;           // 初始驱逐率

    double HR;      // 只需要关注上一个epoch的命中率，不需要命中率记录日志；

    // 驱逐的页面的数量 就相当于是 页面进行替换的数量 
    uint evictNum;      
    uint last_epoch_evict;     // 

    uint HitInBuf;
    uint dirty_evicts;

    // 页面缺失数量，发生页面置换的前提条件是page_misses
    uint page_misses;

    double freeListThreshold;    // freelist提前页面写回阈值
    bool isInitial;
    
    double evictionRate;        // 驱逐率和驱逐阈值是一体的

    uint next;     // clock遍历指针    
    std::queue<PID> freeList;       // 动态大小的数据结构
    mmclock(): mmclock_upper() {} 
    
    // 缓冲区初始化
    void reInit(RamSize ram_size) override {
        mmclock_upper::reInit(ram_size);

        // 1. 主动页面驱逐算法
        alpha = 0.9;       // 页面命中率影响因子
        beta = 1;          // 初始驱逐率影响因子
        e = 0.1;           // 初始驱逐率

        evictNum = 0;
        last_epoch_evict = 0;
        HitInBuf = 0;
        HR = 0.8;

        page_misses = 0;
        freeListThreshold = 0.1; 
        isInitial = true;
        next = 0;
    }

    // 计算驱逐率 E = α * (1 - H) + β * e
    void calculateEvictionRate() {
        // 第一次的驱逐率是原始驱逐率e，而不是根据公式计算得到的 
        if(isInitial){
            evictionRate = e;
            isInitial = false;
            return;
        }

        evictionRate = alpha * (1 - HR) + beta * e;
    }

    // 判断是否进入一个新的epoch 当进入一个新的epoch的时候 开始计算
    // 之前的这个位置存在错误，是因为每次的evictNum都不一定能正好满足下面那个公式 这会导致提前页面驱逐等无法正确更新
    // 因此需要修改判断条件为大于等于才能有可能满足条件
    // 进入新的epoch之后重新计算命中率。
    bool isNewEpoch() {
        uint current_epoch_evict = evictNum - last_epoch_evict;
        if (current_epoch_evict >= (1 - HR) * ram.size()) 
        {
            // 明确当前的命中率的计算方式 分母应该是page_misses而不是evictNum
            HR = 1.0 * HitInBuf / (HitInBuf + page_misses);   
            last_epoch_evict = evictNum;  // 记录当前累计值
            HitInBuf = 0;
            page_misses = 0;
            // std::cout << "进入新的epoch" << std::endl;  
            // std::cout << "当前的命中率是" << HR << std::endl;
            return true;
        }
        return false;
    }

    // 当需要进行驱逐的时候，说明缓冲区已经满了
    // 驱逐一个页面，并从各种容器中erase掉，并返回
    // 优先从free-list中选择驱逐的页面 当free-list中为空的时候再去遍历实际的物理缓冲区
    PID evictOne(Access) override {
        earlyPageWriteBack();  // 执行提前页面写回算法 函数中会判断空闲列表数量是否满足可以执行
        
        // 1. 如果 freeList 不为空，优先使用 freeList 中的页面
        if (!freeList.empty()) {
            PID freePage = freeList.front();  // 获取 freeList 中的页面
            freeList.pop();                  // 从 freeList 中删除该页面
    
            if (dirty_in_ram[freePage]) {    // 如果是脏页面，写回磁盘
                writeBackToDisk(freePage);
            }
    
            // 直接覆盖 ram 中的页面
            fast_finder.erase(freePage);      // 移除旧页面的索引
            in_ram[freePage] = false;         // 标记旧页面不在内存中
        
            return freePage;                  // 返回可用的页面 ID
        }
    
        // 2. 如果 freeList 为空，则执行传统 CLOCK 页面替换策略
        while (true) {
            if (next >= ram.size()) {
                next -= ram.size();           // 环形缓冲区: 循环指针
            }
    
            if (ram[next].second != 0) {
                ram[next].second--;           // 引用计数减一
                next++;
            } else {    // 表示找到了page Value为0的页面
                PID evictedPage = ram[next].first;
    
                if (dirty_in_ram[evictedPage]) {  // 如果是脏页面，写回磁盘
                    writeBackToDisk(evictedPage);
                }
    
                fast_finder.erase(evictedPage);   // 删除旧页面的索引
                in_ram[evictedPage] = false;      // 标记旧页面不在内存中
                
                // 当进入一个新的epoch的时候需要 重置命中次数和页面缺失
                if (isNewEpoch()) {               // 判断是否进入新 epoch
                    // std::cout << "进入一个新的epoch" << std::endl;
                    // evictNum = 0;                 // 驱逐数和命中数都需要清零 因为需要重新计算了
                    HitInBuf = 0; 
                    page_misses = 0; 
                    calculateEvictionRate();      // 计算驱逐率
                    // std::cout << "当前计算得到的驱逐率是：" << evictionRate << std::endl;
                    proactiveEviction();          // 主动驱逐部分页面
                }

                next++;    // 确保指针递增
                return evictedPage;               // 返回可用的页面 ID
            }
        }
    }

    // 根据命中率计算写感知权重的函数
    int getWriteAwareWeight(double hitRate) {
        // 转换为百分比
        hitRate *= 100;
        if (hitRate >= 90) return 4;
        else if (hitRate >= 80) return 2;
        else if (hitRate >= 50) return 1;
        else return 0;
    }

    // 主动页面驱逐算法，根据驱逐率主动驱逐页面
    void proactiveEviction() {
        // 1. 计算需驱逐的页面数量 
        int targetEvictNum = ram.size() * evictionRate;

        // 2. 使用最小堆 (priority_queue) 找出 pageValue 最低的页面
        using PageInfo = std::pair<int, PID>;  // (pageValue, PID)
        std::priority_queue<PageInfo, std::vector<PageInfo>, std::greater<PageInfo>> minHeap;

        // 3. 将所有页面的 (pageValue, PID) 加入最小堆
        for (uint i = 0; i < ram.size(); ++i) {
            if (in_ram[ram[i].first]) {  // 仅考虑当前在内存中的页面
                minHeap.emplace(ram[i].second, ram[i].first);  // (pageValue, PID)
            }
        }

        // 获取写感知权重 跳过一定数量的脏页面
        int dirtypass = getWriteAwareWeight(HR);
        // std::cout << "当前的写感知权重为：" << dirtypass << std::endl;
        int pass = 0; 

        // 4. 选择 pageValue 最低的 targetEvictNum 个页面，加入 freeList
        int evicted = 0;    // 临时变量
        while (!minHeap.empty() && evicted < targetEvictNum) {
            PageInfo page = minHeap.top();
            minHeap.pop();
            PID pid = page.second;
            if(dirty_in_ram[pid] && pass < dirtypass)
            {
                pass++;
                continue;       // 这里的continue表示跳过一个脏页面
            }
            freeList.push(pid);             // 将页面加入 freeList
            evicted++;                      // 更新驱逐计数
        }
    }

    // 提前页面写回算法
    // 之前的代码，直接在free-list上面操作会使得free-list直接被清空了
    void earlyPageWriteBack() {
        if (freeList.size() < ram.size() * freeListThreshold) {
            // std::cout << "执行提前页面写回算法" << std::endl;
            std::queue<PID> newFreeList;
            while (!freeList.empty()) {
                PID pid = freeList.front();
                freeList.pop();
                if (dirty_in_ram[pid]) {
                    writeBackToDisk(pid);
                    dirty_in_ram[pid] = false; // 标记为干净
                }
                newFreeList.push(pid); // 保留页面在 freeList
            }
            freeList = std::move(newFreeList);
        }
    }
    

    // 写回磁盘 
    void writeBackToDisk(PID pid) {
        dirty_in_ram[pid] = false;
        dirty_evicts++;
    }

    // 第一个参数 页面未命中次数 第二个参数 脏页面写回次数
    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override {

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);
        uint select_RAM_SIZE = 2000;

        if(EvictStrategy::RAM_SIZE==select_RAM_SIZE)
        {
            outputFile << "MMCLOCK"<< std::endl;
        }

        for (Access single_access : access_data) {
            checkSizes(single_access.pid);
            single_access.accesscount = 0;

            // 如果pid不在内存中
            access(single_access);

            // 是write操作则true标记为脏页面，否则则保持原来的状态 
            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }
        // 已写回的脏页面数量 + 内存中目前存在的脏页面数量？  
        // 第一个参数page_misses指的是未命中率 第二个参数是写回的次数write的数值也很重要
        return std::pair(page_misses, dirty_evicts + dirtyPages());
    }

    // 实际的页面访问 分为两种情况：
    // 1. 在内存中命中，updateElement 增加命中率次数 
    // 2. 未命中，insertElement 说明需要页面替换 增加未命中率 
    void access(const Access& access) override{
        if(in_ram[access.pid]) {
            updateElement(fast_finder[access.pid], access);
        } else {
            fast_finder[access.pid] = insertElement(access);   // 在指定的替换位置进行插入
            page_misses++;    // 只有在需要插入页面的时候增加，其他都不变，防止重复计算
        }
    }

    // 将access添加到内存中 需要通过evictone获取可以替换的页面的PID
    uint insertElement(const Access& access) override {
        evictNum++;
        PID evictPage = evictOne(access);
        next = fast_finder[evictPage];
        ram[next] = {access.pid, 0};
        return next++;
    }

    void updateElement(uint old, const Access&) override{
        HitInBuf++;
        ram[old].second = ram[old].second + 1;
    }
};