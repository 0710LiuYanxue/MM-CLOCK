#ifndef mclock0211_hpp
#define mclock0211_hpp

#endif //mclock0211

#include "EvictStrategy.hpp"

using mclock0211_type = std::pair<PID, AccessCount>;
using mclock0211_upper = EvictStrategyHashVector<mclock0211_type>;

struct mclock0211: public mclock0211_upper {
    mclock0211(): mclock0211_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0211_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+2;
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