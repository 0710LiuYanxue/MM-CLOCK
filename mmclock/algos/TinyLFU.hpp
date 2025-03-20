//
// Created by dev on 09.10.24.
//

#include "EvictStrategy.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct W_TinyLFU : public EvictStrategy {
    using upper = EvictStrategy;

    W_TinyLFU() : upper() {}

    double window_;
    double probation;
    double protect;

    int window_length;
    int probation_length;
    int protect_length;

    int sum_access;
    int W;

    uint page_misses;
    uint dirty_evicts;

    std::list<Access> window_queue;
    std::list<Access> probation_queue;
    std::list<Access> protect_queue;

    // 保存的是内存的
    std::unordered_map<PID, std::list<Access>::iterator> hash_window;

    // 保存的是在内存中
    std::unordered_map<PID, std::list<Access>::iterator> hash_probation;

    // 保存的是最近被淘
    std::unordered_map<PID, std::list<Access>::iterator> hash_protect;

    // 保存的是所有的频率
    std::unordered_map<PID, int> hash_TinyLFU;


    void reInit(RamSize ram_size) override {

        window_ = 0.01;
        probation = 0.19;
        protect = 0.80;

        window_length = window_ * ram_size;
        probation_length = ram_size * probation;
        protect_length = ram_size - window_length - probation_length;

        sum_access = 0;
        W = ram_size*32;

        page_misses = 0;
        dirty_evicts = 0;

        window_queue.clear();
        protect_queue.clear();
        probation_queue.clear();

        hash_window.clear();
        hash_probation.clear();
        hash_protect.clear();
        hash_TinyLFU.clear();

        EvictStrategy::reInit(ram_size);
    }

    void scanQueue()
    {
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override {

        for (Access single_access : access_data) {
            checkSizes(single_access.pid);
            single_access.accesscount = 0;
            // 访问 single_access
            access(single_access);
            if(EvictStrategy::RAM_SIZE==100)
            {
                //scanQueue();
            }

            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }

        return std::pair(page_misses, dirty_evicts + dirtyPages());
    }


    void removeFromProtation(const Access& pageX) {
        probation_queue.erase(hash_probation[pageX.pid]);
        hash_probation.erase(pageX.pid);
    }

    void insertToWindow(const Access& pageX) {
        window_queue.push_front(pageX);
        hash_window[pageX.pid] = window_queue.begin();
        // window 满了，开始进入 TinyLFU 的判断是否允许阶段
        while (static_cast<int>(window_queue.size()) > window_length)
        {
            // 先从 Window 中剔除
            Access Candidate = *window_queue.rbegin();
            window_queue.erase(hash_window[Candidate.pid]);
            hash_window.erase(Candidate.pid);

            /*
            这个被移出的缓存项称为 Candidate (候选项)，将面临 2 种情况：
            (1)如果 Probation Cache 未满，从 Window Cache 中移出的缓存项会直接写入 Probation Cache 中；
            */
            if(static_cast<int>(probation_queue.size()) < probation_length)
            {
                insertToProtatin(Candidate);
            }
            // （2） 如果 protect 没满，则插入 protect 中，
            //else if(static_cast<int>(protect_queue.size()) < protect_length)

            // (2)如果 Probation Cache 已满，则进入到 TinyLFU 过滤器比较阶段，将 Candidate 和 victim 比较
            else
            {
                Access victim = *probation_queue.rbegin();
                if(hash_TinyLFU[victim.pid] > hash_TinyLFU[Candidate.pid])
                {
                    // pk不过 victim，直接抛弃 Candidate，
                }
                else
                {
                    // pk 过 victim，删除victim， Candidate 进入 protation ，
                    removeFromProtation(victim);
                    insertToProtatin(Candidate);
                }
            }
        }
    }

    void insertToProtatin(const Access& pageX) {
        probation_queue.push_front(pageX);
        hash_probation[pageX.pid] = probation_queue.begin();
    }

    void insertToProtect(const Access& pageX) {
        protect_queue.push_front(pageX);
        hash_protect[pageX.pid] = protect_queue.begin();
        // protect_queue 满了
        while (static_cast<int>(protect_queue.size()) > protect_length)
        {
            // 先从 protect_queue 中剔除
            Access Candidate = *protect_queue.rbegin();
            protect_queue.erase(hash_protect[Candidate.pid]);
            hash_protect.erase(Candidate.pid);

            // 降级到 protation 中
            insertToProtatin(Candidate);
        }
    }

    void removeFromWindow(const Access& pageX) {
        window_queue.erase(hash_window[pageX.pid]);
        hash_window.erase(pageX.pid);
    }



    void removeFromProtect(const Access& pageX) {
        protect_queue.erase(hash_protect[pageX.pid]);
        hash_protect.erase(pageX.pid);
    }



    // 访问 pid，如果 pid 不在内存中，放入容器中，并设置各种容器参数
    // 如果容器已经在内存中，则更新容器的顺序冷热性
    void access(const Access& access) override {
        PID AccessPageId = access.pid;

        // 记录所有pid的次数
        hash_TinyLFU[AccessPageId]++;
        sum_access++;

        if(sum_access>W)
        {
            // 衰减,和衰减的时间有关系
            for (auto pair : hash_TinyLFU) {
                // 可以在这里对 pair.second 进行其他操作，比如修改值等
                pair.second = pair.second/2;
            }
            std::ofstream outputFile("TinyLFUDebug.txt", std::ios_base::app);
            outputFile<<"衰减"<<std::endl;
            sum_access=0;
        }

        // 在 window 缓存中,
        if(hash_window.find(AccessPageId)!=hash_window.end())
        {
            Access tmp = *hash_window[AccessPageId];
            // 提升到 window 头部
            removeFromWindow(tmp);
            insertToWindow(tmp);
        }
        // 在 protation 中,移动到 protect 中
        else if(hash_probation.find(AccessPageId)!=hash_probation.end())
        {
            // 先从 protation 中移除
            Access tmp = *hash_probation[AccessPageId];
            removeFromProtation(tmp);

            // 添加到 protect 中
            insertToProtect(tmp);

        }
        // 在 protect 中,移动到头部
        else if(hash_protect.find(AccessPageId)!=hash_protect.end())
        {
            Access tmp = *hash_protect[AccessPageId];
            // 提升到 lru 头部
            removeFromProtect(tmp);
            insertToProtect(tmp);
        }
        // 不在这三个缓存中缓存中，新数据
        else
        {
            page_misses++;
            // 先插入到 Window 中
            insertToWindow(access);
        }
        assert(protect_queue.size()+window_queue.size()+probation_queue.size()<=EvictStrategy::RAM_SIZE);
    }


    // 驱逐一个页面，并从各种容器中 erase 掉，并返回
    PID evictOne(Access access) override {
        // 根据需求实现驱逐逻辑，这里暂不实现具体的驱逐算法，只返回一个无效的 PID
        return access.pid;
    }
};



