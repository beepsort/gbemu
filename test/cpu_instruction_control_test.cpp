#include <gtest/gtest.h>
#include "cpu_instruction_control.h"
#include "cpu_registers.h"
#include "cpu_init_helper.h"

TEST(DI_test, DisableInterrupts) {
    CpuInitHelper helper;
    helper.registers.IME = true;
    CPU::DI({helper.registers}).tick();
    EXPECT_EQ(helper.registers.IME, false);
}

TEST(EI_test, EnableInterrupts) {
    CpuInitHelper helper;
    helper.registers.IME = false;
    CPU::EI({helper.registers}).tick();
    EXPECT_EQ(helper.registers.IME, true);
}
