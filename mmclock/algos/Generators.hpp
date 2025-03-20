//
// Created by dev on 15.11.21.
//
#pragma once
#include <functional>
// Algos
#include "EvictStrategy.hpp"
#include "random.hpp"
#include "lru.hpp"
#include "opt.hpp"
#include "cf_lru.hpp"
#include "lru_wsr.hpp"
#include "lru_k.hpp"
#include "lfu_k.hpp"
#include "hyperbolic.hpp"
#include "staticOpt.hpp"
#include "lean_evict.hpp"
#include "CLOCK.hpp"
#include "LRFU.hpp"
#include "ARC.hpp"
#include "clocksweep.hpp"
#include "mclock1111.hpp"
#include "mmclock.hpp"



// Here unused, but for good includes
#include "lruStackDist.hpp"
#include "mclock0101.hpp"
#include "mclock0201.hpp"
#include "mclock0211.hpp"
#include "Amclock.hpp"
#include "change_Parameters.hpp"
#include "2Q.hpp"
#include "s3fifo.hpp"
#include "LIRS.hpp"
#include "TinyLFU.hpp"
#include "new_algo.hpp"

#ifndef C_GENERATORS_H
#define C_GENERATORS_H

#endif //C_GENERATORS_H


// Defaults
std::function<OPT()> Opt_Generator();
std::function<StaticOpt()> StaticOpt_Generator();
std::function<Random()> Random_Generator();
std::function<CLOCK()> CLOCK_Generator();
std::function<SECOND_CHANCE()> SECOND_CHANCE_Generator();

std::function<CLOCK_AC()> CLOCK_AC_Generator();
std::function<mclock1111()> mclock1111_Generator();
std::function<mclock0101()> mclock0101_Generator();
std::function<mclock0201()> mclock0201_Generator();
std::function<mclock0211()> mclock0211_Generator();
std::function<mclock0301()> mclock0301_Generator();
std::function<mclock0401()> mclock0401_Generator();
std::function<mclock0411()> mclock0411_Generator();
std::function<mclock0100()> mclock0100_Generator();
std::function<mclock0501()> mclock0501_Generator();
std::function<mclock0601()> mclock0601_Generator();
std::function<mclock0701()> mclock0701_Generator();
std::function<mclock0801()> mclock0801_Generator();
std::function<mclock02401()> mclock02401_Generator();
std::function<mclock02001()> mclock02001_Generator();
std::function<mclock01601()> mclock01601_Generator();
std::function<mclock01401()> mclock01401_Generator();
std::function<mclock01001()> mclock01001_Generator();

std::function<amclock()> amclock_Generator();
std::function<amclock1()> amclock1_Generator();
std::function<amclock2()> amclock2_Generator();
std::function<amclock3()> amclock3_Generator();
std::function<amclock_4()> amclock_4_Generator();
std::function<amclock_5()> amclock_5_Generator();
std::function<amclock_6()> amclock_6_Generator();
std::function<amclock_7()> amclock_7_Generator();
std::function<amclock_8()> amclock_8_Generator();
std::function<amclock_9()> amclock_9_Generator();
std::function<amclock_10()> amclock_10_Generator();
std::function<amclock_11()> amclock_11_Generator();
std::function<amclock_Final()> amclock_Final_Generator();
std::function<amclock_DW()> amclock_DW_Generator();
std::function<amclock_WA()> amclock_WA_Generator();

std::function<amclock_WA35()> amclock_WA35_Generator();
std::function<amclock_WA25()> amclock_WA25_Generator();
std::function<amclock_WA4()> amclock_WA4_Generator();
std::function<amclock_WA3()> amclock_WA3_Generator();
std::function<amclock_WA15()> amclock_WA15_Generator();

// epoch
std::function<amclock_epoch()> amclock_epoch_generator(uint Epoch);
std::function<amclock_Weight()> amclock_Weight_generator(int Weight);
std::function<amclock_action()> amclock_action_generator(int Action);
std::function<amclock_upLimit()> amclock_upLimit_generator(int uplimit);
std::function<amclock_DW_uplimit()> amclock_DW_uplimit_generator(int uplimit);
std::function<amclock_DW_HR()> amclock_DW_HR_generator(double hrpercent);

