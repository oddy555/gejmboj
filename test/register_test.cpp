#include "gtest/gtest.h"
#include "../inc/register.hpp"

TEST (RegisterTest, pointerTest) {
    Register8 B(3,"B");
    Register8 C(5,"C");
    Register16 BC(&B,&C);
    EXPECT_EQ(B.getData(),BC.getHigh()->getData());
    EXPECT_EQ(C.getData(),BC.getLow()->getData());
    EXPECT_EQ(BC.getData(),773);
    B.setData(8);
    EXPECT_EQ(B.getData(),BC.getHigh()->getData());
}
