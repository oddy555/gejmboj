#include <cstdint> 
#include <unistd.h>
#include "../inc/memory.hpp"
#include "../inc/instructions.hpp"
#include "../inc/video.hpp"

static uint8_t ime = 0;

struct registers {
    union {
      uint8_t A;
      uint8_t F;
      uint16_t AF;
    };
    union {
      uint8_t B;
      uint8_t C;
      uint16_t BC;
    };
    union {
      uint8_t D;
      uint8_t E;
      uint16_t DE;
    };
    union {
      uint8_t H;
      uint8_t L;
      uint16_t HL;
    };
    uint16_t sp;
    uint16_t pc;
  } reg;
int z = 0x80;
int n = 0x40;
int h = 0x20;
int c = 0x10;


void eval_opcode(uint16_t opcode,int cycles);

int main() {
  
  init_mem();
  init_video();
  //int a = 0x01;
  //int b = 0x02;
  //int c2 = 0x04;
  //int d = 0x08;    
  int cycles = 0;
  uint16_t opcode;
  uint8_t temp;

  reg.sp = 0;
  reg.pc = 0;

  int running = 0;    
  while(!running) {
    opcode = read_byte(reg.pc++);
    eval_opcode(opcode,cycles);

    uint8_t temp2 = read_byte(IE_R) & read_byte(IF_R); 
    if (ime == 1 && temp2) {
        int cycleTemp = 0;
        LD_16_n_nn(cycleTemp,reg.sp,reg.pc++);
        cycles += 8 + temp;
        
        //TODO: Check if we should enable several interrupts here
        
        // V-Blank
        if (temp2 & 0b00000001) {
            LD_16_n_nn(cycleTemp,reg.pc,0x0040);
            cycles += temp;
            continue;
        // LCD STAT
        } else if (temp2 & 0b00000010) {
            LD_16_n_nn(cycleTemp,reg.pc,0x0048);
            cycles += temp;
            continue;
        // Timer
        } else if (temp2 & 0b00000100) {
            LD_16_n_nn(cycleTemp,reg.pc,0x0050);
            cycles += temp;
            continue;
        // Serial
        } else if (temp2 & 0b00001000) {
            LD_16_n_nn(cycleTemp,reg.pc,0x0058);
            cycles += temp;
            continue;
        // Joypad    
        }  else if (temp2 & 0b00010000) {
            LD_16_n_nn(cycleTemp,reg.pc, 0x0060);
            cycles += temp;
            continue;
        } 
    }
    
    //lcdController(cycles);
    if (eventController() == 1) {
        running = 1; 
    }
    usleep((cycles/4190000)*100000);
  }    
}

