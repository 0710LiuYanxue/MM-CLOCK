//
// Created by dev on 09.10.24.
//

#include "EvictStrategy.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

struct LIRS : public EvictStrategy {
    using upper = EvictStrategy;

    LIRS() : upper() {}

    double Llirs_percent;
    double Lhirs_percent;

    int lirs_length;
    int hirs_length;

    int curr_LIR_sum;
    int curr_HIR_sum;

    int tmp_hit;
    int tmp_miss;
    int count;

    int cacheInbuf;
    uint page_misses;
    uint dirty_evicts;

    std::list<Access> s_lru_queue;
    std::list<Access> listQ_queue;

    // 保存的是 s_lru_queue 队列
    std::unordered_map<PID, std::list<Access>::iterator> hash_s_lru;

    // 保存的是 q_queue 队列
    std::unordered_map<PID, std::list<Access>::iterator> hash_listQ;


    void reInit(RamSize ram_size) override {

        Llirs_percent = 0.95;
        Lhirs_percent = 0.05;

        lirs_length = Llirs_percent*ram_size;
        hirs_length = ram_size - lirs_length;

        curr_LIR_sum = 0;
        curr_HIR_sum = 0;

        cacheInbuf = 0;

        page_misses = 0;
        dirty_evicts = 0;

        s_lru_queue.clear();
        listQ_queue.clear();

        hash_s_lru.clear();
        hash_listQ.clear();

        EvictStrategy::reInit(ram_size);
    }

