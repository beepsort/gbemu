#include "gameboy/gameboy.h"

bool GAMEBOY::Gameboy::tick()
{
    bool drawn_to_buffer = false;
    cpu.tick();
    for (uint8_t i=0; i<4; i++)
    {
        if (ppu.tick())
        {
            drawn_to_buffer = true;
        }
    }
    dma.tick();
    return drawn_to_buffer;
}
