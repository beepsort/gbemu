#ifndef __PPU_SPRITE_H__
#define __PPU_SPRITE_H__

#include <vector>
#include "gameboy/memory.h"
#include "gameboy/ppu_def.h"

namespace GAMEBOY
{
    class PPU_Sprite
    {
    private:
        /*
         * Store colour IDs for each pixel of the sprite
         * Translated to shades using the OBP registers 0xFF48 & 0xFF49
         * Stored as rows of pixels, starting from the top left
         */
        std::vector<uint8_t> m_sprite_data;
        /*
         * True = 8x16 mode
         * False = 8x8 Mode
         */
        bool m_large_mode;
    public:
        PPU_Sprite(AddressDispatcher& memory, uint8_t index, bool large_mode);
        uint8_t get_pixel(uint8_t x, uint8_t y);
    };

    class PPU_Spritecache
    {
    private:
        AddressDispatcher& memory;
        typedef std::shared_ptr<PPU_Sprite> _PPU_SPRITE_PTR;
        std::array<std::optional<_PPU_SPRITE_PTR>, 256> cache;
    public:
        PPU_Spritecache(AddressDispatcher& memory)
        : memory(memory) {}
        void clear();
        std::shared_ptr<PPU_Sprite> get(uint8_t index, bool large_mode);
    };

    class PPU_OamEntry
    {
    private:
        AddressDispatcher& memory;
        uint8_t m_x;
        uint8_t m_y;
        uint8_t m_tile_index;
        // attributes
        uint8_t m_attrs;
        bool m_large_mode;
    public:
        PPU_OamEntry(uint16_t oam_id, AddressDispatcher& memory);
        void render_line(uint8_t line, LINE_PIXELS& bg, std::shared_ptr<LINE_PIXELS> line_buffer, PPU_Spritecache& spritecache);
    };

    class PPU_Spritemap
    {
    private:
        AddressDispatcher& memory;
        PPU_Spritecache spritecache;
    public:
        PPU_Spritemap(AddressDispatcher& memory)
        : memory(memory), spritecache(memory) {}
        void render_line(uint8_t line, std::shared_ptr<GAMEBOY::LINE_PIXELS> line_buffer);
    };
};


#endif
