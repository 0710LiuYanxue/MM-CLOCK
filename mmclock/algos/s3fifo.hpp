//
// Created by dev on 09.10.24.
//

#include "EvictStrategy.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

struct S3fifo : public EvictStrategy {
    using upper = EvictStrategy;

    S3fifo() : upper() {}

    double small;
    double main;
    double ghost;

    int s_length;
    int m_length;
    int g_length;

    uint page_misses;
    uint dirty_evicts;

    int tmp_hit;
    int tmp_miss;
    int count;

    std::list<Access> s_queue;
    std::list<Access> g_queue;
    std::list<Access> m_queue;

    // 保存的是内存的fifo队列
    std::unordered_map<PID, std::list<Access>::iterator> hash_s;

    // 保存的是在内存中的fifo队列
    std::unordered_map<PID, std::list<Access>::iterator> hash_m;

    // 保存的是最近被淘汰的fifo队列，实际已经不在内存中了, ghost
    std::unordered_map<PID, std::list<Access>::iterator> hash_g;


    void reInit(RamSize ram_size) override {

        small = 0.1;
        main = 0.9;
        ghost = 0.9;

        s_length = small * ram_size;
        m_length = ram_size- s_length;
        g_length = ram_size - s_length;

        page_misses = 0;
        dirty_evicts = 0;

        tmp_hit = 0;
        tmp_miss = 0;
        count = 0;

        s_queue.clear();
        g_queue.clear();
        m_queue.clear();

        hash_s.clear();
        hash_m.clear();
        hash_g.clear();

        EvictStrategy::reInit(ram_size);
    }

    void scanQueue()
    {
        std::ofstream outputFile("s3fifo_debug.txt", std::ios_base::app);

        outputFile << "RamSize="<< EvictStrategy::RAM_SIZE<<" s_queue"<< std::endl;
        outputFile << "s_queue.size="<< s_queue.size()<< std::endl;
        outputFile << "hash_s.size="<< hash_s.size()<< std::endl;
        for (const Access& access : s_queue) {
            outputFile <<access.pid<<"."<<access.accesscount << "->";
        }
        outputFile << std::endl;
        outputFile << "RamSize="<< EvictStrategy::RAM_SIZE<<" m_queue"<< std::endl;
        outputFile << "m_queue.size="<< m_queue.size()<< std::endl;
        outputFile << "hash_m.size="<< hash_m.size()<< std::endl;
        for (const Access& access : m_queue) {
            outputFile <<access.pid<<"."<<access.accesscount << "->";
        }
        outputFile << std::endl;
        outputFile << "RamSize="<< EvictStrategy::RAM_SIZE<<" g_queue"<< std::endl;
        outputFile << "g_queue.size="<< g_queue.size()<< std::endl;
        outputFile << "hash_g.size="<< hash_g.size()<< std::endl;
        for (const Access& access : g_queue) {
            outputFile <<access.pid<<"."<<access.accesscount << "->";
        }
        outputFile << std::endl;
        outputFile.close();
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override {

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);
        uint select_RAM_SIZE = 2000;
        if(EvictStrategy::RAM_SIZE==select_RAM_SIZE)
        {
            outputFile << "S3-FIFO"<< std::endl;
        }

        for (Access single_access : access_data) {
            checkSizes(single_access.pid);
            single_access.accesscount = 0;
            // 访问 single_access
            if(EvictStrategy::RAM_SIZE==100)
            {
                if( static_cast<int>(s_queue.size())>s_length || static_cast<int>(m_queue.size())>m_length )
                {
                    outputFile << "Error="<< static_cast<int>(s_queue.size())-s_length <<" "<<static_cast<int>(m_queue.size())-m_length<< std::endl;
                }
            }
            access(single_access);
            if(EvictStrategy::RAM_SIZE==100)
            {
                //scanQueue();
            }

            count++;

            if(EvictStrategy::RAM_SIZE==select_RAM_SIZE)
            {
                int once_sum = 4000;
                if(count%once_sum==0 && count>=400000 && count<=700000)
                    // if(count%(EvictStrategy::RAM_SIZE)==0 && count>=400000 && count<=600000)
                    //if(count%EvictStrategy::RAM_SIZE==0 && count>=0 && count<=200000)
                {
                    outputFile<<1.0*tmp_hit*100/once_sum<< std::endl;
                    tmp_hit =0;
                }
            }
            

            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }
        //outputFile << "RamSize="<< EvictStrategy::RAM_SIZE<<" end"<< std::endl;
        outputFile.close();

        return std::pair(page_misses, dirty_evicts + dirtyPages());
    }

