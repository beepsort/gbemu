#ifndef __MEMORY_STATIC_H__
#define __MEMORY_STATIC_H__

#include <array>
#include <stdint.h>
#include <cstddef>
#include <vector>

#include "gameboy/memory.h"

namespace GAMEBOY
{
    class MapperStatic : public CartMapper
    {
    private:
        std::array<uint8_t, 0x8000> rom = {};
        std::array<uint8_t, 0x2000> ram = {};
    public:
        MapperStatic(ROMDATA& rom);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
    };
};

#endif
