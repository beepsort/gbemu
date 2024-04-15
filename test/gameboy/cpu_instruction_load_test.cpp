#include <gtest/gtest.h>
#include "gameboy/cpu_instruction_load.h"
#include "gameboy/cpu_registers.h"
#include "cpu_init_helper.h"

TEST(LD_r_r_test, LD_B_A) {
    CpuInitHelper helper;
    uint8_t* src = helper.registers.A;
    uint8_t* dest = helper.registers.B;
    *src = 50;
    *dest = 0;
    GAMEBOY::LD_r_r({helper.registers, dest, src}).tick();
    EXPECT_EQ(*src, 50);
    EXPECT_EQ(*dest, 50);
}

TEST(LD_r_n_test, LD_A_d8) {
    CpuInitHelper helper;
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 50);
    GAMEBOY::LD_r_n instr({helper.registers, dest, helper.addressDispatcher});
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
}

TEST(LD_r_absrr_test, LD_A_absHL) {
    CpuInitHelper helper;
    *helper.registers.HL = GAMEBOY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 50);
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    GAMEBOY::LD_r_absrr instr(helper.registers, dest, helper.registers.HL, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
}

TEST(LD_r_absrr_test, LD_A_absHL_inc) {
    CpuInitHelper helper;
    *helper.registers.HL = GAMEBOY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 50);
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    GAMEBOY::LD_r_absrr instr(helper.registers, dest, helper.registers.HL, helper.addressDispatcher, GAMEBOY::AddressMutOperation::INC);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
    EXPECT_EQ(*helper.registers.HL, GAMEBOY::WRAM_LO + 1);
}

TEST(LD_r_absrr_test, LD_A_absHL_dec) {
    CpuInitHelper helper;
    *helper.registers.HL = GAMEBOY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 50);
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    GAMEBOY::LD_r_absrr instr(helper.registers, dest, helper.registers.HL, helper.addressDispatcher, GAMEBOY::AddressMutOperation::DEC);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
    EXPECT_EQ(*helper.registers.HL, GAMEBOY::WRAM_LO - 1);
}

TEST(LD_absrr_r_test, LD_absHL_A) {
    CpuInitHelper helper;
    *helper.registers.HL = GAMEBOY::WRAM_LO;
    uint8_t* src = helper.registers.A;
    *src = 50;
    GAMEBOY::LD_absrr_r instr(helper.registers, helper.registers.HL, src, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*src, 50);
    EXPECT_EQ(helper.addressDispatcher.read(*helper.registers.HL), 50);
}

TEST(LD_absrr_n_test, LD_absHL_d8) {
    CpuInitHelper helper;
    *helper.registers.HL = GAMEBOY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 50);
    GAMEBOY::LD_absrr_n instr(helper.registers, helper.registers.HL, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::WRAM_LO), 50);
    EXPECT_EQ(helper.addressDispatcher.read(*helper.registers.HL), 50);
}

TEST(LD_r_absnn_test, LD_A_absD16) {
    CpuInitHelper helper;
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    helper.addressDispatcher.write(*helper.registers.PC + 1 ,(uint8_t) GAMEBOY::WRAM_LO);
    helper.addressDispatcher.write(*helper.registers.PC + 2 ,(uint8_t) (GAMEBOY::WRAM_LO >> 8));
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO, 50);
    GAMEBOY::LD_r_absnn instr(helper.registers, dest, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*helper.registers.A, 50);
}

TEST(LD_absnn_r_test, LD_absD16_A) {
    CpuInitHelper helper;
    uint8_t* src = helper.registers.A;
    *src = 50;
    helper.addressDispatcher.write(*helper.registers.PC + 1 ,(uint8_t) GAMEBOY::WRAM_LO);
    helper.addressDispatcher.write(*helper.registers.PC + 2 ,(uint8_t) (GAMEBOY::WRAM_LO >> 8));
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO, 0);
    GAMEBOY::LD_absnn_r instr(helper.registers, src, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::WRAM_LO), 50);
}

