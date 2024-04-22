#include <gtest/gtest.h>
#include "gameboy/ppu.h"
#include "cpu_init_helper.h"

TEST(PPU_test, StateMachine) {
    CpuInitHelper helper;
    // set bit 7 to enable PPU
    helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x80);
    GAMEBOY::PPU ppu(helper.addressDispatcher);
    for (int k=0; k<10; k++)
    {
        for (int i=0; i<144; i++)
        {
            for (int j=0; j<80; j++)
            {
                helper.addressDispatcher.write(GAMEBOY::VRAM_LO, 0x11);
                EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::VRAM_LO), 0x11);
                helper.addressDispatcher.write(GAMEBOY::OAM_LO, 0x00);
                EXPECT_NE(helper.addressDispatcher.read(GAMEBOY::OAM_LO), 0x00);
                EXPECT_EQ(ppu.mode_no(), 2);
                ppu.tick();
            }
            for (int j=0; j<172; j++)
            {
                helper.addressDispatcher.write(GAMEBOY::VRAM_LO, 0x00);
                EXPECT_NE(helper.addressDispatcher.read(GAMEBOY::VRAM_LO), 0x00);
                helper.addressDispatcher.write(GAMEBOY::OAM_LO, 0x00);
                EXPECT_NE(helper.addressDispatcher.read(GAMEBOY::OAM_LO), 0x00);
                EXPECT_EQ(ppu.mode_no(), 3);
                ppu.tick();
            }
            for (int j=0; j<204; j++)
            {
                helper.addressDispatcher.write(GAMEBOY::VRAM_LO, 0x33);
                EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::VRAM_LO), 0x33);
                helper.addressDispatcher.write(GAMEBOY::OAM_LO, 0x33);
                EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::OAM_LO), 0x33);
                EXPECT_EQ(ppu.mode_no(), 0);
                ppu.tick();
            }
        }
        for (int i=0; i<4560; i++)
        {
            helper.addressDispatcher.write(GAMEBOY::VRAM_LO, 0x44);
            EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::VRAM_LO), 0x44);
            helper.addressDispatcher.write(GAMEBOY::OAM_LO, 0x44);
            EXPECT_EQ(helper.addressDispatcher.read(GAMEBOY::OAM_LO), 0x44);
            EXPECT_EQ(ppu.mode_no(), 1);
            ppu.tick();
        }
    }
}
