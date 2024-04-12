#ifndef __MEMORY_IO_H__
#define __MEMORY_IO_H__

#include <stdint.h>

namespace MEMORY
{
    class IOHandler
    {
    private:
        uint8_t ioRam[0x80];
        /*
         * 0xFF01 Serial Transfer Data
         * Data either received or to be sent over the serial port
         */
        static const uint16_t SERIAL_DATA = 0x01;
        /*
         * 0xFF02 Serial Transfer Control
         * Bit 0: 0 = External Clock (Slave), 1 = Internal Clock (Master)
         * Bit 7: 1 when a transfer is requested, or in progress
         */
        static const uint16_t SERIAL_CONTROL = 0x02;
        /*
         * 0xFF0F Interrupt Flag
         * Upon triggering an interrupt, the interrupt flag is set
         * to describe the type of interrupt.
         * Specific bits are set depending on the trigger
         * Bits
         * 7-5 Unused
         * 4 Joypad
         * 3 Serial
         * 2 Timer
         * 1 LCD
         * 0 VBlank
         */
        static const uint16_t IF = 0x0F;
        /*
         * 0xFFFF Interrupt Enable
         * Allows each interrupt category to be enabled or disabled separately
         * Uses the same bitpattern as the interrupt flag IF
         */
        uint8_t IE = 0;
    public:
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
    };
}

#endif
