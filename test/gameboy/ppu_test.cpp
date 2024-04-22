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
                EXPECT_EQ(ppu.mode_no(), 2);
                ppu.tick();
            }
            for (int j=0; j<172; j++)
            {
                EXPECT_EQ(ppu.mode_no(), 3);
                ppu.tick();
            }
            for (int j=0; j<204; j++)
            {
                EXPECT_EQ(ppu.mode_no(), 0);
                ppu.tick();
            }
        }
        for (int i=0; i<4560; i++)
        {
            EXPECT_EQ(ppu.mode_no(), 1);
            ppu.tick();
        }
    }
}
