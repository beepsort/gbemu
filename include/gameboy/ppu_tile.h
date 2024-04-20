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
        uint8_t m_tile_data[8*8];
    public:
        PPU_Tile(AddressDispatcher& memory, uint8_t index);
    };

    class PPU_Sprite
    {
    private:
        uint8_t m_sprite_data[8*8];
    public:
        PPU_Sprite(AddressDispatcher& memory, uint8_t index);
    };
};


#endif
