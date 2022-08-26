#include <math.h>

#include "memory_mbc1.h"

MEMORY::MapperMbc1::MapperMbc1(ROMDATA& rom, bool cartRam, bool cartBattery)
    : banked_rom(num_rom_banks(rom))
    , banked_ram(num_ram_banks(rom))
{
    auto src_it = rom.cbegin();
    auto src_end = rom.cend();
    // Fill fixed bank
    auto dest_it = fixed_rom.begin();
    auto dest_end = fixed_rom.end();
    while (src_it != src_end && dest_it != dest_end) // while neither end reached
    {
        *dest_it = *src_it;
        src_it++;
        dest_it++;
    }
    // Fill each banked rom
    int banks = num_rom_banks(rom);
    for (int i=1; i<banks; i++)
    {
        dest_it = banked_rom[i-1].begin();
        dest_end = banked_rom[i-1].end();
        while (src_it != src_end && dest_it != dest_end) // while neither end reached
        {
            *dest_it = *src_it;
            src_it++;
            dest_it++;
        }
    }
}

uint8_t MEMORY::MapperMbc1::read(uint16_t addr)
{
    if (addr < BANK_LO) // lower fixed bank
    {
        return fixed_rom[addr];
    }
    else if (addr >= BANK_LO && addr <= BANK_HI)
    {
        uint16_t index = rom_bank_select - 1;
        if (rom_bank_select == 0) index = 0;
        return banked_rom[index][addr - BANK_LO];
    }
    else if (addr >= CART_RAM_LO && addr <= CART_RAM_HI)
    {
        if (!ram_enabled)
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Attempted to read from ram while disabled\n");
            return 0x00;
        }
        return banked_ram[ram_bank_select][addr - CART_RAM_LO];
    }
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Attempted to read memory address not mapped by cart %#04hx\n", addr);
    return 0x00;
}

void MEMORY::MapperMbc1::write(uint16_t addr, uint8_t data)
{
    if (addr <= REG_RAM_ENABLE_HI)
    {
        ram_enabled = (data & 0x0A) == 0x0A;
    }
    else if (addr >= REG_ROM_BANK_LO && addr <= REG_ROM_BANK_HI)
    {
        rom_bank_select = data & 0x1F;
        // TODO: Limit selection to required number of banks
    }
    else if (addr >= REG_RAM_BANK_LO && addr <= REG_RAM_BANK_HI)
    {
        ram_bank_select = data & 0x03;
    }
    else if (addr >= REG_BANK_MODE_LO && addr <= REG_BANK_MODE_HI)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_ERROR, "Bank mode selection not implemented\n");
    }
    else if (addr >= CART_RAM_LO && addr <= CART_RAM_HI)
    {
        if (!ram_enabled)
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Attempted to write to ram while disabled\n");
            return;
        }
        banked_ram[ram_bank_select][addr - CART_RAM_LO] = data;
    }
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Attemted to write memory address not mapped by cart %#04hx\n", addr);
}