#include <cstdint>
#include "../inc/register.hpp"
#include "../inc/instructions.hpp"
#include "gtest/gtest.h"
#include "../inc/memory.hpp"
#include <fstream>

TEST (InstructionTestLD, LD_8_n_nn) {
    //Setup Registers
    Register8 B(113,"B");
    Register8 C(201,"C");
    Register16 BC(&B,&C);
    //Register16 PC(1111,"PC");

    init_mem();
    write_byte(1111,55);
    int c = 0;
    LD_8_n_nn(c, C,1111);
    EXPECT_EQ(C.getData(),55);
    EXPECT_EQ(B.getData(),113);
    EXPECT_EQ(BC.getData(),113 << 8 | 55);
    free_memory();
}

TEST (InstructionsTestLD, LD_8_r_r) {
    Register8 D(25,"D");
    Register8 E(32,"E");
    Register16 DE(&D,&E);
    Register8 B(255,"B");
    int c = 0;
    LD_8_r_r(c, D, B);    
    EXPECT_EQ(D.getData(), 255);
    EXPECT_EQ(E.getData(), 32);
    EXPECT_EQ(DE.getData(), 255 << 8 |32);    
}

TEST (InstructionsTestLD, LD_8_rn_r) {
    //Setup Registers
    Register8 B(113,"B");
    Register8 C(201,"C");
    Register16 BC(&B,&C);
    //Register16 PC(1111,"PC");

    init_mem();
    //write_byte(1111,55);
    int c = 0;
    LD_8_r_r(c, 1111,C);
    EXPECT_EQ(read_byte(1111),201);
    EXPECT_EQ(B.getData(),113);
    EXPECT_EQ(BC.getData(),113 << 8 | 201);
    free_memory();
}

TEST (InstructionsTestLD, LD_8_rn_rn) {
    //Setup Registers

    init_mem();
    write_byte(1111,55);
    write_byte(2232, 102);

    int c = 0;
    LD_8_r_r(c, 1111,2232);
    EXPECT_EQ(read_byte(1111),102);
    EXPECT_EQ(read_byte(2232),102);
    free_memory();
}

TEST (InstructionsTestLD, LD_16_r_r) {
    //Setup Registers
    Register8 B(113,"B");
    Register8 C(201,"C");
    Register16 BC(&B,&C);
    Register8 D(4,"D");
    Register8 E(87,"E");
    Register16 DE(&D,&E);

    int c = 0;
    LD_16_r_r(c, BC,DE);
    EXPECT_EQ(C.getData(),87);
    EXPECT_EQ(B.getData(),4);
    EXPECT_EQ(BC.getData(),DE.getData());
}


TEST (InstructionsTestLD, LD_16_r_rn) {
    //Setup Registers
    Register8 B(113,"B");
    Register8 C(201,"C");
    Register16 BC(&B,&C);
    init_mem();
    int c = 0;
    write_word(2222, 512);
    LD_16_r_r(c, BC,2222);
    EXPECT_EQ(BC.getData(),512);
    free_memory();
}

TEST (InstructionsTestLD, LD_16_n_nn) {
    Register16 SP(0, "SP");
    Register8 B(113,"B");
    Register8 C(201,"C");
    Register16 BC(&B,&C);

    init_mem();
    write_word(1111,55);
    int cycles = 0;
    LD_16_n_nn(cycles, BC, 1111);
    LD_16_n_nn(cycles, SP, 1111);
    //EXPECT_EQ(C.getData(),55);
    //EXPECT_EQ(B.getData(),113);
    EXPECT_EQ(BC.getData(), 55);
    EXPECT_EQ(SP.getData_(),55);
    free_memory();
}

TEST (InstructionsTestLD,LDD_8) {
    Register8 H(152,"H");
    Register8 L(240,"L");
    Register8 E(23,"E");
    Register16 HL(&H,&L);
    int c = 0;
    uint16_t decHL = HL.getData() - 1;
    init_mem();
    LDD_8(c,HL,E);
    EXPECT_EQ(read_byte(HL.getData()+1),E.getData());
    EXPECT_EQ(HL.getData(),decHL);
    free_memory();

}