    void scanQueue()
    {
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {

        int count = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);
        uint select_RAM_SIZE = 2000;
        if(EvictStrategy::RAM_SIZE==select_RAM_SIZE)
        {
            outputFile << "LIRS:"<<EvictStrategy::RAM_SIZE<< std::endl;
        }

        for (Access single_access : access_data) {
            checkSizes(single_access.pid);
            single_access.accesscount = 0;
            // 访问 single_access
            if(EvictStrategy::RAM_SIZE==100)
            {
                // outputFile << "RamSize= "<< EvictStrategy::RAM_SIZE<<" single_access="<<single_access.pid<< std::endl;
                // outputFile << "count==>"<<count<< std::endl;

                //outputFile << "s_length="<< s_length<< std::endl;
                //outputFile << "m_length="<< m_length<< std::endl;
                //outputFile << "g_length="<< g_length<< std::endl;
            }

            access(single_access);

            assert((curr_LIR_sum+static_cast<int>(listQ_queue.size()))<=EvictStrategy::RAM_SIZE);

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

/*
        for(uint i=0;i<(page_misses);i++)
        {
            sleepRead();
        }

        for(uint i=0;i<(dirty_evicts+ dirtyPages());i++)
        {
            sleepWrite();
        }

        std::cout<<"totalSeconds:";
*/

        return std::pair(page_misses, dirty_evicts + dirtyPages());
    }

    void removeFrom__Q(const Access& pageX) {
        listQ_queue.erase(hash_listQ[pageX.pid]);
        hash_listQ.erase(pageX.pid);
    }

    void insertTolistQ(const Access& pageX) {
        listQ_queue.push_front(pageX);
        hash_listQ[pageX.pid] = listQ_queue.begin();

        // 当 Q 的空间不足时，则剔除 Q 尾部的数据
        while (static_cast<int>(listQ_queue.size()) > hirs_length)
        {
            Access tmp = *listQ_queue.rbegin();
            listQ_queue.erase(hash_listQ[tmp.pid]);
            hash_listQ.erase(tmp.pid);

            if(tmp.write)
                dirty_evicts++;
            
            // 如果数据在 S 中,则将数据块设为非驻留 HIR 块
            // 此时实际上已经不在内存了，所以cacheInbuf--，但是先在s中保留一下历史记录
            // 如果不在 S 中，则将数据历史彻底剔除。
            if(hash_s_lru.find(tmp.pid)!=hash_s_lru.end())
            {
                assert(hash_s_lru[tmp.pid]->isLIR==false);

                hash_s_lru[tmp.pid]->isInbuf = false;
                cacheInbuf--;
            }
        }
    }

    void insertToS(const Access& pageX)
    {
        s_lru_queue.push_front(pageX);
        hash_s_lru[pageX.pid] = s_lru_queue.begin();
    }

    void removeFromSToTop(const Access& pageX)
    {
        s_lru_queue.erase(hash_s_lru[pageX.pid]);
        hash_s_lru.erase(pageX.pid);

        insertToS(pageX);
    }

    void removeFromS(const Access& pageX)
    {
        s_lru_queue.erase(hash_s_lru[pageX.pid]);
        hash_s_lru.erase(pageX.pid);
    }

    /*
    我们在LIRS堆栈S上定义了一个称为“堆栈修剪”的操作，该操作移除堆栈底部的HIR块，直到LIR块位于堆栈底部。
    
    这个操作有两个目的:
    (1)我们保证栈底的块总是属于LIR块集。
    (2)在底部的LIR块被移除之后，那些邻接地位于其上方的HIR块将没有机会将它们的状态从HIR改变为LIR，
    因为它们的新近度大于LIR块的新的最大新近度。
    */
   void stackpruning()
    {
        // 剪枝，直到底部是LIR
        while (1)
        {
            assert(s_lru_queue.size()>0);

            Access tmp = *s_lru_queue.rbegin();
            if(tmp.isLIR==true)
            {
                return;
            }
            else if(tmp.isLIR==false)
            {
                // 从s里面剔除
                removeFromS(tmp);
                if(tmp.isInbuf==true && tmp.write==true)
                {
                    //dirty_evicts++;
                }
            }
            else
                throw std::runtime_error("异常块!");


        }
    }

    void access(const Access& access) override
    {
        Access tmp_access = access;
        PID AccessPageId = access.pid;

        // q中一定保存了所有的 HIR，s中保存的可能是部分HIR
        // s中一定保存了所有的 LIR，

        // 如果命中的是 q 中的HIR，那么将和s中的LIR替换一下优先级
        if(hash_listQ.find(AccessPageId)!=hash_listQ.end())
        {
            Access tmp = *hash_listQ[AccessPageId];

            // tmp.write = (access.write || tmp.write);

            // 是 isInbuf 的 HIR 块
            // 在访问HIR驻留块X时：这是该高速缓存中的命中。我们把它移到栈S的顶部。

            assert(tmp.isLIR==false && tmp.isInbuf==true);

            /*
            对于块X有两种情况:
            (1)如果X在栈S中，我们将其状态更改为LIR,并且移动到s的头部。该块也从列表Q中删除。
            S底部的LIR块移动到列表Q的头部，其状态变为HIR。然后进行堆栈修剪。（升级X，降级另一个）
            */

            if(hash_s_lru.find(AccessPageId)!=hash_s_lru.end())
            {
                tmp.isLIR = true;
                curr_LIR_sum++;

                removeFrom__Q(tmp);
                removeFromSToTop(tmp);

                Access tail = *s_lru_queue.rbegin();
                assert(tail.isLIR==true);
                tail.isLIR = false;
                curr_LIR_sum--;
                assert(tail.isInbuf==true);
                insertTolistQ(tail);

                s_lru_queue.rbegin()->isLIR = false;
                stackpruning();
            }
            // (2)如果X不在堆栈S中，我们将其状态保留在HIR中，并将其移动到列表Q的头部。
            else
            {
                insertToS(tmp);
                removeFrom__Q(tmp);
                insertTolistQ(tmp);
            }

            tmp_hit++;
        }

        // 如果命中在 s中，
        else if(hash_s_lru.find(AccessPageId)!=hash_s_lru.end())
        {
            Access tmp = *hash_s_lru[AccessPageId];

            // tmp.write = (access.write || tmp.write);

            // 是 s 中的 LIR 块
            if(tmp.isLIR==true)
            {
                assert(tmp.isInbuf==true);

                // 如果块不是s的底部，直接移动到头部
                if(tmp.pid != s_lru_queue.rbegin()->pid)
                {
                    removeFromSToTop(tmp);
                }
                // 如果块是s的底部，移动到头部,并剪枝
                else
                {
                    removeFromSToTop(tmp);
                    stackpruning();
                }

                tmp_hit++;
            }

            // 如果命中的是 s 中的 notInbuf 的 HIR 块

            /*
            访问的是非驻留的 HIR X，那么就需要新的内存空间，则把 Q 队头的 HIR 数据移除，
            S 中的对应 HIR 变为非驻留。X 的数据占用内存空间，X 移到 S 栈顶。处理情况同 HIR X。
            
            在访问HIR非驻留块X时：这是未命中。
            我们删除列表Q尾部的HIR驻留块（然后它变成非驻留块），并将其从该高速缓存中替换出来。
            然后我们将请求的块X加载到释放的缓冲区中，并将其放置在堆栈S的顶部。
            对于块X有两种情况：
            (1）如果X在栈S中，我们将其状态更改为LIR，并将栈S底部的LIR块移动到列表Q的末尾，其状态更改为HIR。然后进行堆栈修剪。
            (2)如果X不在堆栈S中，我们将其状态保留在HIR中，并将其放置在列表Q的末尾。
            */
            else if(tmp.isLIR==false && tmp.isInbuf==false)
            {
                // 未命中
                page_misses++;
                tmp_miss++;

                Access q_tail = *listQ_queue.rbegin();
                assert(q_tail.isInbuf==true);

                // 在Q中不一定在S中，因为可能被剪枝掉了
                if(hash_s_lru.find(q_tail.pid)!=hash_s_lru.end())
                    hash_s_lru[q_tail.pid]->isInbuf = false;
                
                removeFrom__Q(q_tail);

                if(q_tail.write)
                    dirty_evicts++;

                if(hash_s_lru.find(AccessPageId)!=hash_s_lru.end())
                {
                    tmp.isLIR = true;
                    curr_LIR_sum++;
                    removeFromSToTop(tmp);

                    Access tail = *s_lru_queue.rbegin();
                    assert(tail.isLIR==true);
                    tail.isLIR = false;
                    curr_LIR_sum--;
                    assert(tail.isInbuf==true);
                    insertTolistQ(tail);

                    s_lru_queue.rbegin()->isLIR = false;
                    stackpruning();
                }
                // (2)如果X不在堆栈S中，我们将其状态保留在HIR中，并将其移动到列表Q的头部。
                else
                {
                    insertToS(tmp);
                    insertTolistQ(tmp);
                }
            }
            else
            {
                throw std::runtime_error("命中异常s中的块!");
                // 未命中
                page_misses++;
            }
        }
        // 如果不在s和q中的话，一定不在内存中,添加
        else
        {
            page_misses++;
            tmp_miss++;

            // 如果 LIR 块的数量还没满，则加进来的都为 LIR 块,添加到 S-queue 队头
            /*
            当LIR块集未满时，所有被引用的块都被赋予LIR状态，直到其大小达到Llirs。
            此后，HIR状态被给予第一次被引用的任何块
            */
            if (curr_LIR_sum < lirs_length)
            {
                tmp_access.isLIR = true;
                curr_LIR_sum++;
                tmp_access.isInbuf = true;
                
                insertToS(tmp_access);
            }
            // 当 LIR 的数量到达上限时，新加入的数据块为 HIR ，同时写入 S-queue 和 Q 的头部
            else
            {
                tmp_access.isLIR = false;
                tmp_access.isInbuf = true;

                insertToS(tmp_access);
                insertTolistQ(tmp_access);
            }

        }

    }

    // 驱逐一个页面，并从各种容器中 erase 掉，并返回
    PID evictOne(Access access) override {
        // 根据需求实现驱逐逻辑，这里暂不实现具体的驱逐算法，只返回一个无效的 PID
        return access.pid;
    }
};