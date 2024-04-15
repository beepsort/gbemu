#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

#include "gbmemory.h"

namespace GAMEBOY
{
    class Timer
    {
    private:
        static Timer* instance;
        // Increments every 64 M-cycles
        uint8_t registerDIV = 0;
        uint8_t registerDIVSubTick = 0;
        uint8_t registerTIMA = 0;
        uint8_t registerTMA = 0;
        uint8_t registerTAC = 0;
        bool timerBitPrevious = false;
        bool timaOverflow = false;
        Timer() = default;
        ~Timer() = default;
    public:
        static Timer& getInstance()
        {
            if (!instance) {
                instance = new Timer();
            }
            return *instance;
        }
        enum class Register
        {
            DIV,
            TIMA,
            TMA,
            TAC
        };
        void write(Register target, uint8_t data);
        uint8_t read(Register target);
        void tick(AddressDispatcher& memory);
    };
};

#endif