TEST (InstructionsTestLD,LDI_8) {
    Register8 H(152,"H");
    Register8 L(240,"L");
    Register8 E(23, "E");
    Register16 HL(&H,&L);
    int c = 0;
    uint16_t incHL = HL.getData() + 1;
    init_mem();
    LDI_8(c,HL,E);
    EXPECT_EQ(read_byte(HL.getData()-1),E.getData());
    EXPECT_EQ(HL.getData(),incHL);
    free_memory();

}

TEST (InstructionsTest_stack,push_addr) {
    uint16_t addr = 5555;
    Register16 SP(52,"SP");
    int c = 0;
    init_mem();
    push(c,addr,SP);
    EXPECT_EQ(read_word(SP.getData()+2),5555);
    EXPECT_EQ(SP.getData(),50);    
    free_memory();
}


TEST (InstructionsTest_stack,push_reg) {
    Register8 D(0x15,"D");
    Register8 E(0xB3,"E");
    Register16 DE(&D,&E);
    Register16 SP(52,"SP");
    int c = 0;
    init_mem();
    push(c,DE,SP);
    EXPECT_EQ(read_word(SP.getData()+2),5555);
    EXPECT_EQ(SP.getData(),50);    
    free_memory();
}

TEST (InstructionsTest_stack,pop_addr) {
    uint16_t addr;
    Register16 SP(52,"SP");
    int c = 0;
    init_mem();
    write_word(52,5555);
    pop(c,addr,SP);
    EXPECT_EQ(addr,5555);
    EXPECT_EQ(SP.getData(),54);
    free_memory();
}


TEST (InstructionsTest_stack,pop_reg) {
    Register8 D(0x15,"D");
    Register8 E(0xB3,"E");
    Register16 DE(&D,&E);
    Register16 SP(52,"SP");
    int c = 0;
    init_mem();
    write_word(52,6666);
    pop(c,DE,SP);
    EXPECT_EQ(DE.getData(),6666);
    EXPECT_EQ(SP.getData(),54);    
    free_memory();
}

TEST (InstructionsTest_Operator, add_8) {
    Register8 B(2, "B");
    Register8 E(3, "E");
    Register8 F(0xf0,"F");
    int c = 0;
    add_8(c,F,B,E);
    EXPECT_EQ(B.getData(),5);
    EXPECT_EQ(F.getData(),0x00);
}


TEST (InstructionsTest_Operator, add_8_FlagTest) {

    // Zero flag + carry
    Register8 B(128, "B");
    Register8 E(128, "E");
    Register8 F(0xf0,"F");
    int c = 0;
    add_8(c,F,B,E);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0x90);

    //Half carry  + zero flags + carry
    B.setData(250);
    E.setData(6);
    F.setData(0x00);
    add_8(c,F,B,E);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0xb0);

    //Carry but nonzero
    B.setData(250);
    E.setData(20);
    F.setData(0x00);
    add_8(c,F,B,E);
    EXPECT_EQ(B.getData(),14);
    EXPECT_EQ(F.getData(),0x10);
}


TEST (InstructionsTest_Operator, add_8n) {
    Register8 B(2, "B");
    Register8 H(0xff, "H");
    Register8 L(0x00, "L");
    Register16 HL(&H,&L);
    Register8 F(0xf0,"F");
    int c = 0;
    init_mem();
    write_byte(0xff00,3);
    add_8(c,F,B,HL);
    EXPECT_EQ(B.getData(),5);
    EXPECT_EQ(F.getData(),0x00);
    free_memory();
}


