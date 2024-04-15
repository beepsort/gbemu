#include <gtest/gtest.h>
#include "gameboy/cpu_instruction_control.h"
#include "gameboy/cpu_interrupt.h"
#include "gameboy/cpu_registers.h"
#include "cpu_init_helper.h"

TEST(InterruptHandlerIsQueued_test, NotMasked) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(
            GAMEBOY::INTERRUPT_ENABLE,
            (uint8_t)GAMEBOY::InterruptType::JOYPAD);
    helper.addressDispatcher.write(
            GAMEBOY::INTERRUPT_FLAG,
            (uint8_t)GAMEBOY::InterruptType::JOYPAD);
    helper.registers.IME = true;
    GAMEBOY::InterruptHandler interruptHandler;
    bool isQueued = interruptHandler.isQueued(helper.addressDispatcher);
    EXPECT_EQ(isQueued, true);
}

TEST(InterruptHandlerIsQueued_test, Masked) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(
            GAMEBOY::INTERRUPT_ENABLE,
            ~(uint8_t)GAMEBOY::InterruptType::JOYPAD);
    helper.addressDispatcher.write(
            GAMEBOY::INTERRUPT_FLAG,
            (uint8_t)GAMEBOY::InterruptType::JOYPAD);
    helper.registers.IME = true;
    GAMEBOY::InterruptHandler interruptHandler;
    bool isQueued = interruptHandler.isQueued(helper.addressDispatcher);
    EXPECT_EQ(isQueued, false);
}

TEST(InterruptHandlerPop_test, Single) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(
            GAMEBOY::INTERRUPT_ENABLE,
            0xFF);
    helper.addressDispatcher.write(
            GAMEBOY::INTERRUPT_FLAG,
            ((uint8_t)GAMEBOY::InterruptType::JOYPAD));
    helper.registers.IME = true;
    GAMEBOY::InterruptHandler interruptHandler;
    auto interruptType = interruptHandler.pop(helper.addressDispatcher);
    EXPECT_EQ(interruptType, GAMEBOY::InterruptType::JOYPAD);
    EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::INTERRUPT_FLAG), 0);
}

TEST(InterruptHandlerPop_test, Priority) {
    CpuInitHelper helper;
    helper.addressDispatcher.write(
            GAMEBOY::INTERRUPT_ENABLE,
            0xFF);
    helper.addressDispatcher.write(
            GAMEBOY::INTERRUPT_FLAG,
            ((uint8_t)GAMEBOY::InterruptType::JOYPAD) | ((uint8_t)GAMEBOY::InterruptType::VBLANK));
    helper.registers.IME = true;
    GAMEBOY::InterruptHandler interruptHandler;
    auto interruptType = interruptHandler.pop(helper.addressDispatcher);
    EXPECT_EQ(interruptType, GAMEBOY::InterruptType::VBLANK);
    EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::INTERRUPT_FLAG), (uint8_t)GAMEBOY::InterruptType::JOYPAD);
}