TEST(LD_r_relr_test, LD_A_relC) {
    CpuInitHelper helper;
    uint8_t* src_addr_lsb = helper.registers.C;
    uint8_t* dest = helper.registers.A;
    *src_addr_lsb = 0x80; // Becomes 0xFF80
    *dest = 0;
    helper.addressDispatcher.write(0xFF80, 50);
    GAMEBOY::LD_r_relr instr(helper.registers, dest, src_addr_lsb, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
}

TEST(LD_relr_r_test, LD_relC_A) {
    CpuInitHelper helper;
    uint8_t* dest_addr_lsb = helper.registers.C;
    uint8_t* src = helper.registers.A;
    *dest_addr_lsb = 0x80; // Becomes 0xFF80
    *src = 50;
    helper.addressDispatcher.write(0xFF80, 0);
    GAMEBOY::LD_relr_r instr(helper.registers, dest_addr_lsb, src, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(0xFF80), 50);
}

TEST(LD_r_reln_test, LD_A_rel_d8) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x80);
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    helper.addressDispatcher.write(0xFF80, 50);
    GAMEBOY::LD_r_reln instr(helper.registers, dest, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
}

TEST(LD_reln_r_test, LD_rel_d8_A) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x80);
    uint8_t* src = helper.registers.A;
    *src = 50;
    helper.addressDispatcher.write(0xFF80, 0);
    GAMEBOY::LD_reln_r instr(helper.registers, src, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(0xFF80), 50);
}

TEST(LD_rr_nn_test, LD_BC_d16) {
    CpuInitHelper helper;
    uint16_t* dest = helper.registers.BC;
    *dest = 0;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 0x34);
    helper.addressDispatcher.write(*helper.registers.PC + 2, 0x12);
    GAMEBOY::LD_rr_nn instr({helper.registers, dest, helper.addressDispatcher});
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 0x1234);
}

TEST(LD_absnn_rr_test, LD_absD16_SP) {
    CpuInitHelper helper;
    uint16_t* src = helper.registers.SP;
    *src = 0x1234;
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO, 0);
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO + 1, 0);
    helper.addressDispatcher.write(*helper.registers.PC + 1, (uint8_t)GAMEBOY::WRAM_LO);
    helper.addressDispatcher.write(*helper.registers.PC + 2, (uint8_t)(GAMEBOY::WRAM_LO >> 8));
    GAMEBOY::LD_absnn_rr instr({helper.registers, src, helper.addressDispatcher});
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::WRAM_LO), 0x34);
    EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::WRAM_LO + 1), 0x12);
}

TEST(LD_rr_rr_test, LD_SP_HL) {
    CpuInitHelper helper;
    uint16_t* src = helper.registers.HL;
    uint16_t* dest = helper.registers.SP;
    *src = 0x1234;
    *dest = 0;
    GAMEBOY::LD_rr_rr instr({helper.registers, dest, src});
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 0x1234);
}

TEST(PUSH_rr_test, PUSH_BC) {
    CpuInitHelper helper;
    uint16_t* src = helper.registers.BC;
    uint16_t* sp = helper.registers.SP;
    *src = 0x1234;
    *sp = GAMEBOY::WRAM_LO + 2;
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO, 0);
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO + 1, 0);
    GAMEBOY::PUSH_rr instr({helper.registers, src, helper.addressDispatcher});
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*sp, GAMEBOY::WRAM_LO);
    EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::WRAM_LO), 0x34);
    EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::WRAM_LO + 1), 0x12);
}

TEST(POP_rr_test, POP_BC) {
    CpuInitHelper helper;
    uint16_t* dest = helper.registers.BC;
    uint16_t* sp = helper.registers.SP;
    *dest = 0;
    *sp = GAMEBOY::WRAM_LO;
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO, 0x34);
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO + 1, 0x12);
    GAMEBOY::POP_rr instr({helper.registers, dest, helper.addressDispatcher});
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*sp, GAMEBOY::WRAM_LO + 2);
    EXPECT_EQ(*dest, 0x1234);
}

TEST(PUSH_POP_test, PUSH_POP_BC) {
    CpuInitHelper helper;
    uint16_t* src = helper.registers.BC;
    uint16_t* dest = helper.registers.BC;
    uint16_t* sp = helper.registers.SP;
    *src = 0x1234;
    *sp = GAMEBOY::WRAM_LO + 2;
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO, 0);
    helper.addressDispatcher.write(GAMEBOY::WRAM_LO + 1, 0);
    GAMEBOY::PUSH_rr instr({helper.registers, src, helper.addressDispatcher});
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    *dest = 0;
    GAMEBOY::POP_rr instr2({helper.registers, dest, helper.addressDispatcher});
    instr2.tick();
    instr2.tick();
    instr2.tick();
    EXPECT_EQ(*sp, GAMEBOY::WRAM_LO + 2);
    EXPECT_EQ(*dest, 0x1234);
}