// new
std::function<TwoQ()> TwoQ_Generator();
std::function<S3fifo()> S3fifo_Generator();
std::function<LIRS()> LIRS_Generator();
std::function<W_TinyLFU()> W_TinyLFU_Generator();
std::function<TinyLFU_3()> TinyLFU_3_Generator();
std::function<newalgo()> newalgo_Generator();
std::function<mmclock()> mmclock_Generator();

std::function<newalgo1()> newalgo1_Generator(int weight);
std::function<newfinal()> newfinal_Generator();
std::function<newfinal_noage()> newfinal_noage_Generator();
std::function<newfinal_fwa()> newfinal_fwa_Generator();

// Others
std::function<CF_LRU()> CfLRUGenerator(int percentage);
std::function<LRU_WSR()> LRU_WSR_Generator();
std::function<hyperbolic()> Hyperbolic_generator(uint randSize);
std::function<LFU_K()> LFU_K_Generator(int K);
std::function<LFU_K_Z()> LFU_K_Z_Generator(int K, int out_of_ram_history_length);
std::function<LFU2_K_Z()> LFU2_K_Z_Generator(int K, int out_of_ram_history_length);
std::function<LFUalt_K()> LFUalt_K_Generator(int K);
std::function<LRU_K_Z()> LRU_K_Z_Generator(int K, int out_of_ram_history_length);
std::function<LFU_2K_Z()> LFU_2K_Z_Generator(uint K_read, uint K_write, int out_of_ram_history_length, bool writes_as_reads);
std::function<LFU_2K_Z_rand()> LFU_2K_Z_rand_Generator(uint K_read, uint K_write, int out_of_ram_history_length, uint randSize, bool writes_as_reads);
std::function<LFU_1K_E_real()> LFU_1K_E_real_Generator(uint K, uint epoch_size, uint randSize, uint randSelector, uint write_cost, int Z);
std::function<LFU_1K_E_real_vers2()> LFU_1K_E_Generator(uint K, uint epoch_size, uint randSize, uint randSelector, uint write_cost, int Z);
std::function<LFU_2K_E_real_ver2()>
LFU_2K_E_real2_Generator(uint KR, uint KW, uint epoch_size, uint randSize, uint randSelector, bool write_as_read,
                         uint writeCost);
std::function<LFU_2K_E_real()>
LFU_2K_E_real_Generator(uint KR, uint KW, uint epoch_size, uint randSize, uint randSelector, bool write_as_read,
                        float write_cost=1, float first_value=1.0, modus mod=mod_max, int Z=-1);
inline std::function<LFU_2K_E_real()>
LFU_Generator(uint KR, uint KW, uint epoch_size, uint randSize, uint randSelector, bool write_as_read, float writeCost=1, float first_value=1.0, modus mod=mod_max, int Z=-1){
    return LFU_2K_E_real_Generator(KR, KW, epoch_size, randSize, randSelector, write_as_read, writeCost, first_value,
                                   mod, Z);
}

std::function<leanEvict()> Lean_Generator(uint cooling_percentage);
std::function<leanEvict2()> Lean_Generator2(uint cooling_percentage);

std::function<LRFU()>
LRFU_Generator(double lambda, uint K);

std::function<ARC()> ARC_Generator();

//std::function<LFU_K_Z_D()> LFU1_K_Z_D_Generator(int K, int Z, int D);
// std::function<LFU2_K_Z_D()> LFU2_K_Z_D_Generator(int K, int Z, int D);

// Redundant ones (differ in implementation)
std::function<LRU()> LRU_Generator();
std::function<LRU1()> LRU1_Generator();
std::function<LRU2()> LRU2_Generator();
std::function<LRU2a()> LRU2a_Generator();
std::function<LRU2b()> LRU2b_Generator();
std::function<LRU3()> LRU3_Generator();

std::function<OPT2()> Opt2_Generator();

// Old and unused
// Has no out_of_memory_history
std::function<LRUalt_K()> LRUalt_K_Generator(int K);

// Broken ones
std::function<OPT3()> Opt3_Generator();