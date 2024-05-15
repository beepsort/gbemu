#include <stdexcept>
#include <SDL2/SDL_log.h>

#include "gameboy/memory_static.h"

GAMEBOY::MapperStatic::MapperStatic(ROMDATA& rom)
{
    auto src_it = rom.cbegin();
    auto src_end = rom.cend();
    auto dest_it = this->rom.begin();
    auto dest_end = this->rom.end();
    while (src_it != src_end && dest_it != dest_end) // while neither end reached
    {
        *dest_it = *src_it;
        src_it++;
        dest_it++;
    }
}

uint8_t GAMEBOY::MapperStatic::read(uint16_t addr)
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

void GAMEBOY::MapperStatic::write(uint16_t addr, uint8_t data)
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