TEST (InstructionsTest_Operator, add_8n_FlagTest) {

    // Zero flag + carry
    Register8 B(128, "B");
    Register8 H(0xff, "H");
    Register8 L(0x00, "L");
    Register16 HL(&H,&L);
    Register8 F(0xf0,"F");
    init_mem();
    write_byte(0xff00,128);
    int c = 0;
    add_8(c,F,B,HL);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0x90);

    //Half carry  + zero flags + carry
    B.setData(250);
    write_byte(0xff00,6);
    F.setData(0x00);
    add_8(c,F,B,HL);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0xb0);

    //Carry but nonzero
    B.setData(250);
    write_byte(0xff00,20);
    F.setData(0x00);
    add_8(c,F,B,HL);
    EXPECT_EQ(B.getData(),14);
    EXPECT_EQ(F.getData(),0x10);
    free_memory();
}


TEST (InstructionsTest_Operator, adc_8) {
    Register8 B(2, "B");
    Register8 E(3, "E");
    Register8 F(0x10,"F");
    int c = 0;
    adc(c,F,B,E);
    EXPECT_EQ(B.getData(),6);
    EXPECT_EQ(F.getData(),0x00);

    B.setData(2);
    E.setData(3);
    F.setData(0x00);

    adc(c,F,B,E);
    EXPECT_EQ(B.getData(),5);
    EXPECT_EQ(F.getData(),0x00);
}


TEST (InstructionsTest_Operator, adc_8_FlagTest) {

    // Zero flag + carry
    Register8 B(128, "B");
    Register8 E(128, "E");
    Register8 F(0xf0,"F");
    int c = 0;
    add_8(c,F,B,E);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0x90);

    //Half carry  + zero flags + carry
    B.setData(250);
    E.setData(6);
    F.setData(0x00);
    add_8(c,F,B,E);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0xb0);

    //Carry but nonzero
    B.setData(250);
    E.setData(20);
    F.setData(0x00);
    add_8(c,F,B,E);
    EXPECT_EQ(B.getData(),14);
    EXPECT_EQ(F.getData(),0x10);
}


TEST (InstructionsTest_Operator, adc_8n) {
    Register8 B(2, "B");
    Register8 H(0xff, "H");
    Register8 L(0x00, "L");
    Register16 HL(&H,&L);
    Register8 F(0xf0,"F");
    int c = 0;
    init_mem();
    write_byte(0xff00,3);
    add_8(c,F,B,HL);
    EXPECT_EQ(B.getData(),5);
    EXPECT_EQ(F.getData(),0x00);
    free_memory();
}


TEST (InstructionsTest_Operator, adc_8n_FlagTest) {

    // Zero flag + carry
    Register8 B(128, "B");
    Register8 H(0xff, "H");
    Register8 L(0x00, "L");
    Register16 HL(&H,&L);
    Register8 F(0xf0,"F");
    init_mem();
    write_byte(0xff00,128);
    int c = 0;
    add_8(c,F,B,HL);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0x90);

    //Half carry  + zero flags + carry
    B.setData(250);
    write_byte(0xff00,6);
    F.setData(0x00);
    add_8(c,F,B,HL);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0xb0);

    //Carry but nonzero
    B.setData(250);
    write_byte(0xff00,20);
    F.setData(0x00);
    add_8(c,F,B,HL);
    EXPECT_EQ(B.getData(),14);
    EXPECT_EQ(F.getData(),0x10);
    free_memory();
}

TEST (InstructionsTest_Operator, sub_8) {
    Register8 B(3,"B");
    Register8 E(2,"E");
    Register8 F(0xff,"F");
    int c = 0;
    sub_8(c, F, B, E);
    EXPECT_EQ(B.getData(),1);
    EXPECT_EQ(F.getData(),0x40);

    B.setData(2);
    E.setData(3);
    sub_8(c, F, B, E);
    EXPECT_EQ(B.getData(),255);
    EXPECT_EQ(F.getData(),0x70);
    
    B.setData(3);
    sub_8(c,F,B,E);
    EXPECT_EQ(B.getData(),0);
    EXPECT_EQ(F.getData(),0xC0);
}

