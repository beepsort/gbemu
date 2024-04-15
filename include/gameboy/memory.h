#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <array>
#include <vector>
#include <stdint.h>
#include <SDL2/SDL_log.h>

#include "gameboy/rom.h"
#include "gameboy/memory_io.h"

// https://gbdev.io/pandocs/Memory_Map.html

namespace GAMEBOY
{
    const static uint16_t CART_ROM_LO = 0x0000;
    const static uint16_t CART_ROM_HI = 0x7FFF;
    const static uint16_t VRAM_LO = 0x8000;
    const static uint16_t VRAM_HI = 0x9FFF;
    const static uint16_t CART_RAM_LO = 0xA000;
    const static uint16_t CART_RAM_HI = 0xBFFF;
    const static uint16_t WRAM_LO = 0xC000;
    const static uint16_t WRAM_HI = 0xDFFF;
    const static uint16_t OAM_LO = 0xFE00;
    const static uint16_t OAM_HI = 0xFE9F;
    const static uint16_t IO_REG_LO = 0xFF00;
    const static uint16_t INTERRUPT_FLAG = 0xFF0F;
    const static uint16_t IO_REG_HI = 0xFF7F;
    const static uint16_t HRAM_LO = 0xFF80;
    const static uint16_t HRAM_HI = 0xFFFE;
    const static uint16_t INTERRUPT_ENABLE = 0xFFFF;

    class CartMapper
    {
    public:
        static CartMapper* create_mapper(ROMDATA& rom);
        virtual uint8_t read(uint16_t addr) = 0;
        virtual void write(uint16_t addr, uint8_t data) = 0;
    };

    class AddressDispatcher
    {
    private:
        CartMapper* cartMapper;
        IOHandler ioHandler;
        std::array<uint8_t, 0x2000> videoRam;
        std::array<uint8_t, 0x2000> workRam;
        std::array<uint8_t, 0x7F> highRam;
    public:
        AddressDispatcher(ROMDATA& rom);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
    };
};

#endif
