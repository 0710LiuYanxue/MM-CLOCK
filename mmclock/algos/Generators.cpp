//
// Created by dev on 15.11.21.
//

#include "Generators.hpp"

template<class T> std::function<T()> defGenerator(){return [](){return T();};}
template<class T, class T2> std::function<T()> defGeneratorOne(T2 type){return [type]() {return T(type);}; }

std::function<OPT()> Opt_Generator() {return defGenerator<OPT>();}
std::function<StaticOpt()> StaticOpt_Generator(){return defGenerator<StaticOpt>();}
std::function<Random()> Random_Generator(){return defGenerator<Random>();}
std::function<SECOND_CHANCE()> SECOND_CHANCE_Generator(){return defGenerator<SECOND_CHANCE>();}
std::function<LRU_WSR()> LRU_WSR_Generator() {return defGenerator<LRU_WSR>();}
std::function<ARC()> ARC_Generator(){return defGenerator<ARC>();}

std::function<CLOCK()> CLOCK_Generator(){return defGenerator<CLOCK>();}
std::function<CLOCK_AC()> CLOCK_AC_Generator(){return defGenerator<CLOCK_AC>();}
std::function<mclock1111()> mclock1111_Generator(){return defGenerator<mclock1111>();}
std::function<mclock0101()> mclock0101_Generator(){return defGenerator<mclock0101>();}
std::function<mclock0201()> mclock0201_Generator(){return defGenerator<mclock0201>();}
std::function<mclock0211()> mclock0211_Generator(){return defGenerator<mclock0211>();}
std::function<mclock0301()> mclock0301_Generator(){return defGenerator<mclock0301>();}
std::function<mclock0401()> mclock0401_Generator(){return defGenerator<mclock0401>();}
std::function<mclock0411()> mclock0411_Generator(){return defGenerator<mclock0411>();}
std::function<mclock0100()> mclock0100_Generator(){return defGenerator<mclock0100>();}
std::function<mclock0501()> mclock0501_Generator(){return defGenerator<mclock0501>();}
std::function<mclock0601()> mclock0601_Generator(){return defGenerator<mclock0601>();}
std::function<mclock0701()> mclock0701_Generator(){return defGenerator<mclock0701>();}
std::function<mclock0801()> mclock0801_Generator(){return defGenerator<mclock0801>();}
std::function<mclock01001()> mclock01001_Generator(){return defGenerator<mclock01001>();}
std::function<mclock01401()> mclock01401_Generator(){return defGenerator<mclock01401>();}
std::function<mclock01601()> mclock01601_Generator(){return defGenerator<mclock01601>();}
std::function<mclock02001()> mclock02001_Generator(){return defGenerator<mclock02001>();}
std::function<mclock02401()> mclock02401_Generator(){return defGenerator<mclock02401>();}

std::function<amclock()> amclock_Generator(){return defGenerator<amclock>();}
std::function<amclock1()> amclock1_Generator(){return defGenerator<amclock1>();}
std::function<amclock2()> amclock2_Generator(){return defGenerator<amclock2>();}
std::function<amclock3()> amclock3_Generator(){return defGenerator<amclock3>();}
std::function<amclock_4()> amclock_4_Generator(){return defGenerator<amclock_4>();}
std::function<amclock_5()> amclock_5_Generator(){return defGenerator<amclock_5>();}
std::function<amclock_6()> amclock_6_Generator(){return defGenerator<amclock_6>();}
std::function<amclock_7()> amclock_7_Generator(){return defGenerator<amclock_7>();}
std::function<amclock_8()> amclock_8_Generator(){return defGenerator<amclock_8>();}
std::function<amclock_9()> amclock_9_Generator(){return defGenerator<amclock_9>();}
std::function<amclock_10()> amclock_10_Generator(){return defGenerator<amclock_10>();}
std::function<amclock_11()> amclock_11_Generator(){return defGenerator<amclock_11>();}
std::function<amclock_Final()> amclock_Final_Generator(){return defGenerator<amclock_Final>();}
std::function<amclock_DW()> amclock_DW_Generator(){return defGenerator<amclock_DW>();}
std::function<amclock_WA()> amclock_WA_Generator(){return defGenerator<amclock_WA>();}