TEST (InstructionsTest_Operator, sub_8n) {
    Register8 B(3,"B");
    //Register8 E(2,"E");
    Register8 H(0xff,"H");
    Register8 L(0xff,"L");
    Register8 F(0xff,"F");
    Register16 HL(&H,&L);
    init_mem();
    write_byte(0xFFFF,2);
    int c = 0;
    sub_8(c, F, B, HL);
    EXPECT_EQ(B.getData(),1);
    EXPECT_EQ(F.getData(),0x40);

    B.setData(2);
    write_byte(0xffff,3);
    sub_8(c, F, B, HL);
    EXPECT_EQ(B.getData(),255);
    EXPECT_EQ(F.getData(),0x70);
    free_memory();
}

TEST (LogicalInstructionsTest, and_8) {
    Register8 A(0xff,"A");
    Register8 D(0xff, "D");
    Register8 F(0x00, "F");
    int c = 0;
    and_8(c,F,A,D);

    EXPECT_EQ(A.getData(),D.getData());
    EXPECT_EQ(F.getData(),0x20);

    D.setData(0x00);
    and_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(D.getData(), 0x00);
    EXPECT_EQ(F.getData(),0xa0);
    
    D.setData(0xaa);
    A.setData(0xff);
    and_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0xaa);
    EXPECT_EQ(D.getData(),0xaa);
    EXPECT_EQ(F.getData(),0x20);

}

TEST (LogicalInstructionsTest, and_8n) {
    Register8 A(0xff,"A");
    Register8 D(0xff, "D");
    Register8 E(0xff,"E");
    Register16 DE(&D,&E);
    init_mem();
    write_byte(DE.getData(),0xff);
    Register8 F(0x00, "F");
    int c = 0;
    and_8(c,F,A,DE);

    EXPECT_EQ(A.getData(),read_byte(DE.getData()));
    EXPECT_EQ(F.getData(),0x20);

    write_byte(DE.getData(),0x00);
    and_8(c,F,A,DE);
    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(read_byte(DE.getData()), 0x00);
    EXPECT_EQ(F.getData(),0xa0);
    
    write_byte(DE.getData(),0xaa);
    A.setData(0xff);
    and_8(c,F,A,DE);
    EXPECT_EQ(A.getData(),0xaa);
    EXPECT_EQ(read_byte(DE.getData()),0xaa);
    EXPECT_EQ(F.getData(),0x20);
    free_memory();
}

TEST (LogicalInstructionsTest, or_8) {
    Register8 A(0xff,"A");
    Register8 D(0xff, "D");
    Register8 F(0x00, "F");
    int c = 0;
    or_8(c,F,A,D);

    EXPECT_EQ(A.getData(),D.getData());
    EXPECT_EQ(F.getData(),0x00);

    D.setData(0x00);
    or_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0xff);
    EXPECT_EQ(D.getData(), 0x00);
    EXPECT_EQ(F.getData(),0x00);
   
    A.setData(0x00);
    or_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(F.getData(),0x80);

    D.setData(0xaa);
    A.setData(0xff);
    or_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0xff);
    EXPECT_EQ(D.getData(),0xaa);
    EXPECT_EQ(F.getData(),0x00);

}

TEST (LogicalInstructionsTest, or_8n) {
    Register8 A(0xff,"A");
    Register8 D(0xff, "D");
    Register8 E(0xff,"E");
    Register16 DE(&D,&E);
    init_mem();
    write_byte(DE.getData(),0xff);
    Register8 F(0x00, "F");
    int c = 0;
    or_8(c,F,A,DE);

    EXPECT_EQ(A.getData(),read_byte(DE.getData()));
    EXPECT_EQ(F.getData(),0x00);

    write_byte(DE.getData(),0x00);
    or_8(c,F,A,DE);
    EXPECT_EQ(A.getData(),0xff);
    EXPECT_EQ(read_byte(DE.getData()), 0x00);
    EXPECT_EQ(F.getData(),0x00);
    
    A.setData(0x00);
    or_8(c,F,A,DE);
    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(F.getData(),0x80);
    
    write_byte(DE.getData(),0xaa);
    A.setData(0xff);
    or_8(c,F,A,DE);
    EXPECT_EQ(A.getData(),0xff);
    EXPECT_EQ(read_byte(DE.getData()),0xaa);
    EXPECT_EQ(F.getData(),0x00);
    free_memory();

}

