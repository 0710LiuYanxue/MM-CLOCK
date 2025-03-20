#ifndef amclock_hpp
#define amclock_hpp

#endif // amclock

#include "EvictStrategy.hpp"


#define addWeight 1

using amclock_type = std::pair<PID, AccessCount>;
using amclock_upper = EvictStrategyHashVector<amclock_type>;

struct amclock: public amclock_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;

    amclock(): amclock_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = addWeight;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        // 比较命中率变化
        if(newHR>lastHR)
        {
            // 重复上一次动作
            lastWeight = lastWeight + lastAction;
        }
        else
        {
            // 做上一次相反的动作
            lastAction = -(lastAction);
            lastWeight = lastWeight + lastAction;
        }
        // 记住这次的命中率，下次来对比
        lastHR = newHR;

        // 如果 lastWeight 不对劲，对其范围进行限制,重新开始

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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

using amclock1_type = std::pair<PID, AccessCount>;
using amclock1_upper = EvictStrategyHashVector<amclock1_type>;

struct amclock1: public amclock1_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;

    amclock1(): amclock1_upper() {}
    uint next =0;

    
    void reInit(RamSize ram_size) override{
        amclock1_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = addWeight;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 1};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        // 比较命中率变化
        if(newHR>lastHR)
        {
            // 重复上一次动作
            lastWeight = lastWeight + lastAction;
        }
        else
        {
            // 做上一次相反的动作
            lastAction = -(lastAction);
            lastWeight = lastWeight + lastAction;
        }
        // 记住这次的命中率，下次来对比
        lastHR = newHR;

        // 如果 lastWeight 不对劲，对其范围进行限制,重新开始

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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


using amclock2_type = std::pair<PID, AccessCount>;
using amclock2_upper = EvictStrategyHashVector<amclock2_type>;

struct amclock2: public amclock2_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;

    amclock2(): amclock2_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock2_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = addWeight;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
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

            // 如果命中率下降超过0.05/5%，可能是显著下降
            if(lastHR-newHR>0.05)
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<10)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    lastWeight = 2;

                    // 将旧数据全部变成lastweight
                    uint i = 0;
                    while(i < ram.size()) {
                        if(ram[i].second != 0){
                            // 这里可能不对，不能全部变成weight，可以都降低，然后还有优先级，不能全部降低后没有优先级了
                            ram[i].second = lastWeight;
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

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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
                ram[next].second = ram[next].second-1;
                next ++;
            }else{
                fast_finder.erase(ram[next].first);
                // 驱逐一个了
                // 更新驱逐数量，如果数量到阈值了，开始更新weight
                evictNum++;
                // 驱逐的频率
                // ram.size()/2 > ram.size()/4
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

using amclock3_type = std::pair<PID, AccessCount>;
using amclock3_upper = EvictStrategyHashVector<amclock3_type>;

struct amclock3: public amclock3_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;

    amclock3(): amclock3_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock3_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = addWeight;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
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

            // 如果命中率下降超过0.05/5%，可能是显著下降
            if(lastHR-newHR>0.05)
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<10)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    lastWeight = 2;

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

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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

using amclock_4_type = std::pair<PID, AccessCount>;
using amclock_4_upper = EvictStrategyHashVector<amclock_4_type>;

struct amclock_4: public amclock_4_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;

    amclock_4(): amclock_4_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_4_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = -1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
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

            // 如果命中率下降超过0.05/5%，可能是显著下降
            if(lastHR-newHR>0.07)
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<10)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    lastWeight = 2;

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

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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


using amclock_5_type = std::pair<PID, AccessCount>;
using amclock_5_upper = EvictStrategyHashVector<amclock_5_type>;

struct amclock_5: public amclock_5_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;

    amclock_5(): amclock_5_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_5_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
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

            // 如果命中率下降超过0.05/5%，可能是显著下降
            if(lastHR-newHR>0.05)
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<10)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    lastWeight = 2;

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

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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
                ram[next].second = ram[next].second/2;
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

using amclock_6_type = std::pair<PID, AccessCount>;
using amclock_6_upper = EvictStrategyHashVector<amclock_6_type>;

struct amclock_6: public amclock_6_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;

    amclock_6(): amclock_6_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_6_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
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

            // 如果命中率下降超过0.05/5%，可能是显著下降
            if(lastHR-newHR>0.4*(1-lastHR))
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<10)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    lastWeight = 2;

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

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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

using amclock_7_type = std::pair<PID, AccessCount>;
using amclock_7_upper = EvictStrategyHashVector<amclock_7_type>;

struct amclock_7: public amclock_7_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;

    amclock_7(): amclock_7_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_7_upper::reInit(ram_size);
        next = 0;
        lastWeight = 2;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        // 获取这段时间的命中率
        double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
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

            // 如果命中率下降超过0.05/5%，可能是显著下降
            if(lastHR-newHR>0.4*(1-lastHR))
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<10)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    lastWeight = 2;

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

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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
                if(lastAction<0)
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