std::function<amclock_WA35()> amclock_WA35_Generator(){return defGenerator<amclock_WA35>();}
std::function<amclock_WA25()> amclock_WA25_Generator(){return defGenerator<amclock_WA25>();}
std::function<amclock_WA4()> amclock_WA4_Generator(){return defGenerator<amclock_WA4>();}
std::function<amclock_WA3()> amclock_WA3_Generator(){return defGenerator<amclock_WA3>();}
std::function<amclock_WA15()> amclock_WA15_Generator(){return defGenerator<amclock_WA15>();}

// epoch
std::function<amclock_epoch()> amclock_epoch_generator(uint Epoch){return defGeneratorOne<amclock_epoch>(Epoch);}
std::function<amclock_Weight()> amclock_Weight_generator(int Weight){return defGeneratorOne<amclock_Weight>(Weight);}
std::function<amclock_action()> amclock_action_generator(int Action){return defGeneratorOne<amclock_action>(Action);}
std::function<amclock_upLimit()> amclock_upLimit_generator(int uplimit){return defGeneratorOne<amclock_upLimit>(uplimit);}
std::function<amclock_DW_uplimit()> amclock_DW_uplimit_generator(int uplimit){return defGeneratorOne<amclock_DW_uplimit>(uplimit);}
std::function<amclock_DW_HR()> amclock_DW_HR_generator(double hrpercent){return defGeneratorOne<amclock_DW_HR>(hrpercent);}

// new
std::function<TwoQ()> TwoQ_Generator() {return defGenerator<TwoQ>();}
std::function<S3fifo()> S3fifo_Generator(){return defGenerator<S3fifo>();}
std::function<LIRS()> LIRS_Generator(){return defGenerator<LIRS>();}
std::function<W_TinyLFU()> W_TinyLFU_Generator(){return defGenerator<W_TinyLFU>();}
std::function<TinyLFU_3()> TinyLFU_3_Generator(){return defGenerator<TinyLFU_3>();}
std::function<newalgo()> newalgo_Generator(){return defGenerator<newalgo>();}
std::function<mmclock()> mmclock_Generator(){return defGenerator<mmclock>();}

std::function<newalgo1()> newalgo1_Generator(int weight){return defGeneratorOne<newalgo1>(weight);}
std::function<newfinal()> newfinal_Generator(){return defGenerator<newfinal>();}
std::function<newfinal_noage()> newfinal_noage_Generator(){return defGenerator<newfinal_noage>();}
std::function<newfinal_fwa()> newfinal_fwa_Generator(){return defGenerator<newfinal_fwa>();}


std::function<CF_LRU()> CfLRUGenerator(int percentage) {return defGeneratorOne<CF_LRU>(percentage);}
std::function<hyperbolic()> Hyperbolic_generator(uint randSize){return defGeneratorOne<hyperbolic>(randSize);}

std::function<LFU_K()> LFU_K_Generator(int K){
    return [K](){return LFU_K(K);};}

std::function<LFU_K_Z()> LFU_K_Z_Generator(int K, int Z){
    return [K, Z](){return LFU_K_Z(K, Z);};}

std::function<LFU2_K_Z()> LFU2_K_Z_Generator(int K, int Z){
    return [K, Z](){return LFU2_K_Z(K, Z);};}

std::function<LFUalt_K()> LFUalt_K_Generator(int K){
    return [K](){return LFUalt_K(K);};}

std::function<LRU_K_Z()> LRU_K_Z_Generator(int K, int Z){
    return [K, Z](){return LRU_K_Z(K, Z);};}

std::function<LFU_2K_Z()> LFU_2K_Z_Generator(uint K_read, uint K_write, int out_of_ram_history_length, bool writes_as_reads){
    return [K_read, K_write, out_of_ram_history_length, writes_as_reads](){return LFU_2K_Z(K_read, K_write, out_of_ram_history_length, writes_as_reads);};}

