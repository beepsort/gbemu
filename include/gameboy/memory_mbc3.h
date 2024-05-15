#ifndef __MEMORY_MBC3_H__
#define __MEMORY_MBC3_H__

#include <array>
#include <stdint.h>
#include <cstddef>
#include <vector>

#include "gameboy/memory.h"

namespace GAMEBOY
{
    class MapperMbc3 : public CartMapper
    {
    private:
        std::vector<std::array<uint8_t, 0x2000>> m_rambanks;
        std::vector<std::array<uint8_t, 0x4000>> m_rombanks;
        bool m_ram_enable = false;
        uint8_t m_sel_rom_bank = 1;
        uint8_t m_sel_ram_bank = 0;
    public:
        MapperMbc3(ROMDATA& rom, bool cartRam, bool cartBattery, bool cartTimer);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
    };
};

#endif
