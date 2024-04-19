#include "gameboy/ppu.h"
#include "gameboy/memory.h"
#include "gameboy/memory_io.h"

std::array<uint8_t, 16>
GAMEBOY::PPU::getTile(uint8_t index)
{
    uint8_t lcdc_register = memory.read(GAMEBOY::IOHandler::PPU_REG_LCDC);
    bool unsigned_mode = lcdc_register & 0x10;
    auto tile_data = std::array<uint8_t, 16>();
    if (unsigned_mode)
    {
        const uint16_t base_addr = GAMEBOY::VRAM_LO;
        uint16_t data_start_addr = base_addr + static_cast<uint16_t>(index)*16;
        for (size_t i=0; i<16; i++)
        {
            uint8_t i_data = memory.read(data_start_addr + i);
            tile_data[i] = i_data;
        }
    }
    else
    {
        const uint16_t base_addr = GAMEBOY::VRAM_LO + 0x1000;
        int8_t index_signed = static_cast<int8_t>(index);
        uint16_t data_start_addr = base_addr + static_cast<int16_t>(index_signed)*16;
        for (size_t i=0; i<16; i++)
        {
            uint8_t i_data = memory.read(data_start_addr + i);
            tile_data[i] = i_data;
        }
    }
    return tile_data;
}

std::array<uint8_t, 16>
GAMEBOY::PPU::getSprite(uint8_t index)
{
    auto tile_data = std::array<uint8_t, 16>();
    const uint16_t base_addr = GAMEBOY::VRAM_LO;
    uint16_t data_start_addr = base_addr + static_cast<uint16_t>(index)*16;
    for (size_t i=0; i<16; i++)
    {
        uint8_t i_data = memory.read(data_start_addr + i);
        tile_data[i] = i_data;
    }
    return tile_data;
}
