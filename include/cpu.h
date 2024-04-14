#ifndef __CPU_H__
#define __CPU_H__

#include "cpu_registers.h"
#include "cpu_instruction.h"
#include "cpu_interrupt.h"
#include "gbmemory.h"
#include "rom.h"
#include "timer.h"

namespace GAMEBOY
{
    class Cpu
    {
    private:
        CpuInstruction* currentInstruction = nullptr;
        CpuRegisters registers;
        AddressDispatcher addressDispatcher;
        InterruptHandler interruptHandler;
        Timer timer;
    public:
        Cpu(ROMDATA& rom);
        const CpuRegisters& tick();
        void report();
    };
};

#endif
