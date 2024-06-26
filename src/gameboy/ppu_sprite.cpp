#include "gameboy/ppu_sprite.h"
#include "gameboy/memory_io.h"
#include <cassert>
#include <stdexcept>

typedef std::array<uint8_t, 16> _TILEBYTES;

std::vector<uint8_t>
_getSpriteData(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t index,
        bool large_sprite)
{
    const uint16_t base_addr = GAMEBOY::VRAM_LO;
    const uint16_t byte_count = large_sprite ? 32 : 16;
    std::vector<uint8_t> sprite_data;
    sprite_data.reserve(byte_count);
    uint16_t data_start_addr = base_addr + static_cast<uint16_t>(index)*byte_count;
    for (size_t i=0; i<byte_count; i++)
    {
        uint8_t i_data = memory.read(data_start_addr + i, GAMEBOY::MemoryAccessSource::PPU);
        sprite_data.push_back(i_data);
    }
    return sprite_data;
}

std::vector<uint8_t> _spriteBytesToXY(std::vector<uint8_t> sprite_bytes)
{
    std::vector<uint8_t> output;
    output.reserve(sprite_bytes.size()*4);
    uint8_t hi_byte;
    uint8_t lo_byte;
    for (size_t i=0; i<sprite_bytes.size()-1; i+=2)
    {
        hi_byte = sprite_bytes[i];
        lo_byte = sprite_bytes[i+1];
        for (size_t pix_col = 0; pix_col<8; pix_col++)
        {
            uint8_t pix_mask = 0x80 >> pix_col;
            uint8_t hi_bit = hi_byte & pix_mask ? 0x02 : 0x00;
            uint8_t lo_bit = lo_byte & pix_mask ? 0x01 : 0x00;
            output.push_back(hi_bit | lo_bit);
        }
    }
    return output;
}


GAMEBOY::PPU_Sprite::PPU_Sprite(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t index,
        bool large_mode)
: m_large_mode(large_mode)
{
    auto sprite_bytes = _getSpriteData(memory, index, m_large_mode);
    m_sprite_data = _spriteBytesToXY(sprite_bytes);
}

uint8_t GAMEBOY::PPU_Sprite::get_pixel(uint8_t x, uint8_t y)
{
    const uint8_t y_length = m_large_mode ? 16 : 8;
    const uint8_t x_length = 8;
    if (x >= x_length || y >= y_length)
    {
        throw std::out_of_range("Requested pixel out of range");
    }
    return m_sprite_data.at(y*x_length + x);
}

void GAMEBOY::PPU_Spritecache::clear()
{
    cache = std::array<std::optional<_PPU_SPRITE_PTR>, 256>();
}

std::shared_ptr<GAMEBOY::PPU_Sprite> GAMEBOY::PPU_Spritecache::get(uint8_t index, bool large_mode)
{
    if (cache[index].has_value())
    {
        return cache[index].value();
    }
    _PPU_SPRITE_PTR tile = std::make_shared<GAMEBOY::PPU_Sprite>(memory, index, large_mode);
    cache[index] = std::make_optional(tile);
    return tile;
}

inline
std::optional<uint8_t>
object_color_id_to_shade(
        GAMEBOY::AddressDispatcher& memory,
        uint8_t pix_color_id,
        uint8_t palette_no)
{
    uint8_t palette;
    if (palette_no == 0)
    {
        palette = memory.read(GAMEBOY::IOHandler::PPU_REG_OBP0, GAMEBOY::MemoryAccessSource::PPU);
    }
    else
    {
        palette = memory.read(GAMEBOY::IOHandler::PPU_REG_OBP1, GAMEBOY::MemoryAccessSource::PPU);
    }
    switch (pix_color_id)
    {
        case 0:
            return {};
        case 1:
            return (palette & 0x0C)>>2;
        case 2:
            return (palette & 0x30)>>4;
        case 3:
            return (palette & 0xC0)>>6;
        default:
            throw std::out_of_range("Invalid pixel color id");
    }
}

GAMEBOY::PPU_OamEntry::PPU_OamEntry(uint16_t oam_id, GAMEBOY::AddressDispatcher& memory)
: memory(memory)
{
    uint8_t lcdc_register = memory.read(GAMEBOY::IOHandler::PPU_REG_LCDC, GAMEBOY::MemoryAccessSource::PPU);
    m_large_mode = lcdc_register & 0x04;
    uint16_t base_addr = GAMEBOY::OAM_LO + 4*oam_id;
    m_y = memory.read(base_addr, GAMEBOY::MemoryAccessSource::PPU);
    m_x = memory.read(base_addr+1, GAMEBOY::MemoryAccessSource::PPU);
    m_tile_index = memory.read(base_addr+2, GAMEBOY::MemoryAccessSource::PPU);
    m_attrs = memory.read(base_addr+3, GAMEBOY::MemoryAccessSource::PPU);
}

void GAMEBOY::PPU_OamEntry::render_line(uint8_t line, LINE_PIXELS& bg, std::shared_ptr<LINE_PIXELS> line_buffer, PPU_Spritecache& spritecache)
{
    bool flip_y = m_attrs & 0x40;
    bool flip_x = m_attrs & 0x20;
    const uint8_t y_len = m_large_mode ? 16 : 8;
    const uint8_t x_len = 8;
    uint8_t obj_y = line + 16; // objs have 16 y pixels off-frame
    auto tile = spritecache.get(m_tile_index, m_large_mode);
    uint8_t palette_no = (m_attrs & 0x10) ? 1 : 0;
    for (uint8_t obj_x=8; obj_x<line_buffer->size()+8; obj_x++)
    {
        if (obj_x>=m_x && obj_x<m_x+x_len && obj_y>=m_y && obj_y<m_y+y_len)
        {
            uint8_t sprite_x = obj_x - m_x;
            auto sprite_buff_pix = &((*line_buffer)[obj_x-8]);
            if (flip_x)
            {
                sprite_x = x_len - sprite_x - 1;
            }
            uint8_t sprite_y = obj_y - m_y;
            if (flip_y)
            {
                sprite_y = y_len - sprite_y - 1;
            }
            auto pix_raw = tile->get_pixel(sprite_x, sprite_y);
            std::optional<uint8_t> shade = object_color_id_to_shade(memory, pix_raw, palette_no);
            // pixel of sprite is transparent
            if (!shade.has_value())
            {
                continue;
            }
            // no BG priority
            else if (!(m_attrs & 0x80))
            {
                *sprite_buff_pix = *shade;
            }
            // only draw over existing shade 0
            else if (bg[obj_x-8] == 0)
            {
                *sprite_buff_pix = *shade;
            }
        }
    }
}

void GAMEBOY::PPU_Spritemap::render_line(uint8_t line, std::shared_ptr<GAMEBOY::LINE_PIXELS> line_buffer)
{
    if (memory.vram_pop_modified())
    {
        spritecache.clear();
    }
    auto bg = *line_buffer;
    bool obj_enabled = memory.read(IOHandler::PPU_REG_LCDC) & 0x02;
    if (!obj_enabled)
    {
        return;
    }
    for (uint16_t i=39; i<40; i--)
    {
        PPU_OamEntry oam_entry(i, memory);
        oam_entry.render_line(line, bg, line_buffer, spritecache);
    }
}
