#include <cstdint>
void init_registers();
void cpu_step();
int eval_opcode(uint16_t opcode,int cycles);
