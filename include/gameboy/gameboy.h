#ifndef __GAMEBOY_H__
#define __GAMEBOY_H__

#include "gameboy/cpu.h"
#include "gameboy/ppu.h"
#include "gameboy/memory.h"

namespace GAMEBOY
{
    class Gameboy
    {
    private:
        AddressDispatcher memory;
        Cpu cpu;
        PPU ppu;
    public:
        Gameboy(ROMDATA& rom)
        : memory(rom), cpu(memory), ppu(memory) {}
        void tick();
    };
};

#endif

