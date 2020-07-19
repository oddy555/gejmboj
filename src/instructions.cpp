#include "../inc/memory.hpp"
#include "../inc/instructions.hpp"
#include "../debug/debug.hpp"
#include <cstdint>
#include <iostream>
#include <string> 

#ifdef DEBUG

uint8_t* tempRegs[8];
uint16_t* tempRegs2[2];

void setup_debug_instr(uint8_t& r,int i) {
    tempRegs[i] = &r;
}

void setup_debug_instr(uint16_t& a, uint16_t& b) {
    tempRegs2[0] = &a;
    tempRegs2[1] = &b;
}


std::string matchRegister(uint8_t& addr) {
   std::string regs[8];
   regs[0] = "A";
   regs[1] = "F";
   regs[2] = "B";
   regs[3] = "C";
   regs[4] = "D";
   regs[5] = "E";
   regs[6] = "H";
   regs[7] = "L";
   
   for (int r = 0; r < 8; r++) {
       if (tempRegs[r] == &addr) {
        return regs[r];
       }
   }
    return "NA";
}
std::string matchRegister(uint16_t& addr) {
    if (tempRegs2[0] == &addr) {
        return "SP";
    } else if (tempRegs2[1]) {
        return "PC";
    } else if ((uint16_t*) tempRegs[6] == &addr) {
        return "HL";
    } else if ((uint16_t*) tempRegs[0] == &addr) {
        return "AF";
    } else if ((uint16_t*) tempRegs[2] == &addr) {
        return "BC";
    } else if ((uint16_t*) tempRegs[4] == &addr) {
        return "DE";
    }
    return "NA";
}
#endif


void LD_8_n_nn(int& cycles,uint8_t& r,uint16_t addr) {
    cycles = 8;
    r = read_byte(addr);    

#ifdef DEBUG
    std::cout << "LD_8_"<<matchRegister(r)<<"_"<<std::hex<<addr<<", cycles: " << std::dec << cycles << std::endl; 
#endif
}
void LD_8_r_r(int &cycles, uint8_t& r1,uint8_t r2) {
    cycles = 4;
    r1 = r2; 

#ifdef DEBUG
    std::cout << "LD_8_"<<matchRegister(r1)<<"_"<<matchRegister(r2)<<", cycles: " << std::dec << cycles << std::endl; 
#endif
}

void LD_8_r_r(int &cycles,uint16_t addr,uint8_t r) {
    cycles = 8;
    write_byte(addr,r);

#ifdef DEBUG
    std::cout << "LD_8_"<<std::hex<<addr<<"_"<<matchRegister(r)<<", cycles: " << std::dec << cycles << std::endl; 
#endif
}

void LD_8_r_r(int& cycles,uint16_t addr1, uint16_t addr2) {
    cycles = 12;
    uint8_t data = read_byte(addr2);
    write_byte(addr1,data);

#ifdef DEBUG
    std::cout << "LD_8_"<<std::hex<<addr1<<"_"<<std::hex<<addr2<<"data: "<< data <<", cycles: " << std::dec << cycles << std::endl; 
#endif
}

void LDD_8(int& cycles, uint8_t& r,uint16_t& addr) {
    cycles = 8;
    r = read_byte(addr);
    addr--;
#ifdef DEBUG
    std::cout << "LD_8_"<<std::hex<<addr<<"_"<<matchRegister(r)<<", cycles: " << std::dec << cycles << std::endl; 
#endif

}

void LDD_8(int& cycles, uint16_t& addr, uint8_t r) {
    cycles = 8;
    write_byte(addr,r);
    addr--;

#ifdef DEBUG
    std::cout << "LD_8_"<<matchRegister(r)<<"_"<<matchRegister(addr)<<std::hex<<addr<<", cycles: " << std::dec << cycles << std::endl; 
#endif

}

