#include <cstdint> 
#include <unistd.h>
#include <iostream>
#include <bitset>
#include <assert.h>
#include <string.h>
#include "../inc/register.hpp"
#include "../inc/memory.hpp"
#include "../inc/instructions.hpp"
#include "../inc/video.hpp"
#include "../debug/debug.hpp"

#define BLARGH
//#define DEBUG_OPCODE
 
#define WORD_LOAD 2
#define D8 int(read_byte(reg.PC.getData_()-1))
#define D16 int(read_word(reg.PC.getData_()-2))

#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10

static uint8_t ime = 0;

struct registers {
    Register8 A;
    Register8 F;
    Register16 AF;
    Register8 B;
    Register8 C;
    Register16 BC;
    Register8 D;
    Register8 E;
    Register16 DE;
    Register8 H;
    Register8 L;
    Register16 HL;
    Register16 SP;
    Register16 PC;
    //Register16 &sp;
    uint16_t pc;
} reg;
void print_registers() {
    std::cout << "---------------" << std::endl;
    std::cout << "| A: " << std::hex << int(reg.A.getData()) << " | F: " << int(reg.F.getData()) << " |" << std::endl;
    std::cout << "| AF: " << std::hex << int(reg.AF.getData()) << " |" << std::endl;
    std::cout << "| B: " << std::hex << int(reg.B.getData()) << " | C: |" << int(reg.C.getData()) << " |" << std::endl;
    std::cout << "| BC: " << std::hex << int(reg.BC.getData()) << " |" << std::endl;
    std::cout << "| D: " << std::hex << int(reg.D.getData()) << " | D: |" << int(reg.E.getData()) << " |" << std::endl;
    std::cout << "| DE: " << std::hex << int(reg.DE.getData()) << " |" << std::endl;
    std::cout << "| H: " << std::hex << int(reg.H.getData()) << " | L: |" << int(reg.L.getData()) << " |"  << std::endl;
    std::cout << "| HL: " << std::hex << int(reg.HL.getData()) << " |" << std::endl;
    std::cout << "------------------" << std::endl;
}

/*void check_registers() {
    assert(reg.AF == (uint16_t(reg.A) >> 8 | uint16_t(reg.F)));
    assert(reg.BC == (uint16_t(reg.B) << 8 | uint16_t(reg.C)));
    assert(reg.DE == (uint16_t(reg.D) << 8 | uint16_t(reg.E)));
    assert(reg.HL == (uint16_t(reg.HL.high) << 8 | uint16_t(reg.L)));
}*/

int z = 0x80;
int n = 0x40;
int h = 0x20;
int c = 0x10;

int eval_opcode(uint16_t opcode,int cycles);
//void init_registers();
//void skip_boot();
void cpu_step(int &running) {
  //init_mem();

  //int a = 0x01;
  //int b = 0x02;
  //int c2 = 0x04;
  //int d = 0x08;    
  int cycles = 0;
  uint16_t opcode;
  uint8_t temp;

  //uint8_t temp = 0;
/*  reg.A = 0;
  reg.F = 0;
  reg.B = 0;
  reg.C = 0;
  reg.D = 0;
  reg.E = 0;
  reg.H = 0;
  reg.L = 0;
  reg.sp = 0;
  reg.pc = 0;*/

  //int running = 1;    
#ifdef DEBUGGER
  int debugF = 0;
  start_debugger();
#endif

/*#ifdef DEBUG
    std::cout << std::hex << "Register values: PC: " <<int(reg.PC.getData_())) << ", SP: " << int(reg.SP) << ", A: " << int(reg.A) << ", F: " << BIN(reg.F) << ", AF: " << int(reg.AF.getData()) << ", B: " << int(reg.B) << ", C: " << int(reg.C) << ", BC: " << int(reg.BC.getData()) << ", D: " << int(reg.D) << ", E: " << ", DE: " << int(reg.DE.getData()) << ", H:" << int(reg.HL.high) << ", L:" << int(reg.L) << ", HL: " << int(reg.HL.getData()) << std::endl;
#endif*/ 
    opcode = read_byte(reg.PC.getData_());
    //reg.PC.inc(1);
#ifdef DEBUGGER
    debugF = step();
   (debugF == 1) ? running = 0: running = 1;
#endif

#ifdef DEBUG
/*   if (opcode != 0xCB) {
       std::cout << "Opcode: " << std::hex << opcode << ", ";
       std::cout << std::hex << "Register values: PC: " <<int(reg.pc-1) << ", SP: " << int(reg.SP) << ", A: " << int(reg.AF.getHigh()  ) << ", F: " << BIN(reg.AF.getLow()  ) << ", AF: " << int(reg.AF.getData()) << ", B: " << int(reg.BC.getHigh()  ) << ", C: " << int(reg.BC.getLow()  ) << ", BC: " << int(reg.BC.getData()) << ", D: " << int(reg.DE.getHigh()  ) << ", E: " << int(reg.DE.getLow()  ) << ", DE: " << int(reg.DE.getData()) << ", H:" << int(reg.HL.high  ) << ", L:" << int(reg.HL.getLow()  ) << ", HL: " << int(reg.HL.getData()) << std::endl;
    }*/
#endif

    //check_registers();
    cycles = eval_opcode(opcode,cycles);

#ifdef BLARGH
    if (read_byte(0xff02 == 0x81)) {
        char c = read_byte(0xff01);
        printf("%c",c);
        write_byte(0xff02,0x0);
    }
#endif
    uint8_t temp2 = read_byte(IE_R) & read_byte(IF_R); 
    if (false && ime == 1 && temp2) {
#ifdef DEBUG
        std::cout << "Interrupt! bitmap: " << BIN(temp2) << std::endl;
#endif
        int cycleTemp = 0;
        //LD_16_n_nn(cycleTemp,reg.SP,reg.PC.getData_()));
        //reg.pc += 2;
        reg.PC.inc(2);
        cycles += 8 + cycleTemp;
        
        //TODO: Check if we should enable several interrupts here
        // V-Blank
        if (temp2 & 0b00000001) {
            LD_16_n_nn(cycleTemp,reg.PC,0x0040);
            cycles += cycleTemp;
            cycles = 0;
            return;
        // LCD STAT
        } else if (temp2 & 0b00000010) {
            LD_16_n_nn(cycleTemp,reg.PC,0x0048);
            cycles += cycleTemp;
            cycles = 0;           
            return;
            // Timer
        } else if (temp2 & 0b00000100) {
            LD_16_n_nn(cycleTemp,reg.PC,0x0050);
            cycles += cycleTemp;
            cycles = 0;           
            return;
            // Serial
        } else if (temp2 & 0b00001000) {
            LD_16_n_nn(cycleTemp,reg.PC,0x0058);
            cycles += cycleTemp;
            cycles = 0;           
            return;
            // Joypad    
        }  else if (temp2 & 0b00010000) {
            LD_16_n_nn(cycleTemp,reg.PC, 0x0060);
            cycles += cycleTemp;
            cycles = 0;           
            return;
        } 
    }
    
    lcdController(cycles);
    if (eventController() == 1) {
        running = 0;
        return;
    }
    usleep((cycles/4190000)*100000);
    cycles = 0;    
}

void init_registers() {

  reg.A = Register8(0,"A");
  reg.F = Register8(0,"F");
  reg.B = Register8(0,"B");
  reg.C = Register8(0,"C");
  reg.D = Register8(0,"D");
  reg.E = Register8(0,"E");
  reg.H = Register8(0,"H");
  reg.L = Register8(0,"L");
  reg.AF = Register16(&reg.A,&reg.F);
  reg.BC = Register16(&reg.B,&reg.C);
  reg.DE = Register16(&reg.D,&reg.E);
  reg.HL = Register16(&reg.H,&reg.L);
  reg.PC = Register16(0x0,"PC"); 
  //reg.pc = &reg.PC;
  reg.SP = Register16(0,"SP");
  //reg.sp = &reg.SP;
}

