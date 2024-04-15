#include "gbmemory.h"
#include "cpu_interrupt.h"
#include "serial.h"
#include "timer.h"

uint8_t GAMEBOY::IOHandler::read(uint16_t addr)
{
    switch (addr)
    {
        case TIMER_REG_DIV:
            return Timer::getInstance().read(Timer::Register::DIV);
        case TIMER_REG_TIMA:
            return Timer::getInstance().read(Timer::Register::TIMA);
        case TIMER_REG_TMA:
            return Timer::getInstance().read(Timer::Register::TMA);
        case TIMER_REG_TAC:
            return Timer::getInstance().read(Timer::Register::TAC);
        case 0xFF44:
            return 0x90;
        case 0xFFFF:
            return IE;
        default:
            if (addr < 0xFF00 || addr > 0xFF80)
            {
                return 0x00;
            }
            return ioRam[addr - 0xFF00];
    }
}

void GAMEBOY::IOHandler::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
        case SERIAL_CONTROL:
            if (data&0x80)
            {
                // TODO: delay output, trigger interrupt, collect input (very low priority)
                // begin transfer
                GAMEBOY::SerialEventSupervisor& events = GAMEBOY::SerialEventSupervisor::getInstance();
                events.publish(GAMEBOY::SerialEventType::SERIAL_OUT, ioRam[SERIAL_DATA - 0xFF00]);
                data &= 0x7F;
            }
            ioRam[SERIAL_CONTROL - 0xFF00] = data;
            break;
        case TIMER_REG_DIV:
            Timer::getInstance().write(Timer::Register::DIV, data);
            break;
        case TIMER_REG_TIMA:
            Timer::getInstance().write(Timer::Register::TIMA, data);
            break;
        case TIMER_REG_TMA:
            Timer::getInstance().write(Timer::Register::TMA, data);
            break;
        case TIMER_REG_TAC:
            Timer::getInstance().write(Timer::Register::TAC, data);
            break;
        case 0xFFFF:
            IE = data;
            break;
        default:
            if (addr < 0xFF00 || addr > 0xFF7F)
            {
                break;
            }
            ioRam[addr - 0xFF00] = data;
            break;
    }
}
