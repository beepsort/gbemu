#include "gameboy/timer.h"
#include "gameboy/memory_io.h"

GAMEBOY::Timer* GAMEBOY::Timer::instance = nullptr;

void GAMEBOY::Timer::tick(GAMEBOY::AddressDispatcher& memory)
{
    if (++registerDIVSubTick == 63)
    {
        ++registerDIV;
        registerDIVSubTick = 0;
    }
    bool timerBitCurrent;
    switch (registerTAC & 0x03)
    {
        case 0b00:
            timerBitCurrent = registerDIV & 0x02;
            break;
        case 0b01:
            timerBitCurrent = registerDIVSubTick & 0x02;
            break;
        case 0b10:
            timerBitCurrent = registerDIVSubTick & 0x08;
            break;
        case 0b11:
            timerBitCurrent = registerDIVSubTick & 0x20;
            break;
    }
    timerBitCurrent = timerBitCurrent && (registerTAC & 0x04);
    if (timaOverflow)
    {
        // previously overflowed
        registerTIMA = registerTMA;
        // request timer interrupt
        uint8_t interrupt_flags = memory.read(IOHandler::INTERRUPT_REG_IF);
        interrupt_flags |= 0x04;
        memory.write(IOHandler::INTERRUPT_REG_IF, interrupt_flags);
        timaOverflow = false;
    }
    if (!timerBitCurrent && timerBitPrevious) // falling edge
    {
        ++registerTIMA;
        // if overflow, queue TMA->TIMA copy for next m-cycle
        timaOverflow = (registerTIMA == 0x00);
    }
    timerBitPrevious = timerBitCurrent;
}

void GAMEBOY::Timer::write(Register target, uint8_t data)
{
    switch (target)
    {
        case Register::DIV:
            // always reset on write, data ignored
            registerDIV = 0;
            break;
        case Register::TIMA:
            registerTIMA = data;
            break;
        case Register::TMA:
            registerTMA = data;
            break;
        case Register::TAC:
            registerTAC = data;
            break;
    }
}

uint8_t GAMEBOY::Timer::read(Register target)
{
    switch (target)
    {
        case Register::DIV:
            return registerDIV;
        case Register::TIMA:
            return registerTIMA;
        case Register::TMA:
            return registerTMA;
        case Register::TAC:
            return registerTAC;
        default:
            // unreachable
            return 0x00;
    }
}
