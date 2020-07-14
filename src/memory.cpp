//#include <cstdint>
#include "../inc/memory.hpp"
#include <new>
#include <iostream> 
#include <fstream>
#include <cstring>
#include <stdio.h>
/*union memory {
    uint_8 *byte_arr;
    uint_16 *word_arr;
}* mem;*/

//uint8_t *memory;
void *memory;

void *rom;

void init_rom();

void set_bootRom() {
  /*memory[0] = 0x31;
  memory[1] =  0xff;
  memory[2] = 0xfe;
  memory[3] = 0xaf;
  memory[4] = 0x21;
  memory[5] = 0x9f;
  memory[6] = 0xff;
  memory[7] = 0x32;
  memory[8] = 0xcb;
  memory[9] = 0x7e;
  memory[0xA] = 0x20;*/
  std::ifstream bootRom;
  bootRom.open("DMG_ROM.bin",std::ios::in | std::ios::binary);
  if (bootRom.is_open()) {
      bootRom.read((char *) memory,256);
  } else {
    std::cout << "Unable to open file" << std::endl;
  }
  
  bootRom.close();
}
  
void init_mem() {
    memory = new uint8_t[65536];
    memset(memory,0,65536);
    init_rom();
    //Scrolling Nintendo graphic
    /*    uint8_t temp[] = {0xCE,0xEE,0x66,0x66,0xCC,0x0D,0x00,0x0B,0x03,0x73,0x00,0x83,0x00,0x0C,0x00,0x0D,0x00,0x08,0x11,0x1F,0x88,0x89,0x00,0x0E,0xDC,0xCC,0x6E,0xE6,0xDD,0xDD,0xD9,0x99,0xBB,0xBB,0x67,0x63,0x6E,0x0E,0xEC,0xCC,0xDD,0xDC,0x99,0x9F,0xBB,0xB9,0x33,0x3E};
    for (int i = 0;i < sizeof(temp)/sizeof(uint8_t *);i++) {
        ((uint8_t *) memory)[0x0105+i] = temp[i]; 
	}*/    
}



void init_rom() {
    int romSize = 8192;
    //read rom here?
    std::ifstream rom;	
    if (rom.is_open()) {
        //@TODO: Read 32kB, this might have to be more flexible
        rom.read((char *) memory + 146,romSize);
    }
    //rom = new uint8_t[romSize]; 
}

void write_byte(int addr,uint8_t data){
    ((uint8_t *) memory)[addr] = data;
}

//TODO: The shift here makes the code platform dependent
void write_word(int addr,uint16_t data) {
  //((uint8_t *) memory)[addr] = data;
  ((uint8_t *) memory)[addr] = data;
  ((uint8_t *) memory)[addr+1] = (data >> 8);
}

uint8_t read_byte(int addr){
    uint8_t data;
    data = ((uint8_t *) memory)[addr];
    return data;
}
//TODO: only reads one word
uint16_t read_word(int addr) {
    uint16_t data;
    uint8_t a;

    a = ((uint8_t *) memory)[addr];
    data = (((uint8_t *) memory)[addr+1] << 8);
    data = data + a;
    //data = (uint16_t) (((uint8_t *) memory)[addr]);
    return data;
} 
