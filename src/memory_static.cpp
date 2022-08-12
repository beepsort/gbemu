#include <stdexcept>
#include <SDL2/SDL_log.h>

#include "memory_static.h"

MEMORY::MapperStatic::MapperStatic(ROMDATA& rom)
{
    auto src_it = rom.cbegin();
    auto src_end = rom.cbegin();
    auto dest_it = this->rom.begin();
    auto dest_end = this->rom.end();
    while (!(src_it != src_end || dest_it != dest_end)) // while neither end reached
    {
        *dest_end = *src_end;
        src_it++;
        dest_it++;
    }
}

bool MEMORY::MapperStatic::is_mapped(uint16_t addr)
{
    bool in_rom = addr >= CART_ROM_LO && addr <= CART_ROM_HI;
    bool in_ram = addr >= CART_RAM_LO && addr <= CART_RAM_HI;
    return in_rom || in_ram;
}

uint8_t MEMORY::MapperStatic::read(uint16_t addr)
{
    if (addr >= CART_ROM_LO && addr <= CART_ROM_HI)
    {
        return rom[addr];
    }
    else if (addr >= CART_RAM_LO && addr <= CART_RAM_HI)
    {
        return ram[addr - CART_RAM_LO];
    }
    throw std::out_of_range("Attempted to access memory address not mapped by cart");
}

void MEMORY::MapperStatic::write(uint16_t addr, uint8_t data)
{
     if (addr >= CART_RAM_LO && addr <= CART_RAM_HI)
     {
        ram[addr - CART_RAM_LO] = data;
     }
     else
     {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Attemted to write memory address not mapped by cart %#04hx\n", addr);
     }
}