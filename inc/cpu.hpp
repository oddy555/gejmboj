#include <cstdint>
#include "../inc/register.hpp"

void cpu_step(int &running);
void init_registers();
void set_registers(Register8 A, Register8 F, 
                   Register8 B, Register8 C,
                   Register8 D, Register8 E,
                   Register8 H, Register8 L,
                   Register16 SP, Register16 PC);

uint8_t get_register_data8(std::string regName);
uint16_t get_register_data16(std::string regName);

void skip_boot();
void print_registers();
int eval_opcode(uint16_t opcode,int cycles);
