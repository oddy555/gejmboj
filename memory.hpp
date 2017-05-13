//struct memory;
#include <cstdint> 
void write_byte(int addr,uint8_t data);
void write_word(int addr,uint16_t data);

uint8_t read_byte(int addr);
uint16_t read_word(int addr);
