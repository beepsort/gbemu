#include <gtest/gtest.h>
#include "cpu_instruction_alu.h"
#include "cpu_registers.h"
#include "cpu_init_helper.h"

TEST(ADD_r_r_test, OnePlusOne) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 1;
    *dest = 1;
    CPU::ADD_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 1);
    EXPECT_EQ(*dest, 2);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(ADD_r_r_test, ZeroPlusZero) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0;
    *dest = 0;
    CPU::ADD_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0);
    EXPECT_EQ(*dest, 0);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(ADD_r_r_test, OnePlusMax_Overflow) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 1;
    *dest = 255;
    CPU::ADD_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 1);
    EXPECT_EQ(*dest, 0);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_TRUE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(ADD_r_r_test, MaxPlusMax_Overflow) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 255;
    *dest = 255;
    CPU::ADD_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 255);
    EXPECT_EQ(*dest, 254);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_TRUE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(ADD_r_r_test, OneUnderOverflow) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0x01;
    *dest = 0xFE;
    CPU::ADD_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0x01);
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(ADD_r_r_test, FifteenPlusOne_Halfcarry) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0xF;
    *dest = 0x1;
    CPU::ADD_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0xF);
    EXPECT_EQ(*dest, 0x10);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(ADD_r_r_test, FourteenPlusOne_NoHalfcarry) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0xE;
    *dest = 0x1;
    CPU::ADD_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0xE);
    EXPECT_EQ(*dest, 0xF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(ADD_r_n_test, OnePlusOne) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 1);
    uint8_t *dest = helper.registers.B;
    *dest = 1;
    CPU::ADD_r_n({dest, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 2);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(ADD_r_absrr_test, OnePlusOne) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 1);
    uint8_t *dest = helper.registers.B;
    *dest = 1;
    CPU::ADD_r_absrr({dest, helper.registers.HL, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 2);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(SUB_r_r_test, OneMinusOne) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 1;
    *dest = 1;
    CPU::SUB_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 1);
    EXPECT_EQ(*dest, 0);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_TRUE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(SUB_r_r_test, ZeroMinusZero) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0;
    *dest = 0;
    CPU::SUB_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0);
    EXPECT_EQ(*dest, 0);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_TRUE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(SUB_r_r_test, ZeroMinusOne_Underflow) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 1;
    *dest = 0;
    CPU::SUB_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 1);
    EXPECT_EQ(*dest, 255);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_TRUE(helper.registers.get_flag_sub());
    EXPECT_TRUE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(SUB_r_r_test, ZeroMinusMax_Underflow) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 255;
    *dest = 0;
    CPU::SUB_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 255);
    EXPECT_EQ(*dest, 1);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_TRUE(helper.registers.get_flag_sub());
    EXPECT_TRUE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(SUB_r_r_test, Minus_HalfCarry) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0x08;
    *dest = 0xF0;
    CPU::SUB_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0x08);
    EXPECT_EQ(*dest, 0xE8);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_TRUE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(SUB_r_n_test, OneMinusOne) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 1);
    uint8_t *dest = helper.registers.B;
    *dest = 1;
    CPU::SUB_r_n({dest, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_TRUE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(SUB_r_absrr_test, OneMinusOne) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 1);
    uint8_t *dest = helper.registers.B;
    *dest = 1;
    CPU::SUB_r_absrr({dest, helper.registers.HL, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_TRUE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(AND_r_r_test, FfAndFf) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0xFF;
    *dest = 0xFF;
    CPU::AND_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0xFF);
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(AND_r_r_test, OpposingBitsAnd) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0xAA;
    *dest = 0x55;
    CPU::AND_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0xAA);
    EXPECT_EQ(*dest, 0x00);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(AND_r_n_test, FfAndFf) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0xFF);
    uint8_t *dest = helper.registers.B;
    *dest = 0xFF;
    CPU::AND_r_n({dest, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(AND_r_absrr_test, FfAndFf) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0xFF);
    uint8_t *dest = helper.registers.B;
    *dest = 0xFF;
    CPU::AND_r_absrr({dest, helper.registers.HL, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_TRUE(helper.registers.get_flag_halfcarry());
}

TEST(XOR_r_r_test, FfXorFf) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0xFF;
    *dest = 0xFF;
    CPU::XOR_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0xFF);
    EXPECT_EQ(*dest, 0x00);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(XOR_r_r_test, OpposingBitsXor) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0xAA;
    *dest = 0x55;
    CPU::XOR_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0xAA);
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(XOR_r_n_test, FfXorFf) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0xFF);
    uint8_t *dest = helper.registers.B;
    *dest = 0xFF;
    CPU::XOR_r_n({dest, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0x00);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(XOR_r_n_test, OpposingBitsXor) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x55);
    uint8_t *dest = helper.registers.B;
    *dest = 0xAA;
    CPU::XOR_r_n({dest, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(XOR_r_absrr_test, FfXorFf) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0xFF);
    uint8_t *dest = helper.registers.B;
    *dest = 0xFF;
    CPU::XOR_r_absrr({dest, helper.registers.HL, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0x00);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(XOR_r_absrr_test, OpposingBitsXor) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x55);
    uint8_t *dest = helper.registers.B;
    *dest = 0xAA;
    CPU::XOR_r_absrr({dest, helper.registers.HL, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_r_test, FfOrFf) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0xFF;
    *dest = 0xFF;
    CPU::OR_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0xFF);
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_r_test, OpposingBitsOr) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0xAA;
    *dest = 0x55;
    CPU::OR_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0xAA);
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_r_test, ZeroOrZero) {
    CpuInitHelper helper;
    uint8_t *src = helper.registers.A;
    uint8_t *dest = helper.registers.B;
    *src = 0x00;
    *dest = 0x00;
    CPU::OR_r_r({dest, src, helper.registers}).tick();
    EXPECT_EQ(*src, 0x00);
    EXPECT_EQ(*dest, 0x00);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_n_test, FfOrFf) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0xFF);
    uint8_t *dest = helper.registers.B;
    *dest = 0xFF;
    CPU::OR_r_n({dest, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_n_test, OpposingBitsOr) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x55);
    uint8_t *dest = helper.registers.B;
    *dest = 0xAA;
    CPU::OR_r_n({dest, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_n_test, ZeroOrZero) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x00);
    uint8_t *dest = helper.registers.B;
    *dest = 0x00;
    CPU::OR_r_n({dest, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0x00);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_absrr_test, FfOrFf) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0xFF);
    uint8_t *dest = helper.registers.B;
    *dest = 0xFF;
    CPU::OR_r_absrr({dest, helper.registers.HL, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_absrr_test, OpposingBitsOr) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x55);
    uint8_t *dest = helper.registers.B;
    *dest = 0xAA;
    CPU::OR_r_absrr({dest, helper.registers.HL, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0xFF);
    EXPECT_FALSE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

TEST(OR_r_absrr_test, ZeroOrZero) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x00);
    uint8_t *dest = helper.registers.B;
    *dest = 0x00;
    CPU::OR_r_absrr({dest, helper.registers.HL, helper.registers, helper.addressDispatcher}).tick();
    EXPECT_EQ(*dest, 0x00);
    EXPECT_TRUE(helper.registers.get_flag_zero());
    EXPECT_FALSE(helper.registers.get_flag_sub());
    EXPECT_FALSE(helper.registers.get_flag_carry());
    EXPECT_FALSE(helper.registers.get_flag_halfcarry());
}

