#include <gtest/gtest.h>
#include "gameboy/ppu_sprite.h"
#include "cpu_init_helper.h"

TEST(PPU_Spritemap_test, SingleLine) {
    CpuInitHelper helper;
    // set bit 7 to enable PPU
    // set bit 1 to enable sprites
    helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x82);
    // set OAM0 palette so 3=3, 2=2, 1=1
    helper.addressDispatcher.write(0xFF48, 0xE4);
    // init tile 0
    uint16_t tile_start = GAMEBOY::VRAM_LO;
    bool lo = true;
    for (int i=0; i<16; i++)
    {
        if (lo)
        {
            helper.addressDispatcher.write(tile_start+i, 0x33);
        }
        else
        {
            helper.addressDispatcher.write(tile_start+i, 0x55);
        }
        lo = !lo;
    }
    // init oam
    // y position
    helper.addressDispatcher.write(GAMEBOY::OAM_LO, 16);
    // x position
    helper.addressDispatcher.write(GAMEBOY::OAM_LO+1, 8);
    // tile index
    helper.addressDispatcher.write(GAMEBOY::OAM_LO+2, 0);
    // oam attrs
    helper.addressDispatcher.write(GAMEBOY::OAM_LO+3, 0);
    GAMEBOY::PPU_Spritemap spritemap(helper.addressDispatcher);
    auto line = spritemap.render_line(0);
    for (size_t i=0; i<8; i++)
    {
        if (i%4==0)
        {
            EXPECT_FALSE((*line)[i].has_value());
        }
        else
        {
            EXPECT_TRUE((*line)[i].has_value());
            EXPECT_EQ((*line)[i].value(), i%4);
        }
    }
    for (size_t i=8; i<line->size(); i++)
    {
        EXPECT_FALSE((*line)[i].has_value());
    }
}

TEST(PPU_Sprite_test, SpriteAlone) {
    CpuInitHelper helper;
    // set bit 7 to enable PPU
    // set bit 1 to enable sprites
    helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x82);
    // init tile 0
    uint16_t tile_start = GAMEBOY::VRAM_LO;
    bool lo = true;
    for (int i=0; i<16; i++)
    {
        if (lo)
        {
            helper.addressDispatcher.write(tile_start+i, 0x33);
        }
        else
        {
            helper.addressDispatcher.write(tile_start+i, 0x55);
        }
        lo = !lo;
    }
    GAMEBOY::PPU_Sprite sprite(helper.addressDispatcher, 0, false);
    for (size_t y=0; y<8; y++)
    {
        for (size_t x=0; x<8; x++)
        {
            EXPECT_EQ(sprite.get_pixel(x, y), x%4);
        }
    }
}
