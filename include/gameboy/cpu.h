#ifndef __CPU_H__
#define __CPU_H__

#include "gameboy/cpu_registers.h"
#include "gameboy/cpu_instruction.h"
#include "gameboy/cpu_interrupt.h"
#include "gameboy/memory.h"
#include "gameboy/rom.h"
#include "gameboy/timer.h"

namespace GAMEBOY
{
    class Cpu
    {
    private:
        CpuInstruction* currentInstruction = nullptr;
        CpuRegisters registers;
        AddressDispatcher& memory;
        InterruptHandler interruptHandler;
    public:
        Cpu(AddressDispatcher& memory)
        : memory(memory) {}
        const CpuRegisters& tick();
    };
};

#endif
