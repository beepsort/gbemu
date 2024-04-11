#ifndef __ROM_H__
#define __ROM_H__

#include <optional>
#include <vector>
#include <SDL2/SDL.h>
#include <stdint.h>

namespace ROM
{
    // https://gbdev.io/pandocs/The_Cartridge_Header.html
    const size_t TITLE_BEGIN = 0x0134;
    const size_t TITLE_END = 0x0143;
    const size_t CART_TYPE = 0x0147;
    const size_t ROM_SIZE = 0x0148;
    const size_t RAM_SIZE = 0x0149;
};

typedef std::vector<uint8_t> ROMDATA;
std::optional<ROMDATA> open_rom(char* rom_path);
int num_rom_banks(ROMDATA& data);
int num_ram_banks(ROMDATA& data);

#endif