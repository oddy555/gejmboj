#include "memory.hpp"
#include "instructions.hpp"
#include <cstdint>

void LD_8_n_nn(int& cycles,uint8_t& r,uint16_t addr) {
    cycles = 8;
    r = read_byte(addr);    
}
void LD_8_r_r(int &cycles, uint8_t& r1,uint8_t r2) {
    cycles = 4;
    r1 = r2; 
}

void LD_8_r_r(int &cycles,uint16_t addr,uint8_t r) {
    cycles = 8;
    write_byte(addr,r);
}

void LD_8_r_r(int& cycles,uint16_t addr1, uint16_t addr2) {
    cycles = 12;
    uint8_t data = read_byte(addr2);
    write_byte(addr1,data);
}

void LDD_8(int& cycles, uint8_t& r,uint16_t& addr) {
    cycles = 8;
    r = read_byte(addr);
    addr--;
}

void LDD_8(int& cycles, uint16_t& addr, uint8_t r) {
    cycles = 8;
    write_byte(addr,r);
    addr--;
}

void LDI_8(int& cycles, uint8_t& r,uint16_t& addr){
    cycles = 8;
    r = read_byte(addr);
    addr++;
}

void LDI_8(int& cycles, uint16_t& addr,uint8_t r) {
    cycles = 8;
    write_byte(addr++,r);
}

void LD_16_n_nn(int& cycles, uint16_t& r,uint16_t addr) {
    cycles = 12;
    r = read_word(addr);
}
void LD_16_r_r(int& cycles, uint16_t& r1,uint16_t r2) {
    cycles = 8;
    r1 = r2;
}

void push(int& cycles,uint16_t& sp,uint16_t r) {
    cycles = 16;
    write_word(sp,r);
    sp-=2; 
}

void pop(int& cycles,uint16_t& r,uint16_t sp) {
    cycles = 12;
    r = read_word(sp);
    sp+=2;
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
       
}

void and_8(int &cycles,uint8_t f, uint8_t r1,uint8_t r2) {
    cycles = 4;
    uint8_t res = r1 & r2;
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f |= 0x20;
        f &= ~0x10; 
}

void or_8(int &cycles,uint8_t f, uint8_t r1,uint8_t r2) {
    cycles = 4;
    uint8_t res = r1 | r2;
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f &= ~0x20;
        f &= ~0x10; 
}

void xor_8(int &cycles,uint8_t f, uint8_t r1,uint8_t r2) {
    uint8_t res = r1 & r2;
    cycles = 4;
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f |= 0x20;
        f &= ~0x10; 
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

}

void inc_16(int& cycles,uint16_t& r1) {
    cycles = 8;
    r1 += (uint16_t) 1;    
}

void dec_16(int& cycles,uint16_t& r1) {
    cycles = 8;
    r1 -= 1;
}

void ccf(int& cycles,uint8_t f) {
    cycles = 4;
    f &= ~0x60 & ~0x40;
    f ^= 0x20;
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
   
}


void cpl(int& cycles,uint8_t& f,uint8_t& r) {
    cycles = 4;
    f |= 0x60;  
    f |= 0x40;
    r = ~r;
}

void scf(int& cycles, uint8_t& f) {
    cycles = 4;
    f |= 0x20;
    f &= ~0x60 & ~0x40;
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
}

void RLA(int& cycles,uint8_t& f,uint8_t& r) {
   cycles = 4;
     f &= ~0x60 & ~0x40;
    r <<= 1;
    r += 1;
    if (r == 0) {
        f |= 0x80;
       }
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
}

void RRA(int& cycles,uint8_t& f,uint8_t& r) {
   cycles = 4;
     f &= ~0x60 & ~0x40;
    r <<= 1;
    r+=1;
    if (r == 0) {
        f |= 0x80;
       }
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
}

void cmpbit_b_r(int& cycles,uint8_t& f,uint8_t r1,uint8_t r2) {
    cycles = 8;
    if (!(r2 & r1)) {
        f |= 0x80;
    }
    f &= ~0x60;
    f |= 0x40;
}

void set_b_r(int& cycles,uint8_t r1,uint8_t r2) {
    cycles = 8;
    r1 |= r2;
}

void res_b_r(int& cycles,uint8_t r1,uint8_t r2) {
    cycles = 8;
    r1 &= ~r2;
}
void rst(int& cycles,uint16_t& sp, uint16_t& pc) {
    push(cycles, sp, pc);
    cycles += 32;
    pc = read_byte(pc);
} 
