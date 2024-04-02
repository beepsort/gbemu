#include "gbmemory.h"
#include "cpu_interrupt.h"

uint8_t MEMORY::IOHandler::read(uint16_t addr)
{
    switch (addr)
    {
        case 0xFF0F:
            return IF;
        case 0xFFFF:
            return IE;
        default:
            return 0;
    }
}

void MEMORY::IOHandler::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
        case 0xFF0F:
            IF = data;
            break;
        case 0xFFFF:
            IE = data;
            break;
    }
}
