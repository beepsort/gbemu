#include "gameboy/ppu_tile.h"
#include "gameboy/memory_io.h"

typedef std::array<uint8_t, 16> _TILEBYTES;

std::unique_ptr<_TILEBYTES>
_getTileData(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t index,
        bool unsigned_mode)
{
    auto tile_data = std::make_unique<std::array<uint8_t, 16>>();
    if (unsigned_mode)
    {
        const uint16_t base_addr = GAMEBOY::VRAM_LO;
        uint16_t data_start_addr = base_addr + static_cast<uint16_t>(index)*16;
        for (size_t i=0; i<16; i++)
        {
            uint8_t i_data = memory.read(data_start_addr + i);
            (*tile_data)[i] = i_data;
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
            (*tile_data)[i] = i_data;
        }
    }
    return tile_data;
}

void _tileBytesToXY(std::unique_ptr<_TILEBYTES> bytes, uint8_t* pixels)
{
    uint8_t hi_byte;
    uint8_t lo_byte;
    for (size_t i=0; i<=bytes->size()-2; i+=2)
    {
        hi_byte = bytes->at(i);
        lo_byte = bytes->at(i+1);
        size_t pix_row = i/8;
        for (size_t pix_col = 0; pix_col<8; pix_col++)
        {
            uint8_t pix_mask = 0x01 << pix_col;
            uint8_t hi_bit = hi_byte & pix_mask ? 0x02 : 0x01;
            uint8_t lo_bit = lo_byte & pix_mask ? 0x02 : 0x01;
            pixels[pix_col*8 + pix_row] = hi_bit | lo_bit;
        }
    }
}

GAMEBOY::PPU_Tile::PPU_Tile(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t index)
{
    uint8_t lcdc_register = memory.read(GAMEBOY::IOHandler::PPU_REG_LCDC);
    bool unsigned_mode = lcdc_register & 0x10;
    auto tile_bytes = _getTileData(memory, index, unsigned_mode);
}

GAMEBOY::PPU_Sprite::PPU_Sprite(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t index)
{
    auto tile_bytes = _getTileData(memory, index, true);
    _tileBytesToXY(std::move(tile_bytes), m_sprite_data);
}

