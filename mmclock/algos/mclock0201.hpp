#ifndef mclock0201_hpp
#define mclock0201_hpp

#endif //mclock0201

#include "EvictStrategy.hpp"

using mclock0201_type = std::pair<PID, AccessCount>;
using mclock0201_upper = EvictStrategyHashVector<mclock0201_type>;

struct mclock0201: public mclock0201_upper {
    mclock0201(): mclock0201_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0201_upper::reInit(ram_size);
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

using mclock0301_type = std::pair<PID, AccessCount>;
using mclock0301_upper = EvictStrategyHashVector<mclock0301_type>;

struct mclock0301: public mclock0301_upper {
    mclock0301(): mclock0301_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0301_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+3;
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

using mclock0401_type = std::pair<PID, AccessCount>;
using mclock0401_upper = EvictStrategyHashVector<mclock0401_type>;

struct mclock0401: public mclock0401_upper {
    mclock0401(): mclock0401_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0401_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+4;
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

using mclock0411_type = std::pair<PID, AccessCount>;
using mclock0411_upper = EvictStrategyHashVector<mclock0411_type>;

struct mclock0411: public mclock0411_upper {
    mclock0411(): mclock0411_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0411_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+4;
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

using mclock0100_type = std::pair<PID, AccessCount>;
using mclock0100_upper = EvictStrategyHashVector<mclock0100_type>;

struct mclock0100: public mclock0100_upper {
    uint evict_count = 0;
    mclock0100(): mclock0100_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0100_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+1;
    }

    PID evictOne(Access) override{
        uint count = 0;
        while(true) {
            count++;
            if(count>ram.size())
            {
                for(uint i=0;i<ram.size();i++)
                {
                    if(ram[i].second != evict_count){
                        ram[i].second = ram[i].second-1;
                    }
                }
                count=0;
            }
            if(next >= ram.size()){
                next -= ram.size();
            }
            if(ram[next].second != evict_count){
                next ++;
            }else{
                fast_finder.erase(ram[next].first);
                return ram[next].first;
            }
        }
    }
};

using mclock0501_type = std::pair<PID, AccessCount>;
using mclock0501_upper = EvictStrategyHashVector<mclock0501_type>;

struct mclock0501: public mclock0501_upper {
    mclock0501(): mclock0501_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0501_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+5;
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

using mclock0601_type = std::pair<PID, AccessCount>;
using mclock0601_upper = EvictStrategyHashVector<mclock0601_type>;

struct mclock0601: public mclock0601_upper {
    mclock0601(): mclock0601_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0601_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+6;
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

using mclock0701_type = std::pair<PID, AccessCount>;
using mclock0701_upper = EvictStrategyHashVector<mclock0701_type>;

struct mclock0701: public mclock0701_upper {
    mclock0701(): mclock0701_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0701_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+7;
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

using mclock0801_type = std::pair<PID, AccessCount>;
using mclock0801_upper = EvictStrategyHashVector<mclock0801_type>;

struct mclock0801: public mclock0801_upper {
    mclock0801(): mclock0801_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock0801_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+8;
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

using mclock01001_type = std::pair<PID, AccessCount>;
using mclock01001_upper = EvictStrategyHashVector<mclock01001_type>;

struct mclock01001: public mclock01001_upper {
    mclock01001(): mclock01001_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock01001_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+10;
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

using mclock01401_type = std::pair<PID, AccessCount>;
using mclock01401_upper = EvictStrategyHashVector<mclock01401_type>;

struct mclock01401: public mclock01401_upper {
    mclock01401(): mclock01401_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock01401_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+14;
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

using mclock01601_type = std::pair<PID, AccessCount>;
using mclock01601_upper = EvictStrategyHashVector<mclock01601_type>;

struct mclock01601: public mclock01601_upper {
    mclock01601(): mclock01601_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock01601_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+16;
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

using mclock02001_type = std::pair<PID, AccessCount>;
using mclock02001_upper = EvictStrategyHashVector<mclock02001_type>;

struct mclock02001: public mclock02001_upper {
    mclock02001(): mclock02001_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock02001_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+20;
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

using mclock02401_type = std::pair<PID, AccessCount>;
using mclock02401_upper = EvictStrategyHashVector<mclock02401_type>;

struct mclock02401: public mclock02401_upper {
    mclock02401(): mclock02401_upper() {}
    uint next =0;

    void reInit(RamSize ram_size) override{
        mclock02401_upper::reInit(ram_size);
        next =0;
    }

    uint insertElement(const Access& access) override {
        ram[next] = {access.pid, 0};
        return next++;
    }
    
    void updateElement(uint old, const Access&) override{
        ram[old].second = ram[old].second+24;
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