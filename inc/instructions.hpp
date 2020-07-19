#include <cstdint>
#include "../debug/debug.hpp"
#ifdef DEBUG
void setup_debug_instr(uint8_t& r,int i);
void setup_debug_instr(uint16_t& a,uint16_t& b);
#endif 
void LD_8_n_nn(int& cycles,uint8_t& r,uint16_t addr);
void LD_8_r_r(int &cycles, uint8_t& r1,uint8_t r2);
void LD_8_r_r(int &cycles,uint16_t addr,uint8_t r);
void LD_8_r_r(int& cycles,uint16_t addr1, uint16_t addr2);
void LDD_8(int& cycles, uint8_t& r,uint16_t& addr);
void LDD_8(int& cycles, uint16_t& addr, uint8_t r);
void LDI_8(int& cycles, uint8_t& r,uint16_t& addr);
void LDI_8(int& cycles, uint16_t& addr,uint8_t r);
void LD_16_n_nn(int& cycles, uint16_t& r,uint16_t addr);
void LD_16_r_r(int& cycles, uint16_t& r1,uint16_t r2);
void push(int& cycles,uint16_t& sp,uint16_t r);
void pop(int& cycles,uint16_t& r,uint16_t sp);

void add_8(int& cycles,uint8_t& f,uint8_t& r1,uint8_t r2);
void adc(int& cycles,uint8_t& f, uint8_t& r1,uint8_t r2);
void sub_8(int& cycles,uint8_t& f ,uint8_t& r1,uint8_t r2);
void subc(int& cycles,uint8_t& f,uint8_t& r1,uint8_t r2);
void and_8(int &cycles,uint8_t f, uint8_t &r1,uint8_t r2);
void or_8(int &cycles,uint8_t f, uint8_t &r1,uint8_t r2);
void xor_8(int &cycles,uint8_t f, uint8_t &r1,uint8_t r2);
void cp_8(int& cycles,uint8_t& f ,uint8_t& r1,uint8_t r2);
void inc_8(int& cycles,uint8_t& f ,uint8_t& r1);
void dec_8(int& cycles,uint8_t& f ,uint8_t& r1);
void add_16(int& cycles,uint8_t& f,uint16_t& r1,uint16_t r2);
void add_sp(int& cycles,uint8_t& f,uint16_t& r1,uint8_t r2);
void inc_16(int& cycles,uint16_t& r1);
void dec_16(int& cycles,uint16_t& r1);
void ccf(int& cycles,uint8_t f);
void swap(int& cycles,uint8_t& f ,uint8_t& r);
void daa(int& cycles,uint8_t& f,uint8_t& r);
void cmpbit_b_r(int& cycles,uint8_t& f,uint8_t r1,uint8_t r2);
void cpl(int& cycles,uint8_t& f,uint8_t& r);
void scf(int& cycles, uint8_t& f);
void RLCA(int& cycles,uint8_t& f,uint8_t& r);
void RLA(int& cycles,uint8_t& f,uint8_t& r);
void RRCA(int& cycles,uint8_t& f,uint8_t& r);
void RRA(int& cycles,uint8_t& f,uint8_t& r);
void SLA(int& cycles,uint8_t& f,uint8_t& r);
void SRL(int& cycles,uint8_t& f,uint8_t& r);
/*void RLA(int& cycles,uint8_t& f,uint8_t& r);*/
void set_b_r(int& cycles,uint8_t r1,uint8_t r2);
void res_b_r(int& cycles,uint8_t r1,uint8_t r2);
void rst(int& cycles,uint16_t& sp, uint16_t& pc);
