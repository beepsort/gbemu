#ifndef __MEMORY_IO_H__
#define __MEMORY_IO_H__

#include <stdint.h>

namespace GAMEBOY
{
    class IOHandler
    {
    private:
        uint8_t ioRam[0x80];
        /*
         * 0xFFFF Interrupt Enable
         * Allows each interrupt category to be enabled or disabled separately
         * Uses the same bitpattern as the interrupt flag IF
         */
        uint8_t IE = 0;
    public:
        /*
         * 0xFF01 Serial Transfer Data
         * Data either received or to be sent over the serial port
         */
        static const uint16_t SERIAL_DATA = 0xFF01;
        /*
         * 0xFF02 Serial Transfer Control
         * Bit 0: 0 = External Clock (Slave), 1 = Internal Clock (Master)
         * Bit 7: 1 when a transfer is requested, or in progress
         */
        static const uint16_t SERIAL_CONTROL = 0xFF02;
        /*
         * DIV register is the visible portion of the system counter
         * The system counter increments every M-cycle and is 6 bits longer than DIV
         * This means the DIV register increments once every 64 M-cycles
         */
        static const uint16_t TIMER_REG_DIV = 0xFF04;
        /*
         * TIMA is incremented based on a combination of the system counter
         * and the TAC register. The TAC register configures the frequency by selecting
         * which bit from the system counter will be used to increment TIMA.
         * When TIMA overflows a timer interrupt is requested, as well as it's
         * value is reset to the value stored in the TMA register.
         * Note this happens the M-cycle AFTER the overflow
         */
        static const uint16_t TIMER_REG_TIMA = 0xFF05;
        /*
         * TMA is the value to reset TIMA to after an overflow.
         * It is one of the two methods of controlling the frequency
         * of timer interrupts
         */
        static const uint16_t TIMER_REG_TMA = 0xFF06;
        /*
         * TAC is the timer control register and controls whether TIMA is
         * incremented, as well as the frequency of those increments if enabled.
         * Bit 2: TIMA increment enable
         * Bit 1&0: TIMA increment frequency, where:
         * - 00: 256 M-cycles
         * - 01: 4 M-cycles
         * - 10: 16 M-cycles
         * - 11: 64 M-cycles
         */
        static const uint16_t TIMER_REG_TAC = 0xFF07;
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
        static const uint16_t INTERRUPT_REG_IF = 0xFF0F;
        /*
         * 0xFF40 LCDC/LCD Control
         * LCDC controls what is displayed on the screen and how
         * Bits
         * 7: LCD & PPU enable (0:OFF, 1:ON)
         * 6: Window tile map area (0:9800-9BFF, 1:9C00-9FFF)
         * 5: Window enable (0:OFF, 1:ON)
         * 4: BG & Window tile data area (0:8800-97FF, 1:8000-8FFF)
         * 3: BG tile map area (0:9800-9BFF, 1:9C00-9FFF)
         * 2: OBJ size (0:8x8, 1:8x16)
         * 1: OBJ enable (0:OFF, 1:ON)
         * 0: BG & Window enable (0:OFF, 1:ON)
         */
        static const uint16_t PPU_REG_LCDC = 0xFF40;
        /*
         * 0xFF47 BGP/BG palette
         * BGP assigns gray shades to color IDs for BG & window tiles
         * Bits
         * 7-6: Shade for ID 3
         * 5-4: Shade for ID 2
         * 3-2: Shade for ID 1
         * 1-0: Shade for ID 0
         * The two bit shade values become
         * 00: White
         * 01: Light gray
         * 10: Dark gray
         * 11: Black
         */
        static const uint16_t PPU_REG_BGP = 0xFF47;
        /*
         * 0xFF48-0xFF49 OBP0 & OBP1
         * OBP work the same as BGP, except bits 1&0 are ignored
         * as color ID 0 is treated as transparent for sprites/OBJS
         */
        static const uint16_t PPU_REG_OBP0 = 0xFF48;
        static const uint16_t PPU_REG_OBP1 = 0xFF49;
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
    };
}

#endif
