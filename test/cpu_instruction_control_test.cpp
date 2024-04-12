#include <gtest/gtest.h>
#include "cpu_instruction_control.h"
#include "cpu_registers.h"
#include "cpu_init_helper.h"

TEST(HALT_test, DoesHalt) {
    CpuInitHelper helper;
    CPU::HALT instr(helper.registers, helper.addressDispatcher);
    CPU::InstructionResult result = instr.tick();
    EXPECT_EQ(*helper.registers.PC, 0xC001);
    EXPECT_EQ(result, CPU::InstructionResult::HALT);
}

TEST(STOP_test, DoesStop) {
    CpuInitHelper helper;
    CPU::STOP instr(helper.registers);
    CPU::InstructionResult result = instr.tick();
    EXPECT_EQ(*helper.registers.PC, 0xC001);
    EXPECT_EQ(result, CPU::InstructionResult::STOP);
}

TEST(JP_HL_test, IsCorrectPc) {
    CpuInitHelper helper;
    uint16_t* hl = helper.registers.HL;
    uint16_t* pc = helper.registers.PC;
    *pc = 0;
    *hl = 0x1234;
    CPU::JP_HL({pc, hl}).tick();
    EXPECT_EQ(*pc, 0x1234);
}

TEST(JP_NN_test, IsCorrectPc) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x34);
    helper.addressDispatcher.write(*helper.registers.PC + 2, 0x12);
    CPU::JP_NN instr(helper.registers, helper.addressDispatcher, &CPU::cond_TRUE);
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*pc, 0x1234);
}

TEST(JP_NN_cond_test, ConditionalZero) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    *pc = 0xC000;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x34);
    helper.addressDispatcher.write(*helper.registers.PC + 2, 0x12);
    helper.registers.set_flag_zero(false);
    CPU::JP_NN instr(helper.registers, helper.addressDispatcher, &CPU::cond_Z);
    instr.tick();
    instr.tick();
    CPU::InstructionResult result = instr.tick();
    EXPECT_EQ(*pc, 0xC003);
    EXPECT_EQ(result, CPU::InstructionResult::FINISHED);
    helper.registers.set_flag_zero(true);
    *pc = 0xC000;
    CPU::JP_NN instr2(helper.registers, helper.addressDispatcher, &CPU::cond_Z);
    instr2.tick();
    instr2.tick();
    instr2.tick();
    CPU::InstructionResult result2 = instr2.tick();
    EXPECT_EQ(*pc, 0x1234);
    EXPECT_EQ(result2, CPU::InstructionResult::FINISHED);
}

TEST(JR_N_test, PositiveJump) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x10);
    CPU::JR_N instr(helper.registers, helper.addressDispatcher, &CPU::cond_TRUE);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*pc, 0xC012);
}

TEST(JR_N_test, NegativeJump) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    *pc = 0xC010;
    helper.addressDispatcher.write(*helper.registers.PC + 1, (int8_t)(-16));
    CPU::JR_N instr(helper.registers, helper.addressDispatcher, &CPU::cond_TRUE);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*pc, 0xC002);
}

TEST(JR_N_test, ConditionTF) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x10);
    helper.registers.set_flag_zero(false);
    CPU::JR_N instr(helper.registers, helper.addressDispatcher, &CPU::cond_Z);
    instr.tick();
    CPU::InstructionResult result = instr.tick();
    EXPECT_EQ(*pc, 0xC002);
    EXPECT_EQ(result, CPU::InstructionResult::FINISHED);
    *pc = 0xC000;
    helper.registers.set_flag_zero(true);
    CPU::JR_N instr2(helper.registers, helper.addressDispatcher, &CPU::cond_Z);
    instr2.tick();
    instr2.tick();
    CPU::InstructionResult result2 = instr2.tick();
    EXPECT_EQ(*pc, 0xC012);
    EXPECT_EQ(result, CPU::InstructionResult::FINISHED);
}

TEST(SCF_test, FlagsTest) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    helper.registers.set_flag_carry(false);
    helper.registers.set_flag_halfcarry(true);
    helper.registers.set_flag_sub(true);
    CPU::SCF({helper.registers}).tick();
    EXPECT_EQ(*pc, 0xC001);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
    EXPECT_EQ(helper.registers.get_flag_halfcarry(), false);
    EXPECT_EQ(helper.registers.get_flag_sub(), false);
}

TEST(CCF_test, FlagsTest) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    helper.registers.set_flag_carry(false);
    helper.registers.set_flag_halfcarry(true);
    helper.registers.set_flag_sub(true);
    CPU::CCF({helper.registers}).tick();
    EXPECT_EQ(*pc, 0xC001);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
    EXPECT_EQ(helper.registers.get_flag_halfcarry(), false);
    EXPECT_EQ(helper.registers.get_flag_sub(), false);
    *pc = 0xC000;
    helper.registers.set_flag_carry(true);
    helper.registers.set_flag_halfcarry(true);
    helper.registers.set_flag_sub(true);
    CPU::CCF({helper.registers}).tick();
    EXPECT_EQ(*pc, 0xC001);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
    EXPECT_EQ(helper.registers.get_flag_halfcarry(), false);
    EXPECT_EQ(helper.registers.get_flag_sub(), false);
}

