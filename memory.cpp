//#include <cstdint>
#include "memory.hpp"
#include <new>
#include <iostream> 
/*union memory {
    uint_8 *byte_arr;
    uint_16 *word_arr;
}* mem;*/

//uint8_t *memory;
void *memory;

void init_mem() {
    memory = new uint8_t[65536];

    //Scrolling Nintendo graphic
    uint8_t temp[] = {0xCE,0xEE,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E};
    for (int i = 0;i < sizeof(temp)/sizeof(uint8_t *);i++) {
        ((uint8_t *) memory)[0x0105+i] = temp[i]; 
    }

}

void write_byte(int addr,uint8_t data){
    ((uint8_t *) memory)[addr] = data;
}

void write_word(int addr,uint16_t data) {
    ((uint8_t *) memory)[addr] = data;
}

uint8_t read_byte(int addr){
    uint8_t data;
    data = ((uint8_t *) memory)[addr];
    return data;
}

uint16_t read_word(int addr) {
    uint16_t data;
    data = (uint16_t) (((uint8_t *) memory)[addr]);
    return data;
} 
