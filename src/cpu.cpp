#include <cstdint> 
#include <unistd.h>
#include <iostream>
#include <bitset>
#include <assert.h>
#include <string.h>
#include "../inc/register.hpp"
#include "../inc/memory.hpp"
#include "../inc/instructions.hpp"
#include "../debug/debug.hpp"

#define BLARGH
#define DEBUG_OPCODE
 
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
void cpu_step() {
  //init_mem();

  //int a = 0x01;
  //int b = 0x02;
  //int c2 = 0x04;
  //int d = 0x08;    
  int cycles = 0;
  
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

  uint16_t opcode;;
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
    if (false &&ime == 1 && temp2) {
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
            std::cout << std::hex << "BC: " << reg.BC.getData()<< " SP: "  << reg.SP.getData() << std::endl;
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
int _eval_opcode(uint16_t opcode,int cycles) {
   //uint8_t temp;
    switch(opcode) {
        case 0x06:
            LD_8_n_nn(cycles,reg.B, reg.PC.getData_());
#ifdef DEBUG
            //std::cout << "LD B " << read_word(reg.PC.getData_())) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_pc" << std::endl; 
#endif                
            reg.PC.inc(1);
            break;
/*        case 0x0E:
            LD_8_n_nn(cycles,reg.C,reg.PC.getData_()));
#ifdef DEBUG
            std::cout << "LD C " << read_word(reg.PC.getData_())) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_pc" << std::endl; 
#endif                
            reg.PC.inc(1);
            break;
        case 0x16:
            LD_8_n_nn(cycles,reg.D,reg.PC.getData_()));
#ifdef DEBUG
            std::cout << "LD D " << read_word(reg.PC.getData_())) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_pc" << std::endl; 
#endif                
            reg.PC.inc(1);
            break;
        case 0x1E:
            LD_8_n_nn(cycles,reg.E,reg.PC.getData_()));
#ifdef DEBUG
            std::cout << "LD E " << read_word(reg.PC.getData_())) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_E_pc" << std::endl; 
#endif                
            reg.PC.inc(1);
            break;
        case 0x26:
            LD_8_n_nn(cycles,reg.H,reg.PC.getData_()));
#ifdef DEBUG
            std::cout << "LD H " << read_word(reg.PC.getData_())) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_pc" << std::endl; 
#endif                
            reg.PC.inc(1);
            break;
        case 0x2E:
            LD_8_n_nn(cycles,reg.L,reg.PC.getData_()));
#ifdef DEBUG
            std::cout << "LD L " << read_word(reg.PC.getData_())) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_pc" << std::endl; 
#endif                
            reg.PC.inc(1);
            break;
        case 0x7F:
            LD_8_r_r(cycles,reg.A,reg.AF.high);
#ifdef DEBUG
            std::cout << "LD A " << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_A" << std::endl; 
#endif                
            break;
        case 0x78:
            LD_8_r_r(cycles,reg.A,reg.B);
#ifdef DEBUG
            std::cout << "LD A " << "B" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_A" << std::endl; 
#endif                
            break;
        case 0x79:
            LD_8_r_r(cycles,reg.A,reg.C);
#ifdef DEBUG
            std::cout << "LD A " << "C" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_C" << std::endl; 
#endif                
            break;
        case 0x7A:
            LD_8_r_r(cycles,reg.A,reg.D);
#ifdef DEBUG
            std::cout << "LD A " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_D" << std::endl; 
#endif                
            break;
        case 0x7B:
            LD_8_r_r(cycles,reg.A,reg.E);
#ifdef DEBUG
            std::cout << "LD A " << "E" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_E" << std::endl; 
#endif                
            break;
        case 0x7C:
            LD_8_r_r(cycles,reg.A,reg.HL.high);
#ifdef DEBUG
            std::cout << "LD A " << "H" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_H" << std::endl; 
#endif                
            break;
        case 0x7D:
            LD_8_r_r(cycles,reg.A,reg.L);
#ifdef DEBUG
            std::cout << "LD A " << "L" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_L" << std::endl; 
#endif                
            break;
        case 0x7E:
            LD_8_n_nn(cycles,reg.A,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LD A " << std::hex << read_byte(reg.HL.getData())  << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_HL" << std::endl; 
#endif                
            break;
        case 0x40:
            LD_8_r_r(cycles,reg.B,reg.BC.high);
#ifdef DEBUG
            std::cout << "LD B " << "B" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_B" << std::endl; 
#endif                
            break;
        case 0x41:
            LD_8_r_r(cycles,reg.B,reg.C);
#ifdef DEBUG
            std::cout << "LD B " << "C" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_C" << std::endl; 
#endif                
            break;
        case 0x42:
            LD_8_r_r(cycles,reg.B,reg.D);
#ifdef DEBUG
            std::cout << "LD B " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_D" << std::endl; 
#endif                
            break;
        case 0x43:
            LD_8_r_r(cycles,reg.B,reg.E);
#ifdef DEBUG
            std::cout << "LD B " << "E" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_E" << std::endl; 
#endif                
            break;
        case 0x44:
            LD_8_r_r(cycles,reg.B,reg.HL.high);
#ifdef DEBUG
            std::cout << "LD B " << "H" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_H" << std::endl; 
#endif                
            break;
        case 0x45:
            LD_8_r_r(cycles,reg.B,reg.L);
#ifdef DEBUG
            std::cout << "LD B " << "L" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_L" << std::endl; 
#endif                
            break;
        case 0x46:
            //temp = read_byte(reg.HL.getData());
            //LD_8_n_nn(cycles,reg.B,temp);
            LD_8_n_nn(cycles,reg.B,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LD B " << std::hex << read_byte(reg.HL.getData())  << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_HL" << std::endl; 
#endif                
            break;
        case 0x48:
            LD_8_r_r(cycles,reg.C,reg.B);
#ifdef DEBUG
            std::cout << "LD C " << "B" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_B" << std::endl; 
#endif                
            break;
        case 0x49:
            LD_8_r_r(cycles,reg.C,reg.BC.low);
#ifdef DEBUG
            std::cout << "LD C " << "C" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_C" << std::endl; 
#endif                
            break;    
        case 0x4A:
            LD_8_r_r(cycles,reg.C,reg.D);
#ifdef DEBUG
            std::cout << "LD C " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_D" << std::endl; 
#endif                
            break;
        case 0x4B:
            LD_8_r_r(cycles,reg.C,reg.E);
#ifdef DEBUG
            std::cout << "LD C " << "E" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_E" << std::endl; 
#endif                
            break;
        case 0x4C:
            LD_8_r_r(cycles,reg.C,reg.HL.high);
#ifdef DEBUG
            std::cout << "LD C " << "H" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_H" << std::endl; 
#endif                
            break;
        case 0x4D:
            LD_8_r_r(cycles,reg.C,reg.L);
#ifdef DEBUG
            std::cout << "LD C " << "L" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_L" << std::endl; 
#endif                
            break;
        case 0x4E:
            LD_8_n_nn(cycles,reg.C,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LD C " << std::hex << read_byte(reg.HL.getData())  << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_HL" << std::endl; 
#endif                
            break;
        case 0x50:
            LD_8_r_r(cycles,reg.D,reg.B);
#ifdef DEBUG
            std::cout << "LD D " << "B" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_B" << std::endl; 
#endif                
            break;
        case 0x51:
            LD_8_r_r(cycles,reg.D,reg.C);
#ifdef DEBUG
            std::cout << "LD D " << "C" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_C" << std::endl; 
#endif                
            break;    
        case 0x52:
            LD_8_r_r(cycles,reg.D,reg.DE.high);
#ifdef DEBUG
            std::cout << "LD D " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_D" << std::endl; 
#endif                
            break;
        case 0x53:
            LD_8_r_r(cycles,reg.D,reg.E);
#ifdef DEBUG
            std::cout << "LD D " << "E" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_E" << std::endl; 
#endif                
            break;
        case 0x54:
            LD_8_r_r(cycles,reg.D,reg.HL.high);
#ifdef DEBUG
            std::cout << "LD D " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_H" << std::endl; 
#endif                
            break;
        case 0x55:
            LD_8_r_r(cycles,reg.D,reg.L);
#ifdef DEBUG
            std::cout << "LD D " << "L" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_L" << std::endl; 
#endif                
            break;
        case 0x56:
            LD_8_n_nn(cycles,reg.D,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LD D " << std::hex << read_byte(reg.HL.getData()) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_HL" << std::endl; 
#endif                
            break;
        case 0x58:
            LD_8_r_r(cycles,reg.E,reg.B);
#ifdef DEBUG
            std::cout << "LD E " << "B" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_E_B" << std::endl; 
#endif                
            break;
        case 0x59:
            LD_8_r_r(cycles,reg.E,reg.C);
#ifdef DEBUG
            std::cout << "LD E " << "C" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_E_C" << std::endl; 
#endif                
            break;    
        case 0x5A:
            LD_8_r_r(cycles,reg.E,reg.D);
#ifdef DEBUG
            std::cout << "LD E " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_E_D" << std::endl; 
#endif                
            break;
        case 0x5B:
            LD_8_r_r(cycles,reg.E,reg.DE.low);
#ifdef DEBUG
            std::cout << "LD E " << "E" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_E_E" << std::endl; 
#endif                
            break;
        case 0x5C:
            LD_8_r_r(cycles,reg.E,reg.HL.high);
#ifdef DEBUG
            std::cout << "LD E " << "H" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_E_H" << std::endl; 
#endif                
            break;
        case 0x5D:
            LD_8_n_nn(cycles,reg.E,reg.L);
#ifdef DEBUG
            std::cout << "LD E " << "L" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_L" << std::endl; 
#endif                
            break;
        case 0x5E:
            LD_8_n_nn(cycles,reg.E,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LD E " << std::hex << read_byte(reg.HL.getData()) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_E_HL" << std::endl; 
#endif                
            break;
        case 0x60:
            cycles = 4;
            LD_8_r_r(cycles,reg.H,reg.B);
#ifdef DEBUG
            std::cout << "LD H " << "B" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_B" << std::endl; 
#endif                
            break;
        case 0x61:
            LD_8_r_r(cycles,reg.H,reg.C);
#ifdef DEBUG
            std::cout << "LD H " << "C" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_C" << std::endl; 
#endif                
            break;    
        case 0x62:
            LD_8_r_r(cycles,reg.H,reg.D);
#ifdef DEBUG
            std::cout << "LD H " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_D" << std::endl; 
#endif                
            break;
        case 0x63:
            LD_8_r_r(cycles,reg.H,reg.E);
#ifdef DEBUG
            std::cout << "LD H " << "E" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_E" << std::endl; 
#endif                
            break;
        case 0x64:
            LD_8_r_r(cycles,reg.H,reg.HL.high);
#ifdef DEBUG
            std::cout << "LD H " << "H" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_H" << std::endl; 
#endif                
            break;
        case 0x65:
            LD_8_r_r(cycles,reg.H,reg.L);
#ifdef DEBUG
            std::cout << "LD H " << "L" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_L" << std::endl; 
#endif                
            break;
        case 0x66:
            LD_8_n_nn(cycles,reg.H,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LD H " << std::hex << read_byte(reg.HL.getData()) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_B" << std::endl; 
#endif                
            break;
        case 0x68:
            LD_8_r_r(cycles,reg.L,reg.B);
#ifdef DEBUG
            std::cout << "LD L " << "B" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_B" << std::endl; 
#endif                
            break;
        case 0x69:
            LD_8_r_r(cycles,reg.L,reg.C);
#ifdef DEBUG
            std::cout << "LD L " << "C" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_C" << std::endl; 
#endif                
            break;    
        case 0x6A:
            LD_8_r_r(cycles,reg.L,reg.D);
#ifdef DEBUG
            std::cout << "LD L " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_D" << std::endl; 
#endif                
            break;
        case 0x6B:
            LD_8_r_r(cycles,reg.L,reg.E);
#ifdef DEBUG
            std::cout << "LD L " << "E" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_E" << std::endl; 
#endif                
            break;
        case 0x6C:
            LD_8_r_r(cycles,reg.L,reg.HL.high);
            break;
#ifdef DEBUG
            std::cout << "LD L " << "H" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_H" << std::endl; 
#endif                
        case 0x6D:
            LD_8_r_r(cycles,reg.L,reg.HL.low);
#ifdef DEBUG
            std::cout << "LD L " << "L" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_L" << std::endl; 
#endif                
            break;
        case 0x6E:
            LD_8_n_nn(cycles,reg.L,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LD L " << std::hex << read_byte(reg.HL.getData()) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_B" << std::endl; 
#endif                
            break;
            //Redo these
        case 0x70:
            LD_8_r_r(cycles,reg.HL.getData(),reg.B);
#ifdef DEBUG
            std::cout << "LD (HL) " << "B" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_HL_B" << std::endl; 
#endif                
            break;
        case 0x71:
            LD_8_r_r(cycles,reg.HL.getData(),reg.C);
#ifdef DEBUG
            std::cout << "LD (HL) " << "C" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_HL_C" << std::endl; 
#endif                
            break;
        case 0x72:
            LD_8_r_r(cycles,reg.HL.getData(),reg.D);
#ifdef DEBUG
            std::cout << "LD (HL) " << "D" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_HL_D" << std::endl; 
#endif                
            break;
        case 0x73:
            LD_8_r_r(cycles,reg.HL.getData(),reg.E);
#ifdef DEBUG
            std::cout << "LD (HL) " << "E" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_HL_E" << std::endl; 
#endif                
            break;
        case 0x74:
            LD_8_r_r(cycles,reg.HL.getData(),reg.HL.high);
#ifdef DEBUG
            std::cout << "LD (HL) " << "H" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_HL_H" << std::endl; 
#endif                
            break;
        case 0x75:
            LD_8_r_r(cycles,reg.HL.getData(),reg.L);
#ifdef DEBUG
            std::cout << "LD (HL) " << "L" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_HL_L" << std::endl; 
#endif                
            break;
        case 0x36:
            LD_8_r_r(cycles,reg.HL.getData(),reg.PC);
#ifdef DEBUG
            std::cout << "LD (HL) " << std::hex << read_byte(reg.PC) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_HL_PC" << std::endl; 
#endif                
            reg.PC.inc(1);
            break;
        case 0x0A:
            LD_8_n_nn(cycles,reg.A,reg.BC.getData());
#ifdef DEBUG
            std::cout << "LD A " << std::hex << read_byte(reg.BC.getData()) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_BC" << std::endl; 
#endif                
            break;               
        case 0x1A:
            LD_8_n_nn(cycles,reg.A,reg.DE.getData());
#ifdef DEBUG
            std::cout << "LD A " << std::hex << read_byte(reg.DE.getData()) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_DE" << std::endl; 
#endif                
            break;               
        case 0xFA:
            cycles = 16;
            reg.AF.setHigh(read_byte(read_word(reg.PC)));
#ifdef DEBUG
            std::cout << "LD A " << std::hex << read_byte(read_word(reg.PC)) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_(pc)" << std::endl; 
#endif                
            reg.pc += 2;
            break;               
        case 0x3E:
            LD_8_n_nn(cycles,reg.A,reg.PC); 
#ifdef DEBUG
            std::cout << "LD A " << std::hex << read_byte(reg.PC) << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_pc" << std::endl; 
#endif                
            reg.PC.inc(1);
            break;
        case 0x47:
            LD_8_r_r(cycles,reg.B,reg.A);
#ifdef DEBUG
            std::cout << "LD B " << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_B_A" << std::endl; 
#endif                
            break;
        case 0x4F:
            LD_8_r_r(cycles,reg.C,reg.A);
#ifdef DEBUG
            std::cout << "LD C " << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_C_A" << std::endl; 
#endif                
            break;
        case 0x57:
            LD_8_r_r(cycles,reg.D,reg.A);
#ifdef DEBUG
            std::cout << "LD D " << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_D_A" << std::endl; 
#endif                
            break;
        case 0x5F:
            LD_8_r_r(cycles,reg.E,reg.A);
#ifdef DEBUG
            std::cout << "LD E " << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_E_A" << std::endl; 
#endif                
            break;
        case 0x67:
            LD_8_r_r(cycles,reg.H,reg.A);
#ifdef DEBUG
            std::cout << "LD H " << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_H_A" << std::endl; 
#endif                
            break;
        case 0x6F:
            LD_8_r_r(cycles,reg.L,reg.A);
#ifdef DEBUG
            std::cout << "LD L " << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_L_A" << std::endl; 
#endif                
            break; 
        case 0x02:
            LD_8_r_r(cycles,reg.BC.getData(),reg.A);
#ifdef DEBUG
            std::cout << "LD (BC)" << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_BC_A" << std::endl; 
#endif                
            break;
        case 0x12:
            LD_8_r_r(cycles,reg.DE.getData(),reg.A);
#ifdef DEBUG
            std::cout << "LD (DE)" << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_DE_A" << std::endl; 
#endif                
            break;
        case 0x77:
            LD_8_r_r(cycles,reg.HL.getData(),reg.A);
#ifdef DEBUG
            std::cout << "LD (HL)" << "A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_HL_A" << std::endl; 
#endif                
            break;
        case 0xEA:
            cycles = 16;
            write_byte(read_word(reg.PC),reg.A);
#ifdef DEBUG
            std::cout << "LD "<< std::hex << read_byte(reg.PC)  << " A" << "; " << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_pc_A" << std::endl; 
#endif                
            reg.pc += 2;
            break;
        case 0xF2:
            LD_8_n_nn(cycles,reg.A,(0xFF00 + reg.C));
#ifdef DEBUG
            std::cout << "LD A  (0xFF00 + C);" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_(0xFF00 + c)" << std::endl; 
#endif 
            break;
        case 0xE2:
            LD_8_r_r(cycles,(0xFF00+reg.C),reg.A);
#ifdef DEBUG
            std::cout << "LD A  (0xFF00 + C);" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_A_(0xFF00 + c)" << std::endl; 
#endif 
            break;
        case 0x3A:
            LDD_8(cycles,reg.A,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LDD A (HL-);" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LDD_8_A_HL" << std::endl; 
#endif 
            break;
        case 0x32:
            LDD_8(cycles,reg.HL.getData(),reg.A);
#ifdef DEBUG
            std::cout << "LDD (HL-) A;" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LDD_8_HL_A" << std::endl; 
#endif 
            break;
        case 0x2A:
            LDI_8(cycles,reg.A,reg.HL.getData());
#ifdef DEBUG
            std::cout << "LDD A (HL+);" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LDI_8_A_HL" << std::endl; 
#endif 
            break;
        case 0x22:
            LDI_8(cycles,reg.HL.getData(),reg.A);
#ifdef DEBUG
            std::cout << "LDD (HL+) A;" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LDI_8_HL_A" << std::endl; 
#endif 
            break;
        case 0xE0:
            LD_8_r_r(cycles,(0xFF00+read_byte(reg.PC)),reg.A);
#ifdef DEBUG
            std::cout << "LD A  (0xFF00 + (pc));" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_(0xFF00 + (pc))_A" << std::endl; 
#endif 
            reg.PC.inc(1);
            cycles = 12;
            break;
        case 0xF0:
            LD_8_r_r(cycles,reg.A,(0xFF00+read_byte(reg.PC)));
#ifdef DEBUG
            std::cout << "LD (0xFF00 + (pc)) A;" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_(0xFF00 + (pc))_A" << std::endl; 
#endif 
            reg.PC.inc(1);
            cycles = 12;
            break;
        case 0x01:
            LD_16_n_nn(cycles,reg.BC,reg.PC);
            reg.pc += 2;
            break;
        case 0x11:
            LD_16_n_nn(cycles,reg.DE,reg.PC);
            reg.pc += 2;
            break;

        case 0x21:
            LD_16_n_nn(cycles,reg.HL.getData(),reg.PC);
#ifdef DEBUG
            std::cout << "LD (HL) " << std::hex << read_word(reg.PC)  <<  ";" << std::hex << reg.pc-1 << ":" << "Cycles: " << std::dec << cycles << " LD_8_(HL)_(pc)" << std::endl; 
#endif 
            reg.pc += 2;
            break;
        case 0x31:
            LD_16_n_nn(cycles,reg.SP,reg.PC);
#ifdef DEBUG
            std::cout << "LD sp " << read_word(reg.PC) << "; " << reg.pc << " :" << "Cycles: " << std::dec  << cycles << " LD_16_sp_pc" << std::endl; 
#endif                
            reg.pc += 2;
            //interruptHandler(read_byte(IE));
            break;
        case 0xF9:
            LD_16_r_r(cycles,reg.SP,reg.HL.getData());
            break;
        case 0xF8:
            if(((reg.sp&0xf) + (reg.pc&0xf))&0x10) {
                reg.AF.getLow() |= h;
            }
            if((int) reg.sp + (int) reg.pc > 255) {
                reg.AF.getLow() |= c;
            }
            LD_16_n_nn(cycles,reg.HL,(reg.sp+reg.PC)); 
            reg.AF.getLow() &= ~z;
            reg.AF.getLow() &= ~n;
            reg.PC.inc(1);
            break;
        case 0x08:
            cycles = 20;
            write_word(reg.PC,reg.SP);
            reg.pc += 2;
            break;
        case 0xF5:
            push(cycles,reg.SP,reg.AF.getData());
            break;
        case 0xC5:
            push(cycles,reg.SP,reg.BC.getData());
            break;
        case 0xD5:
            push(cycles,reg.SP,reg.DE.getData());
            break;
        case 0xE5:
            push(cycles,reg.SP,reg.HL.getData());
            break; 
        case 0xF1:
            pop(cycles,reg.SP,reg.AF.getData());
            break;
        case 0xC1:
            pop(cycles,reg.SP,reg.BC.getData());
            break;
        case 0xD1:
            pop(cycles,reg.SP,reg.DE.getData());
        case 0xE1:
            pop(cycles,reg.SP,reg.HL.getData());
            break; 
        case 0x87:
            add_8(cycles,reg.F,reg.A,reg.AF.high);
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
            add_8(cycles,reg.F,reg.A,reg.HL.high);
            break;
        case 0x85:
            add_8(cycles,reg.F,reg.A,reg.L);
            break;
        case 0x86:
            add_8(cycles,reg.F,reg.A,reg.HL.getData());
            cycles = 8;
            break; 
        case 0xC6:
            add_8(cycles,reg.F,reg.A,reg.PC);
            reg.PC.inc(1);
            cycles = 8;
            break;
        case 0x8F:
            adc(cycles,reg.F,reg.A,reg.AF.high);
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
            adc(cycles,reg.F,reg.A,reg.HL.high);
            break;            
        case 0x8D:
            adc(cycles,reg.F,reg.A,reg.L);
            break;            
        case 0x8E:
            adc(cycles,reg.F,reg.A,reg.HL.getData());
            cycles = 8;
            break;            
        case 0xCE:
            adc(cycles,reg.F,reg.A,reg.PC);
            reg.PC.inc(1);
            cycles = 8;
            break;                
        case 0x97:
            sub_8(cycles,reg.F,reg.A,reg.AF.high);          
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
            sub_8(cycles,reg.F,reg.A,reg.HL.high);
            break;
        case 0x95:
            sub_8(cycles,reg.F,reg.A,reg.L);
            break;
        case 0x96:
            sub_8(cycles,reg.F,reg.A,reg.HL.getData());
            cycles = 8;
            break;
        case 0xD6:
            sub_8(cycles,reg.F,reg.A,reg.PC);
            reg.PC.inc(1);
            cycles = 8;
            break;
        case 0x9F:
            subc(cycles,reg.F,reg.A,reg.AF.high);          
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
            subc(cycles,reg.F,reg.A,reg.HL.high);
            break;
        case 0x9D:
            subc(cycles,reg.F,reg.A,reg.L);
            break;
        case 0x9E:
            subc(cycles,reg.F,reg.A,reg.HL.getData());
            cycles = 8;
            break;
        case 0xDE:
            subc(cycles,reg.F,reg.A,reg.PC);
            reg.PC.inc(1);
            cycles =8;
            break;
        case 0xA7:
            and_8(cycles,reg.F,reg.A,reg.AF.high);
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
            and_8(cycles,reg.F,reg.A,reg.HL.high);
            break; 
        case 0xA5:
            and_8(cycles,reg.F,reg.A,reg.L);
            break; 
        case 0xA6:
            temp = read_byte(reg.HL.getData());
            and_8(cycles,reg.F,reg.A,temp);
            cycles = 8;
            break; 
        case 0xE6:
            temp = read_byte(reg.PC);
            reg.PC.inc(1);
            and_8(cycles,reg.F,reg.A,temp);
            cycles = 8;
            break; 
        case 0xB7:
            or_8(cycles,reg.F,reg.A,reg.AF.high);
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
            or_8(cycles,reg.F,reg.A,reg.HL.high);
            break; 
        case 0xB5:
            or_8(cycles,reg.F,reg.A,reg.L);
            break; 
        case 0xB6:
            temp = read_byte(reg.HL.getData());
            or_8(cycles,reg.F,reg.A,temp);
            cycles = 8;
            break; 
        case 0xF6:
            temp = read_byte(reg.PC);
            reg.PC.inc(1);
            or_8(cycles,reg.F,reg.A,temp);
            cycles = 8;
            break; 
        case 0xAF:
            xor_8(cycles,reg.F,reg.A,reg.AF.high);
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
            xor_8(cycles,reg.F,reg.A,reg.HL.high);
            break; 
        case 0xAD:
            xor_8(cycles,reg.F,reg.A,reg.L);
            break; 
        case 0xAE:
            temp = read_byte(reg.HL.getData());
            xor_8(cycles,reg.F,reg.A,temp);
            cycles = 8;
            break; 
        case 0xEE:
            temp = read_byte(reg.PC);
            reg.PC.inc(1);
            xor_8(cycles,reg.F,reg.A,temp);
            cycles = 8;
            break; 
        case 0xBF:
            cp_8(cycles,reg.F,reg.A,reg.AF.high);
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
            cp_8(cycles,reg.F,reg.A,reg.HL.high);
            break;
        case 0xBD:
            cp_8(cycles,reg.F,reg.A,reg.L);
            break;
        case 0xBE:
            cp_8(cycles,reg.F,reg.A,reg.HL.getData());
            cycles = 8;
            break;
        case 0xFE:
            cp_8(cycles,reg.F,reg.A,reg.PC);
            reg.PC.inc(1);
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
            inc_8(cycles,reg.F,reg.HL.high);
            break;
        case 0x2C:
            inc_8(cycles,reg.F,reg.L);
            break;
        case 0x34:
            temp = read_byte(reg.HL.getData());
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
            dec_8(cycles,reg.F,reg.HL.high);
            break;
        case 0x2D:
            dec_8(cycles,reg.F,reg.L);
            break;
        case 0x35:
            temp = read_byte(reg.HL.getData());
            dec_8(cycles,reg.F,temp);
            cycles = 12;
            break;
        case 0x09:
            add_16(cycles,reg.F,reg.HL,reg.DE.getData());
            break;    
        case 0x19:
            add_16(cycles,reg.F,reg.HL,reg.HL.getData());
            break;    
        case 0x29:
            add_16(cycles,reg.F,reg.HL,reg.SP);
            break;    
        case 0x39:
            add_16(cycles,reg.F,reg.HL,reg.BC.getData());
            break;    
        case 0xE8:
            add_sp(cycles,reg.F,reg.SP,read_byte(reg.PC));
            reg.pc += 2;
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
            inc_16(cycles,reg.SP);
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
            dec_16(cycles,reg.SP);
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
            //reg.pc;
            break;
        case 0x76://HALT
            cycles = 4;
            break;
        case 0x10://STOP 0x10
            uint16_t x = (0x10 << 8) + reg.PC.inc(1); //TODO: This case should perhaps still be commented away??!?!?!
              if (reg.PC.inc(1) == 0x1000) {
              cycles = 4;
              reg.PC.inc(1);
              }
            break;
        case 0xF3://DI disable interrupts
            cycles = 4;
            write_byte(IF_R,0); 
            break;
        case 0xFB://EI enable interrupts
            cycles = 4;
            write_byte(IE_R,0b00011111);
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
            reg.pc = read_word(reg.PC);
            cycles = 12;
            break; 
        case 0xC2:
            cycles = 12;
            if (reg.AF.getHigh() & ~0x80) 
                reg.pc = read_word(reg.PC);
            else
                reg.PC.inc(2);
            break; 
        case 0xCA:
            cycles = 12;
            if (reg.AF.getHigh() & 0x80) 
                reg.pc = read_word(reg.PC);
            else 
                reg.PC.inc(2);
            break; 
        case 0xD2:
            cycles = 12;
            if (reg.AF.getHigh() & ~0x20) 
                reg.pc = read_word(reg.PC);
            else
                reg.PC.inc(2);
            break; 
        case 0xDA:
            cycles = 12;
            if (reg.AF.getHigh() & 0x20) 
                reg.pc = read_word(reg.PC);
            else
                reg.PC.inc(2);
            break; 
        case 0xE9:
            cycles = 4;
            reg.pc = read_word(reg.HL.getData());
            break;
        case 0x18:
            cycles = 8;
            reg.pc = reg.pc + read_byte(reg.PC); 
            break;
        case 0x20:
            cycles = 8;
            if (reg.AF.getHigh() & ~0x80) 
                reg.pc = reg.pc + read_byte(reg.PC);
            else
                reg.PC.inc(1);
            break; 
        case 0x28:
            cycles = 8;
            if (reg.AF.getHigh() & 0x80) 
                reg.pc = reg.pc + read_byte(reg.PC);
            else
                reg.PC.inc(1);
            break; 
        case 0x30:
            cycles = 8;
            if (reg.AF.getHigh() & ~0x20) 
                reg.pc = reg.pc + read_byte(reg.PC);
            else 
                reg.PC.inc(1);
            break; 
        case 0x38:
            cycles = 8;
            if (reg.AF.getHigh() & 0x20) 
                reg.pc = reg.pc + read_byte(reg.PC);
            else
                reg.PC.inc(1);
            break;
        case 0xCD:
            cycles = 12;
            push(cycles,reg.SP,reg.PC);
            reg.pc = read_word(reg.PC);
            break;
        case 0xC4:
            cycles = 12;
            if (reg.AF.getHigh() & ~0x80) { 
                push(cycles,reg.SP,reg.PC);
                reg.pc = reg.pc + read_word(reg.PC);
            } else {
                reg.PC.inc(2);
            }
            break; 
        case 0xCC:
            cycles = 12;
            if (reg.AF.getHigh() & 0x80) { 
                push(cycles,reg.SP,reg.PC);
                reg.pc = reg.pc + read_word(reg.PC);
            } else {
                reg.PC.inc(2);
            }
            break; 
        case 0xD4:
            cycles = 12;
            if (reg.AF.getHigh() & ~0x20) {
                push(cycles,reg.SP,reg.PC); 
                reg.pc = reg.pc + read_word(reg.PC);
            } else {
                reg.PC.inc(2);
            }       
            break; 
        case 0xDC:
            cycles = 12;
            if (reg.AF.getHigh() & 0x20) { 
                push(cycles,reg.SP,reg.PC);
                reg.pc = reg.pc + read_word(reg.PC);
            } else {
                reg.PC.inc(2);
            }
            break;
        case 0xC7:
            rst(cycles,reg.SP,reg.PC,0x00);
            break;
        case 0xCF:
            rst(cycles,reg.SP,reg.PC,0x08);
            break;
        case 0xD7:
            rst(cycles,reg.SP,reg.PC,0x10);
            break;
        case 0xDF:
            rst(cycles,reg.SP,reg.PC,0x18);
            break;
        case 0xE7:
            rst(cycles,reg.SP,reg.PC,0x20);
            break;
        case 0xEF:
            rst(cycles,reg.SP,reg.PC,0x28);
            break;
        case 0xF7:
            rst(cycles,reg.SP,reg.PC,0x30);
            break;
        case 0xFF:
            rst(cycles,reg.SP,reg.PC,0x38);
            break;
        case 0xC9:
            pop(cycles,reg.AF,reg.SP);
            cycles += 8;
            reg.pc = read_word(reg.AF.getData());
            break;
        case 0xC0:
            if (reg.AF.getHigh() & ~0x80) {
                pop(cycles,reg.AF,reg.SP);
                reg.pc = read_word(reg.AF.getData());
            } else {
                reg.PC.inc(1);
            }

            cycles += 8;
            break; 
        case 0xC8:
            if (reg.AF.getHigh() & 0x80) {
                pop(cycles,reg.AF,reg.SP);
                reg.pc = read_word(reg.AF.getData());

            } else {
                reg.PC.inc(1);
            }

            cycles += 8;
            break; 

        case 0xD0:
            if (reg.AF.getHigh() & ~0x20) {
                pop(cycles,reg.AF,reg.SP);
                reg.pc = read_word(reg.AF.getData());

            } else {
                reg.PC.inc(1);
            }

            cycles += 8;
            break; 
        case 0xD8:
            if (reg.AF.getHigh() & 0x20) {
                pop(cycles,reg.AF,reg.SP);
                reg.pc = read_word(reg.AF.getData());

            } else {
                reg.PC.inc(1);
            }

            cycles += 8;
            break;
        case 0xD9:
            pop(cycles,reg.AF,reg.SP);
            cycles += 8;
            reg.pc = read_word(reg.AF.getData());
            //Enable interrupt
            write_word(IE_R,0b00011111); 
            cycles += 8; 
            break;
        case 0xCB:
            uint32_t muxedOpCode = (uint32_t) opcode << 16;
            uint16_t opcode2 = read_byte(reg.PC.inc(1));
            //muxedOpCode = opcode + opcode2;
            muxedOpCode += opcode2; 
            //uint16_t opcode = read_byte(reg.PC.inc(1));	
            //TODO: Make something cleaner and prettier here in the fute
            //But since it is lot of work it will probably never happen
            
#ifdef DEBUG
       std::cout << "muxedOpcode: cb" << std::hex << opcode2 << ", ";
       std::cout << std::hex << "Register values: PC: " <<int(reg.PC) << ", SP: " << int(reg.SP) << ", A: " << int(reg.A) << ", F: " << BIN(reg.AF.getLow) << ", AF: " << int(reg.AF.getData()) << ", B: " << int(reg.B) << ", C: " << int(reg.C) << ", BC: " << int(reg.BC.getData()) << ", D: " << int(reg.D) << ", E: " << ", DE: " << int(reg.DE.getData()) << ", H:" << int(reg.HL.high) << ", L:" << int(reg.L) << ", HL: " << int(reg.HL.getData()) << std::endl;
#endif*/
            /*switch(muxedOpCode) {
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
                    RLCA(cycles,reg.F,reg.HL.high);
                    cycles = 8;
                    break;
                case 0xCB05:
                    RLCA(cycles,reg.F,reg.L);
                    cycles = 8;
                    break;
                case 0xCB06:
                    temp = read_byte(reg.HL.getData());
                    RLCA(cycles,reg.F,temp);
                    cycles = 16;
                    break;
                case 0xCB07:
                    RLCA(cycles,reg.F,reg.A);
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
                    RRCA(cycles,reg.F,reg.HL.high);
                    cycles = 8;
                    break;
                case 0xCB0D:
                    RRCA(cycles,reg.F,reg.L);
                    cycles = 8;
                    break;
                case 0xCB0E:
                    temp = read_byte(reg.HL.getData());
                    RRCA(cycles,reg.F,temp);
                    cycles = 16;
                    break;
                case 0xCB0F:
                    RRCA(cycles,reg.F,reg.A);
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
                    RLA(cycles,reg.F,reg.HL.high);
                    cycles = 8;
                    break;
                case 0xCB15:
                    RLA(cycles,reg.F,reg.L);
                    cycles = 8;
                    break;
                case 0xCB16:
                    temp = read_byte(reg.HL.getData());
                    RLA(cycles,reg.F,temp);
                    cycles = 16;
                    break;
                case 0xCB17:
                    RLA(cycles,reg.F,reg.A);
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
                    RRA(cycles,reg.F,reg.HL.high);
                    cycles = 8;
                    break;
                case 0xCB1D:
                    RRA(cycles,reg.F,reg.L);
                    cycles = 8;
                    break;
                case 0xCB1E:
                    temp = read_byte(reg.HL.getData());
                    RRA(cycles,reg.F,temp);
                    cycles = 16;
                    break;
                case 0xCB1F:
                    RRA(cycles,reg.F,reg.A);
                    cycles = 8;
                    break;
                    //Shifts
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
                    SLA(cycles,reg.F,reg.HL.high);
                    cycles = 8;
                    break;
                case 0xCB25:
                    SLA(cycles,reg.F,reg.L);
                    cycles = 8;
                    break;
                case 0xCB26:
                    temp = read_byte(reg.HL.getData());
                    SLA(cycles,reg.F,temp);
                    cycles = 16;
                    break;   
                case 0xCB27:
                    SLA(cycles,reg.F,reg.A);
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
                    SRL(cycles,reg.F,reg.HL.high);
                    cycles = 8;
                    break;
                case 0xCB2D:
                    SRL(cycles,reg.F,reg.L);
                    cycles = 8;
                    break;
                case 0xCB2E:
                    temp = read_byte(reg.HL.getData());
                    SRL(cycles,reg.F,temp);
                    cycles = 16;
                    break;   
                case 0xCB2F:
                    SRL(cycles,reg.F,reg.A);
                    cycles = 8;
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
                    swap(cycles,reg.F,reg.HL.high);
                    break;
                case 0xCB35:
                    swap(cycles,reg.F,reg.L);
                    break;
                case 0xCB36:
                    temp = read_byte(reg.HL.getData());
                    swap(cycles,reg.F,temp);
                    cycles = 16;
                    break;
                case 0xCB37:
                    swap(cycles,reg.F,reg.A);
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
                    SRL(cycles,reg.F,reg.HL.high);
                    cycles = 8;
                    break;
                case 0xCB3D:
                    SRL(cycles,reg.F,reg.L);
                    cycles = 8;
                    break;
                case 0xCB3E:
                    temp = read_byte(reg.HL.getData());
                    SRL(cycles,reg.F,temp);
                    cycles = 16;
                    break;  
                case 0xCB3F:
                    SRL(cycles,reg.F,reg.A);
                    cycles = 8;
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
                    temp = read_byte(reg.HL.getData());
                    cmpbit_b_r(cycles,reg.F,temp,0);
                    cycles = 16;
                    break;
                case 0xCB47:
                    cmpbit_b_r(cycles,reg.F,reg.A,0);
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
                    temp = read_byte(reg.HL.getData());
                    cmpbit_b_r(cycles,reg.F,temp,1);
                    cycles = 16;
                    break;
                case 0xCB4F:
                    cmpbit_b_r(cycles,reg.F,reg.A,1);
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
                    temp = read_byte(reg.HL.getData());
                    cmpbit_b_r(cycles,reg.F,temp,2);
                    cycles = 16;
                    break;
                case 0xCB57:
                    cmpbit_b_r(cycles,reg.F,reg.A,2);
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
                    temp = read_byte(reg.HL.getData());
                    cmpbit_b_r(cycles,reg.F,temp,3);
                    cycles = 16;
                    break;
                case 0xCB5F:
                    cmpbit_b_r(cycles,reg.F,reg.A,3);
                    break;
                case 0xCB60:
                    cmpbit_b_r(cycles,reg.F,reg.B,4);
                    break;
                case 0xCB61:
                    cmpbit_b_r(cycles,reg.F,reg.C,4);
                    break;
                case 0xCB62:
                    cmpbit_b_r(cycles,reg.F,reg.D,4);
                    break;
                case 0xCB63:
                    cmpbit_b_r(cycles,reg.F,reg.E,4);
                    break;  
                case 0xCB64:
                    cmpbit_b_r(cycles,reg.F,reg.H,4);
                    break;
                case 0xCB65:
                    cmpbit_b_r(cycles,reg.F,reg.L,4);
                    break;
                case 0xCB66:
                    temp = read_byte(reg.HL.getData());
                    cmpbit_b_r(cycles,reg.F,temp,4);
                    cycles = 16;
                    break;
                case 0xCB67:
                    cmpbit_b_r(cycles,reg.F,reg.A,4);
                    break;
                case 0xCB68:
                    cmpbit_b_r(cycles,reg.F,reg.B,5);
                    break;
                case 0xCB69:
                    cmpbit_b_r(cycles,reg.F,reg.C,5);
                    break;
                case 0xCB6A:
                    cmpbit_b_r(cycles,reg.F,reg.D,5);
                    break;
                case 0xCB6B:
                    cmpbit_b_r(cycles,reg.F,reg.E,5);
                    break;  
                case 0xCB6C:
                    cmpbit_b_r(cycles,reg.F,reg.H,5);
                    break;
                case 0xCB6D:
                    cmpbit_b_r(cycles,reg.F,reg.L,5);
                    break;
                case 0xCB6E:
                    temp = read_byte(reg.HL.getData());
                    cmpbit_b_r(cycles,reg.F,temp,5);
                    cycles = 16;
                    break;
                case 0xCB6F:
                    cmpbit_b_r(cycles,reg.F,reg.A,5);
                    break;
                case 0xCB70:
                    cmpbit_b_r(cycles,reg.F,reg.B,6);
                    break;
                case 0xCB71:
                    cmpbit_b_r(cycles,reg.F,reg.C,6);
                    break;
                case 0xCB72:
                    cmpbit_b_r(cycles,reg.F,reg.D,6);
                    break;
                case 0xCB73:
                    cmpbit_b_r(cycles,reg.F,reg.E,6);
                    break;  
                case 0xCB74:
                    cmpbit_b_r(cycles,reg.F,reg.H,6);
                    break;
                case 0xCB75:
                    cmpbit_b_r(cycles,reg.F,reg.L,6);
                    break;
                case 0xCB76:
                    temp = read_byte(reg.HL.getData());
                    cmpbit_b_r(cycles,reg.F,temp,6);
                    cycles = 16;
                    break;
                case 0xCB77:
                    cmpbit_b_r(cycles,reg.F,reg.A,6);
                    break;
                case 0xCB78:
                    cmpbit_b_r(cycles,reg.F,reg.B,7);
                    break;
                case 0xCB79:
                    cmpbit_b_r(cycles,reg.F,reg.C,7);
                    break;
                case 0xCB7A:
                    cmpbit_b_r(cycles,reg.F,reg.D,7);
                    break;
                case 0xCB7B:
                    cmpbit_b_r(cycles,reg.F,reg.E,7);
                    break;  
                case 0xCB7C:
                    cmpbit_b_r(cycles,reg.F,reg.H,7);
                    break;
                case 0xCB7D:
                    cmpbit_b_r(cycles,reg.F,reg.L,7);
                    break;
                case 0xCB7E:
                    temp = read_byte(reg.HL.getData());
                    cmpbit_b_r(cycles,reg.F,temp,7);
                    cycles = 16;
                    break;
                case 0xCB7F:
                    cmpbit_b_r(cycles,reg.F,reg.A,7);
                    break;
                case 0xCB80:
                    res_b_r(cycles,reg.B,0);
                    reg.PC.inc(1);
                    break;
                case 0xCB81:
                    res_b_r(cycles,reg.C,0);
                    reg.PC.inc(1);
                    break;
                case 0xCB82:
                    res_b_r(cycles,reg.D,0);
                    reg.PC.inc(1);
                    break;
                case 0xCB83:
                    res_b_r(cycles,reg.E,0);
                    reg.PC.inc(1);
                    break;  
                case 0xCB84:
                    res_b_r(cycles,reg.H,0);
                    reg.PC.inc(1);
                    break;
                case 0xCB85:
                    res_b_r(cycles,reg.L,0);
                    reg.PC.inc(1);
                    break;
                case 0xCB86:
                    temp = read_byte(reg.HL.getData());
                    res_b_r(cycles,temp,0);
                    reg.PC.inc(1);
                    write_byte(reg.HL.getData(),temp);
                    cycles = 16;
                    break;       	       
                case 0xCB87:
                    res_b_r(cycles,reg.A,0);
                    reg.PC.inc(1);
                    break;
                case 0xCB88:
                    res_b_r(cycles,reg.B,1);
                    reg.PC.inc(1);
                    break;
                case 0xCB89:
                    res_b_r(cycles,reg.C,1);
                    reg.PC.inc(1);
                    break;
                case 0xCB8A:
                    res_b_r(cycles,reg.D,1);
                    reg.PC.inc(1);
                    break;
                case 0xCB8B:
                    res_b_r(cycles,reg.E,1);
                    reg.PC.inc(1);
                    break;  
                case 0xCB8C:
                    res_b_r(cycles,reg.H,1);
                    reg.PC.inc(1);
                    break;
                case 0xCB8D:
                    res_b_r(cycles,reg.L,1);
                    reg.PC.inc(1);
                    break;
                case 0xCB8E:
                    temp = read_byte(reg.HL.getData());
                    res_b_r(cycles,temp,1);
                    reg.PC.inc(1);
                    write_byte(reg.HL.getData(),temp);
                    cycles = 16;
                    break;       	       
                case 0xCB8F:
                    res_b_r(cycles,reg.A,1);
                    reg.PC.inc(1);
                    break;
                case 0xCB90:
                    res_b_r(cycles,reg.B,2);
                    reg.PC.inc(1);
                    break;
                case 0xCB91:
                    res_b_r(cycles,reg.C,2);
                    reg.PC.inc(1);
                    break;
                case 0xCB92:
                    res_b_r(cycles,reg.D,2);
                    reg.PC.inc(1);
                    break;
                case 0xCB93:
                    res_b_r(cycles,reg.E,2);
                    reg.PC.inc(1);
                    break;  
                case 0xCB94:
                    res_b_r(cycles,reg.H,2);
                    reg.PC.inc(1);
                    break;
                case 0xCB95:
                    res_b_r(cycles,reg.L,2);
                    reg.PC.inc(1);
                    break;
                case 0xCB96:
                    temp = read_byte(reg.HL.getData());
                    res_b_r(cycles,temp,2);
                    reg.PC.inc(1);
                    write_byte(reg.HL.getData(),temp);
                    cycles = 16;
                    break;       	       
                case 0xCB97:
                    res_b_r(cycles,reg.A,2);
                    reg.PC.inc(1);
                    break;
                case 0xCB98:
                    res_b_r(cycles,reg.B,3);
                    reg.PC.inc(1);
                    break;
                case 0xCB99:
                    res_b_r(cycles,reg.C,3);
                    reg.PC.inc(1);
                    break;
                case 0xCB9A:
                    res_b_r(cycles,reg.D,3);
                    reg.PC.inc(1);
                    break;
                case 0xCB9B:
                    res_b_r(cycles,reg.E,3);
                    reg.PC.inc(1);
                    break;  
                case 0xCB9C:
                    res_b_r(cycles,reg.H,3);
                    reg.PC.inc(1);
                    break;
                case 0xCB9D:
                    res_b_r(cycles,reg.L,3);
                    reg.PC.inc(1);
                    break;
                case 0xCB9E:
                    temp = read_byte(reg.HL.getData());
                    res_b_r(cycles,temp,3);
                    reg.PC.inc(1);
                    write_byte(reg.HL.getData(),temp);
                    cycles = 16;
                    break;       	       
                case 0xCB9F:
                    res_b_r(cycles,reg.A,3);
                    reg.PC.inc(1);
                    break;
                case 0xCBA0:
                    res_b_r(cycles,reg.B,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBA1:
                    res_b_r(cycles,reg.C,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBA2:
                    res_b_r(cycles,reg.D,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBA3:
                    res_b_r(cycles,reg.E,4);
                    reg.PC.inc(1);
                    break;  
                case 0xCBA4:
                    res_b_r(cycles,reg.H,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBA5:
                    res_b_r(cycles,reg.L,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBA6:
                    temp = read_byte(reg.HL.getData());
                    res_b_r(cycles,temp,4);
                    reg.PC.inc(1);
                    write_byte(reg.HL.getData(),temp);
                    cycles = 16;
                    break;       	       
                case 0xCBA7:
                    res_b_r(cycles,reg.A,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBA8:
                    res_b_r(cycles,reg.B,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBA9:
                    res_b_r(cycles,reg.C,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBAA:
                    res_b_r(cycles,reg.D,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBAB:
                    res_b_r(cycles,reg.E,5);
                    reg.PC.inc(1);
                    break;  
                case 0xCBAC:
                    res_b_r(cycles,reg.H,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBAD:
                    res_b_r(cycles,reg.L,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBAE:
                    temp = read_byte(reg.HL.getData());
                    res_b_r(cycles,temp,5);
                    reg.PC.inc(1);
                    write_byte(reg.HL.getData(),temp);
                    cycles = 16;
                    break;       	       
                case 0xCBAF:
                    res_b_r(cycles,reg.A,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBB0:
                    res_b_r(cycles,reg.B,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBB1:
                    res_b_r(cycles,reg.C,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBB2:
                    res_b_r(cycles,reg.D,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBB3:
                    res_b_r(cycles,reg.E,6);
                    reg.PC.inc(1);
                    break;  
                case 0xCBB4:
                    res_b_r(cycles,reg.H,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBB5:
                    res_b_r(cycles,reg.L,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBB6:
                    temp = read_byte(reg.HL.getData());
                    res_b_r(cycles,temp,6);
                    reg.PC.inc(1);
                    write_byte(reg.HL.getData(),temp);
                    cycles = 16;
                    break;       	       
                case 0xCBB7:
                    res_b_r(cycles,reg.A,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBB8:
                    res_b_r(cycles,reg.B,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBB9:
                    res_b_r(cycles,reg.C,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBBA:
                    res_b_r(cycles,reg.D,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBBB:
                    res_b_r(cycles,reg.E,7);
                    reg.PC.inc(1);
                    break;  
                case 0xCBBC:
                    res_b_r(cycles,reg.H,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBBD:
                    res_b_r(cycles,reg.L,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBBE:
                    temp = read_byte(reg.HL.getData());
                    res_b_r(cycles,temp,7);
                    reg.PC.inc(1);
                    write_byte(reg.HL.getData(),temp);
                    cycles = 16;
                    break;       	       
                case 0xCBBF:
                    res_b_r(cycles,reg.A,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBC0:
                    set_b_r(cycles,reg.B,0);
                    reg.PC.inc(1);
                    break;
                case 0xCBC1:
                    set_b_r(cycles,reg.C,0);
                    reg.PC.inc(1);
                    break;
                case 0xCBC2:
                    set_b_r(cycles,reg.D,0);
                    reg.PC.inc(1);
                    break;
                case 0xCBC3:
                    set_b_r(cycles,reg.E,0);
                    reg.PC.inc(1);
                    break;  
                case 0xCBC4:
                    set_b_r(cycles,reg.H,0);
                    reg.PC.inc(1);
                    break;
                case 0xCBC5:
                    set_b_r(cycles,reg.L,0);
                    reg.PC.inc(1);
                    break;
                case 0xCBC6:
                    temp = read_byte(reg.HL.getData());
                    set_b_r(cycles,temp,0);
                    cycles = 16;
                    write_byte(reg.HL.getData(),temp);
                    reg.PC.inc(1);
                    break;
                case 0xCBC7:
                    set_b_r(cycles,reg.A,0);
                    reg.PC.inc(1);
                    break;
                case 0xCBC8:
                    set_b_r(cycles,reg.B,1);
                    reg.PC.inc(1);
                    break;
                case 0xCBC9:
                    set_b_r(cycles,reg.C,1);
                    reg.PC.inc(1);
                    break;
                case 0xCBCA:
                    set_b_r(cycles,reg.D,1);
                    reg.PC.inc(1);
                    break;
                case 0xCBCB:
                    set_b_r(cycles,reg.E,1);
                    reg.PC.inc(1);
                    break;  
                case 0xCBCC:
                    set_b_r(cycles,reg.H,1);
                    reg.PC.inc(1);
                    break;
                case 0xCBCD:
                    set_b_r(cycles,reg.L,1);
                    reg.PC.inc(1);
                    break;
                case 0xCBCE:
                    temp = read_byte(reg.HL.getData());
                    set_b_r(cycles,temp,1);
                    cycles = 16;
                    write_byte(reg.HL.getData(),temp);
                    reg.PC.inc(1);
                    break;
                case 0xCBCF:
                    set_b_r(cycles,reg.A,1);
                    reg.PC.inc(1);
                    break;
                case 0xCBD0:
                    set_b_r(cycles,reg.B,2);
                    reg.PC.inc(1);
                    break;
                case 0xCBD1:
                    set_b_r(cycles,reg.C,2);
                    reg.PC.inc(1);
                    break;
                case 0xCBD2:
                    set_b_r(cycles,reg.D,2);
                    reg.PC.inc(1);
                    break;
                case 0xCBD3:
                    set_b_r(cycles,reg.E,2);
                    reg.PC.inc(1);
                    break;  
                case 0xCBD4:
                    set_b_r(cycles,reg.H,2);
                    reg.PC.inc(1);
                    break;
                case 0xCBD5:
                    set_b_r(cycles,reg.L,2);
                    reg.PC.inc(1);
                    break;
                case 0xCBD6:
                    temp = read_byte(reg.HL.getData());
                    set_b_r(cycles,temp,2);
                    cycles = 16;
                    write_byte(reg.HL.getData(),temp);
                    reg.PC.inc(1);
                    break;
                case 0xCBD7:
                    set_b_r(cycles,reg.A,2);
                    reg.PC.inc(1);
                    break;
                case 0xCBD8:
                    set_b_r(cycles,reg.B,3);
                    reg.PC.inc(1);
                    break;
                case 0xCBD9:
                    set_b_r(cycles,reg.C,3);
                    reg.PC.inc(1);
                    break;
                case 0xCBDA:
                    set_b_r(cycles,reg.D,3);
                    reg.PC.inc(1);
                    break;
                case 0xCBDB:
                    set_b_r(cycles,reg.E,3);
                    reg.PC.inc(1);
                    break;  
                case 0xCBDC:
                    set_b_r(cycles,reg.H,3);
                    reg.PC.inc(1);
                    break;
                case 0xCBDD:
                    set_b_r(cycles,reg.L,3);
                    reg.PC.inc(1);
                    break;
                case 0xCBDE:
                    temp = read_byte(reg.HL.getData());
                    set_b_r(cycles,temp,3);
                    cycles = 16;
                    write_byte(reg.HL.getData(),temp);
                    reg.PC.inc(1);
                    break;
                case 0xCBDF:
                    set_b_r(cycles,reg.A,3);
                    reg.PC.inc(1);
                    break;
                case 0xCBE0:
                    set_b_r(cycles,reg.B,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBE1:
                    set_b_r(cycles,reg.C,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBE2:
                    set_b_r(cycles,reg.D,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBE3:
                    set_b_r(cycles,reg.E,4);
                    reg.PC.inc(1);
                    break;  
                case 0xCBE4:
                    set_b_r(cycles,reg.H,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBE5:
                    set_b_r(cycles,reg.L,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBE6:
                    temp = read_byte(reg.HL.getData());
                    set_b_r(cycles,temp,4);
                    cycles = 16;
                    write_byte(reg.HL.getData(),temp);
                    reg.PC.inc(1);
                    break;
                case 0xCBE7:
                    set_b_r(cycles,reg.A,4);
                    reg.PC.inc(1);
                    break;
                case 0xCBE8:
                    set_b_r(cycles,reg.B,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBE9:
                    set_b_r(cycles,reg.C,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBEA:
                    set_b_r(cycles,reg.D,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBEB:
                    set_b_r(cycles,reg.E,5);
                    reg.PC.inc(1);
                    break;  
                case 0xCBEC:
                    set_b_r(cycles,reg.H,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBED:
                    set_b_r(cycles,reg.L,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBEE:
                    temp = read_byte(reg.HL.getData());
                    set_b_r(cycles,temp,5);
                    cycles = 16;
                    write_byte(reg.HL.getData(),temp);
                    reg.PC.inc(1);
                    break;
                case 0xCBEF:
                    set_b_r(cycles,reg.A,5);
                    reg.PC.inc(1);
                    break;
                case 0xCBF0:
                    set_b_r(cycles,reg.B,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBF1:
                    set_b_r(cycles,reg.C,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBF2:
                    set_b_r(cycles,reg.D,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBF3:
                    set_b_r(cycles,reg.E,6);
                    reg.PC.inc(1);
                    break;  
                case 0xCBF4:
                    set_b_r(cycles,reg.H,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBF5:
                    set_b_r(cycles,reg.L,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBF6:
                    temp = read_byte(reg.HL.getData());
                    set_b_r(cycles,temp,6);
                    cycles = 16;
                    write_byte(reg.HL.getData(),temp);
                    reg.PC.inc(1);
                    break;
                case 0xCBF7:
                    set_b_r(cycles,reg.A,6);
                    reg.PC.inc(1);
                    break;
                case 0xCBF8:
                    set_b_r(cycles,reg.B,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBF9:
                    set_b_r(cycles,reg.C,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBFA:
                    set_b_r(cycles,reg.D,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBFB:
                    set_b_r(cycles,reg.E,7);
                    reg.PC.inc(1);
                    break;  
                case 0xCBFC:
                    set_b_r(cycles,reg.H,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBFD:
                    set_b_r(cycles,reg.L,7);
                    reg.PC.inc(1);
                    break;
                case 0xCBFE:
                    temp = read_byte(reg.HL.getData());
                    set_b_r(cycles,temp,7);
                    cycles = 16;
                    write_byte(reg.HL.getData(),temp);
                    reg.PC.inc(1);
                    break;
                case 0xCBFF:
                    set_b_r(cycles,reg.A,7);
                    reg.PC.inc(1);
                    break;
            }
            break;
                case 0xIE:
                  write_byte(IE_R,0b00011111);//TODO This case might still have to be commented away.
                  cycles = 4;
                  break;    
                  case 0xDI:
                  write_byte(IE_R,0);
                  cycles = 4;
                  break;*/
    }
    return cycles;
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
