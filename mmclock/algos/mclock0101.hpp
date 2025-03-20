#ifndef mclock0101_hpp
#define mclock0101_hpp

#endif //mclock0101

#include "EvictStrategy.hpp"

using mclock0101_type = std::pair<PID, AccessCount>;
using mclock0101_upper = EvictStrategyHashVector<mclock0101_type>;

struct mclock0101: public mclock0101_upper {
    mclock0101(): mclock0101_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0101_upper::reInit(ram_size);
        next =0;
    }
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+1;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
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