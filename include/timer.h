#ifndef __TIMER_H__
#define __TIMER_H__

#include "gbmemory.h"

namespace GAMEBOY
{
    // Increments every 64 M-cycles
    const uint16_t REG_DIV = 0xFF04;
    const uint16_t REG_TIMA = 0xFF05;
    const uint16_t REG_TMA = 0xFF06;
    const uint16_t REG_TAC = 0xFF07;

    class Timer
    {
    private:
        AddressDispatcher& memory;
    public:
        Timer(AddressDispatcher& memory)
        : memory(memory) {}
        void tick();
    };
};

#endif
