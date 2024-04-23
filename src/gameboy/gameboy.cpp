#include "gameboy/gameboy.h"

void GAMEBOY::Gameboy::tick()
{
    cpu.tick();
    for (uint8_t i=0; i<4; i++)
    {
        ppu.tick();
    }
}
