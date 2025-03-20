#ifndef change_Parameters
#define change_Parameters

#endif // change_Parameters

#include "EvictStrategy.hpp"

#define addWeight 1


using amclock_epoch_type = std::pair<PID, AccessCount>;
using amclock_epoch_upper = EvictStrategyHashVector<amclock_epoch_type>;

struct amclock_epoch: public amclock_epoch_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;
    uint epoch;

    amclock_epoch(uint Epoch): amclock_epoch_upper() ,epoch(Epoch){}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_epoch_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        isChange = false;
        // 比较命中率变化
        if(newHR>lastHR)
        {
            // 重复上一次动作
            lastWeight = lastWeight + lastAction;
        }
        // 命中率降低
        else
        {
            // 做上一次相反的动作
            lastAction = -(lastAction);
            lastWeight = lastWeight + lastAction;
        }
        // 记住这次的命中率，下次来对比
        lastHR = newHR;

        // 如果 lastWeight 不对劲，对其范围进行限制,重新开始

        if(lastWeight<=0)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=16)
        {
            lastWeight = 16;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access&) override
    {
        ram[old].second = ram[old].second+lastWeight;
        // 命中一次buf
        if(starCount)
            HitInBuf++;
    }

    PID evictOne(Access) override{
        // 缓冲区满了之后再开始统计命中率，不然第一次统计的命中率将会偏大。
        starCount = true;

        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                ram[next].second = ram[next].second-1;
                next++;
            }else{
                fast_finder.erase(ram[next].first);
                // 驱逐一个了
                // 更新驱逐数量，如果数量到阈值了，开始更新weight
                evictNum++;
                // 驱逐的频率
                uint yuzhi;
                if(epoch!=11 && epoch!=111)
                    yuzhi = ram.size()/epoch;
                else if(epoch==111)
                    yuzhi = ram.size()*4;
                else{
                    yuzhi = ram.size()*2;
                    }
                if(evictNum>=yuzhi)
                {
                    UpdateWeight();
                    // 重新统计命中率
                    evictNum = 0;
                    HitInBuf = 0;
                }
                return ram[next].first;
            }
        }
    }
};


using amclock_Weight_type = std::pair<PID, AccessCount>;
using amclock_Weight_upper = EvictStrategyHashVector<amclock_Weight_type>;

struct amclock_Weight: public amclock_Weight_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_Weight(int Weight): amclock_Weight_upper() ,lastWeight(Weight){}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_epoch_upper::reInit(ram_size);
        next = 0;
        //lastWeight = 2;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        isChange = false;
        // 比较命中率变化
        if(newHR>lastHR)
        {
            // 重复上一次动作
            lastWeight = lastWeight + lastAction;
        }
        // 命中率降低
        else
        {
            // 做上一次相反的动作
            lastAction = -(lastAction);
            lastWeight = lastWeight + lastAction;
        }
        // 记住这次的命中率，下次来对比
        lastHR = newHR;

        // 如果 lastWeight 不对劲，对其范围进行限制,重新开始

        if(lastWeight<=0)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=28)
        {
            lastWeight = 28;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access&) override
    {
        ram[old].second = ram[old].second+lastWeight;
        // 命中一次buf
        if(starCount)
            HitInBuf++;
    }

    PID evictOne(Access) override{
        // 缓冲区满了之后再开始统计命中率，不然第一次统计的命中率将会偏大。
        starCount = true;

        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                ram[next].second = ram[next].second-1;
                next++;
            }else{
                fast_finder.erase(ram[next].first);
                // 驱逐一个了
                // 更新驱逐数量，如果数量到阈值了，开始更新weight
                evictNum++;
                // 驱逐的频率

                if(evictNum>=ram.size()/2)
                {
                    UpdateWeight();
                    // 重新统计命中率
                    evictNum = 0;
                    HitInBuf = 0;
                }
                return ram[next].first;
            }
        }
    }
};

