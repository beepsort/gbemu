#include <gtest/gtest.h>
#include "gameboy/ppu_tile.h"
#include "cpu_init_helper.h"

TEST(PPU_Tilemap_test, Debug) {
    CpuInitHelper helper;
    // set bit 7 to enable PPU
    // set bit 4 to operate in unsigned 0x8000 base mode
    // unset bit 3 to map BG from 0x9800
    helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x90);
    helper.addressDispatcher.write(0xFF47, 0xE4);
    // init tiles
    for (uint16_t tile_num = 0; tile_num<=0xFF; tile_num++)
    {
        uint16_t tile_start = GAMEBOY::VRAM_LO + tile_num * 16;
        for (int i=0; i<16; i++)
        {
            helper.addressDispatcher.write(tile_start+i, 0x55);
        }
    }
    // init map
    for (uint16_t map_index = 0; map_index < 0x400; map_index++)
    {
        uint16_t map_base_addr = 0x9800;
        helper.addressDispatcher.write(map_base_addr+map_index, (uint8_t)map_index);
    }
    GAMEBOY::PPU_Tilemap tilemap(helper.addressDispatcher);
    auto line = tilemap.render_line(
            GAMEBOY::PPU_Tilemap::MAP_SELECT::MAP0,
            55,
            55,
            9);
    for (size_t i=0; i<line->size(); i++)
    {
        printf("%X ", (*line)[i]);
    }
    printf("\n");
}
//TEST(PPU_getTileData_test, AllUnsigned) {
//    for (uint16_t testing_tile_num = 0; testing_tile_num<=0xFF; testing_tile_num++)
//    {
//        CpuInitHelper helper;
//        // set bit 4 to operate in unsigned 0x8000 base mode
//        helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x10);
//        uint16_t tile_start = GAMEBOY::VRAM_LO + testing_tile_num * 16;
//        for (int i=0; i<16; i++)
//        {
//            helper.addressDispatcher.write(tile_start+i, 0x55);
//        }
//        GAMEBOY::PPU_Tile tile(helper.addressDispatcher, testing_tile_num);
//        for (int i=0; i<16; i++)
//        {
//            EXPECT_EQ((*tile_data)[i], 0x55);
//        }
//    }
//}
//
//TEST(PPU_getTileData_test, PositiveSigned) {
//    for (uint16_t testing_tile_num = 0; testing_tile_num<=0x7F; testing_tile_num++)
//    {
//        CpuInitHelper helper;
//        // unset bit 4 to operate in signed 0x9000 base mode
//        helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x00);
//        uint16_t tile_start = GAMEBOY::VRAM_LO + 0x1000 + testing_tile_num * 16;
//        for (int i=0; i<16; i++)
//        {
//            helper.addressDispatcher.write(tile_start+i, 0x55);
//        }
//        GAMEBOY::PPU_Tile tile(helper.addressDispatcher, testing_tile_num);
//        for (int i=0; i<16; i++)
//        {
//            EXPECT_EQ((*tile_data)[i], 0x55);
//        }
//    }
//}
//
//TEST(PPU_getTileData_test, NegativeSigned) {
//    for (uint16_t testing_tile_num = 0x80; testing_tile_num<=0xFF; testing_tile_num++)
//    {
//        CpuInitHelper helper;
//        // unset bit 4 to operate in signed 0x9000 base mode
//        helper.addressDispatcher.write(GAMEBOY::IOHandler::PPU_REG_LCDC, 0x00);
//        uint16_t tile_start = GAMEBOY::VRAM_LO + testing_tile_num * 16;
//        for (int i=0; i<16; i++)
//        {
//            helper.addressDispatcher.write(tile_start+i, 0x55);
//        }
//        GAMEBOY::PPU_Tile tile(helper.addressDispatcher, testing_tile_num);
//        for (int i=0; i<16; i++)
//        {
//            EXPECT_EQ((*tile_data)[i], 0x55);
//        }
//    }
//}
//
