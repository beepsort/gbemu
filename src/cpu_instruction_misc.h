#ifndef __CPU_INSTRUCTION_MISC_H__
#define __CPU_INSTRUCTION_MISC_H__

#include "cpu_instruction.h"
#include "cpu_registers.h"

namespace CPU
{
    class NOP: public CpuInstruction
    {
    private:
        CPU::CpuRegisters& registers;
    public:
        NOP(CPU::CpuRegisters& registers)
        : registers(registers) {}
        bool tick();
    };
};

#endif