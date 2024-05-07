#include "gameboy/ppu_tile.h"
#include "gameboy/memory_io.h"
#include <cassert>
#include <stdexcept>

typedef std::array<uint8_t, 16> _TILEBYTES;

std::unique_ptr<_TILEBYTES>
_getTileData(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t index,
        bool unsigned_mode)
{
    auto tile_data = std::make_unique<std::array<uint8_t, 16>>();
    const uint8_t byte_count = 16;
    if (unsigned_mode)
    {
        const uint16_t base_addr = GAMEBOY::VRAM_LO;
        uint16_t data_start_addr = base_addr + static_cast<uint16_t>(index)*byte_count;
        for (size_t i=0; i<byte_count; i++)
        {
            uint8_t i_data = memory.read(data_start_addr + i, GAMEBOY::MemoryAccessSource::PPU);
            (*tile_data)[i] = i_data;
        }
    }
    else
    {
        const uint16_t base_addr = GAMEBOY::VRAM_LO + 0x1000;
        int8_t index_signed = static_cast<int8_t>(index);
        uint16_t data_start_addr = base_addr + static_cast<int16_t>(index_signed)*byte_count;
        for (size_t i=0; i<byte_count; i++)
        {
            uint8_t i_data = memory.read(data_start_addr + i, GAMEBOY::MemoryAccessSource::PPU);
            (*tile_data)[i] = i_data;
        }
    }
    return tile_data;
}

void _tileBytesToXY(uint8_t* in_begin, uint8_t* in_end, uint8_t* out_begin, uint8_t* out_end)
{
    uint8_t hi_byte;
    uint8_t lo_byte;
    uint8_t* out_it = out_begin;
    for (uint8_t* in_it=in_begin; in_it!=in_end; in_it++)
    {
        hi_byte = *in_it;
        ++in_it;
        assert(in_it != out_it);
        lo_byte = *in_it;
        for (size_t pix_col = 0; pix_col<8; pix_col++)
        {
            uint8_t pix_mask = 0x80 >> pix_col;
            uint8_t hi_bit = hi_byte & pix_mask ? 0x02 : 0x00;
            uint8_t lo_bit = lo_byte & pix_mask ? 0x01 : 0x00;
            if (out_it != out_end)
            {
                *(out_it++) = hi_bit | lo_bit;
            }
        }
    }
}

GAMEBOY::PPU_Tile::PPU_Tile(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t index)
{
    uint8_t lcdc_register = memory.read(GAMEBOY::IOHandler::PPU_REG_LCDC, MemoryAccessSource::PPU);
    bool unsigned_mode = lcdc_register & 0x10;
    auto tile_bytes = _getTileData(memory, index, unsigned_mode);
    _tileBytesToXY(tile_bytes->begin(), tile_bytes->end(), m_tile_data.begin(), m_tile_data.end());
}

uint8_t GAMEBOY::PPU_Tile::get_pixel(uint8_t x, uint8_t y)
{
    if (x >= 8 || y >= 8)
    {
        throw std::out_of_range("Requested pixel out of range");
    }
    return m_tile_data.at(y*8 + x);
}

void GAMEBOY::PPU_Tilecache::clear()
{
    cache = std::array<std::optional<_PPU_TILE_PTR>, 256>();
}

std::shared_ptr<GAMEBOY::PPU_Tile> GAMEBOY::PPU_Tilecache::get(uint8_t index)
{
    if (cache[index].has_value())
    {
        return cache[index].value();
    }
    _PPU_TILE_PTR tile = std::make_shared<GAMEBOY::PPU_Tile>(memory, index);
    cache[index] = std::make_optional(tile);
    return tile;
}

uint8_t
bg_color_id_to_shade(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t pix_color_id)
{
    uint8_t bg_palette = memory.read(GAMEBOY::IOHandler::PPU_REG_BGP, GAMEBOY::MemoryAccessSource::PPU);
    switch (pix_color_id)
    {
        case 0:
            return bg_palette & 0x03;
        case 1:
            return (bg_palette & 0x0C)>>2;
        case 2:
            return (bg_palette & 0x30)>>4;
        case 3:
            return (bg_palette & 0xC0)>>6;
        default:
            throw std::out_of_range("Invalid pixel color id");
    }
}

std::shared_ptr<GAMEBOY::LINE_PIXELS> GAMEBOY::PPU_Tilemap::render_line(GAMEBOY::PPU_Tilemap::MAP_SELECT map, uint8_t scroll_x, uint8_t scroll_y, uint8_t line)
{
    if (memory.vram_poll_modified())
    {
        tilecache.clear();
    }
    // Visible area is 160x144 pixels out of 256x256 tile map
    const uint8_t SCREEN_SIZE_X = 160;
    const uint8_t SCREEN_SIZE_Y = 144;
    if (line >= SCREEN_SIZE_Y)
    {
        throw std::out_of_range("Line beyond screen size of 160 pixels attempted to be drawn");
    }
    uint16_t map_start = map == MAP_SELECT::MAP0 ? 0x9800 : 0x9C00;
    std::shared_ptr<LINE_PIXELS> line_pix = std::make_shared<LINE_PIXELS>();
    // calculate where in the virtual map image is being drawn
    // note that using uint8_t allows expected overflow/wrap around
    uint8_t map_y = scroll_y + line;
    uint8_t map_x = scroll_x;
    // tiles are 8x8 pixels, so calculate which we are reading from
    uint8_t map_tile_y = map_y / 8;
    uint8_t tile_y = map_y % 8;
    for (uint8_t i = 0; i < SCREEN_SIZE_X; i++)
    {
        uint8_t map_tile_x = (map_x+i) / 8;
        uint8_t tile_index = memory.read(map_start + map_tile_y*32 + map_tile_x, MemoryAccessSource::PPU);
        auto tile = tilecache.get(tile_index);
        uint8_t tile_x = (map_x+i) % 8;
        // copy data from tile for current pixel
        uint8_t pix_color_id = tile->get_pixel(tile_x, tile_y);
        // translate color id to 2 bit shade
        uint8_t pix_shade = bg_color_id_to_shade(memory, pix_color_id);
        (*line_pix)[i] = pix_shade;
    }
    return line_pix;
}

