#ifndef __CPU_INSTRUCTION_MISC_H__
#define __CPU_INSTRUCTION_MISC_H__

#include "cpu_instruction.h"

namespace CPU
{
    class NOP: public CpuInstruction
    {
    public:
        bool tick();
    };
};

#endif