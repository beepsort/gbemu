#ifndef __PPU_H__
#define __PPU_H__

#include <stdint.h>
#include <memory>
#include <array>
#include "gameboy/memory.h"

namespace GAMEBOY
{
    class PPU
    {
    private:
        AddressDispatcher& memory;
    public:
        PPU(AddressDispatcher& memory)
        : memory(memory) {}
        std::array<uint8_t, 16> getTile(uint8_t index);
        std::array<uint8_t, 16> getSprite(uint8_t index);
    };
};


#endif