struct TinyLFU_3 : public EvictStrategy {
    using upper = EvictStrategy;

    TinyLFU_3() : upper() {}

    double window_;
    double probation;
    double protect;

    int window_length;
    int probation_length;
    int protect_length;

    int sum_access;
    int W;

    uint page_misses;
    uint dirty_evicts;

    std::list<Access> window_queue;
    std::list<Access> probation_queue;
    std::list<Access> protect_queue;

    // 保存的是内存的
    std::unordered_map<PID, std::list<Access>::iterator> hash_window;

    // 保存的是在内存中
    std::unordered_map<PID, std::list<Access>::iterator> hash_probation;

    // 保存的是最近被淘
    std::unordered_map<PID, std::list<Access>::iterator> hash_protect;

    // 保存的是所有的频率
    std::unordered_map<PID, int> hash_TinyLFU_1;

    // 保存的是所有的频率
    std::unordered_map<PID, int> hash_TinyLFU_2;

    // 保存的是所有的频率
    std::unordered_map<PID, int> hash_TinyLFU_3;


    void reInit(RamSize ram_size) override {

        window_ = 0.01;
        probation = 0.19;
        protect = 0.80;

        window_length = window_ * ram_size;
        probation_length = ram_size * probation;
        protect_length = ram_size - window_length - probation_length;

        sum_access = 0;
        W = 10000;

        page_misses = 0;
        dirty_evicts = 0;

        window_queue.clear();
        protect_queue.clear();
        probation_queue.clear();

        hash_window.clear();
        hash_probation.clear();
        hash_protect.clear();

        hash_TinyLFU_1.clear();
        hash_TinyLFU_2.clear();
        hash_TinyLFU_3.clear();

        EvictStrategy::reInit(ram_size);
    }

