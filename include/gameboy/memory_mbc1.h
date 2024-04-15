#ifndef __MEMORY_MBC1_H__
#define __MEMORY_MBC1_H__

#include <array>
#include <stdint.h>
#include <cstddef>
#include <vector>

#include "gameboy/rom.h"
#include "gameboy/memory.h"

namespace GAMEBOY
{
    // https://gbdev.io/pandocs/MBC1.html
    class MapperMbc1 : public CartMapper
    {
    private:
        const static size_t BANK_LO = 0x4000;
        const static size_t BANK_HI = CART_ROM_HI;
        const static size_t REG_RAM_ENABLE_LO = 0x0000;
        const static size_t REG_RAM_ENABLE_HI = 0x1FFF;
        const static size_t REG_ROM_BANK_LO = 0x2000;
        const static size_t REG_ROM_BANK_HI = 0x3FFF;
        const static size_t REG_RAM_BANK_LO = 0x4000;
        const static size_t REG_RAM_BANK_HI = 0x5FFF;
        const static size_t REG_BANK_MODE_LO = 0x6000;
        const static size_t REG_BANK_MODE_HI = 0x7FFF;
        typedef std::array<uint8_t, 0x4000> RomBank;
        typedef std::array<uint8_t, 0x2000> RamBank;
        RomBank fixed_rom;
        std::vector<RomBank> banked_rom;
        uint8_t rom_bank_select = 0x00;
        std::vector<RamBank> banked_ram;
        uint8_t ram_bank_select = 0x00;
        bool ram_enabled = false;
    public:
        MapperMbc1(ROMDATA& rom, bool cartRam, bool cartBattery);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
    };
};

#endif
