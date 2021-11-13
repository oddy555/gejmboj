#include "gtest/gtest.h"
#include "../inc/cpu.hpp"
#include "../inc/memory.hpp"
#include "../inc/register.hpp"
TEST (CpuTest, eval_opcodeTest) {
    init_registers();
    Register16 PC(0,"PC");
    init_mem();
    write_word(1,11);
    eval_opcode(0x01,0);
    eval_opcode(0x02,1);
    //PC.inc(2);
    //EXPECT_EQ(11, BC.getData());
    //EXPECT_EQ(PC.getData_(),2);
}
