#ifndef __PPU_SPRITE_H__
#define __PPU_SPRITE_H__

#include <stdint.h>
#include <memory>
#include <array>
#include <vector>
#include "gameboy/memory.h"

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

    typedef std::array<std::optional<uint8_t>, 160> LINE_PIXELS_OPT;

    class PPU_Spritemap
    {
    private:
        AddressDispatcher& memory;
        PPU_Spritecache spritecache;
    public:
        PPU_Spritemap(AddressDispatcher& memory)
        : memory(memory), spritecache(memory) {}
        std::shared_ptr<LINE_PIXELS_OPT> render_line(uint8_t line);
    };
};


#endif
