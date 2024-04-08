#include <gtest/gtest.h>
#include "cpu_instruction_misc.h"
#include "cpu_registers.h"
#include "cpu_init_helper.h"

TEST(NOP_test, Pc) {
    CpuInitHelper helper;
    CPU::NOP(helper.registers).tick();
    EXPECT_EQ(*helper.registers.PC, 0xC001);
}

TEST(RLC_test, Alternating) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0x55;
    CPU::RLC({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0xAA);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
}

TEST(RLC_test, AlternatingWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0xAA;
    CPU::RLC({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x55);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RLC_test, OneBitWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0x80;
    CPU::RLC({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x01);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RLC_absHL_test, OneBitWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x80);
    CPU::RLC_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x01);
    EXPECT_TRUE(helper.registers.get_flag_carry());
}

TEST(RRC_test, Alternating) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0xAA;
    CPU::RRC({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x55);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
}

TEST(RRC_test, AlternatingWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0x55;
    CPU::RRC({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0xAA);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RRC_test, OneBitWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0x01;
    CPU::RRC({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x80);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RRC_absHL_test, OneBitWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x01);
    CPU::RRC_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x80);
    EXPECT_TRUE(helper.registers.get_flag_carry());
}


TEST(RL_test, AlternatingNoCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0x55;
    CPU::RL({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0xAA);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
}

TEST(RL_test, AlternatingWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0xAA;
    CPU::RL({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x55);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RL_test, ZeroWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0x00;
    CPU::RL({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x01);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RR_test, AlternatingNoCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0xAA;
    CPU::RR({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x55);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
}

TEST(RR_test, AlternatingWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0x55;
    CPU::RR({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0xAA);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RR_test, ZeroWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0x00;
    CPU::RR({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x80);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

