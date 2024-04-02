#include <gtest/gtest.h>
#include "cpu_instruction_control.h"
#include "cpu_interrupt.h"
#include "cpu_registers.h"
#include "cpu_init_helper.h"

TEST(InterruptHandlerIsQueued_test, NotMasked) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(
            MEMORY::INTERRUPT_ENABLE,
            (uint8_t)CPU::InterruptType::JOYPAD);
    helper.addressDispatcher.write(
            MEMORY::INTERRUPT_FLAG,
            (uint8_t)CPU::InterruptType::JOYPAD);
    helper.registers.IME = true;
    CPU::InterruptHandler interruptHandler;
    bool isQueued = interruptHandler.isQueued(helper.addressDispatcher);
    EXPECT_EQ(isQueued, true);
}

TEST(InterruptHandlerIsQueued_test, Masked) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(
            MEMORY::INTERRUPT_ENABLE,
            ~(uint8_t)CPU::InterruptType::JOYPAD);
    helper.addressDispatcher.write(
            MEMORY::INTERRUPT_FLAG,
            (uint8_t)CPU::InterruptType::JOYPAD);
    helper.registers.IME = true;
    CPU::InterruptHandler interruptHandler;
    bool isQueued = interruptHandler.isQueued(helper.addressDispatcher);
    EXPECT_EQ(isQueued, false);
}

TEST(InterruptHandlerPop_test, Single) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(
            MEMORY::INTERRUPT_ENABLE,
            0xFF);
    helper.addressDispatcher.write(
            MEMORY::INTERRUPT_FLAG,
            ((uint8_t)CPU::InterruptType::JOYPAD));
    helper.registers.IME = true;
    CPU::InterruptHandler interruptHandler;
    auto interruptType = interruptHandler.pop(helper.addressDispatcher);
    EXPECT_EQ(interruptType, CPU::InterruptType::JOYPAD);
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::INTERRUPT_FLAG), 0);
}

TEST(InterruptHandlerPop_test, Priority) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(
            MEMORY::INTERRUPT_ENABLE,
            0xFF);
    helper.addressDispatcher.write(
            MEMORY::INTERRUPT_FLAG,
            ((uint8_t)CPU::InterruptType::JOYPAD) | ((uint8_t)CPU::InterruptType::VBLANK));
    helper.registers.IME = true;
    CPU::InterruptHandler interruptHandler;
    auto interruptType = interruptHandler.pop(helper.addressDispatcher);
    EXPECT_EQ(interruptType, CPU::InterruptType::VBLANK);
    EXPECT_EQ(helper.addressDispatcher.read(MEMORY::INTERRUPT_FLAG), (uint8_t)CPU::InterruptType::JOYPAD);
}

