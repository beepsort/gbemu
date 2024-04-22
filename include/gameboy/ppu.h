#ifndef __PPU_H__
#define __PPU_H__

#include <stdint.h>
#include "gameboy/memory.h"
#include "gameboy/ppu_tile.h"

namespace GAMEBOY
{
    class PPU
    {
    private:
        AddressDispatcher& memory;
        // define mode lengths in terms of dots
        // note: extra ppu behaviour can delay mode 3
        // this is a later low priority TODO
        const int m_MODE0_LEN = 204;
        const int m_MODE1_LEN = 4560;
        const int m_MODE2_LEN = 80;
        const int m_MODE3_LEN = 172;
        const int m_LINE_LEN = 456;
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
        void transition(m_PPU_STATE new_mode);
    public:
        PPU(AddressDispatcher& memory)
        : memory(memory) {}
        void tick();
        uint8_t mode_no();
    };
};


#endif
