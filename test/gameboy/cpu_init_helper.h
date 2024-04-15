#ifndef __CPU_INIT_HELPER_H__
#define __CPU_INIT_HELPER_H__

#include "gameboy/cpu_registers.h"
#include "gameboy/memory.h"
#include "gameboy/rom.h"
#include <SDL2/SDL_log.h>
#include <vector>

static const ROMDATA rom();

class CpuInitHelper
{
private:
    ROMDATA rom;
public:
    GAMEBOY::CpuRegisters registers;
    GAMEBOY::AddressDispatcher addressDispatcher;
    CpuInitHelper() :
        rom(std::vector<uint8_t>(32768, 0)),
        addressDispatcher(GAMEBOY::AddressDispatcher(rom))
    {
        *registers.PC = 0xC000;
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    };
};

#endif
