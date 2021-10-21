#include "../inc/memory.hpp"
#include "../inc/instructions.hpp"
#include "../debug/debug.hpp"
#include "../inc/register.hpp"
#include <cstdint>
#include <iostream>
#include <string> 
#include <map>

#ifdef DEBUG2

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
    } else if ((uint16_t*) tempRegs[7] == &addr) {
        return "HL";
    } else if ((uint16_t*) tempRegs[0] == &addr) {
        return "AF";
    } else if ((uint16_t*) tempRegs[2] == &addr) {
        return "BC";
    } else if ((uint16_t*) tempRegs[4] == &addr) {
        return "DE";
    }
    return std::to_string(addr);
}

#endif

//Opcode to isntruction mapping, used for debugging at this time.
//0x0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F
//1x0,....,F
//...
//CB Fx0,....F
//std::string [512] instr_debug = 
//{
//"NOP","LDI16 BC"
//};
void LD_8_n_nn(int& cycles,Register8 &r,uint16_t addr) {
    cycles = 8;
    r.setData(read_byte(addr));    

#ifdef DEBUG
    std::cout << "LD_8_"<<r.getName()<<"_"<<std::hex<<addr<<",value: "<<int(r.getData())<<", cycles: " << std::dec << cycles << std::endl; 
#endif
}

