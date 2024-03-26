#ifndef __MEMORY_MBC3_H__
#define __MEMORY_MBC3_H__

#include <array>
#include <stdint.h>
#include <cstddef>
#include <vector>

#include "gbmemory.h"

namespace MEMORY
{
    class MapperMbc3 : public CartMapper
    {
    private:

    public:
        MapperMbc3(ROMDATA& rom, bool cartRam, bool cartBattery, bool cartTimer);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t data);
    };
};

#endif
