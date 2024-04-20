#ifndef __PPU_H__
#define __PPU_H__

#include <stdint.h>
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
    };
};


#endif
