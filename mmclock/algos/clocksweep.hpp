//
// Created by dev on 03.03.22.
//

#ifndef CLOCK_AC_HPP
#define CLOCK_AC_HPP

#endif //CLOCK_AC_HPP

#include "EvictStrategy.hpp"

using clock_AC_type = std::pair<PID, AccessCount>;
using clock_AC_upper = EvictStrategyHashVector<clock_AC_type>;

struct CLOCK_AC: public clock_AC_upper {
    CLOCK_AC(): clock_AC_upper() {}

    uint next =0;
    int count;
    int tmp_hit;

    void reInit(RamSize ram_size) override{
        clock_AC_upper::reInit(ram_size);
        next =0;
        count = 0;
        tmp_hit = 0;
    }
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+1;
        tmp_hit++;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 1};
        return next++;
    }

    std::pair<uint, uint> executeStrategy(const std::vector<Access>& access_data) override
    {
        uint page_misses = 0, dirty_evicts = 0;

        std::ofstream outputFile("hitrate.txt", std::ios_base::app);
        uint select_RAM_SIZE = 2000;

        if(EvictStrategy::RAM_SIZE==select_RAM_SIZE)
        {
            outputFile << "Clock Sweep"<< std::endl;
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

    PID evictOne(Access) override{
        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != 0){
                ram[next].second = ram[next].second-1;
                next ++;
            }else{
                fast_finder.erase(ram[next].first);
                return ram[next].first;
            }
        }
    }

};