    void scanQueue()
    {
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override {

        for (Access single_access : access_data) {
            checkSizes(single_access.pid);
            single_access.accesscount = 0;
            // 访问 single_access
            access(single_access);
            if(EvictStrategy::RAM_SIZE==100)
            {
                //scanQueue();
            }

            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }

        return std::pair(page_misses, dirty_evicts + dirtyPages());
    }


    void removeFromProtation(const Access& pageX) {
        probation_queue.erase(hash_probation[pageX.pid]);
        hash_probation.erase(pageX.pid);
    }

    void insertToWindow(const Access& pageX) {
        window_queue.push_front(pageX);
        hash_window[pageX.pid] = window_queue.begin();
        // window 满了，开始进入 TinyLFU 的判断是否允许阶段
        while (static_cast<int>(window_queue.size()) > window_length)
        {
            // 先从 Window 中剔除
            Access Candidate = *window_queue.rbegin();
            window_queue.erase(hash_window[Candidate.pid]);
            hash_window.erase(Candidate.pid);

            /*
            这个被移出的缓存项称为 Candidate (候选项)，将面临 2 种情况：
            (1)如果 Probation Cache 未满，从 Window Cache 中移出的缓存项会直接写入 Probation Cache 中；
            */
            if(static_cast<int>(probation_queue.size()) < probation_length)
            {
                insertToProtatin(Candidate);
            }
            // （2） 如果 protect 没满，则插入 protect 中，
            //else if(static_cast<int>(protect_queue.size()) < protect_length)

            // (2)如果 Probation Cache 已满，则进入到 TinyLFU 过滤器比较阶段，将 Candidate 和 victim 比较
            else
            {
                Access victim = *probation_queue.rbegin();
                int victim_lfu = getTinyLFU(victim.pid);
                int Candidate_lfu = getTinyLFU(Candidate.pid);
                if(victim_lfu > Candidate_lfu)
                {
                    // pk不过 victim，直接抛弃 Candidate，
                }
                else
                {
                    // pk 过 victim，删除victim， Candidate 进入 protation ，
                    removeFromProtation(victim);
                    insertToProtatin(Candidate);
                }
            }
        }
    }

    void insertToProtatin(const Access& pageX) {
        probation_queue.push_front(pageX);
        hash_probation[pageX.pid] = probation_queue.begin();
    }

    void insertToProtect(const Access& pageX) {
        protect_queue.push_front(pageX);
        hash_protect[pageX.pid] = protect_queue.begin();
        // protect_queue 满了
        while (static_cast<int>(protect_queue.size()) > protect_length)
        {
            // 先从 protect_queue 中剔除
            Access Candidate = *protect_queue.rbegin();
            protect_queue.erase(hash_protect[Candidate.pid]);
            hash_protect.erase(Candidate.pid);

            // 降级到 protation 中
            insertToProtatin(Candidate);
        }
    }

    void removeFromWindow(const Access& pageX) {
        window_queue.erase(hash_window[pageX.pid]);
        hash_window.erase(pageX.pid);
    }



    void removeFromProtect(const Access& pageX) {
        protect_queue.erase(hash_protect[pageX.pid]);
        hash_protect.erase(pageX.pid);
    }

    void resetTinyLFU()
    {
        for(int i=0;i<10000;i++)
        {
            hash_TinyLFU_1[i] = 0;
            hash_TinyLFU_2[i] = 0;
            hash_TinyLFU_3[i] = 0;
        }
    }

    void updateTinyLFU(PID AccessPageId)
    {
        int a = hash_TinyLFU_1[AccessPageId];
        int b = hash_TinyLFU_2[AccessPageId];
        int c = hash_TinyLFU_3[AccessPageId];

        if (a > b) {
            std::swap(a, b);
        }
        if (b > c) {
            std::swap(b, c);
        }
        if (a > b) {
            std::swap(a, b);
        }

        int new_lfu = a+1;

        hash_TinyLFU_1[AccessPageId] = new_lfu;
        hash_TinyLFU_2[AccessPageId] = b;
        hash_TinyLFU_3[AccessPageId] = c;

        sum_access++;

        if(sum_access>W)
        {
            // 衰减,和衰减的时间有关系
            for (auto pair : hash_TinyLFU_1) {
                // 可以在这里对 pair.second 进行其他操作，比如修改值等
                pair.second = pair.second/2;
            }

            for (auto pair : hash_TinyLFU_2) {
                // 可以在这里对 pair.second 进行其他操作，比如修改值等
                pair.second = pair.second/2;
            }

            for (auto pair : hash_TinyLFU_3) {
                // 可以在这里对 pair.second 进行其他操作，比如修改值等
                pair.second = pair.second/2;
            }
            std::ofstream outputFile("TinyLFUDebug.txt", std::ios_base::app);
            outputFile<<"衰减"<<std::endl;
            sum_access=0;
        }
    }

    int getTinyLFU(PID AccessPageId)
    {
        int a = hash_TinyLFU_1[AccessPageId];
        int b = hash_TinyLFU_2[AccessPageId];
        int c = hash_TinyLFU_3[AccessPageId];
        return std::min(a,std::min(b,c));
    }

    // 访问 pid，如果 pid 不在内存中，放入容器中，并设置各种容器参数
    // 如果容器已经在内存中，则更新容器的顺序冷热性
    void access(const Access& access) override {
        PID AccessPageId = access.pid;

        // 更新 TinyLFU
        updateTinyLFU(AccessPageId);

        // 在 window 缓存中,
        if(hash_window.find(AccessPageId)!=hash_window.end())
        {
            Access tmp = *hash_window[AccessPageId];
            // 提升到 window 头部
            removeFromWindow(tmp);
            insertToWindow(tmp);
        }
        // 在 protation 中,移动到 protect 中
        else if(hash_probation.find(AccessPageId)!=hash_probation.end())
        {
            // 先从 protation 中移除
            Access tmp = *hash_probation[AccessPageId];
            removeFromProtation(tmp);

            // 添加到 protect 中
            insertToProtect(tmp);

        }
        // 在 protect 中,移动到头部
        else if(hash_protect.find(AccessPageId)!=hash_protect.end())
        {
            Access tmp = *hash_protect[AccessPageId];
            // 提升到 lru 头部
            removeFromProtect(tmp);
            insertToProtect(tmp);
        }
        // 不在这三个缓存中缓存中，新数据
        else
        {
            page_misses++;
            // 先插入到 Window 中
            insertToWindow(access);
        }
        assert(protect_queue.size()+window_queue.size()+probation_queue.size()<=EvictStrategy::RAM_SIZE);
    }


    // 驱逐一个页面，并从各种容器中 erase 掉，并返回
    PID evictOne(Access access) override {
        // 根据需求实现驱逐逻辑，这里暂不实现具体的驱逐算法，只返回一个无效的 PID
        return access.pid;
    }
};