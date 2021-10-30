#include <cstdint>
#include "register.hpp"
#include "../debug/debug.hpp"
#ifdef DEBUG
void setup_debug_instr(Register8& r,int i);
void setup_debug_instr(uint16_t& a,uint16_t& b);
#endif 
void LD_8_n_nn(int& cycles,Register8& r,uint16_t addr);
void LD_8_r_r(int &cycles, Register8& r1,Register8 &r2);
void LD_8_r_r(int &cycles,uint16_t addr,Register8 &r);
void LD_8_r_r(int& cycles,uint16_t addr1, uint16_t addr2);
void LDD_8(int& cycles, Register8& r,uint16_t addr);
void LDD_8(int& cycles, Register16& r1, Register8 &r2);
void LDI_8(int& cycles, Register8& r,uint16_t addr);
void LDI_8(int& cycles, Register16& r1,Register8 &r2);
void LD_16_n_nn(int& cycles, Register16& r,uint16_t addr);
void LD_16_r_r(int& cycles, Register16 r1,Register16 r2);
void LD_16_r_r(int& cycles, Register16& r1,uint16_t r2);
void push(int& cycles,uint16_t& sp,Register16 r);
void pop(int& cycles,Register16& r,uint16_t sp);
void pop(int& cycles,Register16& r,uint16_t sp);
void add_8(int& cycles,Register8& f,Register8& r1,Register8& r2);
void add_8(int& cycles,Register8& flag,Register8& r1, Register16 r2);
void adc(int& cycles,Register8& f, Register8& r1,Register8& r2);
void adc(int& cycles,Register8& f, Register8& r1, Register16 r2);
void sub_8(int& cycles,Register8& flag ,Register8& r1,Register8& r2);
void subc(int& cycles,Register8& flag,Register8& r1,Register8& r2);
void sub_8(int& cycles,Register8& flag ,Register8& r1,Register16& r2);
void subc(int& cycles,Register8& f,Register8& r1,Register16& r2);
void and_8(int &cycles,Register8 &flag, Register8 &r1,Register8& r2);
void and_8(int &cycles,Register8& flag, Register8 &r1,Register16& r2);
void or_8(int &cycles,Register8& flag, Register8 &r1,Register8 &r2);
void or_8(int &cycles,Register8& flag, Register8 &r1,Register16 &r2);
void xor_8(int &cycles,Register8& f, Register8 &r1,Register8& r2);
void xor_8(int &cycles,Register8& f, Register8 &r1,Register16& r2);
void cp_8(int& cycles,Register8& flag ,Register8& r1,Register8& r2);
void cp_8(int& cycles, Register8& flag ,Register8& r1,Register16& r2);
void inc_8(int& cycles,Register8& f ,Register8& r1);
void inc_8(int& cycles,Register8& f, uint16_t addr);
void dec_8(int& cycles,Register8& f ,Register8& r1);
void dec_8(int& cycles,Register8& f , uint16_t addr);
void add_16(int& cycles,Register8& flag,Register16& r1,Register16& r2);
void add_16(int& cycles,Register8 f,Register16 & r1,uint16_t r2);
void add_sp(int& cycles,Register8 f,Register16& r1,uint8_t r2);
void inc_16(int& cycles,Register16& r1);
void inc_16(int& cycles,Register16 & r1);
void dec_16(int& cycles,Register16& r1);
void dec_16(int& cycles,Register16 & r1);
void ccf(int& cycles,Register8 f);
void swap(int& cycles,Register8 f ,Register8& r);
void daa(int& cycles,Register8& f,Register8& r);
void cmpbit_b_r(int& cycles,Register8 f,Register8& r1,uint8_t r2);
void cpl(int& cycles,Register8 f,Register8& r);
void scf(int& cycles, Register8& f);
void RLCA(int& cycles,Register8& flag, Register8& r);
void RLA(int& cycles,Register8& f,Register8& r);
void RRCA(int& cycles,Register8 f,Register8& r);
void RRA(int& cycles,Register8 f,Register8& r);
void SLA(int& cycles,Register8 f,Register8& r);
void SRL(int& cycles,Register8 f,Register8& r);
/*void RLA(int& cycles,Register8 f,Register8& r);*/
void set_b_r(int& cycles,Register8& r1,uint8_t r2);
void res_b_r(int& cycles,Register8& r1,uint8_t r2);
void rst(int& cycles,Register16& sp, Register16& pc,int n);
