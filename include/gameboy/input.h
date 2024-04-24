#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdint.h>

namespace GAMEBOY
{
    class InputHandler
    {
    private:
        uint8_t m_dpad_state = 0x0F;
        uint8_t m_btn_state = 0x0F;
        bool m_input_irq = false;
    public:
        uint8_t joyp(bool btn_sel, bool dpad_sel);
        enum class BUTTON
        {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            A,
            B,
            START,
            SELECT
        };
        void btn_down(BUTTON btn);
        void btn_up(BUTTON btn);
        uint8_t poll_irq();
        void set_irq(uint8_t state);
    };
};

#endif
