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
        Gameboy(ROMDATA& rom, InputHandler& input_handler, std::shared_ptr<LINE_PIXELS> line_buffer)
        : memory(rom, input_handler), cpu(memory), ppu(memory, line_buffer), dma(memory) {}
        bool tick();
    };
};

#endif

