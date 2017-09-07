#include <gtest/gtest.h>
#include "CPU.h"
#include "EmulateFixture.h"

#include <iostream>

TEST_F(EmulateFixture, TrapOnInt3)
{
    set_instruction({0x90});
    write_mem16(VEC_INT + 2, 0x8000, CS);
    write_mem16(VEC_INT + 0, 0x0100, CS);
    emulate();

    ASSERT_FALSE(cpu->has_trapped());

    reset();

    set_instruction({0xcc});
    emulate();

    ASSERT_TRUE(cpu->has_trapped());
    ASSERT_EQ(0x8000, cpu->read_reg(CS));
    ASSERT_EQ(0x0100, cpu->read_reg(IP));
}
