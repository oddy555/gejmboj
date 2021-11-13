//struct memory;
#include <cstdint> 

#define IE_R 0xFFFF
#define IF_R 0xFF0F

void write_byte(int addr,uint8_t data);
void write_word(int addr,uint16_t data);

uint8_t read_byte(int addr);
uint16_t read_word(int addr);

void init_mem();

void print_memory();
void free_memory();