    // 将 access 添加到内存中
    void insert(const Access& pageX) {

        // 上面的逻辑不对，按照官网给的演示，明显是Squeue满了就驱逐，而不是整个满了才驱逐。而且，他给的实际代码和伪代码不一样

        // 如果在g中
        if(hash_g.find(pageX.pid)!= hash_g.end())
        {
            while(static_cast<int>(m_queue.size()) >= m_length)
            {
                evictM();
            }

            insertToM(pageX);
        }

        else
        {
            while(static_cast<int>(s_queue.size()) >= s_length)
            {
                evictS();
            } 

            insertToS(pageX);
        }
            
    }

    void insertToM(const Access& pageX) {
        m_queue.push_front(pageX);
        hash_m[pageX.pid] = m_queue.begin();
        while (static_cast<int>(m_queue.size()) > m_length)
        {
            Access tmp = *m_queue.rbegin();
            m_queue.erase(hash_m[tmp.pid]);
            hash_m.erase(tmp.pid);
        }
    }

    void removeFromM(const Access& pageX) {
        m_queue.erase(hash_m[pageX.pid]);
        hash_m.erase(pageX.pid);
    }

    void insertToG(const Access& pageX) {
        g_queue.push_front(pageX);
        hash_g[pageX.pid] = g_queue.begin();
        while (static_cast<int>(g_queue.size()) > g_length)
        {
            Access tmp = *g_queue.rbegin();
            g_queue.erase(hash_g[tmp.pid]);
            hash_g.erase(tmp.pid);
        }
    }

    void insertToS(const Access& pageX) {
        s_queue.push_front(pageX);
        hash_s[pageX.pid] = s_queue.begin();
    }

    void removeFromS(const Access& pageX) {
        s_queue.erase(hash_s[pageX.pid]);
        hash_s.erase(pageX.pid);
    }

    // 访问 pid
    void access(const Access& access) override {
        PID AccessPageId = access.pid;
        // 在s里面，增加计数
        if (hash_s.find(AccessPageId)!= hash_s.end())
        {
            tmp_hit++;
            hash_s[AccessPageId]->accesscount = (hash_s[AccessPageId]->accesscount>=3 ? 3 : hash_s[AccessPageId]->accesscount+1);
            // hash_s[AccessPageId]->write = (access.write || hash_s[AccessPageId]->write);
            // √
        }
        // 在m里面，增加计数
        else if (hash_m.find(AccessPageId)!= hash_m.end())
        {
            tmp_hit++;
            // hash_m[AccessPageId]->write = (access.write || hash_m[AccessPageId]->write);
            hash_m[AccessPageId]->accesscount = (hash_m[AccessPageId]->accesscount>=3 ? 3 : hash_m[AccessPageId]->accesscount+1);
        }
        // 如果不在内存中，则插入
        else {
            insert(access);
            page_misses++;

            tmp_miss++;
        }
    }

    void evict()
    {
        if(static_cast<int>(s_queue.size()) >= s_length)
            evictS();
        else evictM();
    }

    void evictS()
    {
        bool evicted = false;

        while ( evicted==false && s_queue.size()>0)
        {
            Access tmp = *s_queue.rbegin();
            if(tmp.accesscount>1)
            {
                insertToM(tmp);
                if(static_cast<int>(m_queue.size()) > m_length)
                {
                    evictM();
                }
                evicted = true;
            }
            else
            {
                insertToG(tmp);
                evicted = true;

                if(tmp.write)
                    dirty_evicts++;
            }

            removeFromS(tmp);
        }
    }

    void evictM()
    {
        bool evicted = false;
        while ( evicted==false && m_queue.size()>0 )
        {
            Access tmp = *m_queue.rbegin();
            if(tmp.accesscount>0)
            {
                removeFromM(tmp);
                tmp.accesscount = tmp.accesscount-1;
                insertToM(tmp);
            }
            else
            {
                removeFromM(tmp);
                evicted = true;

                if(tmp.write)
                    dirty_evicts++;
            }
        }
    }

    // 驱逐一个页面，并从各种容器中 erase 掉，并返回
    PID evictOne(Access access) override {
        // 根据需求实现驱逐逻辑，这里暂不实现具体的驱逐算法，只返回一个无效的 PID
        return access.pid;
    }
};