#ifndef __PPU_H__
#define __PPU_H__

#include <stdint.h>
#include "gameboy/memory.h"
#include "gameboy/ppu_def.h"
#include "gameboy/ppu_tile.h"
#include "gameboy/ppu_sprite.h"

namespace GAMEBOY
{
    class PPU
    {
    private:
        AddressDispatcher& memory;
        PPU_Tilemap tilemap;
        PPU_Spritemap spritemap;
        LINE_BUFFERS m_line_buffers;
        // define mode lengths in terms of dots
        // note: extra ppu behaviour can delay mode 3
        // this is a later low priority TODO
        const int m_MODE0_LEN = 204;
        const int m_MODE2_LEN = 80;
        const int m_MODE3_LEN = 172;
        const int m_LINE_LEN = 456;
        const int m_DRAW_LINES = 144;
        const int m_FRAME_LINES = 154;
        enum class m_PPU_STATE
        {
            MODE0,
            MODE1,
            MODE2,
            MODE3
        };
        m_PPU_STATE m_state = m_PPU_STATE::MODE2;
        int m_dot_x = 0;
        int m_dot_y = 0;
        bool m_int_sel_lyc = false;
        bool m_int_sel_mode2 = false;
        bool m_int_sel_mode1 = false;
        bool m_int_sel_mode0 = false;
        // Stores the current status of all enabled STAT interrupt sources
        bool m_stat_line = false;
        // Updates & then returns true on rising edge of STAT interrupt line
        void m_stat_line_update();
        bool transition(m_PPU_STATE new_mode);
    public:
        PPU(AddressDispatcher& memory, LINE_BUFFERS line_buffers);
        bool tick();
        uint8_t mode_no();
        uint8_t stat();
        void stat(uint8_t value);
    };
};


#endif
