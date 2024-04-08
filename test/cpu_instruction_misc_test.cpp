#include <gtest/gtest.h>
#include "cpu_instruction_misc.h"
#include "cpu_registers.h"
#include "cpu_init_helper.h"

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

