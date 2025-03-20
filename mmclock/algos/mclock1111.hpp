
#ifndef mmclock_HPP
#define mmclock_HPP

#endif //mmclock_HPP

#include "EvictStrategy.hpp"

using mmclock_type = std::pair<PID, AccessCount>;
using mmclock_upper = EvictStrategyHashVector<mmclock_type>;

struct mclock1111: public mmclock_upper {
    mclock1111(): mmclock_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mmclock_upper::reInit(ram_size);
        next =0;
    }
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+1;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 1};
        return next++;
    }

    PID evictOne(Access) override{
        while(true) {
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second > 1){
                ram[next].second = ram[next].second-1;
                next ++;
            }else{
                fast_finder.erase(ram[next].first);
                return ram[next].first;
            }
        }
    }

};