using amclock_8_type = std::pair<PID, AccessCount>;
using amclock_8_upper = EvictStrategyHashVector<amclock_8_type>;

struct amclock_8: public amclock_8_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_8(): amclock_8_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_8_upper::reInit(ram_size);
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

            // 如果命中率下降超过0.05/5%，可能是显著下降
            if(lastHR-newHR>0.4*(1-lastHR))
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<10)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    lastWeight = 2;
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

        if( lastWeight<=0 || lastWeight>20)
        {
            lastWeight = 2;
            lastHR = 0;
            lastAction = addWeight;
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

using amclock_9_type = std::pair<PID, AccessCount>;
using amclock_9_upper = EvictStrategyHashVector<amclock_9_type>;

struct amclock_9: public amclock_9_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_9(): amclock_9_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_9_upper::reInit(ram_size);
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

            // 如果命中率下降超过0.05/5%，可能是显著下降
            if(lastHR-newHR>0.4*(1-lastHR))
            {
                // 如果没几个命中，说明是纯扫描，不做处理
                if(HitInBuf<10)
                {

                }
                // 如果有一些命中，可能是扫描+正常读取，可能是热点转移，可能是热点转移+正常读取，全部按照一种情况进行处理
                else
                {
                    lastWeight = 2;
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
        else if(lastWeight>=20)
        {
            lastWeight = 20;
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

using amclock_10_type = std::pair<PID, AccessCount>;
using amclock_10_upper = EvictStrategyHashVector<amclock_10_type>;

struct amclock_10: public amclock_10_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_10(): amclock_10_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_10_upper::reInit(ram_size);
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
        else if(lastWeight>=20)
        {
            lastWeight = 20;
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

using amclock_11_type = std::pair<PID, AccessCount>;
using amclock_11_upper = EvictStrategyHashVector<amclock_11_type>;

struct amclock_11: public amclock_11_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_11(): amclock_11_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_11_upper::reInit(ram_size);
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

        if(lastWeight<=1)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=20)
        {
            lastWeight = 20;
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
                ram[next].second = ram[next].second/2;
                next++;
            }else{
                fast_finder.erase(ram[next].first);
                // 驱逐一个了
                // 更新驱逐数量，如果数量到阈值了，开始更新weight
                evictNum++;
                // 驱逐的频率
                if(evictNum>=ram.size()/4)
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



using amclock_WA15_type = std::pair<PID, AccessCount>;
using amclock_WA15_upper = EvictStrategyHashVector<amclock_WA15_type>;

struct amclock_WA15: public amclock_WA15_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_WA15(): amclock_WA15_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_WA15_upper::reInit(ram_size);
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

    void updateElement(uint old, const Access& access) override
    {
        if(access.write)
            ram[old].second = ram[old].second+1.5*lastWeight;
        else
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

using amclock_WA3_type = std::pair<PID, AccessCount>;
using amclock_WA3_upper = EvictStrategyHashVector<amclock_WA3_type>;

struct amclock_WA3: public amclock_WA3_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_WA3(): amclock_WA3_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_WA3_upper::reInit(ram_size);
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

    void updateElement(uint old, const Access& access) override
    {
        if(access.write)
            ram[old].second = ram[old].second+3*lastWeight;
        else
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


using amclock_WA25_type = std::pair<PID, AccessCount>;
using amclock_WA25_upper = EvictStrategyHashVector<amclock_WA25_type>;

struct amclock_WA25: public amclock_WA25_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_WA25(): amclock_WA25_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_WA25_upper::reInit(ram_size);
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

    void updateElement(uint old, const Access& access) override
    {
        if(access.write)
            ram[old].second = ram[old].second+2.5*lastWeight;
        else
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

using amclock_WA35_type = std::pair<PID, AccessCount>;
using amclock_WA35_upper = EvictStrategyHashVector<amclock_WA35_type>;

struct amclock_WA35: public amclock_WA35_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_WA35(): amclock_WA35_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_WA35_upper::reInit(ram_size);
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

    void updateElement(uint old, const Access& access) override
    {
        if(access.write)
            ram[old].second = ram[old].second+3.5*lastWeight;
        else
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


using amclock_WA4_type = std::pair<PID, AccessCount>;
using amclock_WA4_upper = EvictStrategyHashVector<amclock_WA4_type>;

struct amclock_WA4: public amclock_WA4_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_WA4(): amclock_WA4_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_WA4_upper::reInit(ram_size);
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

    void updateElement(uint old, const Access& access) override
    {
        if(access.write)
            ram[old].second = ram[old].second+4*lastWeight;
        else
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


using amclock_Final_type = std::pair<PID, AccessCount>;
using amclock_Final_upper = EvictStrategyHashVector<amclock_Final_type>;

/*
struct amclock_Final: public amclock_Final_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_Final(): amclock_Final_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_Final_upper::reInit(ram_size);
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
            //std::cout<<lastAction<<"#";
            //std::cout<<lastWeight<<"*";
        }
        // 命中率降低
        else
        {
            // 做上一次相反的动作
            lastAction = -(lastAction);
            //std::cout<<lastAction<<"*";
            lastWeight = lastWeight + lastAction;
            //std::cout<<lastWeight<<"-";
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

struct amclock_Final: public amclock_Final_upper {

    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    int tmp_hit;
    int tmp_miss;
    int count;
    int writeback;

    amclock_Final(): amclock_Final_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_Final_upper::reInit(ram_size);
        next = 0;
        lastWeight = 20;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;

        count = 0;
        writeback = 0;

    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);

        uint select_RAM_SIZE = 2000;

        if(EvictStrategy::RAM_SIZE==select_RAM_SIZE)
        {
            outputFile << "EACLOCK"<< std::endl;
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

        // std::cout<<"<tmp_miss:"<<tmp_miss<<">";
        // std::cout<<"<page_misses:"<<page_misses<<">";
        /*
        std::cout<<"<dirtyPages:"<<dirtyPages()<<">";

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
            lastAction = -(lastAction);
            lastWeight = lastWeight + lastAction;
        }
        lastHR = newHR;
        if(lastWeight<=2)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=24)
        {
            lastWeight = 24;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access&) override
    {
        ram[old].second = ram[old].second+lastWeight;
        if(starCount)
            HitInBuf++;

        tmp_hit++;
    }

    PID evictOne(Access) override{
        starCount = true;
        int times = 0;
        int min_accesscount=999;

        while(true) {
            times++;
            if(times >= (int)ram.size())
            {
                times=0;
                // 说明找了一圈都没有找到
                std::ofstream outputFile("EAclock.txt", std::ios_base::app);
                outputFile << "RamSize="<< EvictStrategy::RAM_SIZE<<" evict"<< std::endl;
                outputFile<<"min_accesscount="<<min_accesscount<< std::endl;
                outputFile.close();

                // 因此把之前记录的最低的值当做阈值再找一下
            }
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                min_accesscount = min_accesscount>(int)ram[next].second?(int)ram[next].second:min_accesscount;
                ram[next].second = ram[next].second-1;
                next++;
            }else{
                fast_finder.erase(ram[next].first);
                evictNum++;
                if(evictNum>=ram.size()/2)
                {
                    UpdateWeight();
                    evictNum = 0;
                    HitInBuf = 0;
                }
                tmp_miss++;
                return ram[next].first;
            }
        }
    }
};

using amclock_DW_type = std::pair<PID, AccessCount>;
using amclock_DW_upper = EvictStrategyHashVector<amclock_DW_type>;

struct amclock_DW: public amclock_DW_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    amclock_DW(): amclock_DW_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_DW_upper::reInit(ram_size);
        next = 0;
        lastWeight = 24;
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
        else if(lastWeight>=24)
        {
            lastWeight = 24;
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


using amclock_WA_type = std::pair<PID, AccessCount>;
using amclock_WA_upper = EvictStrategyHashVector<amclock_WA_type>;

struct amclock_WA: public amclock_WA_upper {
    int lastWeight;
    int lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    int tmp_hit;
    int tmp_miss;
    int count;
    int writeback;

    amclock_WA(): amclock_WA_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        amclock_WA_upper::reInit(ram_size);
        next = 0;
        lastWeight = 24;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;

        count = 0;
        writeback = 0;

    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);

        uint select_RAM_SIZE = 2000;

        if(EvictStrategy::RAM_SIZE==select_RAM_SIZE)
        {
            outputFile << "EACLOCK-FWA"<< std::endl;
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

            access(single_access);
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
            lastAction = -(lastAction);
            lastWeight = lastWeight + lastAction;
        }
        lastHR = newHR;
        if(lastWeight<=2)
        {
            lastWeight = 1;
            lastHR = newHR-0.01;
            lastAction = -1;
        }
        else if(lastWeight>=24)
        {
            lastWeight = 24;
            lastHR = newHR-0.01;
            lastAction = +1;
        }
    }

    void updateElement(uint old, const Access& access) override
    {

        if(access.write)
            ram[old].second = ram[old].second+2*lastWeight;
        else
            ram[old].second = ram[old].second+lastWeight;
        if(starCount)
            HitInBuf++;

        tmp_hit++;
    }

    PID evictOne(Access) override{
        starCount = true;
        int times = 0;
        int min_accesscount=999;

        while(true) {
            times++;
            if(times >= (int)ram.size())
            {
                times=0;
                // 说明找了一圈都没有找到
                std::ofstream outputFile("EAclock.txt", std::ios_base::app);
                outputFile << "RamSize="<< EvictStrategy::RAM_SIZE<<" evict"<< std::endl;
                outputFile<<"min_accesscount="<<min_accesscount<< std::endl;
                outputFile.close();

                // 因此把之前记录的最低的值当做阈值再找一下
            }
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                min_accesscount = min_accesscount>(int)ram[next].second?(int)ram[next].second:min_accesscount;
                ram[next].second = ram[next].second-1;
                next++;
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


