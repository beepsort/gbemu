#ifndef __CPU_INSTRUCTION_MISC_H__
#define __CPU_INSTRUCTION_MISC_H__

#include "cpu_instruction.h"
#include "cpu_registers.h"

namespace CPU
{
    class NOP: public CpuInstruction
    {
    private:
        CpuRegisters& registers;
    public:
        NOP(CpuRegisters& registers)
        : registers(registers) {}
        InstructionResult tick();
    };
};

#endif