std::function<LFU_2K_Z_rand()> LFU_2K_Z_rand_Generator(uint K_read, uint K_write, int out_of_ram_history_length, uint randSize, bool writes_as_reads){
    return [K_read, K_write, out_of_ram_history_length, randSize, writes_as_reads](){return LFU_2K_Z_rand(K_read, K_write, out_of_ram_history_length, randSize, writes_as_reads);};}

std::function<LFU_1K_E_real()>
LFU_1K_E_real_Generator(uint K, uint epoch_size, uint randSize, uint randSelector, uint write_cost, int Z) {
    return [K, randSize, randSelector, epoch_size, write_cost, Z](){
        return LFU_1K_E_real(K, randSize, randSelector, epoch_size, write_cost, Z);};}

std::function<LFU_1K_E_real_vers2()>
LFU_1K_E_Generator(uint K, uint epoch_size, uint randSize, uint randSelector, uint write_cost, int Z){
    return [K, randSize, randSelector, epoch_size, write_cost, Z](){
        return LFU_1K_E_real_vers2(K, randSize, randSelector, epoch_size, write_cost, Z);};}

std::function<LFU_2K_E_real()>
LFU_2K_E_real_Generator(uint KR, uint KW, uint epoch_size, uint randSize, uint randSelector, bool write_as_read,
                        float write_cost, float first_value, modus mod, int Z) {
    return [KR, KW, randSize, randSelector, write_as_read, epoch_size, write_cost, first_value, mod, Z](){
        return LFU_2K_E_real(KR, KW, randSize, randSelector, write_as_read, epoch_size, write_cost, first_value, mod, Z);};}

std::function<LFU_2K_E_real_ver2()>
LFU_2K_E_real2_Generator(uint KR, uint KW, uint epoch_size, uint randSize, uint randSelector, bool write_as_read, uint writeCost) {
    return [KR, KW, randSize, randSelector, write_as_read, epoch_size, writeCost](){
        return LFU_2K_E_real_ver2(KR, KW, randSize, randSelector, write_as_read, epoch_size, writeCost);};}

std::function<leanEvict()> Lean_Generator(uint cooling_percentage){
    return [cooling_percentage](){return leanEvict(cooling_percentage);};}
std::function<leanEvict2()> Lean_Generator2(uint cooling_percentage){
    return [cooling_percentage](){return leanEvict2(cooling_percentage);};}

std::function<LRFU()> LRFU_Generator(double lambda, uint K) {
    return [lambda, K]() {
        return LRFU(lambda, K);};}

/*
std::function<LFU_K_Z_D()> LFU1_K_Z_D_Generator(int K, int Z, int D){
    return defGeneratorThree<LFU_K_Z_D>(K, Z, D);
}

std::function<LFU2_K_Z_D()> LFU2_K_Z_D_Generator(int K, int Z, int D){
    return defGeneratorThree<LFU2_K_Z_D>(K, Z, D);
}
*/
std::function<LRU()> LRU_Generator() {return defGenerator<LRU>();}
std::function<LRU1()> LRU1_Generator() {return defGenerator<LRU1>();}
std::function<LRU2()> LRU2_Generator() {return defGenerator<LRU2>();}
std::function<LRU2a()> LRU2a_Generator() {return defGenerator<LRU2a>();}
std::function<LRU2b()> LRU2b_Generator() {return defGenerator<LRU2b>();}
std::function<LRU3()> LRU3_Generator() {return defGenerator<LRU3>();}

//slooow
std::function<OPT2()> Opt2_Generator() {return defGenerator<OPT2>();}

// Old and unused
// Has no out_of_memory_history
std::function<LRUalt_K()> LRUalt_K_Generator(int K){
    return [K](){return LRUalt_K(K);};
}

// Broken ones
std::function<OPT3()> Opt3_Generator() {return defGenerator<OPT3>();}