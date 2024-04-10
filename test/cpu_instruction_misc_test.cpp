#include <gtest/gtest.h>
#include "cpu_instruction_misc.h"
#include "cpu_registers.h"
#include "cpu_init_helper.h"

TEST(NOP_test, Pc) {
    CpuInitHelper helper;
    CPU::NOP(helper.registers).tick();
    EXPECT_EQ(*helper.registers.PC, 0xC001);
}

TEST(RLC_r_test, Alternating) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0x55;
    CPU::RLC_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0xAA);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
}

TEST(RLC_r_test, AlternatingWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0xAA;
    CPU::RLC_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x55);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RLC_r_test, OneBitWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0x80;
    CPU::RLC_r({helper.registers, target}).tick();
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

TEST(RRC_r_test, Alternating) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0xAA;
    CPU::RRC_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x55);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
}

TEST(RRC_r_test, AlternatingWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0x55;
    CPU::RRC_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0xAA);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RRC_r_test, OneBitWrapAround) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0x01;
    CPU::RRC_r({helper.registers, target}).tick();
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

TEST(RL_r_test, AlternatingNoCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0x55;
    CPU::RL_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0xAA);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
}

TEST(RL_r_test, AlternatingWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0xAA;
    CPU::RL_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x55);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RL_r_test, ZeroWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0x00;
    CPU::RL_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x01);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RL_absHL_test, AlternatingNoCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x55);
    CPU::RL_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0xAA);
}

TEST(RL_absHL_test, AlternatingWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0xAA);
    CPU::RL_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x55);
}

TEST(RL_absHL_test, ZeroWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x00);
    CPU::RL_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x01);
}

TEST(RR_r_test, AlternatingNoCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    uint8_t* target = helper.registers.B;
    *target = 0xAA;
    CPU::RR_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x55);
    EXPECT_EQ(helper.registers.get_flag_carry(), false);
}

TEST(RR_r_test, AlternatingWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0x55;
    CPU::RR_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0xAA);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RR_r_test, ZeroWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    uint8_t* target = helper.registers.B;
    *target = 0x00;
    CPU::RR_r({helper.registers, target}).tick();
    EXPECT_EQ(*target, 0x80);
    EXPECT_EQ(helper.registers.get_flag_carry(), true);
}

TEST(RR_absHL_test, AlternatingNoCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0xAA);
    CPU::RR_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x55);
}

TEST(RR_absHL_test, AlternatingWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x55);
    CPU::RR_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0xAA);
}

TEST(RR_absHL_test, ZeroWithCarry) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x00);
    CPU::RR_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x80);
}

TEST(SLA_r_test, CarryTrue) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.B = 0x80;
    CPU::SLA_r({helper.registers, helper.registers.B}).tick();
    EXPECT_EQ(*helper.registers.B, 0);
    EXPECT_TRUE(helper.registers.get_flag_carry());
}

TEST(SLA_r_test, CarryFalse) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    *helper.registers.B = 0x01;
    CPU::SLA_r({helper.registers, helper.registers.B}).tick();
    EXPECT_EQ(*helper.registers.B, 2);
    EXPECT_FALSE(helper.registers.get_flag_carry());
}

TEST(SLA_absHL_test, CarryTrue) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x81);
    CPU::SLA_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x02);
    EXPECT_TRUE(helper.registers.get_flag_carry());
}

TEST(SRA_r_test, CarryTrue) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.B = 0x01;
    CPU::SRA_r({helper.registers, helper.registers.B}).tick();
    EXPECT_EQ(*helper.registers.B, 0);
    EXPECT_TRUE(helper.registers.get_flag_carry());
}

TEST(SRA_r_test, CarryFalse) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    *helper.registers.B = 0x80;
    CPU::SRA_r({helper.registers, helper.registers.B}).tick();
    EXPECT_EQ(*helper.registers.B, 0xC0);
    EXPECT_FALSE(helper.registers.get_flag_carry());
}

TEST(SRA_absHL_test, CarryTrue) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x81);
    CPU::SRA_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0xC0);
    EXPECT_TRUE(helper.registers.get_flag_carry());
}

TEST(SWAP_r_test, Alternating) {
    CpuInitHelper helper;
    *helper.registers.B = 0xA5;
    CPU::SWAP_r({helper.registers, helper.registers.B}).tick();
    EXPECT_EQ(*helper.registers.B, 0x5A);
}

TEST(SWAP_absHL_test, CarryTrue) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0xA5);
    CPU::SWAP_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x5A);
}

TEST(SRL_r_test, CarryTrue) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.B = 0x01;
    CPU::SRL_r({helper.registers, helper.registers.B}).tick();
    EXPECT_EQ(*helper.registers.B, 0);
    EXPECT_TRUE(helper.registers.get_flag_carry());
}

TEST(SRL_r_test, CarryFalse) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(true);
    *helper.registers.B = 0x80;
    CPU::SRL_r({helper.registers, helper.registers.B}).tick();
    EXPECT_EQ(*helper.registers.B, 0x40);
    EXPECT_FALSE(helper.registers.get_flag_carry());
}

TEST(SRL_absHL_test, CarryTrue) {
    CpuInitHelper helper;
    helper.registers.set_flag_carry(false);
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 0x81);
    CPU::SRL_absHL instr(helper.registers, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 0x40);
    EXPECT_TRUE(helper.registers.get_flag_carry());
}