void LDI_8(int& cycles, uint8_t& r,uint16_t& addr){
    cycles = 8;
    r = read_byte(addr);
    addr++;
    
#ifdef DEBUG
    std::cout << "LDI_8_" << matchRegister(r) << "_" << matchRegister(addr) << ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void LDI_8(int& cycles, uint16_t& addr,uint8_t r) {
    cycles = 8;
    write_byte(addr++,r);

#ifdef DEBUG
    std::cout << "LDI_8_" << matchRegister(addr) << "_" << matchRegister(r) << ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void LD_16_n_nn(int& cycles, uint16_t& r,uint16_t addr) {
    cycles = 12;
    r = read_word(addr);

#ifdef DEBUG
    std::cout << "LDI_16_" << matchRegister(r) << "_" << std::hex << addr << ", cycles: " << std::dec << cycles << std::endl;
#endif
}
void LD_16_r_r(int& cycles, uint16_t& r1,uint16_t r2) {
    cycles = 8;
    r1 = r2;

#ifdef DEBUG
    std::cout << "LDI_16_" << matchRegister(r1) << "_" << matchRegister(r2) << ", cycles: " << std::dec << cycles << std::endl;
#endif

}

void push(int& cycles,uint16_t& sp,uint16_t r) {
    cycles = 16;
    write_word(sp,r);
    sp-=2;

#ifdef DEBUG
    std::cout << "push_" << matchRegister(sp) << "_" << matchRegister(r) << ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void pop(int& cycles,uint16_t& r,uint16_t sp) {
    cycles = 12;
    r = read_word(sp);
    sp+=2;

#ifdef DEBUG
    std::cout << "pop_" << matchRegister(r) << "_" << matchRegister(sp) << ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void add_8(int& cycles,uint8_t& f,uint8_t& r1,uint8_t r2) {
    cycles = 4; 
    if ((int) r1 + (int) r2 > 255) {
        f |= 0x10;
    } else if ((int) r1 + (int) r2 == 0) {
        f |= 0x80;
    }
    if (((r1&0xf) + (r2&0xf))&0x10){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1 += r2;
#ifdef DEBUG
    std::cout << "add_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void adc(int& cycles,uint8_t& f, uint8_t& r1,uint8_t r2) {
    cycles = 4;
    uint8_t temp = 0; 
    if (f&0x10) {
        temp = 1;
    }
    if ((int) r1 + (int) r2 + (int) temp > 255) {
        f |= 0x10;
    } else if ((int) r1 + (int) r2 + (int) temp == 0) {
        f |= 0x80;
    }
    if (((r1&0xf) + (r2&0xf) + (f&0xf))&0x10){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1 += r2;  

#ifdef DEBUG
    std::cout << "adc_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void sub_8(int& cycles,uint8_t& f ,uint8_t& r1,uint8_t r2) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    temp1  = ~temp1 & r1;
    temp2 = ~temp2 & r2;
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1 - (int) r2 < 0) {
        f |= 0x10;
    } else if ((int) r1 - (int) r2 == 0) {
        f |= 0x80;
    }
    r1 -= r2;
    f |= 0x40;

#ifdef DEBUG
    std::cout << "sub_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void subc(int& cycles,uint8_t& f,uint8_t& r1,uint8_t r2){
    cycles = 4;
    int temp = 0;
    if (f&0x10) {
        temp = 1;
    }
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    temp1  = ~temp1 & r1;
    temp2 = ~temp2 & r2;
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1 - ((int) r2-(int) temp) < 0) {
        f |= 0x10;
    } else if ((int) r1 - ((int) r2 - (int) temp) == 0) {
        f |= 0x80;
    }
    r1 -= (r2+temp);
     f |= 0x40;
       
#ifdef DEBUG
    std::cout << "subc_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void and_8(int &cycles,uint8_t f, uint8_t &r1,uint8_t r2) {
    cycles = 4;
    uint8_t res = r1 & r2;
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f |= 0x20;
        f &= ~0x10; 
        r1 = res;

#ifdef DEBUG
    std::cout << "and_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void or_8(int &cycles,uint8_t f, uint8_t &r1,uint8_t r2) {
    cycles = 4;
    uint8_t res = r1 | r2;
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f &= ~0x20;
        f &= ~0x10; 

#ifdef DEBUG
    std::cout << "or_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void xor_8(int &cycles,uint8_t f, uint8_t &r1,uint8_t r2) {
    uint8_t res = r1 & r2;
    cycles = 4;
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f |= 0x20;
        f &= ~0x10; 

#ifdef DEBUG
    std::cout << "xor_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void cp_8(int& cycles,uint8_t& f ,uint8_t& r1,uint8_t r2) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    temp1  = ~temp1 & r1;
    temp2 = ~temp2 & r2;
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1 - (int) r2 < 0) {
        f |= 0x10;
    } else if ((int) r1 - (int) r2 == 0) {
        f |= 0x80;
    }
    //r1 -= r2;
    f |= 0x40;

#ifdef DEBUG
    std::cout << "cp_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void inc_8(int& cycles,uint8_t& f ,uint8_t& r1) {
    cycles = 4; 
    if ((int) r1 + (int) 1 == 0) {
        f |= 0x80;
    }
    if (((r1&0xf) + (1&0xf))&0x10){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1 += (uint8_t) 1; 

#ifdef DEBUG
    std::cout << "inc_8_" << matchRegister(r1) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void dec_8(int& cycles,uint8_t& f ,uint8_t& r1) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    temp1  = ~temp1 & r1;
    temp2 = ~temp2 & 1;
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1 - (int) 1 == 0) {
        f |= 0x80;
    }
    r1 -= 1;
    f |= 0x40;

#ifdef DEBUG
    std::cout << "dec_8_" << matchRegister(r1) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void add_16(int& cycles,uint8_t& f,uint16_t& r1,uint16_t r2) {
    cycles = 8; 
    if ((int) r1 + (int) r2 > 65535) {
        f |= 0x10;
    } else if ((int) r1 + (int) r2 == 0) {
        f |= 0x80;
    }
    if (((r1&0xfff) + (r2&0xfff))&0x100){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1 += r2;


#ifdef DEBUG
    std::cout << "add_16_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void add_sp(int& cycles,uint8_t& f,uint16_t& r1,uint8_t r2) {
    cycles = 16; 
    if ((int) r1 + (int) r2 > 65535) {
        f |= 0x10;
    } else if ((int) r1 + (int) r2 == 0) {
        f |= 0x80;
    }
    if (((r1&0xfff) + (r2&0xfff))&0x100){
        f |= 0x20;
    }
    f &= ~0x40; 
    f &= ~0x80;
    r1 += r2;

#ifdef DEBUG
    std::cout << "add_sp_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void inc_16(int& cycles,uint16_t& r1) {
    cycles = 8;
    r1 += (uint16_t) 1;    

#ifdef DEBUG
    std::cout << "dec_16_" << matchRegister(r1) << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void dec_16(int& cycles,uint16_t& r1) {
    cycles = 8;
    r1 -= 1;

#ifdef DEBUG
    std::cout << "dec_16_" << matchRegister(r1) << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void ccf(int& cycles,uint8_t f) {
    cycles = 4;
    f &= ~0x60 & ~0x40;
    f ^= 0x20;

#ifdef DEBUG
    std::cout << "ccf" << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void swap(int& cycles,uint8_t& f ,uint8_t& r) {
    uint8_t temp1;
    uint8_t temp2;
    cycles = 8;
    temp1 = r & ~0x80 & ~0x40 & ~0x20 & ~0x10;
    temp1 <<= 4;
    temp2 = r & ~0x01 & ~0x02 & ~0x03 & ~0x04;
    temp2 >>= 4;
    r = temp1 + temp2; 

#ifdef DEBUG
    std::cout << "swap" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void daa(int& cycles,uint8_t& f,uint8_t& r) {
    cycles = 4; 
    if (!(f & 0x60)) {
        if(f & 0x40 || (r & 0xf) > 9) 
            r += 0x06;
        if(f & 0x20 || r > 0x9F)
            r+= 0x60;
    } else {
        if(f & 0x40)  
            r = (r-6) & 0xFF;
        if(f & 0x20)
            r = 0x60;
    }

    f &= ~(0x40 | 0x80);

    if ((r &0x100) == 0x100)
        f |= 0x20;

    r &= 0xFF;

    if (r == 0)
        f |= 0x80;
   

#ifdef DEBUG
    std::cout << "daa" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}


void cpl(int& cycles,uint8_t& f,uint8_t& r) {
    cycles = 4;
    f |= 0x60;  
    f |= 0x40;
    r = ~r;

#ifdef DEBUG
    std::cout << "cpl" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void scf(int& cycles, uint8_t& f) {
    cycles = 4;
    f |= 0x20;
    f &= ~0x60 & ~0x40;

#ifdef DEBUG
    std::cout << "scf" << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RLCA(int& cycles,uint8_t& f,uint8_t& r) {
    if (0x80 & r) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    r <<= 1;
    r += 1;
    if (r == 0) {
        f |= 0x80;
       
    }
    cycles = 4;

#ifdef DEBUG
    std::cout << "RLCA" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RLA(int& cycles,uint8_t& f,uint8_t& r) {
   cycles = 4;
     f &= ~0x60 & ~0x40;
    r <<= 1;
    r += 1;
    if (r == 0) {
        f |= 0x80;
       }

#ifdef DEBUG
    std::cout << "RLA_R" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RRCA(int& cycles,uint8_t& f,uint8_t& r) {
    if (0x01 & r) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    r>>= 1;
    r+=1;
    if (r == 0) {
        f |= 0x80;
       
    }
    cycles = 4;

#ifdef DEBUG
    std::cout << "RRCA_r" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RRA(int& cycles,uint8_t& f,uint8_t& r) {
   cycles = 4;
     f &= ~0x60 & ~0x40;
    r <<= 1;
    r+=1;
    if (r == 0) {
        f |= 0x80;
    }

#ifdef DEBUG
    std::cout << "RRA_r" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void SLA(int& cycles,uint8_t& f,uint8_t& r) {
    if (0x80 & r) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    r <<= 1;
    r &= ~0x01;
    if (r == 0) {
        f |= 0x80;
       
    }
    cycles = 4;

#ifdef DEBUG
    std::cout << "SLA_" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
/*void RLA(int& cycles,uint8_t& f,uint8_t& r) {
    if (0x01 & r) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    r>>= 1;
    if (r == 0) {
        f |= 0x80;
       
    }
    cycles = 8;
}*/

void SRL(int& cycles,uint8_t& f,uint8_t& r) {
    if (0x01 & r) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    r>>= 1;
    r &= ~0x80;
    if (r == 0) {
        f |= 0x80;
       
    }
    cycles = 8;

#ifdef DEBUG
    std::cout << "SRL_" << matchRegister(r) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void cmpbit_b_r(int& cycles,uint8_t& f,uint8_t r1,uint8_t r2) {
    cycles = 8;
    if (!(r2 & r1)) {
        f |= 0x80;
    }
    f &= ~0x60;
    f |= 0x40;

#ifdef DEBUG
    std::cout << "cmpbit_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void set_b_r(int& cycles,uint8_t &r1,uint8_t r2) {
    cycles = 8;
    r1 |= r2;

#ifdef DEBUG
    std::cout << "set_" << matchRegister(r1) << "_" << matchRegister(r2) << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void res_b_r(int& cycles,uint8_t &r1,uint8_t r2) {
    cycles = 8;
    r1 &= ~r2;

#ifdef DEBUG
    std::cout << "res_" << matchRegister(r1) << "_" << matchRegister(r2) << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void rst(int& cycles,uint16_t& sp, uint16_t& pc) {
#ifdef DEBUG
    std::cout << "rst_" << matchRegister(sp) << "_" << matchRegister(pc) << ", cycles: " << std::dec << 16 << std::endl << "  ";
#endif 
    push(cycles, sp, pc);
    cycles += 16; //In doc it says 32
    pc = read_byte(pc);

} 
