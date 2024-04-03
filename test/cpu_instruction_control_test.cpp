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

TEST(RET_test, RetAddress) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    uint16_t* sp = helper.registers.SP;
    *pc = 0;
    *sp = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(MEMORY::WRAM_LO, 0x34);
    helper.addressDispatcher.write(MEMORY::WRAM_LO + 1, 0x12);
    CPU::RET instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*pc, 0x1234);
    EXPECT_EQ(*sp, MEMORY::WRAM_LO + 2);
}
