#include <cstdint>
void init_registers();
void cpu_step();
void skip_boot();
void print_registers();
int eval_opcode(uint16_t opcode,int cycles);