void eval_opcode(uint16_t opcode,int cycles) {
    
    uint8_t temp;

    switch(opcode) {
    case 0x06:
      LD_8_n_nn(cycles,reg.B,reg.pc++);
      break;
    case 0x0E:
      LD_8_n_nn(cycles,reg.C,reg.pc++);
      break;
    case 0x16:
      LD_8_n_nn(cycles,reg.D,reg.pc++);
      break;
    case 0x1E:
      LD_8_n_nn(cycles,reg.E,reg.pc++);
      break;
    case 0x26:
      LD_8_n_nn(cycles,reg.E,reg.pc++);
      break;
    case 0x2E:
      LD_8_n_nn(cycles,reg.L,reg.pc++);
      break;
    case 0x7F:
      LD_8_r_r(cycles,reg.A,reg.A);
      break;
    case 0x78:
      LD_8_r_r(cycles,reg.A,reg.B);
      break;
    case 0x79:
      LD_8_r_r(cycles,reg.A,reg.C);
      break;
    case 0x7A:
      LD_8_r_r(cycles,reg.A,reg.D);
      break;
    case 0x7B:
      LD_8_r_r(cycles,reg.A,reg.E);
      break;
    case 0x7C:
      LD_8_r_r(cycles,reg.A,reg.H);
      break;
    case 0x7D:
      LD_8_r_r(cycles,reg.A,reg.L);
      break;
    case 0x7E:
      LD_8_n_nn(cycles,reg.A,reg.HL);
      break;
    case 0x40:
      LD_8_r_r(cycles,reg.B,reg.B);
      break;
    case 0x41:
      LD_8_r_r(cycles,reg.B,reg.C);
      break;
    case 0x42:
      LD_8_r_r(cycles,reg.B,reg.D);
      break;
    case 0x43:
      LD_8_r_r(cycles,reg.B,reg.E);
      break;
    case 0x44:
      LD_8_r_r(cycles,reg.B,reg.H);
      break;
    case 0x45:
      LD_8_r_r(cycles,reg.B,reg.L);
      break;
    case 0x46:
      //temp = read_byte(reg.HL);
      //LD_8_n_nn(cycles,reg.B,temp);
      LD_8_n_nn(cycles,reg.B,reg.HL);
      break;
    case 0x48:
      LD_8_r_r(cycles,reg.C,reg.B);
      break;
    case 0x49:
      LD_8_r_r(cycles,reg.C,reg.C);
      break;    
    case 0x4A:
      LD_8_r_r(cycles,reg.C,reg.D);
      break;
    case 0x4B:
      LD_8_r_r(cycles,reg.C,reg.E);
      break;
    case 0x4C:
      LD_8_r_r(cycles,reg.C,reg.H);
      break;
    case 0x4D:
      LD_8_r_r(cycles,reg.C,reg.L);
      break;
    case 0x4E:
      LD_8_n_nn(cycles,reg.A,reg.HL);
      break;
    case 0x50:
      LD_8_r_r(cycles,reg.D,reg.B);
      break;
    case 0x51:
      LD_8_r_r(cycles,reg.D,reg.C);
      break;    
    case 0x52:
      LD_8_r_r(cycles,reg.D,reg.D);
      break;
    case 0x53:
      LD_8_r_r(cycles,reg.D,reg.E);
      break;
    case 0x54:
      LD_8_r_r(cycles,reg.D,reg.H);
      break;
    case 0x55:
      LD_8_r_r(cycles,reg.D,reg.L);
      break;
    case 0x56:
      LD_8_n_nn(cycles,reg.D,reg.HL);
      break;
    case 0x58:
      LD_8_r_r(cycles,reg.E,reg.B);
      break;
    case 0x59:
      LD_8_r_r(cycles,reg.E,reg.C);
      break;    
    case 0x5A:
      LD_8_r_r(cycles,reg.E,reg.D);
      break;
    case 0x5B:
      LD_8_r_r(cycles,reg.E,reg.E);
      break;
    case 0x5C:
      LD_8_r_r(cycles,reg.E,reg.H);
      break;
    case 0x5D:
      LD_8_n_nn(cycles,reg.E,reg.L);
      break;
    case 0x5E:
      LD_8_n_nn(cycles,reg.E,reg.HL);
      break;
    case 0x60:
      cycles = 4;
      LD_8_r_r(cycles,reg.H,reg.B);
      break;
    case 0x61:
      LD_8_r_r(cycles,reg.H,reg.C);
      break;    
    case 0x62:
      LD_8_r_r(cycles,reg.H,reg.D);
      break;
    case 0x63:
      LD_8_r_r(cycles,reg.H,reg.E);
      break;
    case 0x64:
      LD_8_r_r(cycles,reg.H,reg.H);
      break;
    case 0x65:
      LD_8_r_r(cycles,reg.H,reg.L);
      break;
    case 0x66:
      LD_8_n_nn(cycles,reg.H,reg.HL);
      break;
    case 0x68:
      LD_8_r_r(cycles,reg.L,reg.B);
      break;
    case 0x69:
      LD_8_r_r(cycles,reg.L,reg.C);
      break;    
    case 0x6A:
      LD_8_r_r(cycles,reg.L,reg.D);
      break;
    case 0x6B:
      LD_8_r_r(cycles,reg.L,reg.E);
      break;
    case 0x6C:
      LD_8_r_r(cycles,reg.L,reg.H);
      break;
    case 0x6D:
      LD_8_r_r(cycles,reg.L,reg.L);
      break;
    case 0x6E:
      LD_8_n_nn(cycles,reg.L,reg.HL);
      break;
      //Redo these
    case 0x70:
      LD_8_r_r(cycles,reg.HL,reg.B);
      break;
    case 0x71:
      LD_8_r_r(cycles,reg.HL,reg.C);
      break;
    case 0x72:
      LD_8_r_r(cycles,reg.HL,reg.D);
      break;
    case 0x73:
      LD_8_r_r(cycles,reg.HL,reg.E);
      break;
    case 0x74:
      LD_8_r_r(cycles,reg.HL,reg.H);
      break;
    case 0x75:
      LD_8_r_r(cycles,reg.HL,reg.L);
      break;
    case 0x36:
      LD_8_r_r(cycles,reg.HL,reg.pc++);
      break;
    case 0x0A:
      LD_8_n_nn(cycles,reg.A,reg.BC);
      break;               
    case 0x1A:
      LD_8_n_nn(cycles,reg.A,reg.DE);
      break;               
    case 0xFA:
      cycles = 16;
      reg.A = read_byte(read_word(reg.pc++));
      break;               
    case 0x3E:
      LD_8_n_nn(cycles,reg.A,reg.pc++); 
      break;
    case 0x47:
      LD_8_r_r(cycles,reg.B,reg.A);
      break;
    case 0x4F:
      LD_8_r_r(cycles,reg.C,reg.A);
      break;
    case 0x57:
      LD_8_r_r(cycles,reg.D,reg.A);
      break;
    case 0x5F:
      LD_8_r_r(cycles,reg.E,reg.A);
      break;
    case 0x67:
      LD_8_r_r(cycles,reg.H,reg.A);
      break;
    case 0x6F:
      LD_8_r_r(cycles,reg.L,reg.A);
      break; 
    case 0x02:
      LD_8_r_r(cycles,reg.BC,reg.A);
      break;
    case 0x12:
      LD_8_r_r(cycles,reg.DE,reg.A);
      break;
    case 0x77:
      LD_8_r_r(cycles,reg.HL,reg.A);
      break;
    case 0xEA:
      cycles = 16;
      write_byte(read_word(reg.pc++),reg.A);
      break;
    case 0xF2:
      LD_8_n_nn(cycles,reg.A,(0xFF00 + reg.C));
      break;
    case 0xE2:
      LD_8_r_r(cycles,(0xFF00+reg.C),reg.A);
      break;
    case 0x3A:
      LDD_8(cycles,reg.A,reg.HL);
      break;
    case 0x32:
      LDD_8(cycles,reg.HL,reg.A);
      break;
    case 0x2A:
      LDI_8(cycles,reg.A,reg.HL);
      break;
    case 0x22:
      LDI_8(cycles,reg.HL,reg.A);
      break;
    case 0xE0:
      LD_8_r_r(cycles,(0xFF00+reg.pc++),reg.A);
      cycles = 12;
      break;
    case 0xF0:
      LD_8_r_r(cycles,reg.A,(0xFF00+reg.pc++));
      cycles = 12;
      break;
    case 0x01:
      LD_16_n_nn(cycles,reg.BC,reg.pc++);
      break;
    case 0x11:
      LD_16_n_nn(cycles,reg.DE,reg.pc++);
      break;
    case 0x21:
      LD_16_n_nn(cycles,reg.HL,reg.pc++);
      break;
    case 0x31:
      LD_16_n_nn(cycles,reg.sp,reg.pc++);
      //interruptHandler(read_byte(IE));
      break;
    case 0xF9:
      LD_16_r_r(cycles,reg.sp,reg.HL);
      break;
    case 0xF8:
      if(((reg.sp&0xf) + (reg.pc&0xf))&0x10) {
	reg.F |= h;
      }
      if((int) reg.sp + (int) reg.pc > 255) {
	reg.F |= c;
      }
      LD_16_n_nn(cycles,reg.HL,(reg.sp+reg.pc++)); 
      reg.F &= ~z;
      reg.F &= ~n; 
      break;
    case 0x08:
      cycles = 20;
      write_word(reg.pc++,reg.sp);
      break;
    case 0xF5:
      push(cycles,reg.sp,reg.AF);
      break;
    case 0xC5:
      push(cycles,reg.sp,reg.BC);
      break;
    case 0xD5:
      push(cycles,reg.sp,reg.DE);
      break;
    case 0xE5:
      push(cycles,reg.sp,reg.HL);
      break; 
    case 0xF1:
      pop(cycles,reg.sp,reg.AF);
      break;
    case 0xC1:
      pop(cycles,reg.sp,reg.BC);
      break;
    case 0xD1:
      pop(cycles,reg.sp,reg.DE);
      break;
    case 0xE1:
      pop(cycles,reg.sp,reg.HL);
      break; 
    case 0x87:
      add_8(cycles,reg.F,reg.A,reg.A);
      break;        
    case 0x80:
      add_8(cycles,reg.F,reg.A,reg.B);
      break;
    case 0x81:
      add_8(cycles,reg.F,reg.A,reg.C);
      break;
    case 0x82:
      add_8(cycles,reg.F,reg.A,reg.D);
      break;
    case 0x83:
      add_8(cycles,reg.F,reg.A,reg.E);
      break;
    case 0x84:
      add_8(cycles,reg.F,reg.A,reg.H);
      break;
    case 0x85:
      add_8(cycles,reg.F,reg.A,reg.L);
      break;
    case 0x86:
      add_8(cycles,reg.F,reg.A,read_byte(reg.HL));
      cycles = 8;
      break; 
    case 0xC6:
      add_8(cycles,reg.F,reg.A,read_byte(reg.pc++));
      cycles = 8;
      break;
    case 0x8F:
      adc(cycles,reg.F,reg.A,reg.A);
      break;
    case 0x88:
      adc(cycles,reg.F,reg.A,reg.B);
      break;            
    case 0x89:
      adc(cycles,reg.F,reg.A,reg.C);
      break;            
    case 0x8A:
      adc(cycles,reg.F,reg.A,reg.D);
      break;            
    case 0x8B:
      adc(cycles,reg.F,reg.A,reg.E);
      break;            
    case 0x8C:
      adc(cycles,reg.F,reg.A,reg.H);
      break;            
    case 0x8D:
      adc(cycles,reg.F,reg.A,reg.L);
      break;            
    case 0x8E:
      adc(cycles,reg.F,reg.A,read_byte(reg.HL));
      cycles = 8;
      break;            
    case 0xCE:
      adc(cycles,reg.F,reg.A,read_byte(reg.pc++));
      cycles = 8;
      break;                
    case 0x97:
      sub_8(cycles,reg.F,reg.A,reg.A);          
      break;
    case 0x90:
      sub_8(cycles,reg.F,reg.A,reg.B);          
      break;
    case 0x91:
      sub_8(cycles,reg.F,reg.A,reg.C);
      break; 
    case 0x92:
      sub_8(cycles,reg.F,reg.A,reg.D);
      break;    
    case 0x93:
      sub_8(cycles,reg.F,reg.A,reg.E);
      break;
    case 0x94:
      sub_8(cycles,reg.F,reg.A,reg.H);
      break;
    case 0x95:
      sub_8(cycles,reg.F,reg.A,reg.L);
      break;
    case 0x96:
      sub_8(cycles,reg.F,reg.A,read_byte(reg.HL));
      cycles = 8;
      break;
    case 0xD6:
      sub_8(cycles,reg.F,reg.A,read_byte(reg.pc++));;
      cycles = 8;
      break;
    case 0x9F:
      subc(cycles,reg.F,reg.A,reg.A);          
      break;
    case 0x98:
      subc(cycles,reg.F,reg.A,reg.B);          
      break;
    case 0x99:
      subc(cycles,reg.F,reg.A,reg.C);
      break; 
    case 0x9A:
      subc(cycles,reg.F,reg.A,reg.D);
      break;    
    case 0x9B:
      subc(cycles,reg.F,reg.A,reg.E);
      break;
    case 0x9C:
      subc(cycles,reg.F,reg.A,reg.H);
      break;
    case 0x9D:
      subc(cycles,reg.F,reg.A,reg.L);
      break;
    case 0x9E:
      subc(cycles,reg.F,reg.A,read_byte(reg.HL));
      cycles = 8;
      break;
    case 0xDE:
      subc(cycles,reg.F,reg.A,read_byte(reg.pc++));;
      cycles =8;
      break;
    case 0xA7:
      and_8(cycles,reg.F,reg.A,reg.A);
      break; 
    case 0xA0:
      and_8(cycles,reg.F,reg.A,reg.B);
      break; 
    case 0xA1:
      and_8(cycles,reg.F,reg.A,reg.C);
      break; 
    case 0xA2:
      and_8(cycles,reg.F,reg.A,reg.D);
      break; 
    case 0xA3:
      and_8(cycles,reg.F,reg.A,reg.E);
      break; 
    case 0xA4:
      and_8(cycles,reg.F,reg.A,reg.H);
      break; 
    case 0xA5:
      and_8(cycles,reg.F,reg.A,reg.L);
      break; 
    case 0xA6:
      temp = read_byte(reg.HL);
      and_8(cycles,reg.F,reg.A,temp);
      cycles = 8;
      break; 
    case 0xE6:
      temp = read_byte(reg.pc++);
      and_8(cycles,reg.F,reg.A,temp);
      cycles = 8;
      break; 
    case 0xB7:
      or_8(cycles,reg.F,reg.A,reg.A);
      break; 
    case 0xB0:
      or_8(cycles,reg.F,reg.A,reg.B);
      break; 
    case 0xB1:
      or_8(cycles,reg.F,reg.A,reg.C);
      break; 
    case 0xB2:
      or_8(cycles,reg.F,reg.A,reg.D);
      break; 
    case 0xB3:
      or_8(cycles,reg.F,reg.A,reg.E);
      break; 
    case 0xB4:
      or_8(cycles,reg.F,reg.A,reg.H);
      break; 
    case 0xB5:
      or_8(cycles,reg.F,reg.A,reg.L);
      break; 
    case 0xB6:
      temp = read_byte(reg.HL);
      or_8(cycles,reg.F,reg.A,temp);
      cycles = 8;
      break; 
    case 0xF6:
      temp = read_byte(reg.pc++);
      or_8(cycles,reg.F,reg.A,temp);
      cycles = 8;
      break; 
    case 0xAF:
      xor_8(cycles,reg.F,reg.A,reg.A);
      break; 
    case 0xA8:
      xor_8(cycles,reg.F,reg.A,reg.B);
      break; 
    case 0xA9:
      xor_8(cycles,reg.F,reg.A,reg.C);
      break; 
    case 0xAA:
      xor_8(cycles,reg.F,reg.A,reg.D);
      break; 
    case 0xAB:
      xor_8(cycles,reg.F,reg.A,reg.E);
      break; 
    case 0xAC:
      xor_8(cycles,reg.F,reg.A,reg.H);
      break; 
    case 0xAD:
      xor_8(cycles,reg.F,reg.A,reg.L);
      break; 
    case 0xAE:
      temp = read_byte(reg.HL);
      xor_8(cycles,reg.F,reg.A,temp);
      cycles = 8;
      break; 
    case 0xEE:
      temp = read_byte(reg.pc++);
      xor_8(cycles,reg.F,reg.A,temp);
      cycles = 8;
      break; 
    case 0xBF:
      cp_8(cycles,reg.F,reg.A,reg.A);
      break;
    case 0xB8:
      cp_8(cycles,reg.F,reg.A,reg.B);
      break;
    case 0xB9:
      cp_8(cycles,reg.F,reg.A,reg.C);
      break;
    case 0xBA:
      cp_8(cycles,reg.F,reg.A,reg.D);
      break;
    case 0xBB:
      cp_8(cycles,reg.F,reg.A,reg.E);
      break;
    case 0xBC:
      cp_8(cycles,reg.F,reg.A,reg.H);
      break;
    case 0xBD:
      cp_8(cycles,reg.F,reg.A,reg.L);
      break;
    case 0xBE:
      cp_8(cycles,reg.F,reg.A,read_byte(reg.HL));
      cycles = 8;
      break;
    case 0xFE:
      cp_8(cycles,reg.F,reg.A,read_byte(reg.pc++));
      cycles = 8;
      break;
    case 0x3C:
      inc_8(cycles,reg.F,reg.A);
      break;
    case 0x04:
      inc_8(cycles,reg.F,reg.B);
      break;
    case 0x0c:
      inc_8(cycles,reg.F,reg.C);
      break;
    case 0x14:
      inc_8(cycles,reg.F,reg.D);
      break;
    case 0x1C:
      inc_8(cycles,reg.F,reg.E);
      break;
    case 0x24:
      inc_8(cycles,reg.F,reg.H);
      break;
    case 0x2C:
      inc_8(cycles,reg.F,reg.L);
      break;
    case 0x34:
      temp = read_byte(reg.HL);
      inc_8(cycles,reg.F,temp);
      cycles = 12;
      break;
    case 0x3D:
      dec_8(cycles,reg.F,reg.A);
      break;
    case 0x05:
      dec_8(cycles,reg.F,reg.B);
      break;
    case 0x0D:
      dec_8(cycles,reg.F,reg.C);
      break;
    case 0x15:
      dec_8(cycles,reg.F,reg.D);
      break;
    case 0x1D:
      dec_8(cycles,reg.F,reg.E);
      break;
    case 0x25:
      dec_8(cycles,reg.F,reg.H);
      break;
    case 0x2D:
      dec_8(cycles,reg.F,reg.L);
      break;
    case 0x35:
      temp = read_byte(reg.HL);
      dec_8(cycles,reg.F,temp);
      cycles = 12;
      break;
    case 0x09:
      add_16(cycles,reg.F,reg.HL,reg.DE);
      break;    
    case 0x19:
      add_16(cycles,reg.F,reg.HL,reg.HL);
      break;    
    case 0x29:
      add_16(cycles,reg.F,reg.HL,reg.sp);
      break;    
    case 0x39:
      add_16(cycles,reg.F,reg.HL,reg.BC);
      break;    
    case 0xE8:
      add_sp(cycles,reg.F,reg.sp,reg.pc++);
      break; 
    case 0x03:
      inc_16(cycles,reg.BC);
      break;
    case 0x13:
      inc_16(cycles,reg.DE);
      break;
    case 0x23:
      inc_16(cycles,reg.HL);
      break;
    case 0x33:
      inc_16(cycles,reg.sp);
      break;
    case 0x0B:
      dec_16(cycles,reg.BC);
      break;
    case 0x1B:
      dec_16(cycles,reg.DE);
      break;
    case 0x2B:
      dec_16(cycles,reg.HL);
      break;
    case 0x3B:
      dec_16(cycles,reg.sp);
      break;
    case 0x27:
      daa(cycles,reg.F,reg.A);
      break;
    case 0x2F:
      cpl(cycles,reg.F,reg.A);
      break;
    case 0x3F:
      ccf(cycles,reg.F);
      break;
    case 0x37:
      scf(cycles,reg.F);
      break;
    case 0x00://NOP
      cycles = 4;
      reg.pc++;
      break;
    case 0x76://HALT
      cycles = 4;
      break;
    case 0x10://STOP 0x10
      /*uint16_t x = (0x10 << 8) + reg.pc++;
      if (reg.pc++ == 0x1000) {
      cycles = 4;
      reg.pc++;*/
      break;
    case 0xF3://DI disable interrupts
        cycles = 4;
        //write_byte(IF_R,0);
        ime = 0; 
        break;
    case 0xFB://EI enable interrupts
        cycles = 4;
        //write_byte(IE_R,0b00011111);
        ime = 1;
        break;    
    case 0x07:
      RLCA(cycles,reg.F,reg.A);
      break;
    case 0x17:
      RLA(cycles,reg.F,reg.A);
      break;
    case 0x0F:
      RLCA(cycles,reg.F,reg.A);
      break;
    case 0x1F:
      RLA(cycles,reg.F,reg.A);
      break;
    case 0xC3:
      reg.pc = read_word(reg.pc);
      cycles = 12;
      break; 
    case 0xC2:
      cycles = 12;
      if (reg.F & ~0x80) 
	reg.pc = read_word(reg.pc);
      else
	reg.pc++;
      break; 
    case 0xCA:
      cycles = 12;
      if (reg.F & 0x80) 
	reg.pc = read_word(reg.pc);
      else 
	reg.pc++;
      break; 
    case 0xD2:
      cycles = 12;
      if (reg.F & ~0x20) 
	reg.pc = read_word(reg.pc);
      else
	reg.pc++;
      break; 
    case 0xDA:
      cycles = 12;
      if (reg.F & 0x20) 
	reg.pc = read_word(reg.pc);
      else
	reg.pc++;
      break; 
    case 0xE9:
      cycles = 4;
      reg.pc = read_word(reg.HL);
      break;
    case 0x18:
      cycles = 8;
      reg.pc = reg.pc + read_byte(reg.pc); 
      break;
    case 0x20:
      cycles = 8;
      if (reg.F & ~0x80) 
	reg.pc = reg.pc + read_byte(reg.pc);
      else
	reg.pc++;
      break; 
    case 0x28:
      cycles = 8;
      if (reg.F & 0x80) 
	reg.pc = reg.pc + read_byte(reg.pc);
      else
	reg.pc++;
      break; 
    case 0x30:
      cycles = 8;
      if (reg.F & ~0x20) 
	reg.pc = reg.pc + read_byte(reg.pc);
      else 
	reg.pc++;
      break; 
    case 0x38:
      cycles = 8;
      if (reg.F & 0x20) 
	reg.pc = reg.pc + read_byte(reg.pc);
      else
	reg.pc++;
      break;
    case 0xCD:
      cycles = 12;
      push(cycles,reg.sp,reg.pc);
      reg.pc = read_word(reg.pc);
      break;
    case 0xC4:
      cycles = 12;
      if (reg.F & ~0x80) { 
	push(cycles,reg.sp,reg.pc);
	reg.pc = reg.pc + read_word(reg.pc);
      } else {
	reg.pc++;
      }
      break; 
    case 0xCC:
      cycles = 12;
      if (reg.F & 0x80) { 
	push(cycles,reg.sp,reg.pc);
	reg.pc = reg.pc + read_word(reg.pc);
      } else {
	reg.pc++;
      }
      break; 
    case 0xD4:
      cycles = 12;
      if (reg.F & ~0x20) {
	push(cycles,reg.sp,reg.pc); 
	reg.pc = reg.pc + read_word(reg.pc);
      } else {
	reg.pc++;
      }        
      break; 
    case 0xDC:
      cycles = 12;
      if (reg.F & 0x20) { 
	push(cycles,reg.sp,reg.pc);
	reg.pc = reg.pc + read_word(reg.pc);
      } else {
	reg.pc++;
      }
      break;
    case 0xC7:
      rst(cycles,reg.sp,reg.pc);
      break;
    case 0xCF:
      rst(cycles,reg.sp,reg.pc);
      break;
    case 0xD7:
      rst(cycles,reg.sp,reg.pc);
      break;
    case 0xDF:
      rst(cycles,reg.sp,reg.pc);
      break;
    case 0xE7:
      rst(cycles,reg.sp,reg.pc);
      break;
    case 0xEF:
      rst(cycles,reg.sp,reg.pc);
      break;
    case 0xF7:
      rst(cycles,reg.sp,reg.pc);
      break;
    case 0xFF:
      rst(cycles,reg.sp,reg.pc);
      break;
    case 0xC9:
      pop(cycles,reg.AF,reg.sp);
      cycles += 8;
      reg.pc = read_word(reg.AF);
      break;
    case 0xC0:
      if (reg.F & ~0x80) {
		pop(cycles,reg.AF,reg.sp);
		reg.pc = read_word(reg.AF);
      } else {
	reg.pc++;
      }
             
      cycles += 8;
      break; 
    case 0xC8:
      if (reg.F & 0x80) {
		pop(cycles,reg.AF,reg.sp);
		reg.pc = read_word(reg.AF);
 
      } else {
		reg.pc++;
      }
             
      cycles += 8;
      break; 
            
    case 0xD0:
      if (reg.F & ~0x20) {
		pop(cycles,reg.AF,reg.sp);
		reg.pc = read_word(reg.AF);
 
      } else {
		reg.pc++;
      }
            
      cycles += 8;
      break; 
    case 0xD8:
      if (reg.F & 0x20) {
		pop(cycles,reg.AF,reg.sp);
		reg.pc = read_word(reg.AF);
            
      } else {
		reg.pc++;
      }
             
      cycles += 8;
      break;
    case 0xD9:
      pop(cycles,reg.AF,reg.sp);
      cycles += 8;
      reg.pc = read_word(reg.AF);
      //Enable interrupt
        //write_word(IE_R,0b00011111); 
      ime = 1;
      cycles += 8; 
      break;
    case 0xCB:
      uint32_t muxedOpCode = (uint32_t) opcode << 16;
      muxedOpCode = opcode + read_byte(reg.pc++);
      //uint16_t opcode = read_byte(reg.pc++);	
      //TODO: Make something cleaner and prettier here in the fute
      //But since it is lot of work it will probably never happen
      switch(muxedOpCode) {
        case 0xCB37:
            swap(cycles,reg.F,reg.A);
            break;
        case 0xCB30:
            swap(cycles,reg.F,reg.B);
            break;
        case 0xCB31:
            swap(cycles,reg.F,reg.C);
            break;
        case 0xCB32:
            swap(cycles,reg.F,reg.D);
            break;
        case 0xCB33:
            swap(cycles,reg.F,reg.E);
            break;
        case 0xCB34:
            swap(cycles,reg.F,reg.H);
            break;
        case 0xCB35:
            swap(cycles,reg.F,reg.L);
            break;
        case 0xCB36:
            temp = read_byte(reg.HL);
            swap(cycles,reg.F,temp);
            cycles = 16;
            break;
        case 0xCB07:
            RLCA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB00:
            RLCA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB01:
            RLCA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB02:
            RLCA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB03:
            RLCA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB04:
            RLCA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB05:
            RLCA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB06:
            temp = read_byte(reg.HL);
            RLCA(cycles,reg.F,temp);
            cycles = 16;
            break;
         case 0xCB17:
            RLA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB10:
            RLA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB11:
            RLA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB12:
            RLA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB13:
            RLA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB14:
            RLA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB15:
            RLA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB16:
            temp = read_byte(reg.HL);
            RLA(cycles,reg.F,temp);
            cycles = 16;
            break;
         case 0xCB0F:
            RRCA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB08:
            RRCA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB09:
            RRCA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB0A:
            RRCA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB0B:
            RRCA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB0C:
            RRCA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB0D:
            RRCA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB0E:
            temp = read_byte(reg.HL);
            RRCA(cycles,reg.F,temp);
            cycles = 16;
            break;
         case 0xCB1F:
            RRA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB18:
            RRA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB19:
            RRA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB1A:
            RRA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB1B:
            RRA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB1C:
            RRA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB1D:
            RRA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB1E:
            temp = read_byte(reg.HL);
            RRA(cycles,reg.F,temp);
            cycles = 16;
            break;
    //Shifts
          case 0xCB27:
            SLA(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB20:
            SLA(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB21:
            SLA(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB22:
            SLA(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB23:
            SLA(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB24:
            SLA(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB25:
            SLA(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB26:
            temp = read_byte(reg.HL);
            SLA(cycles,reg.F,temp);
            cycles = 16;
            break;   
          case 0xCB2F:
            SRL(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB28:
            SRL(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB29:
            SRL(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB2A:
            SRL(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB2B:
            SRL(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB2C:
            SRL(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB2D:
            SRL(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB2E:
            temp = read_byte(reg.HL);
            SRL(cycles,reg.F,temp);
            cycles = 16;
            break;   
          case 0xCB3F:
            SRL(cycles,reg.F,reg.A);
            cycles = 8;
            break;
         case 0xCB38:
            SRL(cycles,reg.F,reg.B);
            cycles = 8;
            break;
         case 0xCB39:
            SRL(cycles,reg.F,reg.C);
            cycles = 8;
            break;
         case 0xCB3A:
            SRL(cycles,reg.F,reg.D);
            cycles = 8;
            break;
         case 0xCB3B:
            SRL(cycles,reg.F,reg.E);
            cycles = 8;
            break;
         case 0xCB3C:
            SRL(cycles,reg.F,reg.H);
            cycles = 8;
            break;
          case 0xCB3D:
            SRL(cycles,reg.F,reg.L);
            cycles = 8;
            break;
         case 0xCB3E:
            temp = read_byte(reg.HL);
            SRL(cycles,reg.F,temp);
            cycles = 16;
            break;  
         case 0xCB47:
            cmpbit_b_r(cycles,reg.F,reg.A,0);
            break;
         case 0xCB40:
            cmpbit_b_r(cycles,reg.F,reg.B,0);
            break;
        case 0xCB41:
            cmpbit_b_r(cycles,reg.F,reg.C,0);
            break;
          case 0xCB42:
            cmpbit_b_r(cycles,reg.F,reg.D,0);
            break;
        case 0xCB43:
            cmpbit_b_r(cycles,reg.F,reg.E,0);
            break;  
         case 0xCB44:
            cmpbit_b_r(cycles,reg.F,reg.H,0);
            break;
        case 0xCB45:
            cmpbit_b_r(cycles,reg.F,reg.L,0);
            break;
        case 0xCB46:
            temp = read_byte(reg.HL);
            cmpbit_b_r(cycles,reg.F,temp,0);
            cycles = 16;
            break;
         case 0xCB4F:
            cmpbit_b_r(cycles,reg.F,reg.A,1);
            break;
         case 0xCB48:
            cmpbit_b_r(cycles,reg.F,reg.B,1);
            break;
        case 0xCB49:
            cmpbit_b_r(cycles,reg.F,reg.C,1);
            break;
        case 0xCB4A:
            cmpbit_b_r(cycles,reg.F,reg.D,1);
            break;
        case 0xCB4B:
            cmpbit_b_r(cycles,reg.F,reg.E,1);
            break;  
         case 0xCB4C:
            cmpbit_b_r(cycles,reg.F,reg.H,1);
            break;
        case 0xCB4D:
            cmpbit_b_r(cycles,reg.F,reg.L,1);
            break;
        case 0xCB4E:
            temp = read_byte(reg.HL);
            cmpbit_b_r(cycles,reg.F,temp,1);
            cycles = 16;
            break;
         case 0xCB57:
            cmpbit_b_r(cycles,reg.F,reg.A,2);
            break;
         case 0xCB50:
            cmpbit_b_r(cycles,reg.F,reg.B,2);
            break;
        case 0xCB51:
            cmpbit_b_r(cycles,reg.F,reg.C,2);
            break;
        case 0xCB52:
            cmpbit_b_r(cycles,reg.F,reg.D,2);
            break;
        case 0xCB53:
            cmpbit_b_r(cycles,reg.F,reg.E,2);
            break;  
         case 0xCB54:
            cmpbit_b_r(cycles,reg.F,reg.H,2);
            break;
        case 0xCB55:
            cmpbit_b_r(cycles,reg.F,reg.L,2);
            break;
        case 0xCB56:
            temp = read_byte(reg.HL);
            cmpbit_b_r(cycles,reg.F,temp,2);
            cycles = 16;
            break;
         case 0xCB5F:
            cmpbit_b_r(cycles,reg.F,reg.A,3);
            break;
         case 0xCB58:
            cmpbit_b_r(cycles,reg.F,reg.B,3);
            break;
        case 0xCB59:
            cmpbit_b_r(cycles,reg.F,reg.C,3);
            break;
        case 0xCB5A:
            cmpbit_b_r(cycles,reg.F,reg.D,3);
            break;
        case 0xCB5B:
            cmpbit_b_r(cycles,reg.F,reg.E,3);
            break;  
         case 0xCB5C:
            cmpbit_b_r(cycles,reg.F,reg.H,3);
            break;
        case 0xCB5D:
            cmpbit_b_r(cycles,reg.F,reg.L,3);
            break;
        case 0xCB5E:
            temp = read_byte(reg.HL);
            cmpbit_b_r(cycles,reg.F,temp,3);
            cycles = 16;
            break;
	    
      case 0xCBC7:
            set_b_r(cycles,reg.A,reg.pc++);
            break;
         case 0xCBC0:
            set_b_r(cycles,reg.B,reg.pc++);
            break;
        case 0xCBC1:
            set_b_r(cycles,reg.C,reg.pc++);
            break;
          case 0xCBC2:
            set_b_r(cycles,reg.D,reg.pc++);
            break;
        case 0xCBC3:
            set_b_r(cycles,reg.E,reg.pc++);
            break;  
         case 0xCBC4:
            set_b_r(cycles,reg.H,reg.pc++);
            break;
        case 0xCBC5:
            set_b_r(cycles,reg.L,reg.pc++);
            break;
        case 0xCBC6:
            temp = read_byte(reg.HL);
            set_b_r(cycles,temp,reg.pc++);
            cycles = 16;
            break;
        case 0xCB87:
            res_b_r(cycles,reg.A,reg.pc++);
            break;
         case 0xCB80:
            res_b_r(cycles,reg.B,reg.pc++);
            break;
        case 0xCB81:
            res_b_r(cycles,reg.C,reg.pc++);
            break;
          case 0xCB82:
            res_b_r(cycles,reg.D,reg.pc++);
            break;
        case 0xCB83:
            res_b_r(cycles,reg.E,reg.pc++);
            break;  
         case 0xCB84:
            res_b_r(cycles,reg.H,reg.pc++);
            break;
        case 0xCB85:
            res_b_r(cycles,reg.L,reg.pc++);
            break;
        case 0xCB86:
            temp = read_byte(reg.HL);
            res_b_r(cycles,temp,reg.pc++);
            cycles = 16;
            break;       	       
      }
        break;
/*    case 0xIE:
        write_byte(IE_R,0b00011111);
        cycles = 4;
        break;    
    case 0xDI:
        write_byte(IE_R,0);
        cycles = 4;
        break;*/
    }
}
