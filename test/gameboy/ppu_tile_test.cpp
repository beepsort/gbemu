#include <gtest/gtest.h>
#include "gameboy/ppu_tile.h"
#include "cpu_init_helper.h"

TEST(PPU_Tilemap_test, BgSingleLine) {
    CpuInitHelper helper;
    // set bit 7 to enable PPU
    // set bit 4 to operate in unsigned 0x8000 base mode
    // unset bit 3 to map BG from 0x9800
    // set bit 0 to enable BG
    helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x91);
    // set BG palette so 3=3, 2=2, 1=1, 0=0
    helper.addressDispatcher.write(0xFF47, 0xE4);
    // init tiles
    for (uint16_t tile_num = 0; tile_num<=0xFF; tile_num++)
    {
        uint16_t tile_start = GAMEBOY::VRAM_LO + tile_num * 16;
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
    }
    // init map
    for (uint16_t map_index = 0; map_index < 0x400; map_index++)
    {
        uint16_t map_base_addr = 0x9800;
        helper.addressDispatcher.write(map_base_addr+map_index, 0);
    }
    GAMEBOY::PPU_Tilemap tilemap(helper.addressDispatcher);
    auto line = tilemap.render_line(
            GAMEBOY::PPU_Tilemap::MAP_SELECT::MAP0,
            8,
            0,
            0);
    for (size_t i=0; i<line->size(); i++)
    {
        EXPECT_EQ((*line)[i], i%4);
    }
}

TEST(PPU_Tile_test, AllUnsigned) {
    for (uint16_t testing_tile_num = 0; testing_tile_num<=0xFF; testing_tile_num++)
    {
        CpuInitHelper helper;
        // set bit 7 to enable PPU
        // set bit 4 to operate in unsigned 0x8000 base mode
        // set bit 0 to enable BG
        helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x91);
        // init tile 0
        uint16_t tile_start = GAMEBOY::VRAM_LO + testing_tile_num * 16;
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
        GAMEBOY::PPU_Tile tile(helper.addressDispatcher, testing_tile_num);
        for (size_t y=0; y<8; y++)
        {
            for (size_t x=0; x<8; x++)
            {
                EXPECT_EQ(tile.get_pixel(x, y), x%4);
            }
        }
    }
}

TEST(PPU_getTileData_test, PositiveSigned) {
    for (uint16_t testing_tile_num = 0; testing_tile_num<=0x7F; testing_tile_num++)
    {
        CpuInitHelper helper;
        // set bit 7 to enable PPU
        // unset bit 4 to operate in signed 0x9000 base mode
        // set bit 0 to enable BG
        helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x81);
        uint16_t tile_start = GAMEBOY::VRAM_LO + 0x1000 + testing_tile_num * 16;
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
        GAMEBOY::PPU_Tile tile(helper.addressDispatcher, testing_tile_num);
        for (size_t y=0; y<8; y++)
        {
            for (size_t x=0; x<8; x++)
            {
                EXPECT_EQ(tile.get_pixel(x, y), x%4);
            }
        }
    }
}

TEST(PPU_getTileData_test, NegativeSigned) {
    for (uint16_t testing_tile_num = 0x80; testing_tile_num<=0xFF; testing_tile_num++)
    {
        CpuInitHelper helper;
        // set bit 7 to enable PPU
        // unset bit 4 to operate in signed 0x9000 base mode
        // set bit 0 to enable BG
        helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x81);
        uint16_t tile_start = GAMEBOY::VRAM_LO + testing_tile_num * 16;
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
        GAMEBOY::PPU_Tile tile(helper.addressDispatcher, testing_tile_num);
        for (size_t y=0; y<8; y++)
        {
            for (size_t x=0; x<8; x++)
            {
                EXPECT_EQ(tile.get_pixel(x, y), x%4);
            }
        }
    }
}

