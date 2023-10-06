#ifndef __CPU_H__
#define __CPU_H__

#include "cpu_registers.h"
#include "cpu_instruction.h"
#include "memory.h"
#include "rom.h"

namespace CPU
{
    class Cpu
    {
    private:
        CpuInstruction* currentInstruction = nullptr;
        CpuRegisters registers;
        MEMORY::AddressDispatcher addressDispatcher;
    public:
        Cpu(ROMDATA& rom);
        const CpuRegisters& tick();
        void report();
    };
};

#endif