int eval_opcode(uint16_t opcode, int cycles) {
    uint16_t lastPC = 0;
    uint16_t  tmpAddr = 0;
    Register16 tmpSP;
    int tmpCycles = 0;

#ifdef DEBUG_OPCODE
            std::cout << "OPCODE: ; " << std::hex << opcode << std::endl;
#endif
    
    switch(opcode) {
        case 0x00:
            cycles = 4;
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE
            std::cout << "NOP ; " << std::hex << reg.PC.getData_() << std::endl;
#endif
            break;
        case 0x01:
            reg.PC.inc(1);
            LD_16_n_nn(cycles, reg.BC, reg.PC.getData_());
            reg.PC.inc(WORD_LOAD);
#ifdef DEBUG_OPCODE 
            std::cout << "LD BC " << std::hex << read_word(reg.PC.getData_()) << "; " << reg.PC.getData_()-3 << std::endl;
#endif
            break;
        case 0x02:
            reg.PC.inc(1);
            LD_8_r_r(cycles,reg.BC.getData(),reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (BC) A" << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x03:
            reg.PC.inc(1);
            inc_16(cycles, reg.BC);
#ifdef DEBUG_OPCODE 
            std::cout << "INC BC " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x04:
            reg.PC.inc(1);
            inc_8(cycles, reg.F, reg.B);
#ifdef DEBUG_OPCODE 
            std::cout << "INC B " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x05:
            reg.PC.inc(1);
            dec_8(cycles, reg.F, reg.B);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC B " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x06:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.B, reg.PC.getData_());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0x07:
            reg.PC.inc(1);
            RLCA(cycles, reg.F, reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "RLCA " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x08:
            reg.PC.inc(1);
            LD_8_r_r(cycles, read_word(reg.PC.getData_()), reg.SP.getData_());
            reg.PC.inc(2);
            cycles += 8;
#ifdef DEBUG_OPCODE 
            std::cout << "LD " << std::hex << D16 << " SP " << " ; " << reg.PC.getData_()-3 << std::endl;
#endif
            break;
        case 0x09:
            reg.PC.inc(1);
            add_16(cycles, reg.F, reg.HL, reg.BC);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD HL BC " << std::hex  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x0A:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.A, reg.BC.getData());
#ifdef DEBUG_OPCODE 
            std::cout << "LD A (BC) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x0B:
            reg.PC.inc(1);
            dec_16(cycles, reg.BC);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC BC " << std::hex  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x0C:
            reg.PC.inc(1);
            inc_8(cycles, reg.F, reg.C);
#ifdef DEBUG_OPCODE 
            std::cout << "INC C " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x0D:
            reg.PC.inc(1);
            dec_8(cycles, reg.F, reg.C);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC C " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x0E:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.C, reg.PC.getData_());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0x0F:
            reg.PC.inc(1);
            RRCA(cycles, reg.F, reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "RRCA " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x10: //TODO :Add halting of cpu and display 
            cycles = 4;
            reg.PC.inc(2);
            break;
        case 0x11:
            reg.PC.inc(1);
            LD_16_n_nn(cycles, reg.DE, reg.PC.getData_());
            reg.PC.inc(WORD_LOAD);
#ifdef DEBUG_OPCODE 
            std::cout << "LD DE $" << std::hex << read_word(reg.PC.getData_()-2) << "; " << reg.PC.getData_()-3 << std::endl;
#endif
            break;
        case 0x12:
            reg.PC.inc(1);
            LD_8_r_r(cycles,reg.DE.getData(),reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (DE) A" << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x13:
            reg.PC.inc(1);
            inc_16(cycles, reg.DE);
#ifdef DEBUG_OPCODE 
            std::cout << "INC DE " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x14:
            reg.PC.inc(1);
            inc_8(cycles, reg.F, reg.D);
#ifdef DEBUG_OPCODE 
            std::cout << "INC D " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x15:
            reg.PC.inc(1);
            dec_8(cycles, reg.F, reg.D);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC D " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x16:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.D, reg.PC.getData_());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0x17:
            reg.PC.inc(1);
            RLA(cycles, reg.F, reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "RLA " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x18:
            lastPC = reg.PC.getData_();
            reg.PC.setData_(reg.PC.getData_() + 2 + (int8_t) read_byte((reg.PC.getData_()+1)));
            cycles += 12;
#ifdef DEBUG_OPCODE 
            std::cout << "JR " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0x19:
            reg.PC.inc(1);
            add_16(cycles, reg.F, reg.HL, reg.DE);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD HL DE " << std::hex  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x1A:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.A, reg.DE.getData());
#ifdef DEBUG_OPCODE 
            std::cout << "LD A (DE) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x1B:
            reg.PC.inc(1);
            dec_16(cycles, reg.DE);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC DE " << std::hex  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x1C:
            reg.PC.inc(1);
            inc_8(cycles, reg.F, reg.E);
#ifdef DEBUG_OPCODE 
            std::cout << "INC E " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x1D:
            reg.PC.inc(1);
            dec_8(cycles, reg.F, reg.B);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC E " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x1E:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.E, reg.PC.getData_());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0x1F:
            reg.PC.inc(1);
            RRA(cycles, reg.F, reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "RRA " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x20:
            cycles = 8;
            if ((reg.F.getData() & FLAG_Z) == 0x00) {
                cycles+=4;
                int16_t tmpAddr = reg.PC.getData_();
                reg.PC.setData_(uint16_t(tmpAddr + 2 +  int8_t(read_byte(tmpAddr + 1))));
#ifdef DEBUG_OPCODE 
            std::cout << "JR NZ " << std::hex << reg.PC.getData_() << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(2);
#ifdef DEBUG_OPCODE 
            std::cout << "JR NZ " << std::hex << "; " << reg.PC.getData_()-2 << std::endl;
#endif
            }
            break;
        case 0x21:
            reg.PC.inc(1);
            LD_16_n_nn(cycles, reg.HL, reg.PC.getData_());
            reg.PC.inc(WORD_LOAD);
#ifdef DEBUG_OPCODE 
            std::cout << "LD HL " << std::hex << read_word(reg.PC.getData_()-WORD_LOAD) << "; " << reg.PC.getData_()-3 << std::endl;
#endif
            break;
        case 0x22:
            reg.PC.inc(1);
            LDI_8(cycles,reg.HL,reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL+) A" << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x23:
            reg.PC.inc(1);
            inc_16(cycles, reg.HL);
#ifdef DEBUG_OPCODE 
            std::cout << "INC HL" << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x24:
            reg.PC.inc(1);
            inc_8(cycles, reg.F, reg.H);
#ifdef DEBUG_OPCODE 
            std::cout << "INC H " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x25:
            reg.PC.inc(1);
            dec_8(cycles, reg.F, reg.H);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC H " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x26:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.H, reg.PC.getData_());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0x27:
            reg.PC.inc(1);
            daa(cycles, reg.F, reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "DAA " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x28:
            cycles = 8;
            if (reg.F.getData() & FLAG_Z) {
                cycles+=4;
                uint16_t tmpAddr = reg.PC.getData_();
                reg.PC.setData_(tmpAddr + 2 + (int8_t) read_byte(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "JR Z " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(2);
#ifdef DEBUG_OPCODE 
            std::cout << "JR Z " << std::hex << "; " << reg.PC.getData_()-2 << std::endl;
#endif
            }
            break;
            case 0x29:
            reg.PC.inc(1);
            add_16(cycles, reg.F, reg.HL, reg.HL);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD HL HL " << std::hex  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x2A:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.A, reg.HL.getData());
            reg.HL.setData(reg.HL.getData() + 1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A (HL+) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x2B:
            reg.PC.inc(1);
            dec_16(cycles, reg.HL);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC HL " << std::hex  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x2C:
            reg.PC.inc(1);
            inc_8(cycles, reg.F, reg.L);
#ifdef DEBUG_OPCODE 
            std::cout << "INC L " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x2D:
            reg.PC.inc(1);
            dec_8(cycles, reg.F, reg.L);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC L " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x2E:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.E, reg.PC.getData_());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0x2F:
            reg.PC.inc(1);
            ccf(cycles, reg.F);
#ifdef DEBUG_OPCODE 
            std::cout << "CCF " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
       case 0x30:
            cycles = 8;
            if ((reg.AF.getHighData() & FLAG_C) == 0x00) { 
                cycles += 4;
                uint16_t prevAddr = reg.PC.getData_();
                reg.PC.setData(reg.PC.getData_() + (int8_t) read_byte(reg.PC.getData_() + 1));
#ifdef DEBUG_OPCODE 
                std::cout << "JR NC " << std::hex << reg.PC.getData_()  << "; " << prevAddr << std::endl;
#endif
            } else { 
                reg.PC.inc(2);
#ifdef DEBUG_OPCODE 
                std::cout << "JR NC " << std::hex << "; " << reg.PC.getData_()-2 << std::endl;
#endif
            }
            break;
        case 0x31:
            reg.PC.inc(1);
            LD_16_n_nn(cycles, reg.SP, reg.PC.getData_());
            reg.PC.inc(WORD_LOAD);
#ifdef DEBUG_OPCODE 
            std::cout << "LD SP " << std::hex << read_word(reg.PC.getData_()-2) << "; " << reg.PC.getData_()-3 << std::endl;
#endif
            break;
        case 0x32:
            reg.PC.inc(1);
            LDD_8(cycles,reg.HL,reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL-) A" << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x33:
            reg.PC.inc(1);
            inc_16(cycles, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "INC SP" << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x34:
            reg.PC.inc(1);
            inc_8(cycles, reg.F, reg.HL.getData());
#ifdef DEBUG_OPCODE 
            std::cout << "INC HL " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x35:
            reg.PC.inc(1);
            dec_8(cycles, reg.F, reg.HL.getData());
#ifdef DEBUG_OPCODE 
            std::cout << "DEC (HL) " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x36:
            reg.PC.inc(1);
            LD_8_r_r(cycles, reg.HL.getData(), reg.PC.getData_());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL) " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0x37:
            reg.PC.inc(1);
            scf(cycles, reg.F);
#ifdef DEBUG_OPCODE 
            std::cout << "SCF " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x38:
            cycles = 8;
            if (reg.AF.getHighData() & FLAG_C) { 
                    cycles += 4;
                    uint16_t prevAddr = reg.PC.getData_();
                    reg.PC.setData(reg.PC.getData_() + (int8_t) read_byte(reg.PC.getData_() + 1));
#ifdef DEBUG_OPCODE 
                    std::cout << "JR C " << std::hex << reg.PC.getData_()  << "; " << prevAddr << std::endl;
#endif
                    } else { 
                    reg.PC.inc(2);
#ifdef DEBUG_OPCODE 
                    std::cout << "JR C " << std::hex << "; " << reg.PC.getData_()-2 << std::endl;
#endif
                    }
                    break;
        case 0x39:
            reg.PC.inc(1);
            add_16(cycles, reg.F, reg.HL, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD HL SP " << std::hex  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x3A:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.A, reg.HL.getData());
            reg.HL.setData(reg.HL.getData() - 1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A (HL-) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x3B:
            reg.PC.inc(1);
            dec_16(cycles, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC SP " << std::hex  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x3C:
            reg.PC.inc(1);
            inc_8(cycles, reg.F, reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "INC A " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x3D:
            reg.PC.inc(1);
            dec_8(cycles, reg.F, reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "DEC A " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x3E:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.A, reg.PC.getData_());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0x40:
            LD_8_r_r(cycles, reg.B, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x41:
            LD_8_r_r(cycles, reg.B, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x42:
            LD_8_r_r(cycles, reg.B, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x43:
            LD_8_r_r(cycles, reg.B, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x44:
            LD_8_r_r(cycles, reg.B, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x45:
            LD_8_r_r(cycles, reg.B, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x46:
            LD_8_n_nn(cycles, reg.B, reg.HL.getData());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x47:
            LD_8_r_r(cycles, reg.B, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD B A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x48:
            LD_8_r_r(cycles, reg.C, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x49:
            LD_8_r_r(cycles, reg.C, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x4A:
            LD_8_r_r(cycles, reg.C, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x4B:
            LD_8_r_r(cycles, reg.C, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x4C:
            LD_8_r_r(cycles, reg.C, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x4D:
            LD_8_r_r(cycles, reg.C, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x4E:
            LD_8_n_nn(cycles, reg.C, reg.HL.getData());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x4F:
            LD_8_r_r(cycles, reg.C, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD C A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x50:
            LD_8_r_r(cycles, reg.D, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x51:
            LD_8_r_r(cycles, reg.D, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x52:
            LD_8_r_r(cycles, reg.D, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x53:
            LD_8_r_r(cycles, reg.D, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x54:
            LD_8_r_r(cycles, reg.D, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x55:
            LD_8_r_r(cycles, reg.D, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x56:
            LD_8_n_nn(cycles, reg.D, reg.HL.getData());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x57:
            LD_8_r_r(cycles, reg.D, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD D A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x58:
            LD_8_r_r(cycles, reg.E, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x59:
            LD_8_r_r(cycles, reg.E, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x5A:
            LD_8_r_r(cycles, reg.E, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x5B:
            LD_8_r_r(cycles, reg.E, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x5C:
            LD_8_r_r(cycles, reg.E, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x5D:
            LD_8_r_r(cycles, reg.E, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x5E:
            LD_8_n_nn(cycles, reg.E, reg.HL.getData());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x5F:
            LD_8_r_r(cycles, reg.E, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD E A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x60:
            LD_8_r_r(cycles, reg.H, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x61:
            LD_8_r_r(cycles, reg.H, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x62:
            LD_8_r_r(cycles, reg.H, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x63:
            LD_8_r_r(cycles, reg.H, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x64:
            LD_8_r_r(cycles, reg.H, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x65:
            LD_8_r_r(cycles, reg.H, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x66:
            LD_8_n_nn(cycles, reg.H, reg.HL.getData());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x67:
            LD_8_r_r(cycles, reg.H, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD H A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x68:
            LD_8_r_r(cycles, reg.L, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD L B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x69:
            LD_8_r_r(cycles, reg.L, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD L C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x6A:
            LD_8_r_r(cycles, reg.L, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD L D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x6B:
            LD_8_r_r(cycles, reg.L, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD L E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x6C:
            LD_8_r_r(cycles, reg.L, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD L H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x6D:
            LD_8_r_r(cycles, reg.L, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD L L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x6E:
            LD_8_n_nn(cycles, reg.L, reg.HL.getData());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD L (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x6F:
            LD_8_r_r(cycles, reg.L, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD L A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x70:
            LD_8_r_r(cycles, reg.HL.getData(), reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL) B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x71:
            LD_8_r_r(cycles, reg.HL.getData(), reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL) C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x72:
            LD_8_r_r(cycles, reg.HL.getData(), reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL) D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x73:
            LD_8_r_r(cycles, reg.HL.getData(), reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL) E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x74:
            LD_8_r_r(cycles, reg.HL.getData(), reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL) H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x75:
            LD_8_r_r(cycles, reg.HL.getData(), reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (HL) L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x76:
            //TODO do this correctly
            // LD_8_n_nn(cycles, reg.HL.getData(), reg.HL.getData());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "HALT " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x77:
            LD_8_r_r(cycles, reg.HL.getData(), reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD  (HL )A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x78:
            LD_8_r_r(cycles, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x79:
            LD_8_r_r(cycles, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x7A:
            LD_8_r_r(cycles, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x7B:
            LD_8_r_r(cycles, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x7C:
            LD_8_r_r(cycles, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x7D:
            LD_8_r_r(cycles, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x7E:
            LD_8_n_nn(cycles, reg.A, reg.HL.getData());
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x7F:
            LD_8_r_r(cycles, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x80:
            add_8(cycles, reg.F, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x81:
            add_8(cycles, reg.F, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x82:
            add_8(cycles, reg.F, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x83:
            add_8(cycles, reg.F, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x84:
            add_8(cycles, reg.F, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x85:
            add_8(cycles, reg.F, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x86:
            add_8(cycles, reg.F, reg.A, reg.HL);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x87:
            add_8(cycles, reg.F, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x88:
            adc(cycles, reg.F, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x89:
            adc(cycles, reg.F, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x8A:
            adc(cycles, reg.F, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x8B:
            adc(cycles, reg.F, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x8C:
            adc(cycles, reg.F, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x8D:
            adc(cycles, reg.F, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x8E:
            adc(cycles, reg.F, reg.A, reg.HL);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x8F:
            adc(cycles, reg.F, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x90:
            sub_8(cycles, reg.F, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x91:
            sub_8(cycles, reg.F, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x92:
            sub_8(cycles, reg.F, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x93:
            sub_8(cycles, reg.F, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x94:
            sub_8(cycles, reg.F, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x95:
            sub_8(cycles, reg.F, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x96:
            sub_8(cycles, reg.F, reg.A, reg.HL);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x97:
            sub_8(cycles, reg.F, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x98:
            subc(cycles, reg.F, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x99:
            subc(cycles, reg.F, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x9A:
            subc(cycles, reg.F, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x9B:
            subc(cycles, reg.F, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x9C:
            subc(cycles, reg.F, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x9D:
            subc(cycles, reg.F, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x9E:
            subc(cycles, reg.F, reg.A, reg.HL);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0x9F:
            subc(cycles, reg.F, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA0:
            and_8(cycles, reg.F, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA1:
            and_8(cycles, reg.F, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA2:
            and_8(cycles, reg.F, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA3:
            and_8(cycles, reg.F, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA4:
            and_8(cycles, reg.F, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA5:
            and_8(cycles, reg.F, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA6:
            and_8(cycles, reg.F, reg.A, reg.HL);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA7:
            and_8(cycles, reg.F, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA8:
            xor_8(cycles, reg.F, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xA9:
            xor_8(cycles, reg.F, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xAA:
            xor_8(cycles, reg.F, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xAB:
            xor_8(cycles, reg.F, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xAC:
            xor_8(cycles, reg.F, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xAD:
            xor_8(cycles, reg.F, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xAE:
            xor_8(cycles, reg.F, reg.A, reg.HL);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xAF:
            xor_8(cycles, reg.F, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB0:
            or_8(cycles, reg.F, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB1:
            or_8(cycles, reg.F, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "OR A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB2:
            or_8(cycles, reg.F, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "OR A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB3:
            or_8(cycles, reg.F, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "OR A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB4:
            or_8(cycles, reg.F, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "OR A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB5:
            or_8(cycles, reg.F, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "OR A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB6:
            or_8(cycles, reg.F, reg.A, reg.HL);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "OR A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB7:
            or_8(cycles, reg.F, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "OR A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB8:
            cp_8(cycles, reg.F, reg.A, reg.B);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xB9:
            cp_8(cycles, reg.F, reg.A, reg.C);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xBA:
            cp_8(cycles, reg.F, reg.A, reg.D);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xBB:
            cp_8(cycles, reg.F, reg.A, reg.E);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xBC:
            cp_8(cycles, reg.F, reg.A, reg.H);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xBD:
            cp_8(cycles, reg.F, reg.A, reg.L);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xBE:
            cp_8(cycles, reg.F, reg.A, reg.HL);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xBF:
            cp_8(cycles, reg.F, reg.A, reg.A);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xC0:
            cycles = 8;
            if ((reg.F.getData() & FLAG_Z) == 0x00) {
                cycles+=4;
                uint16_t tmpAddr =  0;
                pop(cycles, tmpAddr, reg.SP) ;
                reg.PC.setData_(tmpAddr);
#ifdef DEBUG_OPCODE 
            std::cout << "RET NZ " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "RET NZ " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            }
            break;
        case 0xC1:
            reg.PC.inc(1);
            pop(cycles, reg.BC, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "POP BC " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xC2:
            cycles = 12;
            if ((reg.F.getData() & FLAG_Z) == 0x00) {
                cycles+=4;
                uint16_t tmpAddr = reg.PC.getData_();
                reg.PC.inc(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "JR NZ " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(3);
#ifdef DEBUG_OPCODE 
            std::cout << "JR NZ " << std::hex << "; " << reg.PC.getData_()-2 << std::endl;
#endif
            }
            break;
        case 0xC3:
            lastPC = reg.PC.getData_();
            reg.PC.inc(read_word((reg.PC.getData_()+1)));
            cycles += 16;
#ifdef DEBUG_OPCODE 
            std::cout << "JP " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xC4:
            cycles = 12;
            if ((reg.F.getData() & FLAG_Z) == 0x00) {
                cycles+=12;
                uint16_t tmpAddr = reg.PC.getData_();
                push(cycles, tmpAddr+3, reg.SP);
                reg.PC.inc(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "CALL NZ " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(3);
#ifdef DEBUG_OPCODE 
            std::cout << "CALL NZ " << std::hex << "; " << reg.PC.getData_()-3 << std::endl;
#endif
            }
            break;
        case 0xC5:
            reg.PC.inc(1);
            push(cycles, reg.BC, reg.SP);
            std::cout << std::hex << "BC: " << reg.BC.getData()<< " SP: "  << reg.SP.getData() << std::endl;
#ifdef DEBUG_OPCODE 
            std::cout << "PUSH BC " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xC6:
            reg.PC.inc(1);
            add_8(cycles, reg.F, reg.A, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD A " << std::hex << D8 << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xC7:
            lastPC = reg.PC.getData_();
            rst(cycles, reg.SP, reg.PC, 0);
#ifdef DEBUG_OPCODE 
            std::cout << "RST 00H " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xC8:
            cycles = 8;
            if (reg.F.getData() & FLAG_Z) {
                cycles+=4;
                uint16_t tmpAddr =  0;
                pop(cycles, tmpAddr, reg.SP) ;
                reg.PC.setData_(tmpAddr);
#ifdef DEBUG_OPCODE 
            std::cout << "RET Z " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "RET Z " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            }
            break;
        case 0xC9:
            lastPC = reg.PC.getData_();
            pop(cycles, reg.PC, reg.SP);
            cycles += 16;
#ifdef DEBUG_OPCODE 
            std::cout << "RET " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xCA:
            cycles = 12;
            if (reg.F.getData() & FLAG_Z) {
                cycles+=4;
                uint16_t tmpAddr = reg.PC.getData_();
                reg.PC.inc(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "JR Z " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(3);
#ifdef DEBUG_OPCODE 
            std::cout << "JR Z " << std::hex << "; " << reg.PC.getData_()-2 << std::endl;
#endif
            }
            break;
        case 0xCC:
            cycles = 12;
            if (reg.F.getData() & FLAG_Z) {
                cycles+=12;
                uint16_t tmpAddr = reg.PC.getData_();
                push(cycles, tmpAddr+3, reg.SP);
                reg.PC.inc(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "CALL Z " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(3);
#ifdef DEBUG_OPCODE 
            std::cout << "CALL Z " << std::hex << "; " << reg.PC.getData_()-3 << std::endl;
#endif
            }
            break;
        case 0xCD:
            cycles = 12;
            cycles+=12;
            tmpAddr = reg.PC.getData_();
            push(cycles, tmpAddr+3, reg.SP);
            reg.PC.setData_(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "CALL " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            break;
        case 0xCE:
            adc(cycles, reg.F, reg.A, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADC A "<< D8 << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xCF:
            lastPC = reg.PC.getData_();
            rst(cycles, reg.SP, reg.PC, 0x08);
#ifdef DEBUG_OPCODE 
            std::cout << "RST 08H " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xD0:
            cycles = 8;
            if ((reg.F.getData() & FLAG_C) == 0x00) {
                cycles+=4;
                uint16_t tmpAddr =  0;
                pop(cycles, tmpAddr, reg.SP) ;
                reg.PC.setData_(tmpAddr);
#ifdef DEBUG_OPCODE 
            std::cout << "RET NC " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "RET NC " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            }
            break;
        case 0xD1:
            reg.PC.inc(1);
            pop(cycles, reg.DE, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "POP DE " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xD2:
            cycles = 12;
            if ((reg.F.getData() & FLAG_Z) == 0x00) {
                cycles+=4;
                uint16_t tmpAddr = reg.PC.getData_();
                reg.PC.inc(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "JR NC " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(3);
#ifdef DEBUG_OPCODE 
            std::cout << "JR NZ " << std::hex << "; " << reg.PC.getData_()-2 << std::endl;
#endif
            }
            break;
        case 0xD4:
            cycles = 12;
            if ((reg.F.getData() & FLAG_C) == 0x00) {
                cycles+=12;
                uint16_t tmpAddr = reg.PC.getData_();
                push(cycles, tmpAddr+3, reg.SP);
                reg.PC.setData_(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "CALL NC " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(3);
#ifdef DEBUG_OPCODE 
                std::cout << "CALL NC " << std::hex << "; " << reg.PC.getData_()-3 << std::endl;
#endif
            }
            break;
        case 0xD5:
            reg.PC.inc(1);
            push(cycles, reg.DE, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "PUSH DE " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xD6:
            reg.PC.inc(1);
            sub_8(cycles, reg.F, reg.A, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SUB A " << std::hex << D8 << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xD7:
            lastPC = reg.PC.getData_();
            rst(cycles, reg.SP, reg.PC, 0x0010);
#ifdef DEBUG_OPCODE 
            std::cout << "RST 10H " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xD8:
            cycles = 8;
            if (reg.F.getData() & FLAG_C) {
                cycles+=4;
                uint16_t tmpAddr =  0;
                pop(cycles, tmpAddr, reg.SP) ;
                reg.PC.setData_(tmpAddr);
#ifdef DEBUG_OPCODE 
            std::cout << "RET Z " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "RET Z " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            }
            break;
        case 0xDA:
            cycles = 12;
            if (reg.F.getData() & FLAG_C) {
                cycles+=4;
                uint16_t tmpAddr = reg.PC.getData_();
                reg.PC.inc(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "JR C " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(3);
#ifdef DEBUG_OPCODE 
            std::cout << "JR C " << std::hex << "; " << reg.PC.getData_()-2 << std::endl;
#endif
            }
            break;
        case 0xDC:
            cycles = 12;
            if (reg.F.getData() & FLAG_C) {
                cycles+=12;
                uint16_t tmpAddr = reg.PC.getData_();
                push(cycles, tmpAddr+3, reg.SP);
                reg.PC.setData_(read_word(tmpAddr + 1));
#ifdef DEBUG_OPCODE 
            std::cout << "CALL C " << std::hex << reg.PC.getData_()  << "; " << tmpAddr << std::endl;
#endif
            } else {
                reg.PC.inc(3);
#ifdef DEBUG_OPCODE 
            std::cout << "CALL C " << std::hex << "; " << reg.PC.getData_()-3 << std::endl;
#endif
            }
            break;
        case 0xDE:
            subc(cycles, reg.F, reg.A, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "SBC A "<< D8 << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xDF:
            lastPC = reg.PC.getData_();
            rst(cycles, reg.SP, reg.PC, 0x18);
#ifdef DEBUG_OPCODE 
            std::cout << "RST 18H " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xE0:
            reg.PC.inc(1);
            LD_8_r_r(cycles, 0xFF00 + read_byte(reg.PC.getData_()), reg.A);
            reg.PC.inc(1);
            cycles += 4;
#ifdef DEBUG_OPCODE 
            std::cout << "LD ($FF00 + " << std::hex << D8  << ") A ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0xE1:
            reg.PC.inc(1);
            pop(cycles, reg.HL, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "POP HL " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xE2:
            reg.PC.inc(1);
            LD_8_r_r(cycles, 0xFF00 + reg.C.getData(), reg.A);
#ifdef DEBUG_OPCODE 
            std::cout << "LD ($FF00 + C) A:" << std::hex << reg.PC.getData_()-1 << std::endl;
#endif
        break;
        case 0xE5:
            reg.PC.inc(1);
            push(cycles, reg.HL, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "PUSH HL " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xE6:
            reg.PC.inc(1);
            and_8(cycles, reg.F, reg.A, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "AND A " << std::hex << D8  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xE7:
            lastPC = reg.PC.getData_();
            rst(cycles, reg.SP, reg.PC, 0x0020);
#ifdef DEBUG_OPCODE 
            std::cout << "RST 20H " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xE8:
            reg.PC.inc(1);
            add_sp(cycles, reg.F, reg.SP, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "ADD SP " << std::hex << D8  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xE9:
            lastPC = reg.PC.getData_();
            reg.PC.setData_(reg.HL.getData());
            cycles += 4;
#ifdef DEBUG_OPCODE 
            std::cout << "JP (HL) " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xEA:
            reg.PC.inc(1);
            write_byte(read_word(reg.PC.getData_()),reg.A.getData());
            //LD_8_r_r(cycles, reg.PC.getData_() ,reg.A);
            reg.PC.inc(2);
#ifdef DEBUG_OPCODE 
            std::cout << "LD (a16) A" << std::hex << " ; " << reg.PC.getData_()-3 << std::endl;
#endif
            break;
        case 0xEE:
            xor_8(cycles, reg.F, reg.A, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "XOR A "<< D8 << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xEF:
            lastPC = reg.PC.getData_();
            rst(cycles, reg.SP, reg.PC, 0x28);
#ifdef DEBUG_OPCODE 
            std::cout << "RST 28H " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xF0:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.A ,0xFF00 + read_byte(reg.PC.getData_()));
            reg.PC.inc(1);
            cycles += 4;
#ifdef DEBUG_OPCODE 
            std::cout << "LD A ($FF00 + " << std::hex << D8  << ") ; " << reg.PC.getData_()-2 << std::endl;
#endif
        break;
        case 0xF1:
            reg.PC.inc(1);
            pop(cycles, reg.AF, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "POP AF " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xF2:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.A,0xFF00 + reg.C.getData());
#ifdef DEBUG_OPCODE 
            std::cout << "LD A ($FF00 + " << std::hex << D8  << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
        break;
        case 0xF5:
            reg.PC.inc(1);
            push(cycles, reg.AF, reg.SP);
#ifdef DEBUG_OPCODE 
            std::cout << "PUSH AF " << std::hex << "; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xF6:
            reg.PC.inc(1);
            or_8(cycles, reg.F, reg.A, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "OR A " << std::hex << D8  << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xF7:
            lastPC = reg.PC.getData_();
            rst(cycles, reg.SP, reg.PC, 0x0030);
#ifdef DEBUG_OPCODE 
            std::cout << "RST 30H " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xF8:
            reg.PC.inc(1);
            tmpSP = reg.SP; 
            tmpCycles = 0;
            add_sp(tmpCycles, reg.F, tmpSP, reg.PC);
            LD_16_r_r(cycles, reg.HL, tmpSP);
            cycles += 4;
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "LD SP " << std::hex << read_word(reg.PC.getData_()) << "; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0xF9:
            reg.PC.inc(1);
            LD_16_r_r(cycles,reg.SP,reg.HL);
#ifdef DEBUG_OPCODE 
            std::cout << "LD SP HL" << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xFA:
            reg.PC.inc(1);
            LD_8_n_nn(cycles, reg.A, read_word(reg.PC.getData_()));
            cycles += 8;
#ifdef DEBUG_OPCODE 
            std::cout << "LD reg.A (a16) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
            break;
        case 0xFE:
            reg.PC.inc(1);
            cp_8(cycles, reg.F, reg.A, reg.PC);
            reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
            std::cout << "CP A "<< D8 << std::hex << " ; " << reg.PC.getData_()-2 << std::endl;
#endif
            break;
        case 0xFF:
            lastPC = reg.PC.getData_();
            rst(cycles, reg.SP, reg.PC, 0x38);
#ifdef DEBUG_OPCODE 
            std::cout << "RST 38H " << std::hex << reg.PC.getData_() << " ; " << lastPC << std::endl;
#endif
            break;
        case 0xCB:
            reg.PC.inc(1);
            cycles += 4;
            uint16_t tmpOp = read_byte(reg.PC.getData_());
            #ifdef DEBUG_OPCODE
                std::cout << "OPCODE CB ; " << std::hex << tmpOp << std::endl;
            #endif
            switch(read_byte(reg.PC.getData_())) {
                case 0x00:
                    RLC(cycles, reg.F, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RLC B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x01:
                    RLC(cycles, reg.F, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RLC C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x02:
                    RLC(cycles, reg.F, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RLC D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x03:
                    RLC(cycles, reg.F, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RLC E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x04:
                    RLC(cycles, reg.F, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RLC H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x05:
                    RLC(cycles, reg.F, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RLC L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x06:
                    RLC(cycles, reg.F, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RLC (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x07:
                    RLC(cycles, reg.F, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RLC A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x08:
                    RRC(cycles, reg.F, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RRC B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x09:
                    RRC(cycles, reg.F, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RRC C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x0A:
                    RRC(cycles, reg.F, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RRC D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x0B:
                    RRC(cycles, reg.F, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RRC E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x0C:
                    RRC(cycles, reg.F, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RRC H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x0D:
                    RRC(cycles, reg.F, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RRC L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x0E:
                    RRC(cycles, reg.F, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RRC (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x0F:
                    RRC(cycles, reg.F, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "RRC A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x10:
                    RL(cycles, reg.F, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RL B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x11:
                    RL(cycles, reg.F, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RL C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x12:
                    RL(cycles, reg.F, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RL D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x13:
                    RL(cycles, reg.F, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RL E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x14:
                    RL(cycles, reg.F, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RL H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x15:
                    RL(cycles, reg.F, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RL L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x16:
                    RL(cycles, reg.F, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RL (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x17:
                    RL(cycles, reg.F, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RL A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x18:
                    RR(cycles, reg.F, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RR B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x19:
                    RR(cycles, reg.F, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RR C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x1A:
                    RR(cycles, reg.F, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RR D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x1B:
                    RR(cycles, reg.F, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RR E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x1C:
                    RR(cycles, reg.F, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RR H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x1D:
                    RR(cycles, reg.F, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RR L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x1E:
                    RR(cycles, reg.F, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RR (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x1F:
                    RR(cycles, reg.F, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "RR A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x20:
                    SLA(cycles, reg.F, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SLA B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x21:
                    SLA(cycles, reg.F, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SLA C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x22:
                    SLA(cycles, reg.F, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SLA D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x23:
                    SLA(cycles, reg.F, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SLA E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x24:
                    SLA(cycles, reg.F, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SLA H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x25:
                    SLA(cycles, reg.F, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SLA L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x26:
                    SLA(cycles, reg.F, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SLA (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x27:
                    SLA(cycles, reg.F, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SLA A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x28:
                    SRA(cycles, reg.F, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SRA B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x29:
                    SRA(cycles, reg.F, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SRA C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x2A:
                    SRA(cycles, reg.F, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SRA D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x2B:
                    SRA(cycles, reg.F, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SRA E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x2C:
                    SRA(cycles, reg.F, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SRA H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x2D:
                    SRA(cycles, reg.F, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SRA L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x2E:
                    SRA(cycles, reg.F, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SRA (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x2F:
                    SRA(cycles, reg.F, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE
                    std::cout << "SRA A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x30:
                    SWAP(cycles, reg.F, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SLA B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x31:
                    SWAP(cycles, reg.F, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SLA C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x32:
                    SWAP(cycles, reg.F, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SWAP D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x33:
                    SWAP(cycles, reg.F, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SWAP E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x34:
                    SWAP(cycles, reg.F, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SWAP H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x35:
                    SWAP(cycles, reg.F, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SWAP L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x36:
                    SWAP(cycles, reg.F, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SWAP (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x37:
                    SWAP(cycles, reg.F, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SWAP A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x38:
                    SWAP(cycles, reg.F, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SWAP B " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x39:
                    SRL(cycles, reg.F, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SRA C " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x3A:
                    SRL(cycles, reg.F, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SRL D " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x3B:
                    SRL(cycles, reg.F, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SRA E " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x3C:
                    SRL(cycles, reg.F, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SRA H " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x3D:
                    SRL(cycles, reg.F, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SRL L " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x3E:
                    SRL(cycles, reg.F, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SRL (HL) " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x3F:
                    SRL(cycles, reg.F, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SRL A " << std::hex << " ; " << reg.PC.getData_()-1 << std::endl;
#endif
                    break;
                case 0x40:
                    bit_b_r(cycles, reg.F, 0, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SLA B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x41:
                    bit_b_r(cycles, reg.F, 0, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 0 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x42:
                    bit_b_r(cycles, reg.F, 0, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 0 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x43:
                    bit_b_r(cycles, reg.F, 0, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 0 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x44:
                    bit_b_r(cycles, reg.F, 0, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 0 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x45:
                    bit_b_r(cycles, reg.F, 0, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 0 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x46:
                    bit_b_r(cycles, reg.F, 0, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 0 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x47:
                    bit_b_r(cycles, reg.F, 0, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 0 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0x48:
                    bit_b_r(cycles, reg.F, 1, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 1 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x49:
                    bit_b_r(cycles, reg.F, 1, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 1 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x4A:
                    bit_b_r(cycles, reg.F, 1, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 1 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x4B:
                    bit_b_r(cycles, reg.F, 1, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 1 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x4C:
                    bit_b_r(cycles, reg.F,1, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 1 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x4D:
                    bit_b_r(cycles, reg.F, 1, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 1 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x4E:
                    bit_b_r(cycles, reg.F, 1, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 1 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x4F:
                    bit_b_r(cycles, reg.F, 1, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 1 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x50:
                    bit_b_r(cycles, reg.F, 2, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SLA B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x51:
                    bit_b_r(cycles, reg.F, 2, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 2 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x52:
                    bit_b_r(cycles, reg.F, 2, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 2 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x53:
                    bit_b_r(cycles, reg.F, 2, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 2 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x54:
                    bit_b_r(cycles, reg.F, 2, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 2 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x55:
                    bit_b_r(cycles, reg.F, 2, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 2 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x56:
                    bit_b_r(cycles, reg.F, 2, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 2 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x57:
                    bit_b_r(cycles, reg.F, 2, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 2 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0x58:
                    bit_b_r(cycles, reg.F, 3, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 3 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x59:
                    bit_b_r(cycles, reg.F, 3, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 3 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x5A:
                    bit_b_r(cycles, reg.F,3, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 3 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x5B:
                    bit_b_r(cycles, reg.F, 3, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 3 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x5C:
                    bit_b_r(cycles, reg.F, 3, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 3 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x5D:
                    bit_b_r(cycles, reg.F, 3, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 3 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x5E:
                    bit_b_r(cycles, reg.F, 3, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 3 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x5F:
                    bit_b_r(cycles, reg.F, 3, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 3 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x60:
                    bit_b_r(cycles, reg.F, 4, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 4  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x61:
                    bit_b_r(cycles, reg.F, 4, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 4  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x62:
                    bit_b_r(cycles, reg.F, 4, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 4  D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x63:
                    bit_b_r(cycles, reg.F, 4, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 4  E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x64:
                    bit_b_r(cycles, reg.F, 4, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 4  H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x65:
                    bit_b_r(cycles, reg.F, 4, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 4  L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x66:
                    bit_b_r(cycles, reg.F, 4, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 4  (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x67:
                    bit_b_r(cycles, reg.F, 4, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 4  A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0x68:
                    bit_b_r(cycles, reg.F, 5, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 5 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x69:
                    bit_b_r(cycles, reg.F, 5, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 5 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x6A:
                    bit_b_r(cycles, reg.F, 5, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 5 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x6B:
                    bit_b_r(cycles, reg.F, 5, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 5 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x6C:
                    bit_b_r(cycles, reg.F, 5, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 5 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x6D:
                    bit_b_r(cycles, reg.F, 5, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 5 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x6E:
                    bit_b_r(cycles, reg.F, 5, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 5 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x6F:
                    bit_b_r(cycles, reg.F, 5, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 5 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x70:
                    bit_b_r(cycles, reg.F, 6, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 6  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x71:
                    bit_b_r(cycles, reg.F, 6, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 6  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x72:
                    bit_b_r(cycles, reg.F, 6, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 6  D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x73:
                    bit_b_r(cycles, reg.F, 6, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 6  E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x74:
                    bit_b_r(cycles, reg.F, 6, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 6  H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x75:
                    bit_b_r(cycles, reg.F, 6, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 6  L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x76:
                    bit_b_r(cycles, reg.F, 6, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 6  (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x77:
                    bit_b_r(cycles, reg.F, 6, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 6  A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0x78:
                    bit_b_r(cycles, reg.F, 7, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 7 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x79:
                    bit_b_r(cycles, reg.F, 7, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 7 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x7A:
                    bit_b_r(cycles, reg.F, 7, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 7 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x7B:
                    bit_b_r(cycles, reg.F, 7, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 7 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x7C:
                    bit_b_r(cycles, reg.F, 7, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 7 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x7D:
                    bit_b_r(cycles, reg.F, 7, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 7 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x7E:
                    bit_b_r(cycles, reg.F, 7, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 7 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x7F:
                    bit_b_r(cycles, reg.F, 7, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "bit_b_r 7 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x80:
                    res_b_r(cycles, reg.F, 0, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 0 " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x81:
                    res_b_r(cycles, reg.F, 0, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 0 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x82:
                    res_b_r(cycles, reg.F, 0, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 0 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x83:
                    res_b_r(cycles, reg.F, 0, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 0 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x84:
                    res_b_r(cycles, reg.F, 0, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 0 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x85:
                    res_b_r(cycles, reg.F, 0, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 0 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x86:
                    res_b_r(cycles, reg.F, 0, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 0 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x87:
                    res_b_r(cycles, reg.F, 0, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 0 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0x88:
                    res_b_r(cycles, reg.F, 1, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 1 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x89:
                    res_b_r(cycles, reg.F, 1, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 1 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x8A:
                    res_b_r(cycles, reg.F, 1, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 1 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x8B:
                    res_b_r(cycles, reg.F, 1, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 1 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x8C:
                    res_b_r(cycles, reg.F, 1, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 1 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x8D:
                    res_b_r(cycles, reg.F, 1, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 1 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x8E:
                    res_b_r(cycles, reg.F, 1, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 1 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x8F:
                    res_b_r(cycles, reg.F, 1, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 1 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x90:
                    res_b_r(cycles, reg.F, 2, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SLA B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x91:
                    res_b_r(cycles, reg.F, 2, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 2 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x92:
                    res_b_r(cycles, reg.F, 2, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 2 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x93:
                    res_b_r(cycles, reg.F, 2, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 2 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x94:
                    res_b_r(cycles, reg.F, 2, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 2 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x95:
                    res_b_r(cycles, reg.F, 2, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 2 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x96:
                    res_b_r(cycles, reg.F, 2, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 2 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x97:
                    res_b_r(cycles, reg.F, 2, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 2 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0x98:
                    res_b_r(cycles, reg.F, 3, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 3 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x99:
                    res_b_r(cycles, reg.F, 3, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 3 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x9A:
                    res_b_r(cycles, reg.F, 3, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 3 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x9B:
                    res_b_r(cycles, reg.F, 3, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 3 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x9C:
                    res_b_r(cycles, reg.F, 3, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 3 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x9D:
                    res_b_r(cycles, reg.F, 3, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 3 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x9E:
                    res_b_r(cycles, reg.F, 3, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 3 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0x9F:
                    res_b_r(cycles, reg.F, 3, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 3 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA0:
                    res_b_r(cycles, reg.F, 4, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 4  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA1:
                    res_b_r(cycles, reg.F, 4, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 4  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA2:
                    res_b_r(cycles, reg.F, 4, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 4  D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA3:
                    res_b_r(cycles, reg.F, 4, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 4  E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA4:
                    res_b_r(cycles, reg.F, 4, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 4  H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA5:
                    res_b_r(cycles, reg.F, 4, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 4  L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA6:
                    res_b_r(cycles, reg.F, 4, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 4  (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA7:
                    res_b_r(cycles, reg.F, 4, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 4  A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0xA8:
                    res_b_r(cycles, reg.F, 5, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 5 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xA9:
                    res_b_r(cycles, reg.F, 5, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 5 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xAA:
                    res_b_r(cycles, reg.F, 5, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 5 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xAB:
                    res_b_r(cycles, reg.F, 5, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 5 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xAC:
                    res_b_r(cycles, reg.F, 5, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 5 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xAD:
                    res_b_r(cycles, reg.F, 5, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 5 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xAE:
                    res_b_r(cycles, reg.F, 5, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 5 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xAF:
                    res_b_r(cycles, reg.F, 5, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 5 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB0:
                    res_b_r(cycles, reg.F, 6, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 6  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB1:
                    res_b_r(cycles, reg.F, 6, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 6  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB2:
                    res_b_r(cycles, reg.F, 6, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 6  D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB3:
                    res_b_r(cycles, reg.F, 6, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 6  E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB4:
                    res_b_r(cycles, reg.F, 6, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 6  H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB5:
                    res_b_r(cycles, reg.F, 6, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 6  L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB6:
                    res_b_r(cycles, reg.F, 6, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 6  (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB7:
                    res_b_r(cycles, reg.F, 6, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 6  A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0xB8:
                    res_b_r(cycles, reg.F, 7, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 7 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xB9:
                    res_b_r(cycles, reg.F, 7, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 7 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xBA:
                    res_b_r(cycles, reg.F, 7, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 7 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xBB:
                    res_b_r(cycles, reg.F, 7, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 7 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xBC:
                    res_b_r(cycles, reg.F, 7, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 7 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xBD:
                    res_b_r(cycles, reg.F, 7, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 7 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xBE:
                    res_b_r(cycles, reg.F, 7, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 7 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xBF:
                    res_b_r(cycles, reg.F, 7, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "res_b_r 7 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC0:
                    set_b_r(cycles, reg.F, 0, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 0 " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC1:
                    set_b_r(cycles, reg.F, 0, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 0 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC2:
                    set_b_r(cycles, reg.F, 0, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 0 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC3:
                    set_b_r(cycles, reg.F, 0, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 0 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC4:
                    set_b_r(cycles, reg.F, 0, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 0 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC5:
                    set_b_r(cycles, reg.F, 0, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 0 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC6:
                    set_b_r(cycles, reg.F, 0, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 0 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC7:
                    set_b_r(cycles, reg.F, 0, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 0 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0xC8:
                    set_b_r(cycles, reg.F, 1, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 1 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xC9:
                    set_b_r(cycles, reg.F, 1, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 1 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xCA:
                    set_b_r(cycles, reg.F, 1, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 1 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xCB:
                    set_b_r(cycles, reg.F, 1, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 1 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xCC:
                    set_b_r(cycles, reg.F, 1, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 1 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xCD:
                    set_b_r(cycles, reg.F, 1, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 1 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xCE:
                    set_b_r(cycles, reg.F, 1, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 1 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xCF:
                    set_b_r(cycles, reg.F, 1, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 1 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD0:
                    set_b_r(cycles, reg.F, 2, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "SLA B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD1:
                    set_b_r(cycles, reg.F, 2, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 2 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD2:
                    set_b_r(cycles, reg.F, 2, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 2 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD3:
                    set_b_r(cycles, reg.F, 2, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 2 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD4:
                    set_b_r(cycles, reg.F, 2, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 2 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD5:
                    set_b_r(cycles, reg.F, 2, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 2 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD6:
                    set_b_r(cycles, reg.F, 2, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 2 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD7:
                    set_b_r(cycles, reg.F, 2, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 2 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0xD8:
                    set_b_r(cycles, reg.F, 3, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 3 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xD9:
                    set_b_r(cycles, reg.F, 3, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 3 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xDA:
                    set_b_r(cycles, reg.F, 3, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 3 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xDB:
                    set_b_r(cycles, reg.F, 3, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 3 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xDC:
                    set_b_r(cycles, reg.F, 3, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 3 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xDD:
                    set_b_r(cycles, reg.F, 3, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 3 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xDE:
                    set_b_r(cycles, reg.F, 3, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 3 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xDF:
                    set_b_r(cycles, reg.F, 3, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 3 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE0:
                    set_b_r(cycles, reg.F, 4, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 4  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE1:
                    set_b_r(cycles, reg.F, 4, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 4  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE2:
                    set_b_r(cycles, reg.F, 4, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 4  D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE3:
                    set_b_r(cycles, reg.F, 4, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 4  E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE4:
                    set_b_r(cycles, reg.F, 4, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 4  H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE5:
                    set_b_r(cycles, reg.F, 4, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 4  L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE6:
                    set_b_r(cycles, reg.F, 4, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 4  (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE7:
                    set_b_r(cycles, reg.F, 4, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 4  A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0xE8:
                    set_b_r(cycles, reg.F, 5, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 5 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xE9:
                    set_b_r(cycles, reg.F, 5, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 5 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xEA:
                    set_b_r(cycles, reg.F, 5, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 5 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xEB:
                    set_b_r(cycles, reg.F, 5, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 5 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xEC:
                    set_b_r(cycles, reg.F, 5, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 5 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xED:
                    set_b_r(cycles, reg.F, 5, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 5 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xEE:
                    set_b_r(cycles, reg.F, 5, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 5 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xEF:
                    set_b_r(cycles, reg.F, 5, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 5 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF0:
                    set_b_r(cycles, reg.F, 6, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 6  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF1:
                    set_b_r(cycles, reg.F, 6, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 6  C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF2:
                    set_b_r(cycles, reg.F, 6, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 6  D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF3:
                    set_b_r(cycles, reg.F, 6, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 6  E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF4:
                    set_b_r(cycles, reg.F, 6, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 6  H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF5:
                    set_b_r(cycles, reg.F, 6, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 6  L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF6:
                    set_b_r(cycles, reg.F, 6, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 6  (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF7:
                    set_b_r(cycles, reg.F, 6, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 6  A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif      
                    break;
                case 0xF8:
                    set_b_r(cycles, reg.F, 7, reg.B);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 7 B " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xF9:
                    set_b_r(cycles, reg.F, 7, reg.C);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 7 C " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xFA:
                    set_b_r(cycles, reg.F, 7, reg.D);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 7 D " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xFB:
                    set_b_r(cycles, reg.F, 7, reg.E);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 7 E " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xFC:
                    set_b_r(cycles, reg.F, 7, reg.H);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 7 H " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xFD:
                    set_b_r(cycles, reg.F, 7, reg.L);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 7 L " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xFE:
                    set_b_r(cycles, reg.F, 7, reg.HL);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 7 (HL) " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
                case 0xFF:
                    set_b_r(cycles, reg.F, 7, reg.A);
                    reg.PC.inc(1);
#ifdef DEBUG_OPCODE 
                    std::cout << "set_b_r 7 A " << std::hex << " ; " << reg.PC.getData_() - 1 << std::endl;
#endif
                    break;
            }
            break;
            /*case 0x10: //TODO :Add halting of cpu and display 
            cycles = 4;
            regPC.inc(1);
            debug_print(std::string("STOP ;") + std::to_string(reg.PC.getData_()));
            break;*/
    }
    
    return 2;
}

void skip_boot() {
    reg.A.setData(0x1);
    reg.F.setData(0x00);
    reg.B.setData(0xFF);
    reg.C.setData(0x13);
    reg.E.setData(0xC1);
    reg.H.setData(0x84);
    reg.L.setData(0x03);
    reg.PC.setData_(0x100);
    reg.SP.setData_(0xFFFE);

    write_byte(0xFF00,0xCF);
    write_byte(0xFF01,0x00);
    write_byte(0xFF02,0x7E);
    write_byte(0xFF04,0x18);
    write_byte(0xFF05,0x00);
    write_byte(0xFF06,0x00);
    write_byte(0xFF07,0xF8);
    write_byte(0xFF0F,0xE1);
    write_byte(0xFF10,0x80);
    write_byte(0xFF11,0xBF);
    write_byte(0xFF12,0xF3);
    write_byte(0xFF13,0xFF);
    write_byte(0xFF14,0xBF);
    write_byte(0xFF16,0x3F);
    write_byte(0xFF17,0x00);
    write_byte(0xFF18,0xFF);
    write_byte(0xFF19,0xBF);
    write_byte(0xFF1A,0x7F);
    write_byte(0xFF1B,0xFF);
    write_byte(0xFF1C,0x9F);
    write_byte(0xFF1D,0xFF);
    write_byte(0xFF1E,0xBF);
    write_byte(0xFF20,0xFF);
    write_byte(0xFF21,0x00);
    write_byte(0xFF22,0x00);
    write_byte(0xFF23,0xBF);
    write_byte(0xFF24,0x77);
    write_byte(0xFF25,0xF3);
    write_byte(0xFF26,0xF1);
    write_byte(0xFF40,0x91);
    write_byte(0xFF41,0x81);
    write_byte(0xFF42,0x00);
    write_byte(0xFF43,0x00);
    write_byte(0xFF44,0x91);
    write_byte(0xFF45,0x00);
    write_byte(0xFF46,0xFF);
    write_byte(0xFF47,0xFC);
    write_byte(0xFF4A,0x00);
    write_byte(0xFF4B,0x00);
    write_byte(0xFF4D,0xFF);
    write_byte(0xFF4F,0xFF);
    write_byte(0xFF51,0xFF);
    write_byte(0xFF52,0xFF);
    write_byte(0xFF53,0xFF);
    write_byte(0xFF54,0xFF);
    write_byte(0xFF55,0xFF);
    write_byte(0xFF56,0xFF);
    write_byte(0xFF68,0xFF);
    write_byte(0xFF69,0xFF);
    write_byte(0xFF6A,0xFF);
    write_byte(0xFF6B,0xFF);
    write_byte(0xFF70,0xFF);
    write_byte(0xFFFF,0x00);
                                                                   
}
