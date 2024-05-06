#ifndef __GAMEBOY_H__
#define __GAMEBOY_H__

#include "gameboy/cpu.h"
#include "gameboy/ppu.h"
#include "gameboy/memory.h"
#include "gameboy/memory_dma.h"
#include "gameboy/input.h"

namespace GAMEBOY
{
    class Gameboy
    {
    private:
        AddressDispatcher memory;
        Cpu cpu;
        PPU ppu;
        DmaController dma;
    public:
        Gameboy(ROMDATA& rom, InputHandler& input_handler, LINE_BUFFERS line_buffers)
        : memory(rom, input_handler), cpu(memory), ppu(memory, line_buffers), dma(memory) {}
        bool tick();
    };
};

#endif

