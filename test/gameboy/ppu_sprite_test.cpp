#include <gtest/gtest.h>
#include "gameboy/ppu_sprite.h"
#include "gameboy/ppu.h"
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
    helper.addressDispatcher.lock(GAMEBOY::AddressDispatcher::LOCKABLE::OAM);
    helper.addressDispatcher.lock(GAMEBOY::AddressDispatcher::LOCKABLE::VRAM);
    GAMEBOY::PPU_Spritemap spritemap(helper.addressDispatcher);
    auto line_buffer = std::make_shared<GAMEBOY::LINE_PIXELS>();
    spritemap.render_line(0, line_buffer);
    for (size_t i=0; i<8; i++)
    {
        EXPECT_EQ((*line_buffer)[i], i%4);
    }
    for (size_t i=8; i<line_buffer->size(); i++)
    {
        EXPECT_EQ((*line_buffer)[i], 0);
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

TEST(PPU_Sprite_test, ArrowSprite) {
    CpuInitHelper helper;
    // set bit 7 to enable PPU
    // set bit 1 to enable sprites
    helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x82);
    // init tile 0
    uint16_t tile_start = GAMEBOY::VRAM_LO;
    uint8_t sprite_bytes[] = {0x00, 0x00, 0x60, 0x60, 0x70, 0x70, 0x78, 0x78, 0x78, 0x78, 0x70, 0x70, 0x60, 0x60, 0x00, 0x00};
    for (int i=0; i<16; i++)
    {
        helper.addressDispatcher.write(tile_start+i, sprite_bytes[i]);
    }
    GAMEBOY::PPU_Sprite sprite(helper.addressDispatcher, 0, false);
    uint8_t sprite_render_bytes[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 3, 3, 0, 0, 0, 0, 0,
        0, 3, 3, 3, 0, 0, 0, 0,
        0, 3, 3, 3, 3, 0, 0, 0,
        0, 3, 3, 3, 3, 0, 0, 0,
        0, 3, 3, 3, 0, 0, 0, 0,
        0, 3, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };
    for (size_t y=0; y<8; y++)
    {
        for (size_t x=0; x<8; x++)
        {
            EXPECT_EQ(sprite.get_pixel(x, y), sprite_render_bytes[y*8+x]);
        }
    }
}

TEST(PPU_OamEntry_test, ArrowSprite) {
    CpuInitHelper helper;
    // set bit 7 to enable PPU
    // set bit 1 to enable sprites
    helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x82);
    // set OAM0 palette so 3=3, 2=2, 1=1
    helper.addressDispatcher.write(0xFF48, 0xE4);
    // init tile 0
    uint16_t tile_start = GAMEBOY::VRAM_LO;
    uint8_t sprite_bytes[] = {0x00, 0x00, 0x60, 0x60, 0x70, 0x70, 0x78, 0x78, 0x78, 0x78, 0x70, 0x70, 0x60, 0x60, 0x00, 0x00};
    for (int i=0; i<16; i++)
    {
        helper.addressDispatcher.write(tile_start+i, sprite_bytes[i]);
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
    GAMEBOY::PPU_OamEntry oam0(0, helper.addressDispatcher);
    GAMEBOY::PPU_Spritecache sprite_cache(helper.addressDispatcher);
    uint8_t sprite_render_bytes[] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 3, 3, 0, 0, 0, 0, 0,
        0, 3, 3, 3, 0, 0, 0, 0,
        0, 3, 3, 3, 3, 0, 0, 0,
        0, 3, 3, 3, 3, 0, 0, 0,
        0, 3, 3, 3, 0, 0, 0, 0,
        0, 3, 3, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };
    for (uint8_t line=0; line<8; line++)
    {
        GAMEBOY::LINE_PIXELS bg;
        auto line_buffer = std::make_shared<GAMEBOY::LINE_PIXELS>();
        oam0.render_line(line, bg, line_buffer, sprite_cache);
        for (size_t x=0; x<8; x++)
        {
            if (sprite_render_bytes[line*8+x]!=0)
            {
                EXPECT_EQ((*line_buffer)[x], sprite_render_bytes[line*8+x]);
            }
            else
            {
                EXPECT_EQ((*line_buffer)[x], 0);
            }
        }
    }
}


TEST(PPU_test, SpriteIntegration) {
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
    auto line_buffer = std::make_shared<GAMEBOY::LINE_PIXELS>();
    GAMEBOY::PPU ppu(helper.addressDispatcher, line_buffer);
    // run until outputs first line
    while (!ppu.tick()) {}
    for (size_t i=0; i<8; i++)
    {
        EXPECT_EQ((*line_buffer)[i], i%4);
    }
    for (size_t i=8; i<line_buffer->size(); i++)
    {
        EXPECT_EQ((*line_buffer)[i], 0);
    }
}

