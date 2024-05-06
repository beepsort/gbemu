#ifndef __PPU_TILE_H__
#define __PPU_TILE_H__

#include "gameboy/memory.h"
#include "gameboy/ppu_def.h"

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
        uint8_t get_pixel(uint8_t x, uint8_t y);
    };

    class PPU_Tilecache
    {
    private:
        AddressDispatcher& memory;
        typedef std::shared_ptr<PPU_Tile> _PPU_TILE_PTR;
        std::array<std::optional<_PPU_TILE_PTR>, 256> cache;
    public:
        PPU_Tilecache(AddressDispatcher& memory)
        : memory(memory) {}
        void clear();
        std::shared_ptr<PPU_Tile> get(uint8_t index);
    };

    class PPU_Tilemap
    {
    private:
        AddressDispatcher& memory;
        PPU_Tilecache tilecache;
    public:
        enum class MAP_SELECT
        {
            // Use tilemap at 0x9800
            MAP0,
            // Use tilemap at 0x9C00
            MAP1
        };
        PPU_Tilemap(AddressDispatcher& memory)
        : memory(memory), tilecache(memory) {}
        /*
         * Visible area is 160x144 pixels out of 256x256 tile map
         */
        std::shared_ptr<LINE_PIXELS> render_line(MAP_SELECT map, uint8_t scroll_x, uint8_t scroll_y, uint8_t line);
    };
};


#endif
