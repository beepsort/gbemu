#include "gameboy/ppu.h"
#include "gameboy/cpu_interrupt.h"
#include <stdexcept>

GAMEBOY::PPU::PPU(AddressDispatcher& memory, GAMEBOY::LINE_BUFFERS line_buffers)
: memory(memory), tilemap(memory), spritemap(memory), m_line_buffers(line_buffers)
{
    transition(m_PPU_STATE::MODE2);
}

bool GAMEBOY::PPU::transition(m_PPU_STATE new_mode)
{
    bool drawn_to_buffer = false;
    switch (new_mode)
    {
        case m_PPU_STATE::MODE0:
        {
            memory.unlock(AddressDispatcher::LOCKABLE::OAM);
            memory.unlock(AddressDispatcher::LOCKABLE::VRAM);
            break;
        }
        case m_PPU_STATE::MODE1:
        {
            // trigger vblank interrupt
            uint8_t interrupts = memory.read(INTERRUPT_FLAG);
            interrupts |= (uint8_t)InterruptType::VBLANK;
            memory.write(INTERRUPT_FLAG, interrupts);
            break;
        }
        case m_PPU_STATE::MODE2:
        {
            memory.lock(AddressDispatcher::LOCKABLE::OAM);
            break;
        }
        case m_PPU_STATE::MODE3:
        {
            memory.lock(AddressDispatcher::LOCKABLE::VRAM);
            // draw line
            // background
            uint8_t scy = memory.read(IOHandler::PPU_REG_SCY);
            uint8_t scx = memory.read(IOHandler::PPU_REG_SCX);
            uint8_t lcdc = memory.read(IOHandler::PPU_REG_LCDC);
            // bit 3 low = MAP0, high = MAP1
            auto map = lcdc & 0x08 ? PPU_Tilemap::MAP_SELECT::MAP1 : PPU_Tilemap::MAP_SELECT::MAP0;
            auto bg = tilemap.render_line(map, scx, scy, m_dot_y);
            *m_line_buffers.bg = *bg;
            // window
            // sprites
            spritemap.render_line(m_dot_y, m_line_buffers);
            drawn_to_buffer = true;
            break;
        }
        default:
            throw std::invalid_argument("Non-existent PPU mode supplied");
    }
    m_state = new_mode;
    return drawn_to_buffer;
}

bool GAMEBOY::PPU::tick()
{
    bool ppu_enabled = memory.read(IOHandler::PPU_REG_LCDC) & 0x80;
    if (!ppu_enabled)
    {
        m_dot_y = m_FRAME_LINES - 1;
        m_state = m_PPU_STATE::MODE1;
        memory.unlock(AddressDispatcher::LOCKABLE::OAM);
        memory.unlock(AddressDispatcher::LOCKABLE::VRAM);
        return false;
    }
    bool drawn_to_buffer = false;
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
                drawn_to_buffer = transition(m_PPU_STATE::MODE3);
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
    m_stat_line_update();
    memory.write(IOHandler::PPU_REG_LY, m_dot_y, MemoryAccessSource::PPU);
    memory.write(IOHandler::PPU_REG_STAT, stat(), MemoryAccessSource::PPU);
    return drawn_to_buffer;
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

uint8_t GAMEBOY::PPU::stat()
{
    uint8_t output_register = mode_no();
    bool lyc_eq_ly = m_dot_y == memory.read(IOHandler::PPU_REG_LYC);
    output_register |= lyc_eq_ly ? 0x04 : 0x00;
    // keep user writable bits 6-3
    uint8_t stat_mem = memory.read(IOHandler::PPU_REG_STAT) & 0x78;
    output_register |= stat_mem;
    return output_register;
}

void GAMEBOY::PPU::m_stat_line_update()
{
    bool new_stat_line = false;
    uint8_t stat_reg = stat();
    uint8_t mode = mode_no();
    uint8_t lyc = memory.read(IOHandler::PPU_REG_LYC);
    if (stat_reg & 0x04 && m_dot_y == lyc)
    {
        new_stat_line = true;
    }
    else if (stat_reg & 0x08 && mode == 0)
    {
        new_stat_line = true;
    }
    else if (stat_reg & 0x10 && mode == 1)
    {
        new_stat_line = true;
    }
    else if (stat_reg & 0x20 && mode == 2)
    {
        new_stat_line = true;
    }
    if (!m_stat_line && new_stat_line)
    {
        m_stat_line = new_stat_line;
        // trigger interrupt
        uint8_t interrupts = memory.read(INTERRUPT_FLAG);
        interrupts |= (uint8_t)InterruptType::LCD;
        memory.write(INTERRUPT_FLAG, interrupts);
    }
}
