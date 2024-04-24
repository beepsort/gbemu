#include "gameboy/input.h"

uint8_t GAMEBOY::InputHandler::joyp(bool btn_sel, bool dpad_sel)
{
    uint8_t result = 0x0F;
    if (dpad_sel)
    {
        result &= m_dpad_state;
    }
    if (btn_sel)
    {
        result &= m_btn_state;
    }
    return result;
}

void GAMEBOY::InputHandler::btn_down(BUTTON btn)
{
    uint8_t old_dpad_state = m_dpad_state;
    uint8_t old_btn_state = m_btn_state;
    switch (btn)
    {
        case BUTTON::UP:
            m_dpad_state &= 0x0B;
            break;
        case BUTTON::DOWN:
            m_dpad_state &= 0x07;
            break;
        case BUTTON::LEFT:
            m_dpad_state &= 0x0D;
            break;
        case BUTTON::RIGHT:
            m_dpad_state &= 0x0E;
            break;
        case BUTTON::A:
            m_btn_state &= 0x0E;
            break;
        case BUTTON::B:
            m_btn_state &= 0x0D;
            break;
        case BUTTON::START:
            m_btn_state &= 0x07;
            break;
        case BUTTON::SELECT:
            m_btn_state &= 0x0B;
            break;
    }
    if (old_dpad_state != m_dpad_state || old_btn_state != m_btn_state)
    {
        m_input_irq = true;
    }
}

void GAMEBOY::InputHandler::btn_up(BUTTON btn)
{
    switch (btn)
    {
        case BUTTON::UP:
            m_dpad_state |= 0x04;
            break;
        case BUTTON::DOWN:
            m_dpad_state |= 0x08;
            break;
        case BUTTON::LEFT:
            m_dpad_state |= 0x02;
            break;
        case BUTTON::RIGHT:
            m_dpad_state |= 0x01;
            break;
        case BUTTON::A:
            m_btn_state |= 0x01;
            break;
        case BUTTON::B:
            m_btn_state |= 0x02;
            break;
        case BUTTON::START:
            m_btn_state |= 0x08;
            break;
        case BUTTON::SELECT:
            m_btn_state |= 0x04;
            break;
    }
}

uint8_t GAMEBOY::InputHandler::poll_irq()
{
    return m_input_irq ? 0x10 : 0x00;
}

void GAMEBOY::InputHandler::set_irq(uint8_t state)
{
    m_input_irq = state & 0x10;
}