TEST (LogicalInstructionsTest, xor_8) {
    Register8 A(0xff,"A");
    Register8 D(0xff, "D");
    Register8 F(0x00, "F");
    int c = 0;
    xor_8(c,F,A,D);

    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(F.getData(),0x80);

    D.setData(0x00);
    A.setData(0x00);
    xor_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(D.getData(), 0x00);
    EXPECT_EQ(F.getData(),0x80);
   
    A.setData(0x00);
    xor_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(F.getData(),0x80);

    A.setData(0x00);
    D.setData(0xff);
    xor_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0xff);
    EXPECT_EQ(F.getData(),0x00);
    
    D.setData(0xaa);
    A.setData(0xff);
    xor_8(c,F,A,D);
    EXPECT_EQ(A.getData(),0x55);
    EXPECT_EQ(D.getData(),0xaa);
    EXPECT_EQ(F.getData(),0x00);

}

TEST (LogicalInstructionsTest, xor_8n) {
    Register8 A(0xff,"A");
    Register8 D(0xff, "D");
    Register8 E(0xff,"E");
    Register16 DE(&D,&E);
    init_mem();
    write_byte(DE.getData(),0xff);
    Register8 F(0x00, "F");
    int c = 0;
    xor_8(c,F,A,DE);

    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(F.getData(),0x80);

    write_byte(DE.getData(),0x00);
    A.setData(0xff);
    xor_8(c,F,A,DE);
    EXPECT_EQ(A.getData(),0xff);
    EXPECT_EQ(read_byte(DE.getData()), 0x00);
    EXPECT_EQ(F.getData(),0x00);
    
    A.setData(0x00);
    xor_8(c,F,A,DE);
    EXPECT_EQ(A.getData(),0x00);
    EXPECT_EQ(F.getData(),0x80);
    
    write_byte(DE.getData(),0xaa);
    A.setData(0xff);
    xor_8(c,F,A,DE);
    EXPECT_EQ(A.getData(),0x55);
    EXPECT_EQ(read_byte(DE.getData()),0xaa);
    EXPECT_EQ(F.getData(),0x00);
    free_memory();

}


TEST (InstructionsTest_Operator, cp_8) {
    Register8 B(3,"B");
    Register8 E(2,"E");
    Register8 F(0xff,"F");
    int c = 0;
    cp_8(c, F, B, E);
    EXPECT_EQ(B.getData(),3);
    EXPECT_EQ(F.getData(),0x40);

    B.setData(2);
    E.setData(3);
    cp_8(c, F, B, E);
    EXPECT_EQ(B.getData(),2);
    EXPECT_EQ(F.getData(),0x70);
    
    B.setData(3);
    cp_8(c,F,B,E);
    EXPECT_EQ(B.getData(),3);
    EXPECT_EQ(F.getData(),0xC0);
}

TEST (InstructionsTest_Operator, cp_8n) {
    Register8 B(3,"B");
    //Register8 E(2,"E");
    Register8 H(0xff,"H");
    Register8 L(0xff,"L");
    Register8 F(0xff,"F");
    Register16 HL(&H,&L);
    init_mem();
    write_byte(0xFFFF,2);
    int c = 0;
    cp_8(c, F, B, HL);
    EXPECT_EQ(B.getData(),3);
    EXPECT_EQ(F.getData(),0x40);

    B.setData(2);
    write_byte(0xffff,3);
    cp_8(c, F, B, HL);
    EXPECT_EQ(B.getData(),2);
    EXPECT_EQ(F.getData(),0x70);
    free_memory();
}


