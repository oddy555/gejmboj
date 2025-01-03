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

void LDD_8(int& cycles, Register16 &r1, Register8 &r2) {
    cycles = 8;
    uint16_t addr = r1.getData();
    write_byte(addr,r2.getData());
    r1.setData(addr-1);
#ifdef DEBUG2
    std::cout << "LDD_8_"<<"("<<r1.getName()<<")_"<<r2.getName()<<"_"<<std::hex<<int(addr)<<", Value: " <<int(r2.getData())<<", cycles: " << std::dec << cycles << std::endl; 
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

void LDI_8(int& cycles, Register16 &r1,Register8 &r2) {
    cycles = 8;
    uint16_t addr = r1.getData();
    write_byte(addr,r2.getData());
    r1.setData(addr+1);
#ifdef DEBUG2
    std::cout << "LDI_8_" << matchRegister(addr) << "_" << r.getName()<<",value: "<<r.getData()<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void LD_16_n_nn(int& cycles, Register16& r,uint16_t addr) {
    cycles = 12;
    r.setData(read_word(addr));

#ifdef DEBUG
    //std::cout << "LD_16_" << matchRegister(r) << "_" << std::hex << addr<<",value: "<< r  << ", cycles: " << std::dec << cycles << std::endl;
    std::cout << "LD " << r.getName() << " " << std::hex << read_word(addr) << "; " << addr << ":" << addr-1 << " Cycles: " << std::dec << cycles << " LD_16_n_nn" << std::endl; 
#endif
}

void LD_16_n_nn(int& cycles, uint16_t addr1,uint16_t addr2) {
    cycles = 12;
    write_word(read_word(addr1), addr2);

#ifdef DEBUG
    //std::cout << "LD_16_" << matchRegister(r) << "_" << std::hex << addr<<",value: "<< r  << ", cycles: " << std::dec << cycles << std::endl;
    std::cout << "LD " << r.getName() << " " << std::hex << read_word(addr) << "; " << addr << ":" << addr-1 << " Cycles: " << std::dec << cycles << " LD_16_n_nn" << std::endl; 
#endif
}

void LD_16_r_r(int& cycles, Register16& r1,Register16& r2) {
    cycles = 8;
    r1.setData(r2.getData());

#ifdef DEBUG2
    std::cout << "LD_16_" << r1.getName() << "_" << r2.getName() <<",value: "<< r1 << ", cycles: " << std::dec << cycles << std::endl;
#endif

}

void LD_16_r_r(int& cycles, Register16 & r1,uint16_t r2) {
    cycles = 8;
    r1.setData(read_word(r2));

#ifdef DEBUG2
    std::cout << "LD_16_" << r1.getName() << "_" << matchRegister(r2) <<",value: "<< r1 << ", cycles: " << std::dec << cycles << std::endl;
#endif

}
void push(int& cycles, Register16& r,Register16& sp) {
    cycles = 16;
    sp.setData_(sp.getData_()-2);
    write_word(sp.getData_(),r.getData());
    
#ifdef DEBUG2
    std::cout << "push_" << matchRegister(sp) << "_" << r.getName()<<",value: "<<r.getData()<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void push(int& cycles, uint16_t r, Register16& sp) {
    cycles = 16;
    sp.setData_(sp.getData_()-2);
    write_word(sp.getData_(),r);


#ifdef DEBUG2
    std::cout << "push_" << matchRegister(sp) << "_" <<",value: "<< r << ", cycles: " << std::dec << cycles << std::endl;
#endif
}
void pop(int& cycles, uint16_t& addr, Register16& sp) {
    cycles = 8;
    addr = read_word(sp.getData_());
    //sp.setData(sp.getData_());
    sp.inc(2);
#ifdef DEBUG2
    std::cout << "pop_" << matchRegister(r) << "_" << matchRegister(sp)<<",value: "<<r<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void pop(int& cycles,Register16& r, Register16& sp) {
    cycles = 12;

    if (r.getName() == "PC") {
        r.setData_(read_word(sp.getData_()));    
        sp.inc(2);
    } else {
        if (r.getLow()->getName() == "F") {
            r.getLow()->setData(read_byte(sp.getData_()) & 0xf0);
        } else {
            r.getLow()->setData(read_byte(sp.getData_()));
        }
        sp.inc(1);
        r.getHigh()->setData(read_byte(sp.getData_()));
        sp.inc(1);
    }
#ifdef DEBUG2
    std::cout << "pop_" << r.getName() << "_" << matchRegister(sp)<<",value: "<<r.getData()<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void add_8(int& cycles, Register8& f,Register8& r1,Register8 &r2) {
    cycles = 4;
    uint8_t a = r1.getData();
    uint8_t b = r2.getData();
    uint8_t flag = f.getData();
    int tmp = a + b;
    
    flag &= ~0xff;
    if (tmp & 0x100) {
        flag |= 0x10;
    }
    if (uint8_t(a + b) == 0) {
        flag |= 0x80;
    }

    if (((a&0xf) + (b&0xf))&0x10){
        flag |= 0x20;
    }
    flag &= ~0x40; 
    r1.setData(a + b);
    f.setData(flag);
#ifdef DEBUG2
    std::cout << "add_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(flag)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void add_8(int& cycles, Register8& f,Register8& r1,Register16 r2) {
    cycles = 8;
    uint8_t a = r1.getData();
    uint8_t b = read_byte(r2.getData());
    uint8_t flag = f.getData();
    int tmp = a + b;
    
    flag &= ~0xff;
    if (tmp & 0x100) {
        flag |= 0x10;
    }
    if (uint8_t(a + b) == 0) {
        flag |= 0x80;
    }

    if (((a&0xf) + (b&0xf))&0x10){
        flag |= 0x20;
    }
    flag &= ~0x40; 
    r1.setData(a + b);
    f.setData(flag);
#ifdef DEBUG2
    std::cout << "add_8_" << r1.getName() << "_" << r2.getName()<< ", value: " << t2 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void adc(int& cycles, Register8& f, Register8& r1,Register8 &r2) {
    cycles = 4;
    uint8_t a = r1.getData();
    uint8_t b = r2.getData();
    uint8_t flag = f.getData();
    uint8_t carry = 0; 
    carry = ((flag & 0x10) ? 1 : 0);  
    int tmp = a + b + carry ;
    flag &= ~0xff;
    if (tmp & 0x100) {
        flag |= 0x10;
    }
    if (uint8_t(a + b + carry) == 0) {
        flag |= 0x80;
    }

    if (((a&0xf) + (b&0xf) + carry)&0x10){
        flag |= 0x20;
    }
    flag &= ~0x40; 
    r1.setData(a + b + carry);
    f.setData(flag);
#ifdef DEBUG2
    std::cout << "adc_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void adc(int& cycles, Register8& f, Register8& r1,Register16 r2) {
    cycles = 8;
    uint8_t a = r1.getData();
    uint8_t b = read_byte(r2.getData());
    uint8_t flag = f.getData();
    uint8_t carry = 0; 
    carry = ((flag & 0x10) ? 1 : 0);  
    int tmp = a + b + carry ;
    flag &= ~0xff;
    if (tmp & 0x100) {
        flag |= 0x10;
    }
    if (uint8_t(a + b + carry) == 0) {
        flag |= 0x80;
    }

    if (((a&0xf) + (b&0xf) + carry)&0x10){
        flag |= 0x20;
    }
    flag &= ~0x40; 
    r1.setData(a + b + carry);
    f.setData(flag);
#ifdef DEBUG2
    std::cout << "adc_8_" << r1.getName() << "_" << r2.getName() << ",value: " << t2 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void sub_8(int& cycles, Register8& flag ,Register8& r1,Register8 &r2) {
    cycles = 4;
    uint8_t temp1 = 0x0f;
    uint8_t temp2 = 0x0f;
    temp1  = temp1 & r1.getData();
    temp2 = temp2 & r2.getData();
    uint8_t f = flag.getData();
    f = 0x40;
    int tmp = r1.getData() + (~r2.getData()+1); 
    if (tmp & 0x100) {
        f |= 0x10;
    }
    if (uint8_t(r1.getData() - r2.getData()) == 0) {
        f |= 0x80;
    }

    if (((r1.getData()&0x0f) + (~(r2.getData()&0x0f)+1))&0x10){
        f |= 0x20;
    }
    flag.setData(f);
    
    r1.setData(r1.getData() - r2.getData());
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "sub_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void sub_8(int& cycles, Register8& flag ,Register8 & r1,Register16& r2) {
    cycles = 8;
    uint8_t n = read_byte(r2.getData());
    uint8_t temp1 = 0x0f;
    uint8_t temp2 = 0x0f;
    temp1  = temp1 & r1.getData();
    temp2 = temp2 & n;
    uint8_t f = flag.getData();
    f = 0x40;
    int tmp = r1.getData() + (~n+1); 
    if (tmp & 0x100) {
        f |= 0x10;
    }
    if (uint8_t(r1.getData() - n) == 0) {
        f |= 0x80;
    }

    if (((r1.getData()&0x0f) + (~(n&0x0f)+1))&0x10){
        f |= 0x20;
    }
    flag.setData(f);
    
    r1.setData(r1.getData() - n);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "sub_8_" << r1.getName() << "_" << r2.getName() << ", value: " << temp3 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void subc(int& cycles,Register8& flag, Register8& r1,Register8& r2){
    cycles = 4;
    uint8_t f = flag.getData();
    int carry = (flag.getData() & 0x10) >> 4;

    f = 0x40;
    int tmp = r1.getData() - (r2.getData()+carry); 
    if (tmp < 0) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    
    if (uint8_t(r1.getData() - (r2.getData() + carry)) == 0) {
        f |= 0x80;
    } else {
         f &= ~0x80;
    }
    
    int tmp2 = r2.getData() & 0x0f;
    tmp2 += carry;
    tmp = r1.getData() & 0x0f;
    tmp = tmp - tmp2;

    if (tmp < 0) {
        f |= 0x20;
     } else {
        f &= ~0x20;
    }

    r1.setData(r1.getData() - (r2.getData() + ((flag.getData() & 0x10) >> 4)));
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "subc_8_" << matchRegister(r1) << "_" << matchRegister(r2) << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void subc(int& cycles, Register8& flag,Register8& r1,Register16& r2){
    cycles = 8;
    uint8_t f = flag.getData();
    int carry = (flag.getData() & 0x10) >> 4;
    int data = read_byte(r2.getData());
    f = 0x40;
    int tmp = r1.getData() - (data+carry); 
    if (tmp < 0) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    
    if (uint8_t(r1.getData() - (data + carry)) == 0) {
        f |= 0x80;
    } else {
         f &= ~0x80;
    }
    
    int tmp2 = data & 0x0f;
    tmp2 += carry;
    tmp = r1.getData() & 0x0f;
    tmp = tmp - tmp2;

    if (tmp < 0) {
        f |= 0x20;
     } else {
        f &= ~0x20;
    }

    r1.setData(r1.getData() - (data + ((flag.getData() & 0x10) >> 4)));
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "subc_8_" << r1.getName() << "_" << r2.getName() << ", value: " << temp3 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void and_8(int &cycles,Register8 &flag, Register8 &r1,Register8 &r2) {
    cycles = 4;
    uint8_t f = flag.getData();
    uint8_t res = r1.getData() & r2.getData();
    if (res == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
        f &= ~0x40;
        f |= 0x20;
        f &= ~0x10; 
        r1.setData(res);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "and_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void and_8(int &cycles,Register8 &flag, Register8 &r1,Register16 &r2) {
    cycles = 8;
    uint8_t f = flag.getData();
    uint8_t res = r1.getData() & read_byte(r2.getData());
    if (res == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
        f &= ~0x40;
        f |= 0x20;
        f &= ~0x10; 
        r1.setData(res);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "and_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void or_8(int &cycles,Register8 &flag, Register8 &r1,Register8 &r2) {
    cycles = 4;
    uint8_t f = flag.getData();
    uint8_t res = r1.getData() | r2.getData();
    if (res == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
        f &= ~0x40;
        f &= ~0x20;
        f &= ~0x10; 
    r1.setData(res);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "or_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void or_8(int &cycles,Register8 &flag, Register8 &r1,Register16 &r2) {
    cycles = 4;
    uint8_t f = flag.getData();
    uint8_t res = r1.getData() | read_byte(r2.getData());
    if (res == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
        f &= ~0x40;
        f &= ~0x20;
        f &= ~0x10; 
    r1.setData(res);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "or_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void xor_8(int &cycles,Register8 &flag, Register8& r1,Register8& r2) {
    uint8_t res = r1.getData() ^ r2.getData();
    cycles = 4;
    uint8_t f = flag.getData();
    if (res == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
        f &= ~0x40;
        f &= ~0x20;
        f &= ~0x10; 
    r1.setData(res);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "xor_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void xor_8(int &cycles,Register8 &flag, Register8& r1,Register16& r2) {
    uint8_t res = r1.getData() ^ read_byte(r2.getData());
    cycles = 4;
    uint8_t f = flag.getData();
    f &= ~0x80;
    if (res == 0) {
        f |= 0x80;
    }
        f &= ~0x40;
        f &= ~0x20;
        f &= ~0x10; 
    r1.setData(res);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "xor_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}


void cp_8(int& cycles,Register8& flag ,Register8& r1,Register8& r2) {
    cycles = 4;
    uint8_t temp1 = 0x0f;
    uint8_t temp2 = 0x0f;
    temp1  = temp1 & r1.getData();
    temp2 = temp2 & r2.getData();
    uint8_t f = flag.getData();
    f = 0x40;
    int tmp = r1.getData() + (~r2.getData()+1); 
    if (tmp & 0x100) {
        f |= 0x10;
    }
    if (uint8_t(r1.getData() - r2.getData()) == 0) {
        f |= 0x80;
    }

    if (((r1.getData()&0x0f) + (~(r2.getData()&0x0f)+1))&0x10){
        f |= 0x20;
    }
    
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "cp_8_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void cp_8(int& cycles, Register8& flag ,Register8& r1,Register16& r2) {
    cycles = 8;
    uint8_t f = flag.getData();
    int n = read_byte(r2.getData());
    f = 0x40;
    int tmp = r1.getData() - (n); 
    if (tmp < 0) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    
    if (uint8_t(r1.getData() - (n)) == 0) {
        f |= 0x80;
    } else {
         f &= ~0x80;
    }
    
    int tmp2 = n & 0x0f;
    tmp = r1.getData() & 0x0f;
    tmp = tmp - tmp2;

    if (tmp < 0) {
        f |= 0x20;
     } else {
        f &= ~0x20;
    }

    //r1.setData(r1.getData() - (data + ((flag.getData() & 0x10) >> 4)));
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "cp_8_" << r1.getName() << "_" << r2.getName() <<", value: " << temp3 << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void inc_8(int& cycles,Register8& f ,Register8& r1) {
    cycles = 4; 
    uint8_t flag = f.getData();
    uint8_t tmp = r1.getData() + 1;
    if (tmp == 0) {
        flag |= 0x80;
    } else {
        flag &= ~0x80;
    }
   
    if (((r1.getData()&0xf) + (1&0xf))&0x10){
        flag |= 0x20;
    } else {
        flag &= ~0x20;   
    }
    flag &= ~0x40; 
    f.setData(flag);
    r1.setData(r1.getData() + (uint8_t) 1); 

#ifdef DEBUG2
    std::cout << "inc_8_" << r1.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void inc_8(int& cycles,Register8& f , uint16_t addr) {
    cycles = 12; 
    uint8_t flag = f.getData();
    if ((read_byte(addr) + (uint8_t) 1) == 256) {
        flag |= 0x90;
    } else {
        flag &= ~0x80;
    }
    if (((read_byte(addr)&0xf) + (1&0xf))&0x10){
        flag |= 0x20;
    } else {
        flag &= ~0x20;
    }
    flag &= ~0x40;
    write_byte(addr, read_byte(addr)+1);
    f.setData(flag);
    //write_byte(addr, read_byte(addr) + (uint8_t) 1); 

#ifdef DEBUG2
    std::cout << "inc_8_" << r1.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void dec_8(int& cycles, Register8& f ,Register8& r1) {
    cycles = 4;
    uint8_t temp1 = 0x0f;
    uint8_t temp2 = 0x0f;
    uint8_t flag = f.getData();
    temp1  = temp1 & r1.getData();
    temp2 = temp2 & 1;
    int temp3 = ((int) temp1 - (int) temp2);
    if ( temp3 >= 0) {
        flag &=~0x20;
    } else {
        flag |=0x20;
    }
    if ((int) r1.getData() - (int) 1 == 0) {
        flag |= 0x80;
    } else {
        flag &= ~0x80;
    }
    r1.setData(r1.getData() - 1);
    flag |= 0x40;
    f.setData(flag);
#ifdef DEBUG2
    std::cout << "dec_8_" << r1.getName() << ", f: " << std::hex <<int(f.getData())<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void dec_8(int& cycles, Register8& f ,uint16_t addr) {
    cycles = 4;
    uint8_t temp1 = 0xf0;
    uint8_t temp2 = 0xf0;
    uint8_t flag = f.getData();
    temp1  = ~temp1 & read_byte(addr);
    temp2 = ~temp2 & 1;
    if (((int) temp1 - (int) temp2) >= 0) {
        flag &= ~0x20;
    } else {
        flag |= 0x20;
    }
    if ((int) read_byte(addr) - (int) 1 == 0) {
        flag |= 0x80;
    } else {
        flag &= ~0x80;
    }
    write_byte(addr,read_byte(addr) - 1);
    flag |= 0x40;
    f.setData(flag);
#ifdef DEBUG2
    std::cout << "dec_8_" << r1.getName() << ", f: " << std::hex <<int(f.getData())<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}


void add_16(int& cycles, Register8& flag,Register16& r1,Register16& r2) {
    cycles = 8; 
    int a = r1.getData();
    int b = r2.getData();
    int ab = a + b;
    uint8_t f = flag.getData();
    if ((ab) > 65535) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }

    if (((a&0xfff) +(b&0xfff))&0x1000){
        f |= 0x20;
    } else {
        f &= ~0x20;    
    }

    f &= ~0x40; 
    r1.setData(a += b);
    flag.setData(f);

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

void add_sp(int& cycles,Register8& flag,Register16& r1,Register16& r2) {
    cycles = 16; 
    int a = r1.getData_();
    int b = (int8_t) read_byte(r2.getData_());
    uint8_t f = flag.getData();
    if (((a&0x00ff) + (b&0x00ff)) > 0x00ff) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }

    /*if (((int) a)  + ((int) b) > 0xff) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }*/

    /*if ((((a&0x0fff) + (b&0x0fff))>0xfff) || (((a&0x00ff) + (b&0x00ff)) > 0x00ff) || (((a&0x000f) + (b&0x000f)) > 0x000f)){
        f |= 0x20;
    } else {
        f &= ~0x20;
    }*/
    if ((((a&0x000f) + (b&0x000f)) > 0x000f)){
        f |= 0x20;  
    } else {
        f &= ~0x20;
    }
    f &= ~0xc0;

    r1.setData_(r1.getData_() + ((int8_t) read_byte(r2.getData())));
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "add_sp_" << r1.getName() << "_" << r2.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif
}

void inc_16(int& cycles,Register16& r1) {
    cycles = 8;
    r1.setData(r1.getData() + 1);    

#ifdef DEBUG2
    std::cout << "inc_16_" << r1.getName() << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void dec_16(int& cycles,Register16& r1) {
    cycles = 8;
    r1.setData(r1.getData() - 1);    
#ifdef DEBUG2
    std::cout << "dec_16_" << r1.getName() << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void ccf(int& cycles, Register8& flag) {
    uint8_t f = flag.getData();
    std::cout << "HEJ1: " << int(f) << std::endl;
    cycles = 4;
    f &= ~0x60;
    std::cout << "HEJ2: " << int(f) << std::endl;
    if (f & 0x10) {
        f &= ~0x10;
    } else {
        f |= 0x10;
    }
    std::cout << "HEJ3: " << int(f) << std::endl;
    flag.setData(f);
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

void daa(int& cycles, Register8& flag, Register8& r) {
    cycles = 4;
    uint8_t a = r.getData();
    uint8_t f = flag.getData();
    uint8_t offset = 0;
    if ((!(f & 0x40) && ((a & 0xf) > 0x09)) || f & 0x20) {
        offset |= 0x06;
    } 
    f &= ~0x20;
    if ( (!(f & 0x40) && (a > 0x99)) || f & 0x10) {
        offset |= 0x60;
        f |= 0x10;
    } else {
        f &= ~0x10;
    }

    if (f & 0x40) {
        a -= offset;
    } else {
        a += offset;
    }
    if (a == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }

    /*if (!(f & 0x60)) {
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

    f &= ~( 0x80);

    if ((a &0x100) == 0x100)
        f |= 0x20;

    a &= 0xFF;

    if (a == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }*/
   
    r.setData(a);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "daa" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}


void cpl(int& cycles,Register8& flag,Register8& r) {
    cycles = 4;
    uint8_t f = flag.getData();
    f |= 0x60;  
    f |= 0x40;
    r.setData(~r.getData());
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "cpl" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void scf(int& cycles, Register8& flag) {
    cycles = 4;
    uint8_t f = flag.getData();
    f |= 0x10;
    f &= ~0x60 & ~0x40;
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "scf" << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RLCA(int& cycles, Register8& flag, Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();
    uint8_t prevMsb = (x & 0x80)>>7;
    if (0x80 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60 & ~0x40;
    x <<= 1;
    x |= prevMsb;
    if (x == 0) {
        f |= 0x80;
       
    }
    f &= ~0x80;
    cycles = 4;
    r.setData(x);
flag.setData(f);
#ifdef DEBUG2
    std::cout << "RLCA" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RLC(int& cycles, Register8& flag, Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();
    uint8_t prevMsb = (x & 0x80)>>7;
    if (0x80 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60 & ~0x40;
    x <<= 1;
    x |= prevMsb;
    if (x == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
    r.setData(x);
    cycles = 4;
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "RLC" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void RLC(int& cycles, Register8& flag, Register16& r) {
    uint8_t x = read_byte(r.getData());
    uint8_t f = flag.getData();
    uint8_t prevMsb = (x & 0x80)>>7;
    if (0x80 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60 & ~0x40;
    x <<= 1;
    x |= prevMsb;
    if (x == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }

    cycles = 16;
flag.setData(f);
write_byte(r.getData(), x);
#ifdef DEBUG2
    std::cout << "RLC" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void RLA(int& cycles,Register8& flag,Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();
    uint8_t carry = (f & 0x10 ) >> 4;
    cycles = 4;
    if (x & 0x80)
        f |= 0x10;
    else
        f &= ~0x10;
    f &= ~0x60 & ~0x40;
    x <<= 1;
    x |= carry;
    /*if (x == 0) {
        f |= 0x80;
       }*/
    f &= ~0xa0;
    r.setData(x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "RLA_R" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RLA(int& cycles,Register8& flag,Register16& r) {
    uint8_t x = read_byte(r.getData());
    uint8_t f = flag.getData();
    uint8_t carry = (f & 0x10 ) >> 8;
    cycles = 16;
    if (x & 0x80)
        f |= 0x10;
    else
        f &= ~0x10;
    f &= ~0x60 & ~0x40;
    x <<= 1;
    x |= carry;
    /*if (x == 0) {
        f |= 0x80;
       }*/
    //f &= ~0x80;
    write_byte(r.getData(), x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "RLA_R" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void RL(int& cycles,Register8& flag,Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();
    uint8_t carry = (f & 0x10 ) >> 4;
    cycles = 8;
    if (x & 0x80) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60;
    x <<= 1;
    x |= carry;

    if (x == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
    r.setData(x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "RL_R" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void RL(int& cycles,Register8& flag,Register16& r) {
    uint8_t x = read_byte(r.getData());
    uint8_t f = flag.getData();
    uint8_t carry = (f & 0x10 ) >> 4;
    cycles = 16;
    if (x & 0x80) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60;
    x <<= 1;
    x |= carry;
    if (x == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
    write_byte(r.getData(),x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "RL_R" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RRCA(int& cycles,Register8& flag,Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();
    uint8_t prevMSB = (x & 0x01) << 7;
    if (0x01 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60 & ~0x40;
    x>>= 1;
    x += prevMSB;
    if (x == 0) {
        f |= 0x80;
       
    }
    f &= ~0x80;
    cycles = 4;
    flag.setData(f);
    r.setData(x);
#ifdef DEBUG2
    std::cout << "RRCA_r" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void RRC(int& cycles,Register8& flag,Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();
    uint8_t prevMSB = (x & 0x01) << 7;
    if (0x01 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60 & ~0x40;
    x>>= 1;
    x += prevMSB;
    if (x == 0) {
        f |= 0x80;
       
    } else {
        f &= ~0x80;
    }
    cycles = 8;
    flag.setData(f);
    r.setData(x);
#ifdef DEBUG2
    std::cout << "RRC_r" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void RRC(int& cycles,Register8& flag,Register16& r) {
    uint8_t x = read_byte(r.getData());
    uint8_t f = flag.getData();
    uint8_t prevMSB = (x & 0x01) << 7;
    if (0x01 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60 & ~0x40;
    x>>= 1;
    x += prevMSB;
    if (x == 0) {
        f |= 0x80;
       
    } else {
        f &= ~0x80;
    }
    cycles = 16;
    flag.setData(f);
    write_byte(r.getData(),x);
#ifdef DEBUG2
    std::cout << "RRC_r" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RRA(int& cycles,Register8& flag,Register8& r) {
   uint8_t x = r.getData();
   uint8_t f = flag.getData();
   uint8_t carry = (f & 0x10) << 3;  
   f &= ~0x10;
   if (x & 0x01) {
    f |= 0x10;
   } else {
    f &= ~0x01;
   }
   cycles = 4;
   x >>= 1;
   x|=carry;
   if (x == 0) {
    f |= 0x80;
   } else {
    f &=  ~0x80;
   }
   f &=  ~0x60;
   r.setData(x);
   flag.setData(f);
#ifdef DEBUG2
    std::cout << "RRA_r" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RR(int& cycles,Register8& flag, Register8& r) {
   uint8_t x = r.getData();
   uint8_t f = flag.getData();
   uint8_t carry = (f & 0x10) << 3 ;  
   cycles = 8;
   if (x & 0x01) {
    f |= 0x10;
   } else {
    f &= ~0x10;
   }
   cycles = 8;
   f &= ~0x60;
   x >>= 1;
   x|=carry;
   
   if (x == 0) {
       f |= 0x80;
   } else {
        f &= ~0x80;
   }
   r.setData(x);
   flag.setData(f);
#ifdef DEBUG2
    std::cout << "RRA_r" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void RR(int& cycles,Register8& flag,Register16& r) {
    uint8_t x = read_byte(r.getData());
    uint8_t f = flag.getData();
    uint8_t carry = (f & 0x10) << 3 ;  
    cycles = 16;
    if (x & 0x01) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }

    f &= ~0x60;
    x >>= 1;
    x+=carry;
    f &= ~0x80;
    if (x == 0) {
        f |= 0x80;
    }
    //write_word(r.getData(), x)
    flag.setData(f);
    write_byte(r.getData(),x);
#ifdef DEBUG2
    std::cout << "RRA_r" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void SLA(int& cycles, Register8& flag, Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();
    if (0x80 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60;
    x <<= 1;
    x &= ~0x01;
    f &= ~0x80;
    if (x == 0) {
        f |= 0x80; 
    }
    cycles = 8;
    r.setData(x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "SLA_" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void SLA(int& cycles, Register8& flag, Register16& r) {
    uint8_t x = read_byte(r.getData());
    uint8_t f = flag.getData();
    if (0x80 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60 & ~0x40;
    x <<= 1;
    x &= ~0x01;
    f &= ~0x80;
    if (x == 0) {
        f |= 0x80;
       
    }
    cycles = 16;
    write_byte(r.getData(),x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "SLA_" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void SRA(int& cycles, Register8& flag, Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();
    uint8_t msb = x & 0x80;
    if (0x01 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60;
    x >>= 1;
    x += msb;
    //x &= ~0x01;
    if (x == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;    
    }
    cycles = 8;
    r.setData(x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "SRA_" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void SRA(int& cycles, Register8& flag, Register16& r) {
    uint8_t x = read_byte(r.getData());
    uint8_t f = flag.getData();
    uint8_t msb = x & 0x80;
    if (0x01 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60;
    x >>= 1;
    x += msb;
    if (x == 0) {
        f |= 0x80;  
    } else {
        f &= ~0x80;    
    }
    cycles = 16;
    write_byte(r.getData(),x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "SRA_" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
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

void SRL(int& cycles, Register8& flag,Register8& r) {
    uint8_t x = r.getData();
    uint8_t f = flag.getData();

    if (0x01 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60;
    x >>= 1;
    x &= ~0x80;
    f &= ~0x80;
    if (x == 0) {
        f |= 0x80;
       
    }
    cycles = 8;
    r.setData(x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "SRL_" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void SRL(int& cycles,Register8& flag,Register16& r) {
    uint8_t x = read_byte(r.getData());
    uint8_t f = flag.getData();
    if (0x01 & x) {
        f |= 0x10;
    } else {
        f &= ~0x10;
    }
    f &= ~0x60;
    x >>= 1;
    x &= ~0x80;
    if (x == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
    cycles = 16;
    write_byte(r.getData(),x);
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "SRL_" << r.getName() << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void SWAP(int& cycles, Register8& flag, Register8& r) {
    uint8_t f = flag.getData();
    uint8_t data = r.getData();
    uint8_t upper = data & 0xf0;
    uint8_t lower = data & 0x0f;
    data = (upper >>4) | (lower << 4);
    if (data == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
    f &= ~0x70;
    flag.setData(f);
    r.setData(data);
    cycles = 8;
}


void SWAP(int& cycles, Register8& flag, Register16& r) {
    uint8_t f = flag.getData();
    uint8_t data = read_byte(r.getData());
    uint8_t upper = data & 0xf0;
    uint8_t lower = data & 0x0f;
    data = (upper >>4) | (lower << 4);
    if (data == 0) {
        f |= 0x80;
    } else {
        f &= ~0x80;
    }
    f &= ~0x70;
    flag.setData(f);
    write_byte(r.getData(),data);
    cycles = 16;
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
    std::cout << "cmpbit_" << r1.getName() << "_" << bit << ", f: " << std::hex <<int(f)<< ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void set_b_r(int& cycles, Register8& flag, uint8_t bit, Register8& r) {
    cycles = 8;
    r.setData(r.getData() | (1 << bit));
     
#ifdef DEBUG2
    std::cout << "set_" << r1.getName() << "_" << bit << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void set_b_r(int& cycles, Register8& flag, uint8_t bit, Register16& r) {
    cycles = 16;
    write_byte(r.getData(),read_byte(r.getData()) | (1 << bit));
     
#ifdef DEBUG2
    std::cout << "set_" << r1.getName() << "_" << bit << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void res_b_r(int& cycles, Register8& flag, uint8_t bit, Register8& r) {
    cycles = 8;
    r.setData(r.getData() & ~(1 << bit));
     
#ifdef DEBUG2
    std::cout << "res_" << r1.getName() << "_" << bit << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void res_b_r(int& cycles, Register8& flag, uint8_t bit, Register16& r) {
    cycles = 16;
    write_byte(r.getData(),read_byte(r.getData()) & ~(1 << bit));
     
#ifdef DEBUG2
    std::cout << "res_" << r1.getName() << "_" << bit << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void bit_b_r(int& cycles, Register8& flag, uint8_t bit, Register8& r) {
    cycles = 8;
    uint8_t f = flag.getData();
    if ((1 << bit) & r.getData()) {
        f &= ~0x80;
    } else {
        f |= 0x80;
    }
    f &= ~0x40;
    f |= 0x20;
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "bit_" << r1.getName() << "_" << bit << tmpF << f << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}

void bit_b_r(int& cycles, Register8& flag, uint8_t bit, Register16& r) {
    cycles = 16;
     
    uint8_t f = flag.getData();
    if ((1 << bit) & read_byte(r.getData())) {
        f &= ~0x80;
    } else {
        f |= 0x80;
    }
    f |= 0x20;
    f &= ~0x40;
    flag.setData(f);
#ifdef DEBUG2
    std::cout << "bit_" << r1.getName() << "_" << bit << ", cycles: " << std::dec << cycles << std::endl;
#endif 
}
void rst(int& cycles,Register16& sp, Register16& pc, int n) {
#ifdef DEBUG2
    std::cout << "rst_" << sp.getName() << "_" << pc.getName() << ", cycles: " << std::dec << 16 << std::endl << "  ";
#endif 
    push(cycles, pc, sp);
    cycles += 32; //In doc it says 32
    //pc = read_byte(pc);
    pc.setData(n);  
}