TEST(DI_test, DisableInterrupts) {
    CpuInitHelper helper;
    helper.registers.IME = true;
    CPU::DI({helper.registers}).tick();
    EXPECT_EQ(*helper.registers.PC, 0xC001);
    EXPECT_EQ(helper.registers.IME, false);
}

TEST(EI_test, EnableInterrupts) {
    CpuInitHelper helper;
    helper.registers.IME = false;
    CPU::EI({helper.registers}).tick();
    EXPECT_EQ(*helper.registers.PC, 0xC001);
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

TEST(RET_cond_test, ConditionalZero) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    uint16_t* sp = helper.registers.SP;
    *pc = 0;
    *sp = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(MEMORY::WRAM_LO, 0x34);
    helper.addressDispatcher.write(MEMORY::WRAM_LO + 1, 0x12);
    helper.registers.set_flag_zero(false);
    CPU::RET_CC instr(helper.registers, helper.addressDispatcher, &CPU::cond_Z);
    instr.tick();
    CPU::InstructionResult result = instr.tick();
    EXPECT_EQ(*pc, 1);
    EXPECT_EQ(*sp, MEMORY::WRAM_LO);
    EXPECT_EQ(result, CPU::InstructionResult::FINISHED);
    helper.registers.set_flag_zero(true);
    CPU::RET_CC instr2(helper.registers, helper.addressDispatcher, &CPU::cond_Z);
    instr2.tick();
    instr2.tick();
    instr2.tick();
    instr2.tick();
    CPU::InstructionResult result2 = instr2.tick();
    EXPECT_EQ(*pc, 0x1234);
    EXPECT_EQ(*sp, MEMORY::WRAM_LO + 2);
    EXPECT_EQ(result, CPU::InstructionResult::FINISHED);
}

TEST(RETI_test, RetAddress) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    uint16_t* sp = helper.registers.SP;
    *pc = 0;
    *sp = MEMORY::WRAM_LO;
    helper.registers.IME = false;
    helper.addressDispatcher.write(MEMORY::WRAM_LO, 0x34);
    helper.addressDispatcher.write(MEMORY::WRAM_LO + 1, 0x12);
    CPU::RETI instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*pc, 0x1234);
    EXPECT_EQ(*sp, MEMORY::WRAM_LO + 2);
    EXPECT_EQ(helper.registers.IME, true);
}

TEST(CALL_NN_test, IsCorrectPcStack) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    uint16_t* sp = helper.registers.SP;
    *sp = MEMORY::WRAM_LO + 2;
    helper.addressDispatcher.write(*pc + 1, 0x34);
    helper.addressDispatcher.write(*pc + 2, 0x12);
    CPU::CALL_NN instr(helper.registers, helper.addressDispatcher, &CPU::cond_TRUE);
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*pc, 0x1234);
    EXPECT_EQ(*sp, MEMORY::WRAM_LO);
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x03);
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO+1), 0xC0);
}

TEST(CALL_NN_test, ConditionalZero) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    uint16_t* sp = helper.registers.SP;
    *sp = MEMORY::WRAM_LO + 2;
    helper.addressDispatcher.write(*pc + 1, 0x34);
    helper.addressDispatcher.write(*pc + 2, 0x12);
    helper.registers.set_flag_zero(false);
    CPU::CALL_NN instr(helper.registers, helper.addressDispatcher, &CPU::cond_Z);
    instr.tick();
    instr.tick();
    CPU::InstructionResult result = instr.tick();
    EXPECT_EQ(result, CPU::InstructionResult::FINISHED);
    EXPECT_EQ(*pc, 0xC003);
    EXPECT_EQ(*sp, MEMORY::WRAM_LO+2);
    helper.registers.set_flag_zero(true);
    *pc = 0xC000;
    CPU::CALL_NN instr2(helper.registers, helper.addressDispatcher, &CPU::cond_Z);
    instr2.tick();
    instr2.tick();
    instr2.tick();
    instr2.tick();
    instr2.tick();
    CPU::InstructionResult result2 = instr2.tick();
    EXPECT_EQ(result2, CPU::InstructionResult::FINISHED);
    EXPECT_EQ(*pc, 0x1234);
    EXPECT_EQ(*sp, MEMORY::WRAM_LO);
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x03);
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO+1), 0xC0);
}

TEST(RST_test, IsCorrectPcStack) {
    CpuInitHelper helper;
    uint16_t* pc = helper.registers.PC;
    uint16_t* sp = helper.registers.SP;
    *sp = MEMORY::WRAM_LO + 2;
    CPU::RST instr(helper.registers, helper.addressDispatcher, 0x10);
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*pc, 0x0010);
    EXPECT_EQ(*sp, MEMORY::WRAM_LO);
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x01);
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO+1), 0xC0);
}

