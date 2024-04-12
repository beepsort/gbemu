#include "gbmemory.h"
#include "cpu_interrupt.h"
#include "serial.h"

uint8_t MEMORY::IOHandler::read(uint16_t addr)
{
    switch (addr)
    {
        case 0xFF01:
            return ioRam[SERIAL_DATA];
        case 0xFF02:
            return ioRam[SERIAL_CONTROL];
        case 0xFF0F:
            return ioRam[IF];
        case 0xFFFF:
            return IE;
        default:
            return ioRam[addr - 0xFF00];
    }
}

void MEMORY::IOHandler::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
        case 0xFF01:
            ioRam[SERIAL_DATA] = data;
            break;
        case 0xFF02:
            if (data&0x80)
            {
                // TODO: delay output, trigger interrupt, collect input (very low priority)
                // begin transfer
                GAMEBOY::SerialEventSupervisor& events = GAMEBOY::SerialEventSupervisor::getInstance();
                events.publish(GAMEBOY::SerialEventType::SERIAL_OUT, ioRam[SERIAL_DATA]);
                data &= 0x7F;
            }
            ioRam[SERIAL_CONTROL] = data;
            break;
        case 0xFF0F:
            ioRam[IF] = data;
            break;
        case 0xFFFF:
            IE = data;
            break;
        default:
            ioRam[addr - 0xFF00] = data;
            break;
    }
}