using amclock_action_type = std::pair<PID, AccessCount>;
using amclock_action_upper = EvictStrategyHashVector<amclock_action_type>;

struct amclock_action: public amclock_action_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_action(int Action): amclock_action_upper() ,lastAction(Action){}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_epoch_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        //lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        isChange = false;
        // 比较命中率变化
        if(newHR>lastHR)
        {
            // 重复上一次动作
            lastWeight = lastWeight + lastAction;
        }
        // 命中率降低
        else
        {
            // 做上一次相反的动作
            lastAction = -(lastAction);
            lastWeight = lastWeight + lastAction;
        }
        // 记住这次的命中率，下次来对比
        lastHR = newHR;

        // 如果 lastWeight 不对劲，对其范围进行限制,重新开始

        if(lastWeight<=0)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=28)
        {
            lastWeight = 28;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access&) override
    {
        ram[old].second = ram[old].second+lastWeight;
        // 命中一次buf
        if(starCount)
            HitInBuf++;
    }

    PID evictOne(Access) override{
        // 缓冲区满了之后再开始统计命中率，不然第一次统计的命中率将会偏大。
        starCount = true;

        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                ram[next].second = ram[next].second-1;
                next++;
            }else{
                fast_finder.erase(ram[next].first);
                // 驱逐一个了
                // 更新驱逐数量，如果数量到阈值了，开始更新weight
                evictNum++;
                // 驱逐的频率

                if(evictNum>=ram.size()/2)
                {
                    UpdateWeight();
                    // 重新统计命中率
                    evictNum = 0;
                    HitInBuf = 0;
                }
                return ram[next].first;
            }
        }
    }
};

using amclock_upLimit_type = std::pair<PID, AccessCount>;
using amclock_upLimit_upper = EvictStrategyHashVector<amclock_upLimit_type>;

struct amclock_upLimit: public amclock_upLimit_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;
    int upLimit;

    amclock_upLimit(int uplimit): amclock_upLimit_upper() ,upLimit(uplimit){}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_upLimit_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        isChange = false;
        // 比较命中率变化
        if(newHR>lastHR)
        {
            // 重复上一次动作
            lastWeight = lastWeight + lastAction;
        }
        // 命中率降低
        else
        {
            // 做上一次相反的动作
            lastAction = -(lastAction);
            lastWeight = lastWeight + lastAction;
        }
        // 记住这次的命中率，下次来对比
        lastHR = newHR;

        // 如果 lastWeight 不对劲，对其范围进行限制,重新开始

        if(lastWeight<=0)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=upLimit)
        {
            lastWeight = upLimit;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access&) override
    {
        ram[old].second = ram[old].second+lastWeight;
        // 命中一次buf
        if(starCount)
            HitInBuf++;
    }

    PID evictOne(Access) override{
        // 缓冲区满了之后再开始统计命中率，不然第一次统计的命中率将会偏大。
        starCount = true;

        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                ram[next].second = ram[next].second-1;
                next++;
            }else{
                fast_finder.erase(ram[next].first);
                // 驱逐一个了
                // 更新驱逐数量，如果数量到阈值了，开始更新weight
                evictNum++;
                // 驱逐的频率

                if(evictNum>=ram.size()/2)
                {
                    UpdateWeight();
                    // 重新统计命中率
                    evictNum = 0;
                    HitInBuf = 0;
                }
                return ram[next].first;
            }
        }
    }
};

using amclock_DW_uplimit_type = std::pair<PID, AccessCount>;
using amclock_DW_uplimit_upper = EvictStrategyHashVector<amclock_DW_uplimit_type>;

