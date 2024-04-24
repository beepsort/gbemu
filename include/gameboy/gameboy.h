#ifndef __GAMEBOY_H__
#define __GAMEBOY_H__

#include "gameboy/cpu.h"
#include "gameboy/ppu.h"
#include "gameboy/memory.h"
#include "gameboy/input.h"

namespace GAMEBOY
{
    class Gameboy
    {
    private:
        AddressDispatcher memory;
        Cpu cpu;
        PPU ppu;
    public:
        Gameboy(ROMDATA& rom, InputHandler& input_handler, std::shared_ptr<std::array<uint8_t, 160>> line_buffer)
        : memory(rom, input_handler), cpu(memory), ppu(memory, line_buffer) {}
        bool tick();
    };
};

#endif

