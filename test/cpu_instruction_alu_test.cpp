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