struct amclock_DW_uplimit: public amclock_DW_uplimit_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;
    int upLimit;

    amclock_DW_uplimit(int uplimit): amclock_DW_uplimit_upper() ,upLimit(uplimit){}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_DW_uplimit_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        isChange = false;
        // 比较命中率变化
        if(newHR>lastHR)
        {
            // 重复上一次动作
            lastWeight = lastWeight + lastAction;
        }
        // 命中率降低
        else
        {
            // 如果命中率显著下降，且没有新数据被再次读取，为扫描

            // 如果命中率显著下降，但是有新数据被再次读取，为热点转移

            // 如果命中率下降超过，可能是显著下降
            if(lastHR-newHR>0.4*(1-lastHR))
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<0.2*evictNum)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    isChange = true;
                    // 将旧数据全部变成lastweight
                    uint i = 0;
                    while(i < ram.size()) {
                        if(ram[i].second != 0){
                            // 这里可能不对，不能全部变成weight，可以都降低，然后还有优先级，不能全部降低后没有优先级了
                            ram[i].second = ram[i].second/2;
                        }
                        i++;
                    }
                }
            }
            // 如果命中率正常下降
            else
            {
                // 做上一次相反的动作
                lastAction = -(lastAction);
                lastWeight = lastWeight + lastAction;
            }
        }
        // 记住这次的命中率，下次来对比
        lastHR = newHR;

        // 如果 lastWeight 不对劲，对其范围进行限制,重新开始

        if(lastWeight<=1)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=upLimit)
        {
            lastWeight = upLimit;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+lastWeight;
        // 命中一次buf
        if(starCount)
            HitInBuf++;
    }

    PID evictOne(Access) override{
        // 缓冲区满了之后再开始统计命中率，不然第一次统计的命中率将会偏大。
        starCount = true;

        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                if(isChange)
                    ram[next].second = ram[next].second/2;
                else
                    ram[next].second = ram[next].second-1;
                next ++;
            }else{
                fast_finder.erase(ram[next].first);
                // 驱逐一个了
                // 更新驱逐数量，如果数量到阈值了，开始更新weight
                evictNum++;
                // 驱逐的频率
                if(evictNum>=ram.size()/2)
                {
                    UpdateWeight();
                    // 重新统计命中率
                    evictNum = 0;
                    HitInBuf = 0;
                }
                return ram[next].first;
            }
        }
    }
};


using amclock_DW_HR_type = std::pair<PID, AccessCount>;
using amclock_DW_HR_upper = EvictStrategyHashVector<amclock_DW_HR_type>;

/*
struct amclock_DW_HR: public amclock_DW_HR_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;
    int upLimit;
    double HRpercent;

    amclock_DW_HR(double hrpercent): amclock_DW_HR_upper() ,HRpercent(hrpercent){}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_DW_HR_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;
        upLimit = 24;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        isChange = false;
        // 比较命中率变化
        if(newHR>lastHR)
        {
            // 重复上一次动作
            lastWeight = lastWeight + lastAction;
        }
        // 命中率降低
        else
        {
            // 如果命中率显著下降，且没有新数据被再次读取，为扫描

            // 如果命中率显著下降，但是有新数据被再次读取，为热点转移

            // 如果命中率下降超过，可能是显著下降
            //if(lastHR-newHR>0.4*(1-lastHR))
            if(lastHR-newHR>HRpercent*(1-lastHR))
            {

                    isChange = true;
                    // 将旧数据全部变成lastweight
                    uint i = 0;
                    while(i < ram.size()) {
                        if(ram[i].second != 0){
                            // 这里可能不对，不能全部变成weight，可以都降低，然后还有优先级，不能全部降低后没有优先级了
                            ram[i].second = ram[i].second/2;
                        }
                        i++;
                    }
            }
            // 如果命中率正常下降
            else
            {
                // 做上一次相反的动作
                lastAction = -(lastAction);
                lastWeight = lastWeight + lastAction;
            }
        }
        // 记住这次的命中率，下次来对比
        lastHR = newHR;

        // 如果 lastWeight 不对劲，对其范围进行限制,重新开始

        if(lastWeight<=1)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=upLimit)
        {
            lastWeight = upLimit;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+lastWeight;
        // 命中一次buf
        if(starCount)
            HitInBuf++;
    }

    PID evictOne(Access) override{
        // 缓冲区满了之后再开始统计命中率，不然第一次统计的命中率将会偏大。
        starCount = true;

        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                if(isChange)
                    ram[next].second = ram[next].second/2;
                else
                    ram[next].second = ram[next].second-1;
                next ++;
            }else{
                fast_finder.erase(ram[next].first);
                // 驱逐一个了
                // 更新驱逐数量，如果数量到阈值了，开始更新weight
                evictNum++;
                // 驱逐的频率
                if(evictNum>=ram.size()/2)
                {
                    UpdateWeight();
                    // 重新统计命中率
                    evictNum = 0;
                    HitInBuf = 0;
                }
                return ram[next].first;
            }
        }
    }
};
*/

