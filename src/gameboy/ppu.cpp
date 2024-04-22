#include "gameboy/ppu.h"
#include <stdexcept>

GAMEBOY::PPU::PPU(AddressDispatcher& memory)
: memory(memory)
{
    transition(m_PPU_STATE::MODE2);
}

void GAMEBOY::PPU::transition(m_PPU_STATE new_mode)
{
    switch (new_mode)
    {
        case m_PPU_STATE::MODE0:
            memory.unlock(AddressDispatcher::LOCKABLE::OAM);
            memory.unlock(AddressDispatcher::LOCKABLE::VRAM);
            // trigger stat interrupt if mode0 stat on
            break;
        case m_PPU_STATE::MODE1:
            // trigger stat interrupt if mode1 stat on
            // trigger vblank interrupt
            break;
        case m_PPU_STATE::MODE2:
            memory.lock(AddressDispatcher::LOCKABLE::OAM);
            // trigger stat interrupt if mode2 stat on
            break;
        case m_PPU_STATE::MODE3:
            memory.lock(AddressDispatcher::LOCKABLE::VRAM);
            // draw line
            break;
        default:
            throw std::invalid_argument("Non-existent PPU mode supplied");
    }
    m_state = new_mode;
}

void GAMEBOY::PPU::tick()
{
    switch (m_state)
    {
        case m_PPU_STATE::MODE0:
            if (++m_dot_x == m_LINE_LEN)
            {
                m_dot_x = 0;
                if (++m_dot_y == m_DRAW_LINES)
                {
                    transition(m_PPU_STATE::MODE1);
                }
                else
                {
                    transition(m_PPU_STATE::MODE2);
                }
            }
            break;
        case m_PPU_STATE::MODE1:
            if (++m_dot_x == m_LINE_LEN)
            {
                m_dot_x = 0;
                if (++m_dot_y == m_FRAME_LINES)
                {
                    m_dot_y = 0;
                    transition(m_PPU_STATE::MODE2);
                }
            }
            break;
        case m_PPU_STATE::MODE2:
            if (++m_dot_x == m_MODE2_LEN)
            {
                transition(m_PPU_STATE::MODE3);
            }
            break;
        case m_PPU_STATE::MODE3:
            if (++m_dot_x == m_MODE2_LEN+m_MODE3_LEN)
            {
                transition(m_PPU_STATE::MODE0);
            }
            break;
        default:
            throw std::invalid_argument("Non-existent PPU mode enabled, possible memory corruption");
    }
}

uint8_t GAMEBOY::PPU::mode_no()
{
    switch (m_state)
    {
        case m_PPU_STATE::MODE0:
            return 0;
        case m_PPU_STATE::MODE1:
            return 1;
        case m_PPU_STATE::MODE2:
            return 2;
        case m_PPU_STATE::MODE3:
            return 3;
        default:
            throw std::invalid_argument("Non-existent PPU mode enabled, possible memory corruption");
    }
}
