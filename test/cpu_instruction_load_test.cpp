#include <gtest/gtest.h>
#include "cpu_instruction_load.h"
#include "cpu_registers.h"
#include "cpu_init_helper.h"

TEST(LD_r_r_test, LD_B_A) {
    CpuInitHelper helper;
    uint8_t* src = helper.registers.A;
    uint8_t* dest = helper.registers.B;
    *src = 50;
    *dest = 0;
    CPU::LD_r_r({helper.registers, dest, src}).tick();
    EXPECT_EQ(*src, 50);
    EXPECT_EQ(*dest, 50);
}

TEST(LD_r_n_test, LD_A_d8) {
    CpuInitHelper helper;
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 50);
    CPU::LD_r_n instr({helper.registers, dest, helper.addressDispatcher});
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
}

TEST(LD_r_absrr_test, LD_A_absHL) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 50);
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    CPU::LD_r_absrr instr(helper.registers, dest, helper.registers.HL, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
}

TEST(LD_r_absrr_test, LD_A_absHL_inc) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 50);
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    CPU::LD_r_absrr instr(helper.registers, dest, helper.registers.HL, helper.addressDispatcher, CPU::AddressMutOperation::INC);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
    EXPECT_EQ(*helper.registers.HL, MEMORY::WRAM_LO + 1);
}

TEST(LD_r_absrr_test, LD_A_absHL_dec) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.HL, 50);
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    CPU::LD_r_absrr instr(helper.registers, dest, helper.registers.HL, helper.addressDispatcher, CPU::AddressMutOperation::DEC);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
    EXPECT_EQ(*helper.registers.HL, MEMORY::WRAM_LO - 1);
}

TEST(LD_absrr_r_test, LD_absHL_A) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    uint8_t* src = helper.registers.A;
    *src = 50;
    CPU::LD_absrr_r instr(helper.registers, helper.registers.HL, src, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*src, 50);
    EXPECT_EQ(helper.addressDispatcher.read(*helper.registers.HL), 50);
}

TEST(LD_absrr_n_test, LD_absHL_d8) {
    CpuInitHelper helper;
    *helper.registers.HL = MEMORY::WRAM_LO;
    helper.addressDispatcher.write(*helper.registers.PC + 1, 50);
    CPU::LD_absrr_n instr(helper.registers, helper.registers.HL, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 50);
    EXPECT_EQ(helper.addressDispatcher.read(*helper.registers.HL), 50);
}

TEST(LD_r_absnn_test, LD_A_absD16) {
    CpuInitHelper helper;
    uint8_t* dest = helper.registers.A;
    *dest = 0;
    helper.addressDispatcher.write(*helper.registers.PC + 1 ,(uint8_t) MEMORY::WRAM_LO);
    helper.addressDispatcher.write(*helper.registers.PC + 2 ,(uint8_t) (MEMORY::WRAM_LO >> 8));
    helper.addressDispatcher.write(MEMORY::WRAM_LO, 50);
    CPU::LD_r_absnn instr(helper.registers, dest, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(*helper.registers.A, 50);
}

TEST(LD_absnn_r_test, LD_absD16_A) {
    CpuInitHelper helper;
    uint8_t* src = helper.registers.A;
    *src = 50;
    helper.addressDispatcher.write(*helper.registers.PC + 1 ,(uint8_t) MEMORY::WRAM_LO);
    helper.addressDispatcher.write(*helper.registers.PC + 2 ,(uint8_t) (MEMORY::WRAM_LO >> 8));
    helper.addressDispatcher.write(MEMORY::WRAM_LO, 0);
    CPU::LD_absnn_r instr(helper.registers, src, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    instr.tick();
    instr.tick();
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::WRAM_LO), 50);
}

TEST(LD_r_relr_test, LD_A_relC) {
    CpuInitHelper helper;
    uint8_t* src_addr_lsb = helper.registers.C;
    uint8_t* dest = helper.registers.A;
    *src_addr_lsb = 0x80; // Becomes 0xFF80
    *dest = 0;
    helper.addressDispatcher.write(0xFF80, 50);
    CPU::LD_r_relr instr(helper.registers, dest, src_addr_lsb, helper.addressDispatcher);
    instr.tick();
    instr.tick();
    EXPECT_EQ(*dest, 50);
}