/*
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<0.2*evictNum)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    isChange = true;
                    // 将旧数据全部变成lastweight
                    uint i = 0;
                    while(i < ram.size()) {
                        if(ram[i].second != 0){
                            // 这里可能不对，不能全部变成weight，可以都降低，然后还有优先级，不能全部降低后没有优先级了
                            ram[i].second = ram[i].second/2;
                        }
                        i++;
                    }
                }

                if(isChange)
                    ram[next].second = ram[next].second/2;
                else
                    ram[next].second = ram[next].second-1;
                next ++;
*/


struct amclock_DW_HR: public amclock_DW_HR_upper {
    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;
    int upLimit;
    double HRpercent;

    int tmp_hit;
    int tmp_miss;
    int count;

    amclock_DW_HR(double hrpercent): amclock_DW_HR_upper() ,HRpercent(hrpercent){}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_DW_uplimit_upper::reInit(ram_size);
        next = 0;
        lastWeight = 20;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;
        upLimit = 24;
    }


    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);

        uint select_RAM_SIZE = 2000;

        if(EvictStrategy::RAM_SIZE==select_RAM_SIZE)
        {
            outputFile << "EACLOCK-FDW" << std::endl;
        }

        for(const Access& single_access: access_data){
            checkSizes(single_access.pid);
            // 如果pid不在内存中
            if(!in_ram[single_access.pid]){
                page_misses++;
                if(curr_count >= RAM_SIZE){
                    dirty_evicts += evict(single_access);
                }
                curr_count ++;
            }
            // 如果在内存中
            access(single_access);
            count++;

            // 假设手动弱化
            /*
            if(count==500000)
            {
                uint i = 0;
                while(i < ram.size()) {
                    ram[i].second = 0;
                    i++;
                }
            }
            */

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

        outputFile.close();

// change,lx,throughput
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

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        isChange = false;
        if(newHR>lastHR)
        {
            lastWeight = lastWeight + lastAction;
        }
        else
        {
            if(lastHR-newHR>HRpercent*(1-lastHR))
            {
                isChange = true;
                uint i = 0;
                while(i < ram.size()) {
                    if(ram[i].second != 0){
                        ram[i].second = ram[i].second/2;
                    }
                    i++;
                }
            }
            else
            {
                lastAction = -(lastAction);
                lastWeight = lastWeight + lastAction;
            }
        }
        lastHR = newHR;
        if(lastWeight<=1)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=upLimit)
        {
            lastWeight = upLimit;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+lastWeight;
        if(starCount)
            HitInBuf++;
        
        tmp_hit++;
    }

    PID evictOne(Access) override{
        starCount = true;
        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                if(isChange)
                    ram[next].second = ram[next].second/2;
                else
                    ram[next].second = ram[next].second-1;
                next ++;
            }else{
                fast_finder.erase(ram[next].first);
                evictNum++;
                if(evictNum>=ram.size()/2)
                {
                    UpdateWeight();
                    evictNum = 0;
                    HitInBuf = 0;
                }
                return ram[next].first;
            }
        }
    }
};