void LD_8_r_r(int &cycles, Register8 &r1,Register8 &r2) {
    cycles = 4;
    r1.setData(r2.getData()); 
#ifdef DEBUG2
    std::cout << "LD_8_"<<r1.getName()<<"_"<<r2.getName()<<",value: "<<int((r2.getData())<<", cycles: " << std::dec << cycles << std::endl; 
#endif
}

void LD_8_r_r(int &cycles,uint16_t addr,Register8 &r) {
    cycles = 8;
    write_byte(addr,r.getData());
#ifdef DEBUG2
    std::cout << "LD_8_"<<std::hex<<addr<<"_"<<r.getName()<<",value: "<<int(r.getData())<<", cycles: " << std::dec << cycles << std::endl; 
#endif
}


void LD_8_r_r(int& cycles,uint16_t addr1, uint16_t addr2) {
    cycles = 12;
    uint8_t data = read_byte(addr2);
    write_byte(addr1,data);

#ifdef DEBUG2
    std::cout << "LD_8_"<<std::hex<<addr1<<"_"<<std::hex<<addr2<<",value: "<< data <<", cycles: " << std::dec << cycles << std::endl; 
#endif
}

void LDD_8(int& cycles, Register8& r,uint16_t addr) {
    cycles = 8;
    r.setData(read_byte(addr));
    addr--;
#ifdef DEBUG2
    std::cout << "LDD_8_"<<std::hex<<addr<<"_"<<r.getName()<<",value: "<<r.getData()<<", cycles: " << std::dec << cycles << std::endl; 
#endif

}

void LDD_8(int& cycles, uint16_t addr, Register8 &r) {
    cycles = 8;
    write_byte(addr,r.getData());
    addr--;

#ifdef DEBUG2
    std::cout << "LDD_8_"<<matchRegister(addr)<<"_"<<r.getName()<<"_"<<std::hex<<addr<<", Value: " <<r.getData()<<", cycles: " << std::dec << cycles << std::endl; 
#endif

}

void LDI_8(int& cycles, Register8& r,uint16_t addr){
    cycles = 8;
    r.setData(read_byte(addr));
    addr++;
    
#ifdef DEBUG2
    std::cout << "LDI_8_" << r.getName() << "_" << matchRegister(addr)<<",value: "<<r.getData()<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void LDI_8(int& cycles, uint16_t addr,Register8 &r) {
    cycles = 8;
    write_byte(addr++,r.getData());

#ifdef DEBUG2
    std::cout << "LDI_8_" << matchRegister(addr) << "_" << r.getName()<<",value: "<<r.getData()<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void LD_16_n_nn(int& cycles, Register16& r,uint16_t addr) {
    cycles = 12;
    r.setData(read_word(addr));

#ifdef DEBUG
    //std::cout << "LD_16_" << matchRegister(r) << "_" << std::hex << addr<<",value: "<< r  << ", cycles: " << std::dec << cycles << std::endl;
    std::cout << "LD " << r.getName() << " " << std::hex << read_word(addr) << "; " << addr << ":" << addr-1 << "Cycles: " << std::dec << cycles << " LD_16_n_nn" << std::endl; 
#endif
}

void LD_16_r_r(int& cycles, Register16 r1,Register16 r2) {
    cycles = 8;
    r1.setData(r2.getData());

#ifdef DEBUG2
    std::cout << "LD_16_" << r1.getName() << "_" << r2.getName() <<",value: "<< r1 << ", cycles: " << std::dec << cycles << std::endl;
#endif

}

void LD_16_r_r(int& cycles, Register16 & r1,uint16_t r2) {
    cycles = 8;
    r1.setData(r2);

#ifdef DEBUG2
    std::cout << "LD_16_" << r1.getName() << "_" << matchRegister(r2) <<",value: "<< r1 << ", cycles: " << std::dec << cycles << std::endl;
#endif

}
void push(int& cycles,Register16& sp,Register16 r) {
    cycles = 16;
    write_word(sp.getData(),r.getData());
    sp.setData(sp.getData()-2);

#ifdef DEBUG2
    std::cout << "push_" << matchRegister(sp) << "_" << r.getName()<<",value: "<<r.getData()<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void pop(int& cycles,uint16_t& r,uint16_t sp) {
    cycles = 12;
    r = read_word(sp);
    sp+=2;

#ifdef DEBUG2
    std::cout << "pop_" << matchRegister(r) << "_" << matchRegister(sp)<<",value: "<<r<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void pop(int& cycles,Register16& r,uint16_t sp) {
    cycles = 12;
    r.setData(read_word(sp));
    sp+=2;

#ifdef DEBUG2
    std::cout << "pop_" << r.getName() << "_" << matchRegister(sp)<<",value: "<<r.getData()<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void add_8(int& cycles,uint8_t& f,Register8& r1,Register8 &r2) {
    cycles = 4;
    int a = r1.getData();
    int b = r2.getData();
    if (a + b > 255) {
        f |= 0x10;
    } else if (a + b == 0) {
        f |= 0x80;
    }
    if (((a&0xf) + (b&0xf))&0x10){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1.setData(a + b);
#ifdef DEBUG2
    std::cout << "add_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void add_8(int& cycles,uint8_t& f,Register8& r1,Register16 r2) {
    cycles = 4;
    uint8_t t2 = read_byte(r2.getData());
    int a = (int) r1.getData();
    if (a + (int) t2 > 255) {
        f |= 0x10;
    } else if ((int) a + (int) t2 == 0) {
        f |= 0x80;
    }
    if (((a&0xf) + (t2&0xf))&0x10){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1.setData(a + t2);;
#ifdef DEBUG2
    std::cout << "add_8_" << r1.getName() << "_" << r2.getName()<< ", value: " << t2 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void adc(int& cycles,uint8_t& f, Register8& r1,Register8 &r2) {
    cycles = 4;
    uint8_t temp = 0; 
    int a = (int) r1.getData();
    int b = (int) r2.getData();
    if (f&0x10) {
        temp = 1;
    }
    if (a + b + (int) temp > 255) {
        f |= 0x10;
    } else if (a + b + (int) temp == 0) {
        f |= 0x80;
    }
    if (((a&0xf) + (b&0xf) + (f&0xf))&0x10){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1.setData(a+b);  

#ifdef DEBUG2
    std::cout << "adc_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void adc(int& cycles,uint8_t& f, Register8& r1,Register16 r2) {
    cycles = 4;
    uint8_t temp = 0;
    uint8_t t2 = read_byte(r2.getData());
    uint8_t a = r1.getData();
    if (f&0x10) {
        temp = 1;
    }
    if ((int) a + (int) t2 + (int) temp > 255) {
        f |= 0x10;
    } else if ((int) a + (int) t2 + (int) temp == 0) {
        f |= 0x80;
    }
    if (((a&0xf) + (t2&0xf) + (f&0xf))&0x10){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1.setData(a + t2);  

#ifdef DEBUG2
    std::cout << "adc_8_" << r1.getName() << "_" << r2.getName() << ",value: " << t2 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void sub_8(int& cycles,uint8_t& f ,Register8& r1,Register8 &r2) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    temp1  = ~temp1 & r1.getData();
    temp2 = ~temp2 & r2.getData();
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1.getData() - (int) r2.getData() < 0) {
        f |= 0x10;
    } else if ((int) r1.getData() - (int) r2.getData() == 0) {
        f |= 0x80;
    }
    r1.setData(r1.getData() +r2.getData());
    f |= 0x40;

#ifdef DEBUG2
    std::cout << "sub_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void sub_8(int& cycles,uint8_t& f ,Register16 & r1,Register16 r2) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    uint8_t temp3 = read_byte(r2.getData());
    temp1  = ~temp1 & r1.getData();
    temp2 = ~temp2 & temp3;
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1.getData() - (int) temp3 < 0) {
        f |= 0x10;
    } else if ((int) r1.getData() - (int) temp3 == 0) {
        f |= 0x80;
    }
    r1.setData(r1.getData() - r2.getData());
    f |= 0x40;

#ifdef DEBUG2
    std::cout << "sub_8_" << r1.getName() << "_" << r2.getName() << ", value: " << temp3 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void subc(int& cycles,uint8_t& f,uint8_t& r1,uint8_t &r2){
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
       
#ifdef DEBUG2
    std::cout << "subc_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void subc(int& cycles,uint8_t& f,Register8& r1,Register16 r2){
    cycles = 4;
    int temp = 0;
    if (f&0x10) {
        temp = 1;
    }
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    uint8_t temp3 = read_byte(r2.getData());
    temp1  = ~temp1 & r1.getData();
    temp2 = ~temp2 & temp3;
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1.getData() - ((int) r2.getData() -(int) temp) < 0) {
        f |= 0x10;
    } else if ((int) r1.getData() - ((int) temp3 - (int) temp) == 0) {
        f |= 0x80;
    }
    r1.setData(r1.getData() + (temp3+temp));
     f |= 0x40;
       
#ifdef DEBUG2
    std::cout << "subc_8_" << r1.getName() << "_" << r2.getName() << ", value: " << temp3 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void and_8(int &cycles,uint8_t &f, Register8 &r1,Register8 &r2) {
    cycles = 4;
    uint8_t res = r1.getData() & r2.getData();
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f |= 0x20;
        f &= ~0x10; 
        r1.setData(res);

#ifdef DEBUG2
    std::cout << "and_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void or_8(int &cycles,uint8_t &f, Register8 &r1,Register8 &r2) {
    cycles = 4;
    uint8_t res = r1.getData() | r2.getData();
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f &= ~0x20;
        f &= ~0x10; 
    r1.setData(res);
#ifdef DEBUG2
    std::cout << "or_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void xor_8(int &cycles,uint8_t &f, Register8 r1,Register8 r2) {
    uint8_t res = r1.getData() & r2.getData();
    cycles = 4;
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f |= 0x20;
        f &= ~0x10; 
    r1.setData(res);
#ifdef DEBUG2
    std::cout << "xor_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void cp_8(int& cycles,uint8_t& f ,Register8 r1,Register8 r2) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    temp1  = ~temp1 & r1.getData();
    temp2 = ~temp2 & r2.getData();
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1.getData() - (int) r2.getData() < 0) {
        f |= 0x10;
    } else if ((int) r1.getData() - (int) r2.getData() == 0) {
        f |= 0x80;
    }
    //r1 -= r2;
    f |= 0x40;

#ifdef DEBUG2
    std::cout << "cp_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void cp_8(int& cycles,uint8_t& f ,Register8 r1,Register16 r2) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    uint8_t temp3 = read_byte(r2.getData());
    temp1  = ~temp1 & r1.getData();
    temp2 = ~temp2 & temp3;
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1.getData() - (int) temp3 < 0) {
        f |= 0x10;
    } else if ((int) r1.getData() - (int) temp3 == 0) {
        f |= 0x80;
    }
    //r1 -= r2;
    f |= 0x40;

#ifdef DEBUG2
    std::cout << "cp_8_" << r1.getName() << "_" << r2.getName() <<", value: " << temp3 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void inc_8(int& cycles,uint8_t& f ,Register8& r1) {
    cycles = 4; 
    if ((int) r1.getData() + (int) 1 == 0) {
        f |= 0x80;
    }
    if (((r1.getData()&0xf) + (1&0xf))&0x10){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1.setData(r1.getData() + (uint8_t) 1); 

#ifdef DEBUG2
    std::cout << "inc_8_" << r1.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void dec_8(int& cycles,uint8_t& f ,Register8 r1) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    temp1  = ~temp1 & r1.getData();
    temp2 = ~temp2 & 1;
    if ((int) temp1 - (int) temp2) {
        f |=0x20;
    }
    if ((int) r1.getData() - (int) 1 == 0) {
        f |= 0x80;
    }
    r1.setData(r1.getData() - 1);
    f |= 0x40;

#ifdef DEBUG2
    std::cout << "dec_8_" << r1.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void add_16(int& cycles,uint8_t& f,Register16& r1,Register16& r2) {
    cycles = 8; 
    int a = r1.getData();
    int b = r2.getData();
    if ((int) a + (int) b > 65535) {
        f |= 0x10;
    } else if ((int) a + (int) b == 0) {
        f |= 0x80;
    }
    if (((a&0xfff) + (b&0xfff))&0x100){
        f |= 0x20;
    }
    f &= ~0x40; 
    r1.setData(a += b);


#ifdef DEBUG2
    std::cout << "add_16_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void add_16(int& cycles,uint8_t& f, Register16& r1,Register16 r2) {
    cycles = 8; 
    if ((int) r1.getData() + (int) r2.getData() > 65535) {
        f |= 0x10;
    } else if ((int) r1.getData() + (int) r2.getData() == 0) {
        f |= 0x80;
    }
    if (((r1.getData()&0xfff) + (r2.getData()&0xfff))&0x100){
        f |= 0x20;
    }
    f &= ~0x40; 
    uint16_t tmp = r1.getData();
    r1.setData(tmp +r2.getData());


#ifdef DEBUG2
    std::cout << "add_16_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void add_sp(int& cycles,uint8_t& f,Register16& r1,Register16 r2) {
    cycles = 16; 
    int a = r1.getData();
    int b = r2.getData();
    if ((int) a + (int) b > 65535) {
        f |= 0x10;
    } else if ((int) a + (int) b == 0) {
        f |= 0x80;
    }
    if (((a&0xfff) + (b&0xfff))&0x100){
        f |= 0x20;
    }
    f &= ~0x40; 
    f &= ~0x80;
    r1.setData(r1.getData() + r2.getData());

#ifdef DEBUG2
    std::cout << "add_sp_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void inc_16(int& cycles,Register16& r1) {
    cycles = 8;
    r1.setData(r1.getData() + 1);    

#ifdef DEBUG2
    std::cout << "dec_16_" << r1.getName() << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void dec_16(int& cycles,Register16& r1) {
    cycles = 8;
    r1.setData(r1.getData() - 1);    
#ifdef DEBUG2
    std::cout << "dec_16_" << r1.getName() << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void ccf(int& cycles,uint8_t &f) {
    cycles = 4;
    f &= ~0x60 & ~0x40;
    f ^= 0x20;

#ifdef DEBUG2
    std::cout << "ccf" << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void swap(int& cycles,uint8_t& f ,Register8& r) {
    uint8_t temp1;
    uint8_t temp2;
    cycles = 8;
    temp1 = r.getData() & ~0x80 & ~0x40 & ~0x20 & ~0x10;
    temp1 <<= 4;
    temp2 = r.getData() & ~0x01 & ~0x02 & ~0x03 & ~0x04;
    temp2 >>= 4;
    r.setData(temp1 + temp2); 

#ifdef DEBUG2
    std::cout << "swap" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void daa(int& cycles,uint8_t& f,Register8& r) {
    cycles = 4;
    uint8_t a = r.getData();
    if (!(f & 0x60)) {
        if(f & 0x40 || (a & 0xf) > 9) 
            a = a + 0x06;
        if(f & 0x20 || a > 0x9F)
            r.setData(a + 0x60);
    } else {
        if(f & 0x40)  
            r.setData((a-6) & 0xFF);
        if(f & 0x20)
            r.setData(0x60);
    }

    f &= ~(0x40 | 0x80);

    if ((a &0x100) == 0x100)
        f |= 0x20;

    a &= 0xFF;

    if (a == 0)
        f |= 0x80;
   
    r.setData(a);

#ifdef DEBUG2
    std::cout << "daa" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}


void cpl(int& cycles,uint8_t& f,Register8& r) {
    cycles = 4;
    f |= 0x60;  
    f |= 0x40;
    r.setData(~r.getData());

#ifdef DEBUG2
    std::cout << "cpl" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void scf(int& cycles, uint8_t& f) {
    cycles = 4;
    f |= 0x20;
    f &= ~0x60 & ~0x40;

#ifdef DEBUG2
    std::cout << "scf" << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RLCA(int& cycles,uint8_t& f,Register8& r) {
    uint8_t x = r.getData();
    if (0x80 & x) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    x <<= 1;
    x += 1;
    if (x == 0) {
        f |= 0x80;
       
    }
    cycles = 4;

#ifdef DEBUG2
    std::cout << "RLCA" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RLA(int& cycles,uint8_t& f,Register8& r) {
    uint8_t x = r.getData();
    cycles = 4;
     f &= ~0x60 & ~0x40;
    x <<= 1;
    x += 1;
    if (x == 0) {
        f |= 0x80;
       }
    r.setData(x);
#ifdef DEBUG2
    std::cout << "RLA_R" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RRCA(int& cycles,uint8_t& f,Register8& r) {
    uint8_t x = r.getData();
    if (0x01 & x) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    x>>= 1;
    x+=1;
    if (x == 0) {
        f |= 0x80;
       
    }
    cycles = 4;

#ifdef DEBUG2
    std::cout << "RRCA_r" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RRA(int& cycles,uint8_t& f,Register8& r) {
   uint8_t x = r.getData();
   cycles = 4;
     f &= ~0x60 & ~0x40;
    x <<= 1;
    x+=1;
    if (x == 0) {
        f |= 0x80;
    }
    r.setData(x);
#ifdef DEBUG2
    std::cout << "RRA_r" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void SLA(int& cycles,uint8_t& f,Register8& r) {
    uint8_t x = r.getData();
    if (0x80 & x) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    x <<= 1;
    x &= ~0x01;
    if (x == 0) {
        f |= 0x80;
       
    }
    cycles = 4;
    r.setData(x);
#ifdef DEBUG2
    std::cout << "SLA_" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
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

void SRL(int& cycles,uint8_t& f,Register8& r) {
    uint8_t x = r.getData();
    if (0x01 & x) {
        f |= 0x20;
    } else {
        f &= ~0x20;
    }
    f &= ~0x60 & ~0x40;
    x>>= 1;
    x &= ~0x80;
    if (x == 0) {
        f |= 0x80;
       
    }
    cycles = 8;
    r.setData(x);
#ifdef DEBUG2
    std::cout << "SRL_" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void cmpbit_b_r(int& cycles,uint8_t& f,Register8 &r1,Register8 r2) {
    cycles = 8;
    uint8_t a = r1.getData();
    uint8_t b = r2.getData();
    uint8_t tmp = 1 << b;
    if (!(tmp & a)) {
        f |= 0x80;
    }
    f &= ~0x60;
    f |= 0x40;

#ifdef DEBUG2
    std::cout << "cmpbit_" << r1.getName() << "_" << BIN(b) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void set_b_r(int& cycles,Register8 &r1,Register8 r2) {
    cycles = 8;
    r1.setData(r1.getData() |(1 << r2.getData()));
    
#ifdef DEBUG2
    std::cout << "set_" << r1.getName() << "_" << BIN(r2.getData()) << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void res_b_r(int& cycles,Register8 &r1,Register8 r2) {
    cycles = 8;
    uint8_t a = r1.getData();
    uint8_t b = r2.getData();
    a &= ~(1 << b);
    r1.setData(a);
#ifdef DEBUG2
    std::cout << "res_" << r1.getName() << "_" << BIN(b) << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void rst(int& cycles,Register16& sp, Register16& pc, int n) {
#ifdef DEBUG2
    std::cout << "rst_" << sp.getName() << "_" << pc.getName() << ", cycles: " << std::dec << 16 << std::endl << "  ";
#endif 
    push(cycles, sp, pc);
    cycles += 16; //In doc it says 32
    //pc = read_byte(pc);
    pc.setData(read_word(n));  
}


