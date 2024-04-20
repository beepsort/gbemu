#ifndef __PPU_TILE_H__
#define __PPU_TILE_H__

#include <stdint.h>
#include <memory>
#include <array>
#include "gameboy/memory.h"

namespace GAMEBOY
{
    class PPU_Tile
    {
    private:
        /*
         * Store colour IDs for each pixel of the tile
         * Translated to shades using the BGP register at 0xFF47
         * Stored as rows of pixels, starting from the top left
         */
        std::array<uint8_t, 8*8> m_tile_data;
    public:
        PPU_Tile(AddressDispatcher& memory, uint8_t index);
    };

    class PPU_Sprite
    {
    private:
        /*
         * Store colour IDs for each pixel of the sprite
         * Translated to shades using the OBP registers 0xFF48 & 0xFF49
         * Stored as rows of pixels, starting from the top left
         */
        std::array<uint8_t, 8*8> m_sprite_data;
    public:
        PPU_Sprite(AddressDispatcher& memory, uint8_t index);
    };
};


#endif
