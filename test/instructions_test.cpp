#include <cstdint>
#include "../inc/register.hpp"
#include "../inc/instructions.hpp"
#include "gtest/gtest.h"
#include "../inc/memory.hpp"

TEST (InstructionTest, LD_8_n_nn) {
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
    free_memory();
}

TEST (InstructionTest, LD_16_r_r) {
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
}

TEST (InstructionTest, LD_16_n_nn) {
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
