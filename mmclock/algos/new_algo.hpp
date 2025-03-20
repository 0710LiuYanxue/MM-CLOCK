#ifndef newalgo_hpp
#define newalgo_hpp

#endif // newalgo_hpp

#include "EvictStrategy.hpp"
#include <functional>
#include <iostream>
#include <fstream>

using newalgo_type = std::pair<PID, AccessCount>;
using newalgo_upper = EvictStrategyHashVector<newalgo_type>;


struct newalgo: public newalgo_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    int min_accesscount;

    int tmp_hit;
    int tmp_miss;
    int count;

    newalgo(): newalgo_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        newalgo_upper::reInit(ram_size);
        next = 0;
        lastWeight = 1;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;

        min_accesscount = 10;

        count = 0;
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("newalgo.txt", std::ios_base::app);

        if(EvictStrategy::RAM_SIZE==900)
        {
            outputFile << "newalgo"<< std::endl;
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

            if(count%EvictStrategy::RAM_SIZE==0 && count>=470000 && count<=530000)
            {
                if(EvictStrategy::RAM_SIZE==900)
                {
                    if(tmp_hit< (int)EvictStrategy::RAM_SIZE)
                    {
                        outputFile<<tmp_hit<< std::endl;
                        outputFile<<"min_accesscount = "<<min_accesscount<< std::endl;
                    }
                    tmp_hit =0;
                }
            }
            
            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }

        outputFile.close();

        return std::pair(page_misses, dirty_evicts + dirtyPages());
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }

    void UpdateWeight()
    {
        //double newHR = 1.0*HitInBuf/(HitInBuf+evictNum);
        //isChange = false;
    }

    void updateElement(uint old, const Access&) override
    {
        ram[old].second = ram[old].second+lastWeight;
        if(starCount)
            HitInBuf++;

        tmp_hit++;
    }

    PID evictOne(Access) override
    {

        starCount = true;
        int times = 0;

        // int yuzhi = min_accesscount>10?0:min_accesscount;
        int yuzhi = 0;

        while(true)
        {
            times++;
            if(times >= (int)ram.size())
            {
                times=0;
                // 说明找了一圈都没有找到
                std::ofstream outputFile("newalgo.txt", std::ios_base::app);
                outputFile << "RamSize="<< EvictStrategy::RAM_SIZE<<" evict"<< std::endl;
                outputFile<<"min_accesscount = "<<min_accesscount<< std::endl;
                outputFile.close();

                // 因此把之前记录的最低的值当做阈值再找一下
                yuzhi = min_accesscount;
            }

            if(next >= ram.size()){
                next -= ram.size();
            }
            
            if( (int)ram[next].second > yuzhi)
            {
                if(yuzhi>0)
                {
                    ram[next].second = ram[next].second-1;
                }

                min_accesscount = min_accesscount>(int)ram[next].second ? (int)ram[next].second : min_accesscount;
                next++;
            }
            else
            {
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


struct newalgo1: public newalgo_upper {

    int lastWeight;
    bool lastAction;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    int tmp_hit;
    int tmp_miss;
    int count;

    int agetimes;

    newalgo1(int  weight): newalgo_upper() ,lastWeight(weight){}
    uint next =0;

    void reInit(RamSize ram_size) override{
        newalgo_upper::reInit(ram_size);
        next = 0;
        // lastWeight = 24;
        lastAction = +1;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;

        count = 0;
        agetimes = 0;
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);

        if(EvictStrategy::RAM_SIZE==4000)
        {
            outputFile << "newalgo1:"<<lastWeight<< std::endl;
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

            // if(count%EvictStrategy::RAM_SIZE==0 && count>=470000 && count<=530000)
            if(count%EvictStrategy::RAM_SIZE==0 && count>=0 && count<=200000)
            {
                if(EvictStrategy::RAM_SIZE==4000)
                {
                    if(tmp_hit< (int)EvictStrategy::RAM_SIZE)
                    {
                        outputFile<<tmp_hit<< std::endl;
                    }
                    tmp_hit =0;
                }
            }
            
            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }

        outputFile.close();

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
            //lastWeight = lastWeight + lastAction;
        }
        else
        {
            if(lastHR-newHR>0.3*(1-lastHR))
            {
                isChange = true;
                uint i = 0;
                while(i < ram.size()) {
                    if(ram[i].second != 0){
                        ram[i].second = ram[i].second/2;
                    }
                    i++;
                }
                agetimes++;
                if(EvictStrategy::RAM_SIZE == 900 || EvictStrategy::RAM_SIZE == 4000)
                {
                    if(lastWeight==2 || lastWeight==24)
                    {
                    std::ofstream outputFile("aging.csv", std::ios_base::app);
                    outputFile<<EvictStrategy::RAM_SIZE<<":"<<lastWeight<<":"<<agetimes<< std::endl;
                    outputFile.close();
                    }
                }
            }
            else
            {
                //lastAction = -(lastAction);
                //lastWeight = lastWeight + lastAction;
            }
        }
        lastHR = newHR;
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
                if(isChange)
                    ram[next].second = ram[next].second/2;
                else
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


struct newfinal: public newalgo_upper {

    int Weight;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    int tmp_hit;
    int tmp_miss;
    int count;

    int agetimes;

    newfinal(): newalgo_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        newalgo_upper::reInit(ram_size);
        next = 0;
        Weight = 24;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;

        count = 0;
        agetimes = 0;
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);

        if(EvictStrategy::RAM_SIZE==1000)
        {
            outputFile << "newalgofinal:"<<Weight<< std::endl;
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

            if(count%EvictStrategy::RAM_SIZE==0 && count>=470000 && count<=550000)
            // if(count%EvictStrategy::RAM_SIZE==0 && count>=0 && count<=200000)
            {
                if(EvictStrategy::RAM_SIZE==1000)
                {
                    if(tmp_hit< (int)EvictStrategy::RAM_SIZE)
                    {
                        outputFile<<tmp_hit<< std::endl;
                    }
                    tmp_hit =0;
                }
            }
            
            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }

        outputFile.close();

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
        if(newHR<0.9)
        {
            Weight = 24;
        }
        else
        {
            Weight = 2;
        }

        if(lastHR-newHR>0.3*(1-lastHR))
        {
            isChange = true;
            uint i = 0;
            while(i < ram.size()) {
                if(ram[i].second != 0){
                    ram[i].second = ram[i].second/2;
                }
                i++;
            }
            agetimes++;
            if(EvictStrategy::RAM_SIZE == 900 || EvictStrategy::RAM_SIZE == 4000)
            {
                if(Weight==2 || Weight==24)
                {
                    std::ofstream outputFile("aging.csv", std::ios_base::app);
                    outputFile<<EvictStrategy::RAM_SIZE<<":"<<Weight<<":"<<agetimes<< std::endl;
                    outputFile.close();
                }
            }
        }

        lastHR = newHR;

    }

    void updateElement(uint old, const Access&) override
    {
        ram[old].second = ram[old].second+Weight;
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
                if(isChange)
                    ram[next].second = ram[next].second/2;
                else
                    ram[next].second = ram[next].second-1;
                next++;
            }else{
                fast_finder.erase(ram[next].first);
                evictNum++;
                if((evictNum)>=ram.size()/2)
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

struct newfinal_noage: public newalgo_upper {

    int Weight;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    int tmp_hit;
    int tmp_miss;
    int count;

    int agetimes;

    newfinal_noage(): newalgo_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        newalgo_upper::reInit(ram_size);
        next = 0;
        Weight = 24;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;

        count = 0;
        agetimes = 0;
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);

        if(EvictStrategy::RAM_SIZE==1000)
        {
            outputFile << "newfinal_noage:"<<Weight<< std::endl;
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

            if(count%EvictStrategy::RAM_SIZE==0 && count>=470000 && count<=550000)
            // if(count%EvictStrategy::RAM_SIZE==0 && count>=0 && count<=200000)
            {
                if(EvictStrategy::RAM_SIZE==1000)
                {
                    if(tmp_hit< (int)EvictStrategy::RAM_SIZE)
                    {
                        outputFile<<tmp_hit<< std::endl;
                    }
                    tmp_hit =0;
                }
            }
            
            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }

        outputFile.close();

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
        if(newHR<0.9)
        {
            Weight = 24;
        }
        else
        {
            Weight = 2;
        }

        lastHR = newHR;

    }

    void updateElement(uint old, const Access&) override
    {
        ram[old].second = ram[old].second+Weight;
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
                if((evictNum)>=ram.size()/2)
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


struct newfinal_fwa: public newalgo_upper {

    int Weight;
    double lastHR;
    uint evictNum;
    uint HitInBuf;
    bool starCount;
    bool isChange;

    int tmp_hit;
    int tmp_miss;
    int count;

    int agetimes;

    newfinal_fwa(): newalgo_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        newalgo_upper::reInit(ram_size);
        next = 0;
        Weight = 24;
        evictNum = 0;
        HitInBuf = 0;
        lastHR = 0;
        starCount = false;
        isChange = false;

        count = 0;
        agetimes = 0;
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);

        if(EvictStrategy::RAM_SIZE==1000)
        {
            outputFile << "newfinal_fwa:"<<Weight<< std::endl;
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

            if(count%EvictStrategy::RAM_SIZE==0 && count>=470000 && count<=550000)
            // if(count%EvictStrategy::RAM_SIZE==0 && count>=0 && count<=200000)
            {
                if(EvictStrategy::RAM_SIZE==1000)
                {
                    if(tmp_hit< (int)EvictStrategy::RAM_SIZE)
                    {
                        outputFile<<tmp_hit<< std::endl;
                    }
                    tmp_hit =0;
                }
            }
            
            dirty_in_ram[single_access.pid] = dirty_in_ram[single_access.pid] || single_access.write;
            in_ram[single_access.pid] = true;
        }

        outputFile.close();

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
        if(newHR<0.9)
        {
            Weight = 24;
        }
        else
        {
            Weight = 2;
        }

        if(lastHR-newHR>0.3*(1-lastHR))
        {
            isChange = true;
            uint i = 0;
            while(i < ram.size()) {
                if(ram[i].second != 0){
                    ram[i].second = ram[i].second/2;
                }
                i++;
            }
            agetimes++;
            if(EvictStrategy::RAM_SIZE == 900 || EvictStrategy::RAM_SIZE == 4000)
            {
                if(Weight==2 || Weight==24)
                {
                    std::ofstream outputFile("aging.csv", std::ios_base::app);
                    outputFile<<EvictStrategy::RAM_SIZE<<":"<<Weight<<":"<<agetimes<< std::endl;
                    outputFile.close();
                }
            }
        }

        lastHR = newHR;

    }

    void updateElement(uint old, const Access& access) override
    {
        if(access.write)
            ram[old].second = ram[old].second+2*Weight;
        else
            ram[old].second = ram[old].second+Weight;
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
                if(isChange)
                    ram[next].second = ram[next].second/2;
                else
                    ram[next].second = ram[next].second-1;
                next++;
            }else{
                fast_finder.erase(ram[next].first);
                evictNum++;
                if((evictNum)>=ram.size()/2)
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